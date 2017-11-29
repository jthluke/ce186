// Need to download the following 3 libraries for this code to work! Put with your other Arduino libraries in Program Files:
// 1) Updated SD library (back-up or overwrite the default SD library): https://github.com/adafruit/SD
// 2) Adafruit_GPS:  https://github.com/adafruit/Adafruit_GPS
// 3) NeoPixel: https://github.com/adafruit/Adafruit_NeoPixel

/* USED PINS
mySerial 8 7
chipSelect 10
ledPin 13  (SD's card ledPin)
neopixel1 uses 5
neopixel2 uses 5
trigPin 9
echoPin 2
motorPin 3
Cannot use 11 and 12 (maybe???)
*/

/* -------------------------------------- */
/* -----IMPORTS AND GLOBAL VARIABLES----- */
/* -------------------------------------- */
/* DATA LOGGING / GPS STUFF */
#include <SPI.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <SD.h>
#include <avr/sleep.h>

SoftwareSerial mySerial(8, 7);
Adafruit_GPS GPS(&mySerial);
#define GPSECHO  false // Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
#define LOG_FIXONLY true // set to true to only log to SD when GPS has a fix, for debugging, keep it false
boolean usingInterrupt = false; // this keeps track of whether we're using the interrupt; off by default
void useInterrupt(boolean); // Func prototype keeps Arduino 0023 happy

// Set the pins used
#define chipSelect 10
#define ledPin 13

File logfile;

// read a Hex value and return the decimal equivalent
uint8_t parseHex(char c) {
  if (c < '0')
    return 0;
  if (c <= '9')
    return c - '0';
  if (c < 'A')
    return 0;
  if (c <= 'F')
    return (c - 'A')+10;
}

void error(uint8_t errno) { // blink out an error code
  while(1) {
    uint8_t i;
    for (i=0; i<errno; i++) {
      digitalWrite(ledPin, HIGH);
      delay(100);
      digitalWrite(ledPin, LOW);
      delay(100);
    }

    for (i=errno; i<10; i++) {
      delay(200);
    }
  }
}

/* HARDWARE STUFF */
#include <Adafruit_NeoPixel.h>
#define myPIN1 5
#define myPIN2 5
//avoid 10, 8, 7 bc they're used by the gps
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(16, myPIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(16, myPIN2, NEO_GRB + NEO_KHZ800);

// defines pins numbers
const byte trigPin = 9;
const byte echoPin = 2;
const byte motorPin = 3;
// defines variables
long duration;
int distance;
int AvgDist;
int toAvg[5];
int total;
boolean alert = 0;
byte trigMax = 30;  
byte trigMin = 10;

//Photoresistor resistor
int R2 = 10000;

// Global RGB values, change to suit your needs
byte r = 255;
byte g = 0;
byte b = 255;


/* -------------------------------------- */
/* -----SETUP----- */
/* -------------------------------------- */
void setup() {
  /* DATA LOGGING / GPS STUFF */
  Serial.begin(115200);
  Serial.println("\r\nUltimate GPSlogger Shield");
  pinMode(ledPin, OUTPUT);
   
  pinMode(10, OUTPUT); // make sure that the default chip select pin is set to output, even if you don't use it:

  if (!SD.begin(chipSelect)) { // see if the card is present and can be initialized:
    Serial.println("Card init. failed!");
    error(2);
  }
  char filename[15];
  strcpy(filename, "GPSLOG00.csv");
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = '0' + i/10;
    filename[7] = '0' + i%10;
    if (! SD.exists(filename)) { // create if does not exist, do not open existing, write, sync after write
      break;
    }
  }

  logfile = SD.open(filename, FILE_WRITE);
  if( ! logfile ) {
    Serial.print("Couldnt create "); 
    Serial.println(filename);
    error(3);
  }
  Serial.print("Writing to "); 
  Serial.println(filename);

  // connect to the GPS at the desired rate
  GPS.begin(9600);
  
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // Set the update rate: 100 millihertz (once every 10 seconds), 1Hz or 5Hz update rate

  GPS.sendCommand(PGCMD_NOANTENNA); // Turn off updates on antenna status, if the firmware permits it

  useInterrupt(true);

  delay(1000); Serial.println("Ready!");

  /* HARDWARE */
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(motorPin, OUTPUT);
  
  //sets up neopixel lights
  strip1.begin();
  strip1.setBrightness(100);
  strip1.show();
  
  //sets up neopixel lights
  strip2.begin();
  strip2.setBrightness(100);
  strip2.show();
}

/* -------------------------------------- */
/* -----HELPER FUNCTIONS----- */
/* -------------------------------------- */
/* DATA LOGGING / GPS STUFF */
// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
SIGNAL(TIMER0_COMPA_vect) {
  char c = GPS.read();
  #ifdef UDR0
      if (GPSECHO)
        if (c) UDR0 = c;  
  #endif
}

void useInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    usingInterrupt = true;
  } 
  else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
    usingInterrupt = false;
  }
}

/* HARDWARE */
boolean alertFunction(int dist_array) {
  if (toAvg[3] < toAvg[1] | toAvg[4] < toAvg[2]){
    alert = true;
  } else {
    alert = false;
  }
  return alert;
}

void allOff() {
  strip1.clear();
  strip2.clear();
  strip1.show();
  strip2.show();
}

// Turns the NeoPixels on, according to RGB settings
void activate(byte g2) {
  for( int i = 0; i < 16; i++ ) {
    strip1.setPixelColor(i, r,g2,b );
    strip2.setPixelColor(i, r,g2,b );
    strip1.show();
    strip2.show();
  }
}

void blinky(int repeats, byte g2) {
  for (int i = 0; i < repeats; i++)
  {
    activate(g2);
    delay(100);
    allOff();
    delay(40);
  }
}

/* -------------------------------------- */
/* -----MAIN LOOP----- */
/* -------------------------------------- */
uint32_t timer = millis();
void loop() {
  /* HARDWARE */
  total = 0;
  //take average of 5 measurements
  for (int i = 0; i < 5; i = i + 1){
  //Ultrasonic Sensor
  // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
  
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
     
    // Calculating the distance
    distance= duration*0.034/2;
    if (distance >400){
      distance = 0;
    }
    toAvg[i] = distance;
    total = total + distance;
    // Prints the distance on the Serial Monitor
    //Serial.print("Distance (cm): ");
    //Serial.println(distance);
    delay(30);
  }
  AvgDist = total/5;
  alert = alertFunction(toAvg);
  
  boolean warning = AvgDist < trigMax && alert && AvgDist > trigMin;
  
  //turns on light and vibration motor when object is near and approaching
  if (warning){
    blinky(5, 0);
    digitalWrite(motorPin, HIGH);
    delay(500);
    digitalWrite(motorPin, LOW);
    delay(50);
  }

  //Photoresistor
  int sensorValue = analogRead(A0);
  int mapVolts = map(sensorValue, 0, 1023, 0, 5);
  int mapmV = map(sensorValue, 0, 1023, 0, 5000);

//  int R1 =  ((((5*1000)/mapmV)-1)*R2)/1000;
//  Serial.print("--->");Serial.println(mapmV); // in mV (proportional to brightness)
//  Serial.print("--->");Serial.println(R1); // in kilo-Ohms (inversely prop to brightness)

  boolean nightlight = mapmV < 100;
  if (nightlight) {
    activate(255);
    delay(1500);
  } else {
    allOff();
  }

//  boolean warning = true;
//  boolean nightlight = false;
  /* DATA LOGGING / GPS STUFF */
  if (! usingInterrupt) {
    char c = GPS.read(); // read data from the GPS in the 'main loop'
    if (GPSECHO) // if you want to debug, this is a good time to do it!
      if (c) Serial.print(c);
  }
  
  if (GPS.newNMEAreceived()) {
//    char *stringptr = GPS.lastNMEA(); // Don't call lastNMEA more than once between parse calls!
    char *stringptr = "a"; // Don't call lastNMEA more than once between parse calls!

    
    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another

    Serial.println("OK"); // Sentence parsed! 
    uint8_t stringsize = strlen(stringptr);
    if (stringsize != logfile.write((uint8_t *)stringptr, stringsize))    //write the string to the SD file
        error(4);
//    if (strstr(stringptr, "RMC") || strstr(stringptr, "GGA"))   logfile.flush();
    logfile.flush();
  }

  if (timer > millis()){
    timer = millis();
  }

  if (millis() - timer > 1000) { 
    timer = millis();
    if (!GPS.fix) {
      Serial.println("No fix.");
//      Serial.println(AvgDist);
      Serial.println(mapmV);
      Serial.print(warning); Serial.println(nightlight);
    } else {
      Serial.println("Log"); // Rad. lets log it!
//      Serial.println(AvgDist);
      Serial.println(mapmV);
      Serial.print(warning); Serial.println(nightlight);
      logfile.print(GPS.hour, DEC); logfile.print(':');
      logfile.print(GPS.minute, DEC); logfile.print(':');
      logfile.print(GPS.seconds, DEC); logfile.print('.');
      logfile.print(GPS.milliseconds); logfile.print(' ');
      logfile.print(GPS.day, DEC); logfile.print('/');
      logfile.print(GPS.month, DEC); logfile.print("/20");
      logfile.print(GPS.year, DEC); logfile.print(','); 
      //Lat
      logfile.print(GPS.latitudeDegrees, 4); logfile.print(',');
      //Lng
      logfile.print(GPS.longitudeDegrees, 4); logfile.print(',');
      //Speed (knots)
      logfile.print(GPS.speed); logfile.print(',');
      //Altitude
      logfile.print(GPS.altitude); logfile.print(',');
      //Warning
      logfile.print(warning); logfile.print(',');
      //NightLight
      logfile.println(nightlight);
   }
  }
}

