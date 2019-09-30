// Output Pin definitions
#define STEP_PIN 9
#define DIRECTION_PIN 10
#define ENDSTOP_BACK_PIN 11

// Input Pin definitions
#define ON_OFF_PIN 22
#define DIR_INPUT_PIN 24

// Distance Sensor constants
#include <Wire.h>
#include <VL6180X.h>
VL6180X sensor;

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
  pinMode(ON_OFF_PIN, INPUT); pinMode(DIR_INPUT_PIN, INPUT); pinMode(ENDSTOP_BACK_PIN, INPUT);

  // sets up distance sensor
  Wire.begin();
  sensor.init(); sensor.configureDefault();
  sensor.setScaling(2); sensor.setTimeout(500);
}

void loop() {
  if(digitalRead(ON_OFF_PIN)){
    Serial.println("Stepper On");
    if(digitalRead(DIR_INPUT_PIN)){
      // if the movement is forward, check the distance sensor
      if (sensor.readRangeSingleMillimeters() < 500){ step(digitalRead(DIR_INPUT_PIN), PULSE_WIDTH); }
      else{ Serial.println("DISTANCE SENSOR ACTIVATED"); }
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
  if(forward){ digitalWrite(DIRECTION_PIN, HIGH); }                 // changes direction of motor depending if steps negative or positive
  else{ digitalWrite(DIRECTION_PIN, LOW); }               
  digitalWrite(STEP_PIN, HIGH);                                     // starts pulse by setting step pin to high 
  delayMicroseconds(delayMs);                                       // delay must be greater than 3 us according to Arduino docs
  digitalWrite(STEP_PIN, LOW);                                      // finishes pulse by going to low
  delayMicroseconds(100);
}

void retractPosition(){
  Serial.println("Retract Extruder");
  while(digitalRead(ENDSTOP_BACK_PIN) == SWITCH_OFF){ step(false, RETRACT_PULSE_WIDTH); }
  Serial.println("Extruder Fully Retracted");
}
