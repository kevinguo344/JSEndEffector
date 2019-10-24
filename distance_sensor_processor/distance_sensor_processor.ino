// based on https://www.arduino.cc/en/tutorial/smoothing

#include "Adafruit_VL53L0X.h"
#define DISTANCE_PIN 9
#define NUM_READINGS 10

Adafruit_VL53L0X sensor = Adafruit_VL53L0X();
VL53L0X_RangingMeasurementData_t measurement;

int readings[NUM_READINGS];
int readIndex = 0;
int total = 0;
int average = 0;

void setup() {
  Serial.begin(115200);
  pinMode(DISTANCE_PIN, OUTPUT);
  if (!sensor.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  for(int i = 0; i < NUM_READINGS; i++){ readings[i] = 0; }
}

void loop() {
  sensor.rangingTest(&measurement, false);
  if (measurement.RangeStatus != 4) {
    total = total - readings[readIndex];
    readings[readIndex] = measurement.RangeMilliMeter;
    total = total + readings[readIndex];
    readIndex = readIndex + 1;
    if(readIndex >= NUM_READINGS){
      readIndex = 0;
    }
    average = total/NUM_READINGS;
  }
  Serial.print("Distance (mm): "); Serial.println(average);
  
  if(average > 420){
      digitalWrite(DISTANCE_PIN, HIGH);
      Serial.println("OVER LIMIT");
  }
  else{ digitalWrite(DISTANCE_PIN, LOW); }
  delay(100);
}
