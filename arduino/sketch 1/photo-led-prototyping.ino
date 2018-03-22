/* With the help of this sketch, 
 * I made a green, a yellow and a red led light light up
 * according to the amount of light detected by a
 * photoresistor.
 */

 //defining the arduino pins for the led lights and the photoresistor
int greenLedPin = 2;
int yellowLedPin = 3;
int redLedPin = 4;

int lightSensorPin = A0;
int analogValue = 0;

void setup() {
  //setting up the led lights as outputs 
  pinMode(greenLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  //beginning serial communication
  Serial.begin(9600);
  
}

void loop() {
  //serial printing the value of the photoresistor
  Serial.println(analogRead(lightSensorPin));
  analogValue = analogRead(lightSensorPin);
  // turning on the red led if the light value is more than 120
  if (analogValue > 120) {
    digitalWrite(redLedPin, HIGH);
    delay(10);
    digitalWrite(redLedPin, LOW);
  }
  // turning on the yellow led if the light value is between 80 and 120
  else if(analogValue >= 80 && analogValue <= 120) {
    digitalWrite(yellowLedPin, HIGH);
    delay(10);
    digitalWrite(yellowLedPin, LOW);
  } 
  // turning on the green led if the light value is between 50 and 80
  else if(analogValue > 50 && analogValue <= 80){
    digitalWrite(greenLedPin, HIGH);
    delay(10);
    digitalWrite(greenLedPin, LOW);
  }
  //otherwise, the leds are turned off
  else {
    digitalWrite(greenLedPin && yellowLedPin && redLedPin, LOW);
  }
 
  delay(10);
}
