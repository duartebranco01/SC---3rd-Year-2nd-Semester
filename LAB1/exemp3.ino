const int ledPin=3;
const int ledBut=15;

bool buzz_isactive=true;
volatile byte state = 1;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledBut, INPUT);
  pinMode(ledPin, OUTPUT);

  noInterrupts();
  TCCR1A=0;
  TCCR1B=0;

  TCNT1=34286
  TCCR1B|=(1<<CS12);
  TIMSK1|=(1<<TOIE1);
  interrupts();
}

void isr(){
    state=!state;
}

void loop() {
  // put your main code here, to run repeatedly:

  attachInterrupt(digitalPinToInterrupt(S1), isr, LOW);

}
