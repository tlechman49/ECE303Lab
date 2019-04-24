int fast_PWM_pin = 11;
int PC_PWM_pin = 5;
int PFC_PWM_pin = 6;

void setup() {
  noInterrupts();

  // Fast PWM - Using Pin 11
  TCCR1A=0;
  TCCR1B=0;
  TCCR1A=0b10000010; // Initialize Timer/Counter Control Register 1A
  TCCR1B=0b00011010; // Initialize Timer/Counter Control Register 1B
  OCR1A=600;
  TCNT1=0; // Reset counter 1
  ICR1 =2000; // TOP 1

  // Phase Corrected PWM - Using Pin 5
  TCCR3A=0;
  TCCR3B=0;
  TCCR3A=0b10000010; // Initialize Timer/Counter Control Register 3A
  TCCR3B=0b00010010; // Initialize Timer/Counter Control Register 3B
  ICR3=2000; // TOP 3
  OCR3A=600;
  TCNT3=0; // Reset counter 3

  // Phase and Frequency Corrected PWM - Using Pin 6
  TCCR4A=0;
  TCCR4B=0;
  TCCR4A=0b10000000; // Initialize Timer/Counter Control Register 4A
  TCCR4B=0b00010010; // Initialize Timer/Counter Control Register 4B
  ICR4=2000; // TOP 4
  OCR4A=600;
  TCNT4=0; // Reset counter 4

  pinMode(fast_PWM_pin,OUTPUT);
  pinMode(PC_PWM_pin,OUTPUT);
  pinMode(PFC_PWM_pin,OUTPUT);

  interrupts();

}

void loop() {
  
}



ISR(TIMER3_COMPC_vect){
  
}

