#define RPM_ADJ_IN_PIN 9
#define RELAY_PIN 52

int rpmAdjVal=0;
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

String inputString3 = "";         // a String to hold incoming data
bool stringComplete3 = false;  // whether the string is complete

int pwmSet = 0;
int rpmTarget = 0;

void setup() {  
  
  pinMode(RPM_ADJ_IN_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  Serial.begin(9600);
  Serial3.begin(9600);
  Serial.println("Hello World");
  
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  inputString3.reserve(200);
}

void loop() {
  unsigned int countsPerSecond;
  int liquidAlarm;
  int tempAlarm;

  if(stringComplete)
  {
    Serial.println(inputString);

    rpmAdjVal = inputString.toInt();
    if (rpmAdjVal > 300 || rpmAdjVal < 0)
    {
      Serial.println("Value is incorrect. Must be between 0 and 300.");
    }
    else
    {
      rpmTarget = rpmAdjVal;
      pwmSet = (float)rpmTarget*255/300;
    }
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  
  if (stringComplete3) {
    Serial.println(inputString3);
    
    char char_array[12];
    strcpy(char_array, inputString3.c_str());
    int ret = sscanf(char_array, "%d.%d.%d", &countsPerSecond, &liquidAlarm, &tempAlarm); 
    Serial.println(ret);
    Serial.println(countsPerSecond);
    Serial.println(liquidAlarm);
    Serial.print("pwmSet: ");
    Serial.println(pwmSet);
    
    // clear the string:
    inputString3 = "";
    stringComplete3 = false;
  }
  if ((liquidAlarm || tempAlarm) == 1)
  {
    analogWrite(RPM_ADJ_IN_PIN,0); 
    digitalWrite(RELAY_PIN, LOW);
  }
  else
  {
    int rpmVal = countsPerSecond * 60 / 32;
    if (rpmVal-480/32 > rpmTarget && pwmSet > 0)
    {
      pwmSet--;
    }
    else if (rpmVal+480/32 < rpmTarget && pwmSet < 255)
    {
      pwmSet++;
    }
    analogWrite(RPM_ADJ_IN_PIN,pwmSet);
    digitalWrite(RELAY_PIN, HIGH);
  }
  delay(100);
}

void serialEvent3() {
  while (Serial3.available()) {
    // get the new byte:
    char inChar = (char)Serial3.read();
    // add it to the inputString:
    inputString3 += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete3 = true;
    }
  }
}

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
