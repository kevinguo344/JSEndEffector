#define BITS 16
#define MIN_DELAY 10

const int digital_pins[] = {22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52};
const int bit_values[] = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,-1};

int num_steps = 0, pulse_width = 0, prev_input = 0;
int change_time = 0;

int inputs[] = {537427968, 537427968, 537427968, 537427968, 537427968};

void setup(){
  Serial.begin(115200);
  //setps up input pins
  for(int i = 0; i < BITS; i++){ pinMode(digital_pins[i], INPUT); }
}

void loop(){
  // SEQUENCE: NUMBER OF STEPS, -1, PULSE_WIDTH
  /*for(int i=0; i<BITS-1; i++){
      Serial.print(digitalRead(digital_pins[i]));
      Serial.print(",");
    }
    Serial.println(digitalRead(digital_pins[BITS-1]));*/
  int curr_input[BITS+1];
  readBinary(curr_input);
  if (curr_input[BITS] != prev_input){
    Serial.print("Reading: ");
    for(int i = 0; i < BITS; i++){
      Serial.print(curr_input[i]); Serial.print(",");
    }
    Serial.println("");
    Serial.println(curr_input[BITS]);
    prev_input = curr_input[BITS];
  }
  
  /*if(curr_input != prev_input){
    //Serial.println(curr_input)
    prev_input = curr_input;/*
    int curr_time = millis();
    if(curr_time-change_time > 600/* && inputs[6] == 537427968){ //inserts number if 6th array element is non-null
      //Serial.print("Input: "); Serial.print(prev_input); Serial.print("\t Time:"); Serial.println(curr_time-change_time);
      /*for(int i = 0; i < 5; i++){
        if(inputs[i] == 537427968){
          inputs[i] = prev_input;
          break;
        }
      }
      prev_input = curr_input;
      change_time = curr_time;
      for(int i = 0; i < 5; i++){
        Serial.println(inputs[i]);
      }
      Serial.println("--------");
    }
    if (prev_input == 0 && inputs[4] != 537427968){
      Serial.println("------START NEW TRANSMISSION------");
      Serial.print("Steps: "); Serial.print(inputs[2]); Serial.print("\tPulse Width: "); Serial.println(inputs[4]);
      for(int i = 0; i < 5; i++){
        inputs[i] = 537427968;
      }
    }
  }*/
  /**/
  /*int curr_input = readBinary();
  if (curr_input != prev_input){ change_time = millis(); }
  if (millis() - change_time > MIN_DELAY && curr_input != prev_input && curr_input == readBinary()){
    Serial.println(curr_input);
    prev_input = curr_input;
  }*/
}
int readBinary(int * signals){
  int sum = 0;
  for(int i=0; i<BITS-1; i++){
    int reading = digitalRead(digital_pins[i]);
    if(reading == HIGH){ sum += bit_values[i]; }
    signals[i] = reading;
  }
  int sign = digitalRead(digital_pins[BITS-1]);
  signals[BITS-1] = sign;
  if (sign == 0){ sum *= -1; }
  signals[BITS] = sum;
}
