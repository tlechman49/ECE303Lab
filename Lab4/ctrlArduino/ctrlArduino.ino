#define RPM_ADJ_OUT_PIN A0
#define RPM_ADJ_IN_PIN 9

int rpmAdjVal=0;
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void setup() {  

  Serial.begin(9600);
  Serial3.begin(9600);
  Serial.println("Hello World");
  
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
}

void loop() {
  rpmAdjVal=analogRead(RPM_ADJ_OUT_PIN);
  analogWrite(RPM_ADJ_IN_PIN,rpmAdjVal/4);
  unsigned int countsPerSecond;
  bool liquidAlarm;
  bool tempAlarm;

  if (stringComplete) {
    //Serial.println(inputString);
    
    char char_array[12];
    strcpy(char_array, inputString.c_str());
    int ret = sscanf(char_array, "%d.%d.%d", &countsPerSecond, &liquidAlarm, &tempAlarm); 
    Serial.println(ret);
    Serial.println(countsPerSecond);
    
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  
}

void serialEvent3() {
  while (Serial3.available()) {
    // get the new byte:
    char inChar = (char)Serial3.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
