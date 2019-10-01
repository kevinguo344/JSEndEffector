void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.println("Starting to read from RX1, TX1");
}

void loop() {
  if(Serial1.available()){
    Serial.println("Recieving from RX1, TX1");
    String message = Serial1.readString();
    Serial.println(message);
  }
}
