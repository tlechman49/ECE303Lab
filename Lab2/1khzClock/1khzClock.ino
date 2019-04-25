int PC_PWM_pin = 5;

void setup() {
  noInterrupts();

  // Phase Corrected PWM - Using Pin 5
  TCCR3A=0;
  TCCR3B=0;
  TCCR3A=0b10000010; // Initialize Timer/Counter Control Register 3A
  TCCR3B=0b00010010; // Initialize Timer/Counter Control Register 3B
  ICR3=1000; // TOP 3
  OCR3A=500;
  TCNT3=0; // Reset counter 3

  pinMode(PC_PWM_pin,OUTPUT);
  interrupts();

}

void loop() {
  
}



ISR(TIMER3_COMPC_vect){
  
}

