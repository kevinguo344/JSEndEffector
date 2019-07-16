#define B0 1
#define B1 2
#define B2 3
#define B3 4
#define B4 5
#define B5 6
#define B6 7
#define B7 8
#define B8 9
#define B9 10
#define BA 11
#define BB 12
#define BC 13
#define BD 14
#define BE 15
#define BF 16

int result = 0;

void setup() {
  Serial.begin(115200);
  //setps up input pins
  pinMode(B0, INPUT); pinMode(B1, INPUT); pinMode(B2, INPUT); pinMode(B3, INPUT);
  pinMode(B4, INPUT); pinMode(B5, INPUT); pinMode(B6, INPUT); pinMode(B7, INPUT);
  pinMode(B8, INPUT); pinMode(B9, INPUT); pinMode(BA, INPUT); pinMode(BB, INPUT);
  pinMode(BC, INPUT); pinMode(BD, INPUT); pinMode(BE, INPUT); pinMode(BF, INPUT);
}

void loop() {
  if(readBin() != result){
    result = readBin();
    Serial.println(result);
  }
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
