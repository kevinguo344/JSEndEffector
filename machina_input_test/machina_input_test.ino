#define BITS 16
#define MIN_DELAY 300
#define EMPTY 537427968
#define BUFFER_SIZE 5
#define DELIMETER -1

const int digital_pins[] = {22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52};
const int bit_values[] = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,-1};

int num_steps = 0, pulse_width = 0, prev_input = 0;
int change_time = 0;

int inputs[] = {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY};

int curr_input[BITS+1];

void setup(){
  Serial.begin(115200);
  //setps up input pins
  for(int i = 0; i < BITS; i++){ pinMode(digital_pins[i], INPUT); }
}

void loop(){
  // SEQUENCE: NUMBER OF STEPS, -1, PULSE_WIDTH
  
  readBinary(curr_input);
  if (curr_input[BITS] != prev_input){
    int this_change = millis();
    if(this_change - change_time > MIN_DELAY){
      for(int i = 0; i < 4; i++){
        if(inputs[i] == EMPTY){
          inputs[i] = prev_input;
          break;
        }
      }
    }
    change_time = this_change;
    prev_input = curr_input[BITS];
    readBuffer();
  }
}
// READS INPUT SIGNALS AS BINARY NUMBERS
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

// READS INPUT BUFFER TO FIND INPUTS
void readBuffer(){
  
  boolean isDelimeter = false;
  int delimeter_ind = 0;
  Serial.println("----------------------");
  for(int i = 0; i < BUFFER_SIZE; i++){
    Serial.println(inputs[i]);
    if(inputs[i] == -1){
      isDelimeter = true;
      delimeter_ind = i;
    }
  }
  Serial.println("----------------------");
  if(isDelimeter && delimeter_ind != 0 && delimeter_ind != BUFFER_SIZE-1){
    num_steps = inputs[delimeter_ind-1];
    pulse_width = inputs[delimeter_ind+1];
    if(num_steps != 0 && pulse_width != 0 && num_steps != EMPTY && pulse_width != EMPTY){
      Serial.println("---------Input Found---------");
      Serial.print("Steps:\t"); Serial.println(num_steps);
      Serial.print("Pulse Width:\t"); Serial.println(pulse_width);
      emptyBuffer();
    }
  }
}

void emptyBuffer(){
  for(int i = 0; i < BUFFER_SIZE; i++){
    inputs[i] = EMPTY;
  }
  num_steps = 0; pulse_width = 0;
}
