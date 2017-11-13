
// defines pins numbers
const int trigPin = 9;
const int echoPin = 11;
// defines variables
long duration;
int distance;
const int motorPin = 3;
int trigDist = 100; //distance at which we turn on the vibration motor 


void setup() {
  // put your setup code here, to run once:
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
pinMode(motorPin, OUTPUT);
Serial.begin(9600); // Starts the serial communication
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


// Calculating the distance in cm
distance= duration*0.034/2;
// Prints the distance on the Serial Monitor
Serial.print("Distance (cm): ");
Serial.println(distance);

//vibrate motor if distance is close 
if (distance < trigDist) {
  digitalWrite(motorPin, HIGH);
  delay(500);
  digitalWrite(motorPin, LOW);
  delay(50);
}
delay(500);
}




