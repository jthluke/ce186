// Need to download the following 3 libraries for this code to work! Put with your other Arduino libraries in Program Files:
// 1) Updated SD library (back-up or overwrite the default SD library): https://github.com/adafruit/SD
// 2) Adafruit_GPS:  https://github.com/adafruit/Adafruit_GPS
// 3) NeoPixel: https://github.com/adafruit/Adafruit_NeoPixel

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
#define GPSECHO  true // Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
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
/*#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, 6, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(13, 7, NEO_GRB + NEO_KHZ800);

// defines pins numbers
const int trigPin = 9;
const int echoPin = 11;
// defines variables
long duration;
int distance;
int AvgDist;
int toAvg[5];
int total;
const int motorPin = 3;
boolean alert = 0;
int trigDist = 90; 

// Global RGB values, change to suit your needs
int r = 255;
int g = 0;
int b = 255;*/


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
  strcpy(filename, "GPSLOG00.TXT");
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
  /*pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(motorPin, OUTPUT);
  
  //sets up neopixel lights
  strip.begin();
    strip.setBrightness(20);
    strip.show();
  
  strip2.begin();
    strip2.setBrightness(20);
    strip2.show();*/
}

/* -------------------------------------- */
/* -----HELPER FUNCTIONS----- */
/* -------------------------------------- */
/* DATA LOGGING / GPS STUFF */
// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
SIGNAL(TIMER0_COMPA_vect) {
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  #ifdef UDR0
      if (GPSECHO)
        if (c) UDR0 = c;  
      // writing direct to UDR0 is much much faster than Serial.print 
      // but only one character can be written at a time. 
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
/*boolean alertFunction(int dist_array) {
  if (toAvg[3] < toAvg[1] | toAvg[4] < toAvg[2]){
    alert = 1;
  } else {
    alert = 0;
  }
  return alert;
}


void allOff() {
  strip.clear();
  strip.show();
  strip2.clear();
  strip2.show();
}

// Turns the NeoPixels on, according to RGB settings
void activate() {
  for( int i = 0; i < 16; i++ ) {
       strip.setPixelColor(i, r,g,b );
       strip2.setPixelColor(i, r,g,b );
  }
      
  strip.show();
  strip2.show();
}

void blinky(int repeats) {
  for (int i = 0; i < repeats; i++)
  {
    activate();
    delay(100);
    allOff();
    delay(40);
  }
} */

/* -------------------------------------- */
/* -----MAIN LOOP----- */
/* -------------------------------------- */
uint32_t timer = millis();
void loop() {
  /* DATA LOGGING / GPS STUFF */
  if (! usingInterrupt) {
    char c = GPS.read(); // read data from the GPS in the 'main loop'
    if (GPSECHO) // if you want to debug, this is a good time to do it!
      if (c) Serial.print(c);
  }
  
  if (GPS.newNMEAreceived()) {
    char *stringptr = GPS.lastNMEA(); // Don't call lastNMEA more than once between parse calls!  
    
    if (!GPS.parse(stringptr))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another

    Serial.println("OK"); // Sentence parsed! 

    logfile.println("----------OK------------");
    uint8_t stringsize = strlen(stringptr);
      if (stringsize != logfile.write((uint8_t *)stringptr, stringsize))    //write the string to the SD file
          error(4);
      if (strstr(stringptr, "RMC") || strstr(stringptr, "GGA"))   logfile.flush();
      Serial.println();
  }

  if (timer > millis()){
//    Serial.println("oooooooops");
    timer = millis();
  }
//  Serial.print("timer: ");Serial.println(timer);
//  Serial.print("millis: ");Serial.println(millis());
  if (millis() - timer > 1000) { 
    Serial.println("asdf1");
    timer = millis();
    if (!GPS.fix) {
      Serial.println("asdf2");
    } else {
      Serial.println("Log"); // Rad. lets log it!
      //Time Date
      logfile.print(GPS.hour, DEC); logfile.print(':');
      logfile.print(GPS.minute, DEC); logfile.print(':');
      logfile.print(GPS.seconds, DEC); logfile.print('.');
      logfile.print(GPS.milliseconds);
      logfile.print(GPS.day, DEC); logfile.print('/');
      logfile.print(GPS.month, DEC); logfile.print("/20");
      logfile.print(GPS.year, DEC); logfile.print(","); 
      //Lat
      logfile.print(GPS.latitudeDegrees, 4); logfile.print(","); 
      //Lng
      logfile.print(GPS.longitudeDegrees, 4);  logfile.print(","); 
      //Speed (knots)
      logfile.print(GPS.speed); logfile.print(","); 
      //Altitude
      logfile.print(GPS.altitude); logfile.print(","); 
      //Warning
      boolean warning = true;
      logfile.print(warning); logfile.print(",");
      //NightLight
      boolean nightlight = false;
      logfile.print(nightlight); logfile.print(",");
      logfile.println();      
      Serial.println();
   }
  }
  
  /* HARDWARE */
  /*total = 0;
  //take average of 5 measurements
  for (int i = 0; i < 5; i = i + 1){ 
    digitalWrite(trigPin, LOW); // Clears the trigPin 
    delayMicroseconds(2);
    
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
  
    duration = pulseIn(echoPin, HIGH); // Reads the echoPin, returns the sound wave travel time in microseconds
     
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
  }*/
}

