
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
const int ledPin = 6;
int trigDist = 90;


void setup() {
  // put your setup code here, to run once:
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
pinMode(motorPin, OUTPUT);
pinMode(ledPin, OUTPUT); //Sets LED as Output

Serial.begin(9600); // Starts the serial communication
}

void loop() {
 total = 0;
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
  toAvg[i] = distance;
  total = total + distance;
  // Prints the distance on the Serial Monitor
  //Serial.print("Distance (cm): ");
  //Serial.println(distance);
  delay(30);
}
AvgDist = total/5;
alert = alertFunction(toAvg);


// print distance 
/*
Serial.println(toAvg[0]);
Serial.println(toAvg[4]);
Serial.print("Avg Dist: ");
Serial.println(AvgDist);
Serial.print("Alert: ");
Serial.println(alert);
*/
Serial.println(AvgDist);
warning = AvgDist < trigDist && alert == 1;
Serial.println(warning);


if (AvgDist < trigDist && alert == 1){
  
    analogWrite(ledPin, 255);
    digitalWrite(motorPin, HIGH);
    delay(500);
    analogWrite(ledPin, 0);
    digitalWrite(motorPin, LOW);
    delay(50);

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






