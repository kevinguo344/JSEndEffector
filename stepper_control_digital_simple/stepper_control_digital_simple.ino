#define STEP_PIN 9
#define DIRECTION_PIN 10

#define ON_OFF_PIN 22
#define DIR_INPUT_PIN 24

#define PULSE_WIDTH 25

bool curr_on_off;
bool curr_dir;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(STEP_PIN, OUTPUT); pinMode(DIRECTION_PIN, OUTPUT); 
  pinMode(ON_OFF_PIN, INPUT); pinMode(DIR_INPUT_PIN, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(ON_OFF_PIN)){
    Serial.println("Stepper On");
    if(digitalRead(DIR_INPUT_PIN)){ Serial.println("Forward"); }
    else{ Serial.println("Backwards"); }
    step(digitalRead(DIR_INPUT_PIN), PULSE_WIDTH);
  }
}

void step(bool forward, float delayMs){
  // does a step
  if(forward){ digitalWrite(DIRECTION_PIN, HIGH); }                 // changes direction of motor depending if steps negative or positive
  else{ digitalWrite(DIRECTION_PIN, LOW); }               
  digitalWrite(STEP_PIN, HIGH);                                     // starts pulse by setting step pin to high 
  delayMicroseconds(delayMs);                                       // delay must be greater than 3 us according to Arduino docs
  digitalWrite(STEP_PIN, LOW);                                      // finishes pulse by going to low
  delayMicroseconds(100);
}
