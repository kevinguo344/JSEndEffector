#define BITS 16
#define MIN_DELAY 10

const int digital_pins[] = {22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52};
const int bit_values[] = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,-1};

int num_steps = 0, pulse_width = 0, prev_input = 0;
int last_change = 0;

int inputs[] = {537427968, 537427968, 537427968, 537427968, 537427968};

void setup(){
  Serial.begin(115200);
  //setps up input pins
  for(int i = 0; i < BITS; i++){ pinMode(digital_pins[i], INPUT); }
}

void loop(){
  // SEQUENCE: NUMBER OF STEPS, -1, PULSE_WIDTH
  int curr_input[BITS+1];
  readBinary(curr_input);
  if (curr_input[BITS] != prev_input){
    int curr_time = millis();
    if(curr_time - last_change > 500){
      if(prev_input == 0){
        num_steps = curr_input[BITS];
        Serial.print("Number of Steps: "); Serial.println(num_steps);
      }
      if(prev_input == -1){
        pulse_width = curr_input[BITS];
        Serial.print("Pulse Width: "); Serial.println(pulse_width);
      }
      prev_input = curr_input[BITS];
    }
    last_change = curr_time;
    Serial.print("Reading: ");
    for(int i = 0; i < BITS; i++){
      Serial.print(curr_input[i]); Serial.print(",");
    }
    Serial.println("");
    Serial.println(curr_input[BITS]);
  }
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
