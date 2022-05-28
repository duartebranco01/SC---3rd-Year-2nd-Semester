const int ledPin = LED_BUILTIN; // define which LED to use
const long interval = 200; // define interval to toggle LED
//unsigned long myTime;
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
int ledState = LOW; 
volatile byte stop = LOW;
const int button = 2;

// initialization
void setup() {
  
	pinMode (ledPin, OUTPUT); // set the ledPin as output
  	pinMode (button, INPUT);
	Serial.begin(9600);
 	attachInterrupt(digitalPinToInterrupt(button), isr, FALLING); 

}
// run

void isr(){
  
  	//digitalWrite(ledPin, LOW); //obriga a ficar desligado
    stop=!stop;                //ativa stop, se pressionar button novamente desativa stop
}

void loop() {

  	currentMillis = millis();
  	
  	Serial.print(stop);
  	Serial.println(digitalRead(button));
  
	if (currentMillis - previousMillis >= interval) { //previousMillis corresponde à ultima vez que trocou, ja passou tempo que chege?
    	// save the last time you blinked the LED
      	Serial.println("--------------");
    	previousMillis = currentMillis;

         	// if the LED is off turn it on and vice-versa:
        	if (ledState == LOW && (stop==LOW)) { //so volta a ligar se stop low
            	ledState = HIGH;
          	} else if (stop==LOW){ //com stop==LOW impede troca
            	ledState = LOW;
          	}

    	// set the LED with the ledState of the variable:
    	digitalWrite(ledPin, ledState);
    }
}