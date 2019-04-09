int val=0;
int pin_in=A0;
int pin_out=9;
void setup() {
  // put your setup code here, to run once:
  TCCR2B = (TCCR2B & 0xF8) | 0x03;
}
void loop() {
  // put your main code here, to run repeatedly:
  val=analogRead(pin_in);
  analogWrite(pin_out,val);
}
