const int digital_pins[] = {22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52};
const int bit_values[] = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,-1};

int result = 0;

void setup() {
  Serial.begin(115200);
  //setps up input pins
  for(int i = 0; i < BITS; i++){ pinMode(digital_pins[i], INPUT); }
}

void loop() {
  if(readBin() != result){
    result = readBin();
    Serial.println(result);
  }
  delay(50);
}

int readBin(){
  int sum = 0;
  for(int i=0; i<BITS-1; i++){
    if(digitalRead(digital_pins[i]) == 1){
      Serial.print("Pin "); Serial.print(i+1); Serial.println(" is HIGH");
      sum += bit_values[i];
    }
  }

  if (digitalRead(digital_pins[BITS-1]) == 0){
    sum *= -1;
    Serial.println("Pin 16 is LOW");
  }
  Serial.println("---------");
  return sum;
}
