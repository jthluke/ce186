// prints proximity (int), warning (boolean), and light level (kOhms)

#include <Adafruit_NeoPixel.h>
#define PIN1 12
#define PIN2 13
//avoid 10, 8, 7 bc they're used by the gps
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(16, PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(16, PIN2, NEO_GRB + NEO_KHZ800);

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
//const int ledPin = 6;
int trigDist = 90; 

//Photoresistor resistor
int R2 = 10000;

// Global RGB values, change to suit your needs
int r = 255;
int g = 0;
int b = 255;

void setup() {
  // put your setup code here, to run once:
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
pinMode(motorPin, OUTPUT);
//pinMode(ledPin, OUTPUT); //Sets LED as Output

Serial.begin(9600); // Starts the serial communication
//sets up neopixel lights
strip1.begin();
strip1.setBrightness(100);
strip1.show();

//sets up neopixel lights
strip2.begin();
strip2.setBrightness(100);
strip2.show();

}

void loop() {
 total = 0;
//take average of 5 measurements
for (int i = 0; i < 5; i = i + 1){
  // put your main code here, to run repeatedly:
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

//Print average of 3 values
//Serial.print("Avg Dist: ");
Serial.println(AvgDist);
//Serial.print("Alert: ");
//Serial.println(alert);
boolean warning = AvgDist < trigDist && alert == 1;
Serial.println(warning);

//turns on light and vibration motor when object is near and approaching
if (AvgDist < trigDist && alert == 1){
  
    blinky(5);
    digitalWrite(motorPin, HIGH);
    delay(500);
    digitalWrite(motorPin, LOW);
    delay(50);
  }

//Photoresistor
// read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // print out the value you read:
  int mapVolts = map(sensorValue, 0, 1023, 0, 5);
  int mapmV = map(sensorValue, 0, 1023, 0, 5000);

  int R1 =  ((((5*1000)/mapmV)-1)*R2)/1000;
  Serial.println(R1); // in kilo-Ohms (indicates brightness)

 //turn on both night lights if dark (higher R1 values means lower brightness)
 if (R1 > 700) {
  activate();
 } else {
  allOff();
 }
}
boolean alertFunction(int dist_array) {
  if (toAvg[3] < toAvg[1] | toAvg[4] < toAvg[2]){
  alert = 1;
} else {
  alert = 0;
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
void activate() {
  for( int i = 0; i < 16; i++ ) {
       strip1.setPixelColor(i, r,g,b );
       strip2.setPixelColor(i, r,g,b );
  strip1.show();
  strip2.show();
}
}

void blinky(int repeats) {
  for (int i = 0; i < repeats; i++)
  {
    activate();
    delay(100);
    allOff();
    delay(40);
  }
}




