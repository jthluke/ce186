#include <Adafruit_NeoPixel.h>
#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN, NEO_GRB + NEO_KHZ800);

int potPin = 2;
int val = 0;
int colorVal = 0;
int reading = 0;
int x;
int prevVal = 0;
int switchPin = 2;
int buttonState = 0; 
int lastButton = 0;
boolean NeopixelColor = false;

// defines pins numbers
const int trigPin = 9;
const int echoPin = 11;
// defines variables
long duration;
int distance;


// Global RGB values, change to suit your needs
int r = 255;
int g = 0;
int b = 255;

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(20);
  strip.show();
  pinMode(switchPin, INPUT);

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}

void loop() {
   // put your main code here, to run repeatedly:
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
  // Prints the distance on the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distance);
  
  if (distance <= 10) {
    delay(200); // Account for contact debounce
    blinky(3);
  } 

}

void allOff() {
  strip.clear();
  strip.show();
}

// Turns the NeoPixels on, according to RGB settings
void activate() {
  for( int i = 0; i < 16; i++ ) 
       strip.setPixelColor(i, r,g,b );
      
  strip.show();
}

void blinky(int repeats) {
  for (int i = 0; i < repeats; i++)
  {
    activate();
    delay(500);
    allOff();
    delay(300);
  }
}

