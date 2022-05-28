#include <avr/io.h>
#include <util/delay.h>
// HW Timer2 CTC mode
#define T2TOP 250
// Count of SW timers
#define TIME1 125
#define TIME2 63
// LEDs that will be used
#define LED1 PB5
#define LED2 PB2
// Global variables…
// variables for SW timers (tick counters)
uint8_t time1 = TIME1;
uint8_t time2 = TIME2;
// Functions needed…
/////////////////////////
// program timer2 in CTC mode
// generate interrupt every 4ms
void tc2_init(void) {
TCCR2B = 0; // stop TC2
TIFR2 |= (7<<TOV2); // clear pending interrupt
TCCR2A = 2; // mode CTC
TCNT2 = 0; // BOTTOM value
OCR2A = T2TOP; // TOP value
TIMSK2 |= (1<<OCIE2A); // enable COMPA interrupt
TCCR2B = 6; // start TC2 (TP=256)
}
////////////////
// task_LED1
// toggles LED1 every TIME1 ticks (SW timer 1)
void task_LED1(void) {
if(time1)
time1--; // if time1 not 0, decrement
else {
// do here the task code, to run every TIME1 ticks
PORTB ^= (1<<LED1); // toggles LED1 in PORTB
time1 = TIME1; // reset to SW timer 1
}
}

////////////////
// task_LED2
// toggles LED2 every TIME2 ticks (SW timer 2)
void task_LED2(void) {
if(time2)
time2--; // if time2 not 0, decrement
else {
// do here the task code, to run every TIME2 ticks
PORTB ^= (1<<LED2); // toggles LED2 in PORTB
time2 = TIME2; // reset to SW timer 2
}
}
////////////////////////////
// Timer 2 ISR for CTC mode
// every interrupt invoke existing tasks
ISR(TIMER2_COMPA_vect) {
task_LED1();
task_LED2();
}
////////////////////
// main
int main(void) {
// initialization
DDRB |= (1<<LED1) | (1<<LED2); // LED1, LED2 pins -> outputs
tc2_init(); // initialize timer2
sei(); // enable interrupts
// enter endless loop (processing done in the tasks)
// this loop can be used to run code, too, but remember
// it is recurrently interrupted by the tasks
while(1) { };
}

