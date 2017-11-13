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



// Global RGB values, change to suit your needs
int r = 255;
int g = 0;
int b = 255;

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(50);
  strip.show();
  pinMode(switchPin, INPUT);

}

void loop() {
  buttonState = digitalRead(switchPin);
  if (buttonState == 1) {
    delay(200); // Account for contact debounce
    blinky(10);
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

