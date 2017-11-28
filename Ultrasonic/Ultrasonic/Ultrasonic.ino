
// defines pins numbers
const int trigPin = 9;
const int echoPin = 11;
// defines variables
long duration;
int distance;
int AvgDist;
int toAvg = 0;

void setup() {
  // put your setup code here, to run once:
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input

Serial.begin(9600); // Starts the serial communication
}

void loop() {
 toAvg = 0;
//take average of 5 measurements
for (int i = 0; i < 5; i = i + 1){
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
if (distance >400){
  distance = 0;
}
toAvg = toAvg + distance;
// Prints the distance on the Serial Monitor
//Serial.print("Distance (cm): ");
//Serial.println(distance);
delay(100);
}
AvgDist = toAvg/5;
Serial.print("Avg Dist: ");
Serial.println(AvgDist);

}

