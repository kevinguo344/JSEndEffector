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
#define PULSE_WIDTH 25
#define RETRACT_PULSE_WIDTH 50

void setup() {
  Serial.begin(115200);

  // sets up mode for digital pins
  pinMode(STEP_PIN, OUTPUT); pinMode(DIRECTION_PIN, OUTPUT); 
  pinMode(ON_OFF_PIN, INPUT); pinMode(DIR_INPUT_PIN, INPUT); pinMode(ENDSTOP_BACK_PIN, INPUT); pinMode(TYPE_START, INPUT);

  // sets up distance sensor
  Wire.begin();
  sensor.init(); sensor.configureDefault();
  sensor.setScaling(3); sensor.setTimeout(500);

  if(digitalRead(TYPE_START)){  retractPosition(); } // if clean start, retract extruder all the way
}

void loop() {
  if(digitalRead(ON_OFF_PIN)){
    Serial.println("Stepper On");
    if(digitalRead(DIR_INPUT_PIN)){
      // if the movement is forward, check the distance sensor/
      if (sensor.readRangeSingleMillimeters() < 760){ step(digitalRead(DIR_INPUT_PIN), PULSE_WIDTH); }
      else{ Serial.println("DISTANCE SENSOR ACTIVATED"); retractPosition(); }
    }
    else{
      // if the movement is backwards, check the endstop sensor
      if(digitalRead(ENDSTOP_BACK_PIN) == SWITCH_OFF){ step(digitalRead(DIR_INPUT_PIN), PULSE_WIDTH); }
      else{ Serial.println("ENDSTOP ACTIVATED"); }
    }
  }
}

void step(bool forward, float delayMs){
  // does a step
  // Step: |<------HIGH for Pulse Width (in microseconds)------>||<------ LOW for 100 microseconds)------>| 
  int curr_step = (int) dueFlashStorage.read(ADDRESS);
  if(forward){
    digitalWrite(DIRECTION_PIN, HIGH);
    curr_step++;
  }
  else{
    digitalWrite(DIRECTION_PIN, LOW);
    curr_step--;
  }
  dueFlashStorage.write(ADDRESS, (byte) curr_step);
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
