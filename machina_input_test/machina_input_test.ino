#define BITS 16
#define MIN_DELAY 10

const int digital_pins[] = {22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52};
const int bit_values[] = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,-1};

int num_steps = 0, pulse_width = 0, prev_input = 0;
int change_time = 0;

void setup(){
  Serial.begin(115200);
  //setps up input pins
  for(int i = 0; i < BITS; i++){ pinMode(digital_pins[i], INPUT); }
}

void loop(){
  // SEQUENCE: NUMBER OF STEPS, -1, PULSE_WIDTH
  int curr_input = readBinary();
  if (curr_input != prev_input){ change_time = millis(); }
  if (millis() - change_time > MIN_DELAY && curr_input != prev_input && curr_input == readBinary()){
    Serial.println(curr_input);
    prev_input = curr_input;
  }
}

int readBinary(){
  int sum = 0;
  for(int i=0; i<BITS-1; i++){
    if(digitalRead(digital_pins[i]) == HIGH){ sum += bit_values[i]; }
  }
  if (digitalRead(digital_pins[BITS-1]) == 0){ sum *= -1; }
  return sum;
}
