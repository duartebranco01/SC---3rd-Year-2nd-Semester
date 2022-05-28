#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <MultiFunctionShield.h>

#define WAIT_TIMEOUT 10 

//int var_global = 0;
int freq=69;

MultiFunctionShield MFS;

//SemaphoreHandle_t buffer_mutex;
SemaphoreHandle_t buffer_mutex_freq;  

void setup() {
  // put your setup code here, to run once:
  MFS.begin();
  Serial.begin(9600);
  
  //buffer_mutex = xSemaphoreCreateMutex();
  buffer_mutex_freq = xSemaphoreCreateMutex();

   
  xTaskCreate(
                TaskBlink1,   //name of fuction to be called
                "Task3",      //name of task
                128,          //stack size em words(4bytes, 16bits)
                NULL,         //parameter to funtion, null pk void
                1,            //task priority
                NULL);        //task handle 
  
  //xTaskCreate(TaskInc,"Task1", 128, NULL, 1, NULL);
  //xTaskCreate(TaskInc,"Task2", 128, NULL, 1, NULL);
  xTaskCreate(TaskSerial,"Task4", 128, NULL, 1, NULL);
  xTaskCreate(TaskButtonS2,"Task5", 128, NULL, 1, NULL);
  xTaskCreate(TaskButtonS1,"Task6", 128, NULL, 1, NULL);
  xTaskCreate(TaskBuzz,"Task7", 128, NULL, 1, NULL); 
  
  // Start Scheduler that will manage tasks
  vTaskStartScheduler();
}

/*void TaskInc( void *pvParameters )
{
  int var_local = var_global;

  while (1)
  {
    if( xSemaphoreTake( buffer_mutex, WAIT_TIMEOUT ) == pdTRUE )
    {
      var_local++;
      vTaskDelay( random(80, 200)/ portTICK_PERIOD_MS ); 
      var_global = var_local;
      //Serial.println(var_global);
      xSemaphoreGive( buffer_mutex );
      delay(1);
    }
    else
    {
      //fazer coisas que nao necessitam de recursos partilhados
    }
  }
}*/

void TaskBlink1(void *pvParameters){

  pinMode(LED_BUILTIN, OUTPUT);

  while(1)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    vTaskDelay(freq/portTICK_PERIOD_MS);    
    digitalWrite(LED_BUILTIN, LOW);
    vTaskDelay(freq/portTICK_PERIOD_MS);
  } 
}

void TaskSerial(void *pvParameters){
  
  int freq_in=0;
  
  while(1)
  {
    Serial.println(freq);
    if(Serial.available()>0)
    {
      if( xSemaphoreTake( buffer_mutex_freq, WAIT_TIMEOUT ) == pdTRUE )
      {
        freq_in=Serial.parseInt();
        if(freq_in>0 && freq_in<1001) freq=freq_in;
        //else Serial.println("ERROR"); 
        xSemaphoreGive( buffer_mutex_freq );
      }
    }
    vTaskDelay(1000/portTICK_PERIOD_MS);
  }
}

void TaskButtonS2(void *pvParameters){
  
  int old_2=0;
  int new_2=0;
  pinMode(BUTTON_2_PIN, INPUT);  
  
  while(1)
  {
    //Serial.println(digitalRead(BUTTON_1_PIN));
    new_2=digitalRead(BUTTON_2_PIN);
    
    if(!new_2 && old_2 && freq*2<1001)
    {
      if( xSemaphoreTake( buffer_mutex_freq, WAIT_TIMEOUT ) == pdTRUE )
      {   
        Serial.println("******");
        freq=freq*2;
        MFS.Display(freq);
        xSemaphoreGive( buffer_mutex_freq );
      }
    }
    //old_2=digitalRead(BUTTON_2_PIN);
    old_2=new_2;
    vTaskDelay(300/portTICK_PERIOD_MS);
  }
}

void TaskButtonS1(void *pvParameters){
  
  int old_1=0;
  int new_1=0;
  pinMode(BUTTON_1_PIN, INPUT);  
  
  while(1)
  {
    //Serial.println(digitalRead(BUTTON_1_PIN));
    new_1=digitalRead(BUTTON_1_PIN);
    
    if(!new_1 && old_1 && freq/2>0)
    {
      if( xSemaphoreTake( buffer_mutex_freq, WAIT_TIMEOUT ) == pdTRUE )
      {   
        Serial.println("///////");
        freq=freq/2;
        MFS.Display(freq);
        xSemaphoreGive( buffer_mutex_freq );
      }
    }
    //old_2=digitalRead(BUTTON_2_PIN);
    old_1=new_1;
    vTaskDelay(300/portTICK_PERIOD_MS);
  }
}

void TaskBuzz(void *pvParameters) 
{
  //Declaring LED pin as output
  pinMode(BUZZER_PIN, OUTPUT);
  
  while(1)
  {     
      //Fading the LED
      for(int i=0; i<255; i++)
      {
        analogWrite(BUZZER_PIN, i);
        Serial.println(i);
        vTaskDelay (freq/ portTICK_PERIOD_MS);
      }
    
      for(int i=255; i>0; i--)
      {
        analogWrite(BUZZER_PIN, i);
        Serial.println(i);
        vTaskDelay (freq/ portTICK_PERIOD_MS);
      }
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
