// Output Pin definitions
#define STEP_PIN 9
#define DIRECTION_PIN 10
#define ENDSTOP_BACK_PIN 11

// Input Pin definitions
#define ON_OFF_PIN 22
#define DIR_INPUT_PIN 24
#define TYPE_START 26

// Distance Sensor constants
#include <Wire.h>
#include <VL6180X.h>
VL6180X sensor;

// Nonvolatile Memory
#include <DueFlashStorage.h>
DueFlashStorage dueFlashStorage;
#define ADDRESS 0

// Endstop Switch constants
#define SWITCH_ON 0
#define SWITCH_OFF 1

// Pulse Width constants
#define RETRACT_PULSE_WIDTH 50
int PULSE_WIDTH = 25;

// if EMERGENCY_STOP is true, will not continue to move
boolean EMERGENCY_STOP = false;

void setup() {
  Serial.begin(115200);

  // sets up mode for digital pins
  pinMode(STEP_PIN, OUTPUT); pinMode(DIRECTION_PIN, OUTPUT);
  pinMode(ON_OFF_PIN, INPUT); pinMode(DIR_INPUT_PIN, INPUT); pinMode(ENDSTOP_BACK_PIN, INPUT); pinMode(TYPE_START, INPUT);

  // sets up distance sensor
  Wire.begin();
  sensor.init(); sensor.configureDefault();
  sensor.setScaling(3); sensor.setTimeout(500);

  if(digitalRead(TYPE_START)){ retractPosition(); } // if clean start, retract extruder all the way
}

void loop() {
  int curr_step = (int) dueFlashStorage.read(ADDRESS);
  if(Serial.available()){
    String message = Serial.readString();
    Serial.print("Received: "); Serial.println(message);
    
    // Check whether it's a STOP or PULSE WIDTH message
    if(message.chartAt(0) == 'S'){ EMERGENCY_STOP = !EMERGENCY_STOP; } // Turns EMERGENCY_STOP On or Off 
    else if(!isnan(message.toFloat())){ // Changes pulse width
      Serial.print("Changing Pulse Width from "); Serial.print(PULSE_WIDTH); Serial.print(" to "); Serial.println(max(message.toFloat(), 5));
      PULSE_WIDTH = max(message.toFloat(), 5); // Makes sure that any new pulse width is at least 5 us
    }
    else{ Serial.println("Serial Message not understood"); }
  }
  if(digitalRead(ON_OFF_PIN) && !EMERGENCY_STOP){
    Serial.println("Stepper On");
    if(digitalRead(DIR_INPUT_PIN)){
      // if the movement is forward, check the distance sensor
      if (sensor.readRangeSingleMillimeters() < 760){
        step(digitalRead(DIR_INPUT_PIN), PULSE_WIDTH);
        curr_step++;
        Serial.print("Current Position"); Serial.println(dueFlashStorage.read(ADDRESS));
      }
      else{ Serial.println("DISTANCE SENSOR ACTIVATED"); retractPosition(); }
    }
    else{
      // if the movement is backwards, check the endstop sensor
      if(digitalRead(ENDSTOP_BACK_PIN) == SWITCH_OFF){
        step(digitalRead(DIR_INPUT_PIN), PULSE_WIDTH);
        curr_step--;
        Serial.print("Current Position"); Serial.println(dueFlashStorage.read(ADDRESS));
      }
      else{ Serial.println("ENDSTOP ACTIVATED"); }
    }
  }
  dueFlashStorage.write(ADDRESS, (byte) curr_step);
}

void step(bool forward, float delayMs){
  // does a step
  // Step: |<------HIGH for Pulse Width (in microseconds)------>||<------ LOW for 100 microseconds)------>| 
  if(forward){ digitalWrite(DIRECTION_PIN, HIGH); }
  else{ digitalWrite(DIRECTION_PIN, LOW); }
  digitalWrite(STEP_PIN, HIGH);                                     // starts pulse by setting step pin to high 
  delayMicroseconds(delayMs);                                       // delay must be greater than 3 us according to Arduino docs
  digitalWrite(STEP_PIN, LOW);                                      // finishes pulse by going to low
  delayMicroseconds(100);
}

void retractPosition(){
  Serial.println("Retracting Extruder");
  while(digitalRead(ENDSTOP_BACK_PIN) == SWITCH_OFF){ step(false, RETRACT_PULSE_WIDTH); }
  dueFlashStorage.write(ADDRESS, 0);
  Serial.println("Extruder Fully Retracted");
}
