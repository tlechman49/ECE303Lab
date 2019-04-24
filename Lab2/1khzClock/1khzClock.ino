int clk_pin = 2;

void setup() {
  pinMode(2,OUTPUT);
  digitalWrite(clk_pin, LOW);
  noInterrupts();
  TCCR3A=0; // Initializes all bits to zero in TCCR3A
  TCCR3B=0; // Initializes all bits to zero in TCCR3B
  TCNT3=0; // Initializes counter TCNT3 to 0
  OCR3C=1999; // Set Output Compare Register 3C value 16mhz/(8*1000) - 1 = 1999
  TCCR3B |= (1<<CS31) ; //Set prescalar to 8
  TIMSK3 |=(1<<OCIE3C); // Enable timer 3 output compare flag
  interrupts();
}

void loop() {
  
}

ISR(TIMER3_COMPC_vect){
  digitalWrite(clk_pin, !digitalRead(clk_pin));
}

