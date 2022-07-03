#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <Stepper.h>
#include <LiquidCrystal.h>

SemaphoreHandle_t xSerialMutex;       // Lock for serial communication flag
LiquidCrystal lcd(2,3,4,5,6,7); /// REGISTER SELECT PIN,ENABLE PIN,D4 PIN,D5 PIN, D6 PIN, D7 PIN

const int stepsPerRevolution = 300;  //number of steps per revolution

// create an instance of the stepper class, specifying
// the number of steps of the motor and the pins it's attached to
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

void TasksetSpeed( void *pvParameters );
void Taskrotatemotor( void *pvParameters );

int rev =0;           //shared variable

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("Auto Packaging");
  delay(6000);
  lcd.clear();
  
  xSerialMutex = xSemaphoreCreateMutex();  // Create a mutex semaphore to manage the Serial Port
  if ( ( xSerialMutex ) != NULL ){
    xSemaphoreGive( ( xSerialMutex ) );    // Make the Serial Port available for use, by "Giving" the Semaphore.
  }
  
//create the task
xTaskCreate(
  TasksetSpeed
  ,  "TurnLeft"  
  ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
  ,  NULL
  ,  1  
  ,  NULL );
  
xTaskCreate(
  Taskrotatemotor
  ,  "TurnRight"  
  ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
  ,  NULL
  ,  1 
  ,  NULL );

vTaskStartScheduler();
}

void loop() {
  // put your main code here, to run repeatedly:
}

void TasksetSpeed(void *pvParameters) {

  for(;;){
        
      if ( xSemaphoreTake( xSerialMutex, ( TickType_t ) 5 ) == pdTRUE )
      {
        int a = random(200,2000);
        rev =map(a, 200, 2000, 0, 100);
        lcd.setCursor(0,0);
        Serial.print("Speed set at : ");
        Serial.print(rev);
        Serial.println(" from Task 1");
        lcd.print("Speed = ");
        lcd.print(rev);
        xSemaphoreGive( xSerialMutex ); 
       }
        vTaskDelay(pdMS_TO_TICKS(5000));      //5 second delay
   
  }

}

void Taskrotatemotor(void *pvParameters) {
  
    for(;;){
          
        if ( xSemaphoreTake( xSerialMutex, ( TickType_t ) 5 ) == pdTRUE )
        {
          if (rev<50)
          {
            Serial.print("Rotate Left at: ");
            Serial.print(rev);
            Serial.println(" rpm");
            lcd.setCursor(0,1);
            lcd.print("Heavy-Rotate <<");
            myStepper.setSpeed(rev);
            myStepper.step(-stepsPerRevolution);
            rev = 0;
          }

          else
          {
            Serial.print("Rotate Right at: ");
            Serial.print(rev);
            Serial.println(" rpm");
            lcd.setCursor(0,1);
            lcd.print("Light-Rotate >>");
            myStepper.setSpeed(rev);
            myStepper.step(stepsPerRevolution);
            rev = 0;
          }
          xSemaphoreGive( xSerialMutex );
          lcd.clear();  
         }
          vTaskDelay(pdMS_TO_TICKS(5000));
  }
}
