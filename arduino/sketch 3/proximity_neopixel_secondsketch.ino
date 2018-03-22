

//Tell the Arduino to include the FastLED library
#include <FastLED.h>
 
//Setting up the variables for the HC-SR04 proximity sensor
const int trigPin = 9;
const int echoPin = 10;
int i;
//setting the variables for the force sencitive reistor
const int sensorPin = A0;
int value;

// Measuring the voltages at 5V and resistance of the 3.3k resistor
const float VCC = 4.98; // Measured voltage of Ardunio 5V line
const float R_DIV = 3230.0; // Measured resistance of 3.3k resistor

//Setup the variables for the neopixel ring
#define NUM_LEDS 12 // There are 12 Leds on the neopixel ring
#define DATA_PIN 6 // the neopixel's input line is connected to pin 6
CRGB leds[NUM_LEDS]; // Defining the array of leds
 
void setup() {
  // initialising serial communication:
  Serial.begin(9600);
  //combining the fastled library with the neopixel ring
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  //defining the sensor pin as an input
  pinMode(sensorPin, INPUT);
}
 
void loop(){
  //defining, reading and printing the value of the sensor pin
  value = analogRead(sensorPin);
  Serial.print(value);
  value = map(value, 0, 1023, 0, 255);
  
  delay(100);
  // if the value of the sensor pin is higher than 10 (if the user stepped on the doormat), light up the neopixels in the colour pink
  if (value >= 10) {
    Serial.print ("Welcome Home! Music available.");
    fill_solid( &(leds[0]), NUM_LEDS, CRGB::Pink);
    FastLED.show();
    delay(1000);
    
  }
    
  // establishing variables for duration of the ping, 
  // and the distance result in inches and centimeters:
  long duration;
  long inches; 
  long cm;
 
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  //defining the triggerpin as output
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // Reading the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
 
  // converting the time into a distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
  //serial printing the result in inches and cm
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  
  //if the distances is under or equals 20 inches, light up the neopixels in blue
  //and maximise the brightness
   if (inches <= 20)  {fill_solid( &(leds[0]), NUM_LEDS, CRGB::Blue); 
    
     leds[12].maximizeBrightness();
     FastLED.show();
   
} // if the distance is 21 inches or more, light up the neopixels in orange
//and fade the light to a weaker brightness
  else if (inches >= 21) {fill_solid( &(leds[0]), NUM_LEDS, CRGB::OrangeRed);
 
     leds[12].fadeLightBy( 192 );
     FastLED.show();
     delay(10);
  } 
  
  delay(100);
}
long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}
 
long microsecondsToCentimeters(long microseconds) 
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}






