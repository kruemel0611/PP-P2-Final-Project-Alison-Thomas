/* For this sketch, I experimented with gradual darkening and brightening the 
 *  colours of the neopixel depending on the distance to the device. 
*/

#include <Adafruit_NeoPixel.h>

#define PIN 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
// defining that the neopixel strip has 12 pixels, the input pin on the arduino is digital input pin 6,
//the pixels are wired for GRB bitstream which most neopixel products use and are using an 800 KHZ
//bitstream.
Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, 6, NEO_GRB + NEO_KHZ800);
//defining the pins for the proximity sensor HC-SR04
const int trigPin = 9;
const int echoPin = 10;
//setting variables for the duration and the distance
long duration;
int distance;

void setup() {
  //defining the echo pin as input and the trigger pin of the proximity sensor as output
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  //start up the neopixels
  strip.begin();
  strip.show(); // initialize all pixels to "off"
}

void loop() {
  //setting a clean slate for the HC-SR04
  digitalWrite(trigPin, LOW);
  delayMicroseconds(10);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  //the duration is defined by the incoming pulses of the echo pin 
  duration = pulseIn(echoPin, HIGH);
  // the distance is defined as the duration times the speed of sound
  distance = duration*0.034/2;
 
// if the distance is higher than 30, brighten the colours on the neopixel
  if (distance > 30) {
    brighten();
  } //else if the distance is under or equals 30, darken the colours of the neopixel.
  else if (distance <= 30) {
  darken();
}
  
}
//defining the intervals for brightening the neopixel
void brighten() {
  uint16_t i, j;
  for (j = 45; j < 255; j++) {
  for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, j, j, j);
    }
    strip.show();
    delay(10);
  }
}
//defining the intervals for darkening the neopixel
void darken() {
  Serial.begin(9600);
  uint16_t i, j; 
  for (j = 255; j > 45; j--) {
  for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, j, j, j);
    }
    strip.show();
    delay(10);
    Serial.println(j);
  }
 
}







