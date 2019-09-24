#include "DHT.h"

// pin definitions
#define liquidSensor A3
#define DHTPIN 8     // Digital pin connected to the DHT sensor
#define BLUPIN 3
#define REDPIN 4
#define WHTPIN 5
#define BUZPIN 6
#define CNTPIN 2  //counts pulses from motor
#define FANPIN 52

#define DHTTYPE DHT11   // DHT 11

#define MAX_9V_TEMP 25 //celcius
#define MIN_WATER_LEVEL 100 
#define MAXCOUNTS 160

DHT dht(DHTPIN, DHTTYPE);

int liquidLevel = 1023;
float dhtTempC = 0;
bool liquidAlarm = false;
bool tempAlarm = false;

volatile int count = 0;
volatile unsigned int countsPerSecond = 0;
volatile int updateCtrl = 0;

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

char parsedData[8];

void setup() {

  Serial.begin(9600);
  Serial3.begin(9600);
  pinMode(liquidSensor, INPUT);
  pinMode(BLUPIN, OUTPUT);
  pinMode(REDPIN, OUTPUT);
  pinMode(WHTPIN, OUTPUT);
  pinMode(DHTPIN, INPUT);
  pinMode(BUZPIN, OUTPUT);
  pinMode(FANPIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(CNTPIN), incrementCount, RISING);

  dht.begin();

  noInterrupts();
  TCCR3A=0; // Initializes all bits to zero in TCCR3A
  TCCR3B=0; // Initializes all bits to zero in TCCR3B
  TCNT3=0; // Initializes counter TCNT3 to 0
  OCR3C=62499; // Set Output Compare Register 3C value (max is 62500)
  TCCR3B |= (1<<CS32) ; //Set prescalar to 256
  TIMSK3 |=(1<<OCIE3C); // Enable timer 3 output compare flag

  // put your setup code here, to run once:
  TCCR2B = (TCCR2B & 0xF8) | 0x03;
  interrupts();

  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
}

void loop() {

  processSensors();

  digitalWrite(BLUPIN, liquidAlarm);
  digitalWrite(REDPIN, tempAlarm);
  digitalWrite(WHTPIN, !(liquidAlarm || tempAlarm));

  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.println(inputString);
    Serial.println(countsPerSecond);
    Serial.println(liquidLevel);
    Serial.print(dhtTempC);
    Serial.println("°C");
    Serial.println();
    Serial.println();

    // clear the string:
    inputString = "";
    stringComplete = false;
  }

   if (updateCtrl == 1)
   {
     updateCtrl = 0;
     if (countsPerSecond > MAXCOUNTS/2)
     {
       digitalWrite(FANPIN, HIGH);
     }
     else
     {
       digitalWrite(FANPIN, LOW);
     }
     sprintf(parsedData,"%d.%d.%d",countsPerSecond, int(liquidAlarm), int(tempAlarm));
     Serial3.println(parsedData);
     Serial.println(parsedData);
   }
}

void incrementCount() {
  count += 1;
}

ISR(TIMER3_COMPC_vect){
  countsPerSecond = count;
  count = 0;
  updateCtrl = 1;
}

void processSensors()
{
    if (dhtTempC < MAX_9V_TEMP)
  {
    dhtTempC = dht.readTemperature();
    if (dhtTempC > MAX_9V_TEMP)
    {
      tempAlarm = true;
      tone(BUZPIN, 500);
      delay(500);
      noTone(BUZPIN);
    }
  }
  else
  {
    dhtTempC = dht.readTemperature();
    if (dhtTempC < MAX_9V_TEMP)
    {
      tempAlarm = false;
    }
  }

  if (liquidLevel > MIN_WATER_LEVEL)
  {
    delay(1);
    liquidLevel = analogRead(liquidSensor);
    if (liquidLevel < MIN_WATER_LEVEL)
    {
      liquidAlarm = false;
    }
  }
  else
  {
    delay(1);
    liquidLevel = analogRead(liquidSensor);
    if (liquidLevel > MIN_WATER_LEVEL)
    {
      tone(BUZPIN, 500);
      delay(500);
      noTone(BUZPIN);
      liquidAlarm = true;
    }
  }
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
