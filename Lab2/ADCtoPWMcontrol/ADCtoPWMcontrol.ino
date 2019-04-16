int val=0;
int pin_in=A0;
int pin_out=9;
void setup() {
  // put your setup code here, to run once:
  TCCR2B = (TCCR2B & 0xF8) | 0x03;

  Serial.begin(9600);
  Serial.println("Hello World");
}
void loop() {
  // put your main code here, to run repeatedly:
  val=analogRead(pin_in);
  Serial.println(val);
  analogWrite(pin_out,val/4);
}
