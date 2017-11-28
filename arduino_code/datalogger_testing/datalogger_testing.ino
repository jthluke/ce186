#include <SPI.h>
#include <SD.h>

File myFile;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("Initializing SD card...");

  //debug start
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  //debug end

  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println(" initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  String filename = "test.csv";
  myFile = SD.open("test.csv", FILE_WRITE);
  unsigned long timestamp = 1510613377;
  int lat = 111;
  int lng = 222;
  int vel = 333;
  int ultrasonic = 444;
  int photoresistor = 555;

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to "); Serial.print(filename); Serial.print("...");
  myFile.print(timestamp); myFile.print(",");
    myFile.print(lat); myFile.print(",");
    myFile.print(lng); myFile.print(",");
    myFile.print(vel); myFile.print(",");
    myFile.print(ultrasonic); myFile.print(",");
    myFile.println(photoresistor);
    // close the file:
    myFile.close();
    Serial.println(" done.");
  } else {
    Serial.println("error opening test.csv");
  }

  // re-open the file for reading:
  myFile = SD.open("test.csv");
  if (myFile) {
    Serial.print("Reading ");Serial.print(filename);Serial.println(":");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening ");Serial.print(filename);
  }
}

void loop() {
  // nothing happens after setup
}
