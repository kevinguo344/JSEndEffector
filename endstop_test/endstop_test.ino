#define ENDSTOP_PIN 11

int switch_read = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ENDSTOP_PIN, INPUT);
}

void loop() {
  if(digitalRead(ENDSTOP_PIN) != switch_read){
    switch_read = digitalRead(ENDSTOP_PIN);
    if(switch_read == 1){ Serial.println("Not Switched On"); }
    else{ Serial.println("Switched On"); }
  }
}
