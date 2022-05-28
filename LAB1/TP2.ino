#include <Arduino_FreeRTOS.h>
#include <MultiFunctionShield.h>

//#define INCLUDE_vTaskSuspend 1

TaskHandle_t print_handler;



void TaskBlink1(void *pvParameters){

  pinMode(LED_BUILTIN, OUTPUT);

  while(1)

  {

    digitalWrite(LED_BUILTIN, HIGH);

    vTaskDelay(500/portTICK_PERIOD_MS);    

    digitalWrite(LED_BUILTIN, LOW);

    vTaskDelay(500/portTICK_PERIOD_MS);

  } 

}

void TaskPrint(void *pvParameters){

  int counter = 0;

  while(1)

  {

    counter++;

    Serial.println(counter);

    vTaskDelay(1000/portTICK_PERIOD_MS);

  }

}



void TaskDisplay(void *pvParameters){

  MultiFunctionShield MFS;
  MFS.begin();

  pinMode(BUTTON_1_PIN, INPUT);



  long counter=0;

  while(1)

  {

    //Serial.println(digitalRead(BUTTON_1_PIN));

    if(!digitalRead(BUTTON_1_PIN))

    {

      counter++;

      MFS.Display(counter);  

    }

    vTaskDelay(1000/portTICK_PERIOD_MS);

  }

}

void TaskBuzz(void *pvParameters){

  MultiFunctionShield MFS;
  MFS.begin();

  pinMode(BUTTON_3_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  

  while(1){

    if(!digitalRead(BUTTON_3_PIN)){

      vTaskSuspend(print_handler);

      digitalWrite(BUZZER_PIN, LOW);

      Serial.println("Emergency");

    }

    else{

      vTaskResume(print_handler);

      digitalWrite(BUZZER_PIN, HIGH);

      }

  }
}

void setup() {

  // put your setup code here, to run once:

  Serial.begin(9600);



  xTaskCreate(

    TaskBlink1,   //name of fuction to be called

    "Task1",      //name of task

    128,          //stack size em words(4bytes, 16bits)

    NULL,         //parameter to funtion, null pk void

    1,            //task priority

    NULL);        //task handle 

    

  xTaskCreate(TaskPrint,"Task2", 128, NULL, 1, &print_handler);

  xTaskCreate(TaskDisplay,"Task3", 128, NULL, 1, NULL);

  xTaskCreate(TaskBuzz,"Task4", 128, NULL, 1, NULL);

  vTaskStartScheduler();

  

}



void loop() {

  // put your main code here, to run repeatedly:



}