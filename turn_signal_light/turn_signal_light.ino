#include <Adafruit_NeoPixel.h>
#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leftstrip = Adafruit_NeoPixel(16, 11, NEO_GRB + NEO_KHZ800);
int potPin = 2;
int val = 0;
int colorVal = 0;
int reading = 0;
int x;
int prevVal = 0;
int switchPin = 2;
int leftPin = 9;
int buttonState = 0; 
int leftState = 0; 
int lastButton = 0;
boolean NeopixelColor = false;



// Global RGB values, change to suit your needs
int r = 255;
int g = 0;
int b = 255;

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(100);
  strip.show();
  leftstrip.begin();
  leftstrip.setBrightness(100);
  leftstrip.show();
  pinMode(switchPin, INPUT);
  pinMode(leftPin, INPUT);
}

void loop() {
  buttonState = digitalRead(switchPin);
  leftState = digitalRead(leftPin);
  if (buttonState == 1) {
    delay(200); // Account for contact debounce
    blinky(10);
  } 
  if (leftState == 1) {
    delay(200);
    leftblinky(10);
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

void lallOff() {
  leftstrip.clear();
  leftstrip.show();
}

// Turns the NeoPixels on, according to RGB settings
void lactivate() {
  for( int i = 0; i < 16; i++ ) 
       leftstrip.setPixelColor(i, r,g,b );
      
  leftstrip.show();
}

void leftblinky(int repeats) {
  for (int i = 0; i < repeats; i++)
  {
    lactivate();
    delay(500);
    lallOff();
    delay(300);
  }
}
