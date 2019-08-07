#define STEP_PIN 9
#define DIRECTION_PIN 10
#define ENDSTOP_BACK_PIN 11

#define SWITCH_ON 0
#define SWITCH_OFF 1

#define BITS 16

//#include <Wire.h>
//#include <VL6180X.h>

//VL6180X sensor;

const int digital_pins[] = {22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52};
const int bit_values[] = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,-1};

int num_steps = 0, pulse_width = 0, prev_input = 0, change_time = 0;
int inputs[] = {537427968, 537427968, 537427968, 537427968, 537427968};

void setup(){
  Serial.begin(115200);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIRECTION_PIN, OUTPUT);
  pinMode(ENDSTOP_BACK_PIN, INPUT);

  for(int i = 0; i < BITS; i++){ pinMode(digital_pins[i], INPUT); }
}

void loop(){
  int curr_input = readBinary();
  if(curr_input != prev_input){
    int curr_time = millis();
    if(curr_time-change_time > 600){
      for(int i = 0; i < 5; i++){
        if(inputs[i] == 537427968){
          inputs[i] = prev_input;
          break;
        }
      }
      prev_input = curr_input;
      change_time = curr_time;
    }
    if (prev_input == 0 && inputs[4] != 537427968){
      Serial.print("Steps: "); Serial.print(inputs[2]); Serial.print("\tPulse Width: "); Serial.println(inputs[4]);
      steps(inputs[2],inputs[4]);
      for(int i = 0; i < 5; i++){
        inputs[i] = 537427968;
      }
    }
  }
}

void step(bool forward, float pulseWidth){
  // does a step
  if(forward){ digitalWrite(DIRECTION_PIN, HIGH); }                 // changes direction of motor depending if steps negative or positive
  else{ digitalWrite(DIRECTION_PIN, LOW); }               
  digitalWrite(STEP_PIN, HIGH);                                     // starts pulse by setting step pin to high 
  delayMicroseconds(pulseWidth);                                    // delay must be greater than 3 us according to Arduino docs
  digitalWrite(STEP_PIN, LOW);                                      // finishes pulse by going to low
}

void steps(float numSteps, float pulseWidth){
  bool isForward = true;
  unsigned long start_time, end_time;
  start_time = millis();
  if(pulseWidth < 5){ pulseWidth = 5; }                             // makes any delay less than 5 us to 5 us
  if(numSteps < 0){ isForward = false; numSteps = -numSteps; }      // checks whether to step forward or backwards
  for(float i = 0; i < numSteps; i++){                              // does the steps
    if(!isForward && digitalRead(ENDSTOP_BACK_PIN) == SWITCH_OFF){  // if the motor moves backwards and the switch isn't on, do the steps
      step(isForward, pulseWidth);
      delayMicroseconds(100);
    }
    else if(isForward){
      step(isForward, pulseWidth);
      delayMicroseconds(100);
    }
    else if(digitalRead(ENDSTOP_BACK_PIN) == SWITCH_ON){
      Serial.println("ENDSTOP ACTIVATED");  
    }
  }
  end_time = millis();
}

int readBinary(){
  int sum = 0;
  for(int i=0; i<BITS-1; i++){
    if(digitalRead(digital_pins[i]) == HIGH){ sum += bit_values[i]; }
  }
  if (digitalRead(digital_pins[BITS-1]) == 0){ sum *= -1; }
  return sum;
}
