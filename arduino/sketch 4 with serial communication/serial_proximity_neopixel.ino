// Serial I/O based on example http://forum.arduino.cc/index.php?topic=396450


// Serial communication
const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];
char messageFromPC[numChars] = {0};
int integerFromPC = 0;
float floatFromPC = 0.0;
boolean newData = false;
//boolean newData1 = false; //test
int Color = false;

long lastFakeReport = 0;

enum {
  MsgAcknowledge, // 0
  MsgError,       // 1
  MsgMove,        // 2
  MsgMoveResult,  // 3
  MsgPosition,    // 4
};

//Tell the Arduino IDE to include the FastLED library
 #include <FastLED.h>
 
 //Setup the variables for the HC-SR04
const int trigPin = 9;
const int echoPin = 10;
int i;
const int sensorPin = A0;
int value;
//int fsrADC = analogRead(FSR_PIN);
// Measure the voltage at 5V and resistance of your 3.3k resistor, and enter
// their value's below:
const float VCC = 4.98; // Measured voltage of Ardunio 5V line
const float R_DIV = 3230.0; // Measured resistance of 3.3k resistor

//Setup the variables for the NeoPixel Strip
#define NUM_LEDS 12 // How many leds in your strip?
#define DATA_PIN 6 // What pin is the NeoPixel's data line connected to?
CRGB leds[NUM_LEDS]; // Define the array of leds

 void setup() {
  // initialize serial communication:
  //Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  pinMode(sensorPin, INPUT);

  Serial.begin(115200);
  //Serial1.begin(115200);//test
  
 report(MsgAcknowledge, "Ready");
}

void loop(){
  value = analogRead(sensorPin);
  Serial.println(value);//
  value = map(value, 0, 1023, 0, 255);
  //digitalWrite(sensorPin, value);
  
  delay(100);

  if (value >= 23) {
    //newData = true;
    Serial.println("2");
    fill_solid( &(leds[0]), NUM_LEDS, CRGB::White);
    FastLED.show();
    delay(4000);
    //leds[i].fadeToBlackBy( 64 );
    //leds[i].fadeLightBy( 100 );
    //leds[i].subtractFromRGB(20);
    //FastLED.show();
    //delay(2000);
    //leds[i] = CHSV( 160, 255, 100);
    //FastLED.show();
  }
    // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:
  long duration, inches, cm;
 
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
 
  // convert the time into a distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
  
  Serial.println(inches);
  //Serial.print("in, ");
  //Serial.print(cm);
  //Serial.print("cm");
  //Serial.println();
  
   if /*((Color == true ) &&*/ (inches <= 20)  {
    //newData = true;
    Serial.println("0");
    fill_solid( &(leds[0]), NUM_LEDS /*number of leds*/, CRGB::Blue); //{whitestrobe(30);
     //FastLED.setBrightness(200);
     //leds[12].maximizeBrightness();
     leds[i].maximizeBrightness();
     //leds[i].fadeLightBy( 64 );
     FastLED.show();
     
     delay(2000);
   
}
  else if (inches >= 21)  {
    //newData = true;
    //Color = true;
    Serial.println("1");
    fill_solid( &(leds[0]), NUM_LEDS /*number of leds*/, CRGB::OrangeRed);
     //FastLED.setBrightness(15);
     //leds[i].maximizeBrightness();
     leds[12].fadeLightBy( 192 );
     FastLED.show();
     delay(10);
  } 
  //else {
    //Color = false;
  //}
  
  delay(100);
  

  //else {
    //Serial.print(inches);
    //Serial.print(cm);
  //}

  // Process serial communucation
  recvWithStartEndMarkers();

  // If we received a command, process it
  if (newData == true) {
    strcpy(tempChars, receivedChars);
    parseData();
    switch (integerFromPC) {
      case MsgPosition:
        // Report fake result
        report(MsgPosition, 10);
        break;
      case MsgMove:
        // Pretend to do something
        report(MsgMoveResult, 10);
        break;
      }
      
      // Debug: print parsed command to serial
      // showParsedData();
      newData = false;
    }
    //TEST
    /*if (newData1 == true) {
    strcpy(tempChars, receivedChars);
    parseData();
    switch (integerFromPC) {
      case MsgPosition:
        // Report fake result
        report(MsgPosition, 10);
        break;
      case MsgMove:
        // Pretend to do something
        report(MsgMoveResult, 10);
        break;
      }
      
      // Debug: print parsed command to serial
      // showParsedData();
      newData1 = false;
    }*/

  // Every so often report a fake position
  /*if (millis() - lastFakeReport > 2000) {
    report(MsgPosition, (random(0,100)));
    lastFakeReport = millis();    
  }*/
  
 }
 

 // ---- Serial communication
void report(int code, const char *message) {
  Serial.print("<");
  Serial.print("ws-bridge,");
  Serial.write(code);
  Serial.write(",");
  Serial.write(message);
  Serial.print(">\r\n");
  Serial.flush();
}

void report(int code, int message) {
  Serial.print("<");
  Serial.print("ws-bridge,");
  Serial.print(code);
  Serial.print(",");
  Serial.print(message);
  Serial.print(">\r\n");
  Serial.flush();
}

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
   
}


void parseData() {      // split the data into its parts
    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    strcpy(messageFromPC, strtokIndx); // copy it to messageFromPC
 
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    integerFromPC = atoi(strtokIndx);     // convert this part to an integer

    strtokIndx = strtok(NULL, ",");
    floatFromPC = atof(strtokIndx);     // convert this part to a float
}

void showParsedData() {
    Serial.print("Message ");
    Serial.println(messageFromPC);
    Serial.print("Integer ");
    Serial.println(integerFromPC);
    Serial.print("Float ");
    Serial.println(floatFromPC);
    //TEST
    /*Serial1.print("Message ");
    Serial1.println(messageFromPC);
    Serial1.print("Integer ");
    Serial1.println(integerFromPC);
    Serial1.print("Float ");
    Serial1.println(floatFromPC);*/
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


