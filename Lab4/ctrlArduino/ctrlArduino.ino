#define RPM_ADJ_OUT_PIN A0
#define RPM_ADJ_IN_PIN 9

int rpmAdjVal=0;

void setup() {  

  Serial.begin(9600);
  Serial.println("Hello World");
}

void loop() {

  rpmAdjVal=analogRead(RPM_ADJ_OUT_PIN);
  analogWrite(RPM_ADJ_IN_PIN,rpmAdjVal/4);
}
