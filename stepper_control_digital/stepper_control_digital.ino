#define STEP_PIN 9
#define DIRECTION_PIN 10
#define ENDSTOP_BACK_PIN 11

#define SWITCH_ON 0
#define SWITCH_OFF 1

#define B0 22
#define B1 24
#define B2 26
#define B3 28
#define B4 30
#define B5 32
#define B6 34
#define B7 36
#define B8 38
#define B9 40
#define BA 42
#define BB 44
#define BC 46
#define BD 48
#define BE 50
#define BF 52

int num_steps = 0; int pulse_width = 0;

void setup(){
  Serial.begin(115200);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIRECTION_PIN, OUTPUT);
  pinMode(ENDSTOP_BACK_PIN, INPUT);

  //setps up input pins
  pinMode(B0, INPUT); pinMode(B1, INPUT); pinMode(B2, INPUT); pinMode(B3, INPUT);
  pinMode(B4, INPUT); pinMode(B5, INPUT); pinMode(B6, INPUT); pinMode(B7, INPUT);
  pinMode(B8, INPUT); pinMode(B9, INPUT); pinMode(BA, INPUT); pinMode(BB, INPUT);
  pinMode(BC, INPUT); pinMode(BD, INPUT); pinMode(BE, INPUT); pinMode(BF, INPUT);
}

void loop(){
  // SEQUENCE: NUMBER OF STEPS, -1, PULSE_WIDTH
  if(readBin() != 0 && num_steps == 0){                                               // checks if there's a Serial message
    num_steps = readBin();
  }
  if(readBin() != 0 && readBin() != 1){
    pulse_width = readBin();
    Serial.print("Steps: "); Serial.print(num_steps); Serial.print("/"); Serial.println(pulse_width);
    num_steps = 0; pulse_width = 0;
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
  if(pulseWidth < 5){ pulseWidth = 5; }                                   // makes any delay less than 5 us to 5 us
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
  Serial.print("Number of steps: "); Serial.println(numSteps);
  Serial.print("Delay: "); Serial.println(pulseWidth);
  float elapsed_time = (end_time - start_time)*0.001;
  float est_time = (numSteps*(pulseWidth+100))/1000000;
  Serial.print("Elapsed Time: "); Serial.println(elapsed_time);
  Serial.print("Estimated Time: "); Serial.println(est_time);
  Serial.print("Error: "); Serial.println(elapsed_time - est_time);
}

int readBin(){
  int sum = 0;
  sum += digitalRead(B0) * 1; sum += digitalRead(B1) * 2; sum += digitalRead(B2) * 4; sum += digitalRead(B3) * 8;
  sum += digitalRead(B4) * 16; sum += digitalRead(B5) * 32; sum += digitalRead(B6) * 64;
  sum += digitalRead(B7) * 128; sum += digitalRead(B8) * 256; sum += digitalRead(B9) * 512;
  sum += digitalRead(BA) * 1024; sum += digitalRead(BB) * 2048; sum += digitalRead(BC) * 4096; sum += digitalRead(BD) * 8192;
  sum += digitalRead(BE) * 16384;

  // if BF is HIGH, number is positive
  if (digitalRead(BF) == 0){ sum *= -1; }
  return sum;
}
