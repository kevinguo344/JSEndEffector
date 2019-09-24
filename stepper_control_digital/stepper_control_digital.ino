#define BITS 16
#define MIN_DELAY 200
#define EMPTY 537427968
#define BUFFER_SIZE 5
#define DELIMETER -1

#define STEP_PIN 9
#define DIRECTION_PIN 10

const int digital_pins[] = {22,24,26,28,30,32,34,36,23,25,27,29,31,33,35,37};
const int bit_values[] = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,-1};

int num_steps = 0, pulse_width = 0, prev_input = 0, change_time = 0, prev_inserted_input = 0;

int inputs[] = {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY};

int curr_input[BITS+1];

void setup(){
  Serial.begin(115200);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIRECTION_PIN, OUTPUT);

  for(int i = 0; i < BITS; i++){ pinMode(digital_pins[i], INPUT); }
}

void loop(){
  // SEQUENCE: NUMBER OF STEPS, -1, PULSE_WIDTH
  readBinary(curr_input);
  if (curr_input[BITS] != prev_input){
    int this_change = millis();
    if(this_change - change_time > MIN_DELAY){
      if(prev_input != prev_inserted_input){
        Serial.print("New Input: "); Serial.print(prev_input); Serial.print("\tElapsed Time: "); Serial.println(this_change - change_time);
        Serial.println("---------");
        for(int i = 0; i < BUFFER_SIZE; i++){
          Serial.println(inputs[i]);
        }
        Serial.println("---------");
        for(int i = 0; i < BUFFER_SIZE; i++){
          if(inputs[i] == EMPTY){
            inputs[i] = prev_input;
            prev_inserted_input = inputs[i];
            break;
          }
        }
      }
    }
    change_time = this_change;
    prev_input = curr_input[BITS];
    readBuffer();
  }
}

void step(bool forward, float pulseWidth){
  // does a step
  if(forward){ digitalWrite(DIRECTION_PIN, HIGH); }                 // changes direction of motor depending if steps negative or positive
  else{ digitalWrite(DIRECTION_PIN, LOW); }
  delayMicroseconds(20);
  digitalWrite(STEP_PIN, HIGH);                                     // starts pulse by setting step pin to high 
  delayMicroseconds(abs(pulseWidth));                                    // delay must be greater than 3 us according to Arduino docs
  digitalWrite(STEP_PIN, LOW);                                      // finishes pulse by going to low
  delayMicroseconds(100);
}

void steps(float numSteps, float pulseWidth){
  bool isForward = true;
  unsigned long start_time, end_time;
  if(pulseWidth < 5){ pulseWidth = 5; }                             // makes any delay less than 5 us to 5 us
  if(numSteps < 0){ isForward = false; numSteps = -numSteps; }      // checks whether to step forward or backwards
  for(float i = 0; i < numSteps; i++){                              // does the steps
    step(isForward, pulseWidth);
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
  for(int i = 0; i < BUFFER_SIZE; i++){
    //Serial.println(inputs[i]);
    if(inputs[i] == -1){
      isDelimeter = true;
      delimeter_ind = i;
    }
  }
  if(isDelimeter && delimeter_ind != 0 && delimeter_ind != BUFFER_SIZE-1){
    num_steps = inputs[delimeter_ind-1];
    pulse_width = inputs[delimeter_ind+1];
    if(num_steps != 0 && pulse_width != 0 && num_steps != EMPTY && pulse_width != EMPTY){
      Serial.println("---------Input Found---------");
      Serial.print("Steps:\t"); Serial.println(num_steps);
      Serial.print("Pulse Width:\t"); Serial.println(pulse_width);
      if(pulse_width < 0){ pulse_width = abs(pulse_width); }
      if(pulse_width < 10){ pulse_width = 10; }
      steps(num_steps, pulse_width);
      emptyBuffer();
    }
  }
  else if(inputs[BUFFER_SIZE-1] != EMPTY){
    emptyBuffer();
  }
}

// EMPTIES OUT INPUT BUFFER TO GET NEW INPUTS
void emptyBuffer(){
  Serial.println("Emptying Buffer");
  for(int i = 0; i < BUFFER_SIZE; i++){ inputs[i] = EMPTY; }
  num_steps = 0; pulse_width = 0;
}
