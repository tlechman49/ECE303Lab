const byte interruptPin = 2;
const byte automatePin = 8;
volatile int count = 0;
volatile bool printCount = 0;
volatile bool automate = 0;
volatile int countsPerSecond = 0;

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void setup() {
  pinMode(interruptPin, INPUT_PULLUP);
  pinMode(automatePin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), incrementCount, RISING);

  noInterrupts();
  TCCR3A=0; // Initializes all bits to zero in TCCR3A
  TCCR3B=0; // Initializes all bits to zero in TCCR3B
  TCNT3=0; // Initializes counter TCNT3 to 0
  OCR3C=62499; // Set Output Compare Register 3C value (max is 62500)
  TCCR3B |= (1<<CS32) ; //Set prescalar to 256
  TIMSK3 |=(1<<OCIE3C); // Enable timer 3 output compare flag
  interrupts();

  Serial.begin(9600);
  Serial.println("Hello World");
  
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
}

void loop() {
  automate = digitalRead(automatePin);
  if (printCount) {
    printCount = 0;
    Serial.println(countsPerSecond);
  }

  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.println(inputString);
    Serial.println();
    Serial.println(countsPerSecond);
    
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

void incrementCount() {
  count += 1;
}

ISR(TIMER3_COMPC_vect){
  countsPerSecond = count;
  count = 0;
  if (!automate)
  {
    printCount = !printCount;
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
