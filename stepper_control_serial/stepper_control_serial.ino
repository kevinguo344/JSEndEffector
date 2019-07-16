#define STEP_PIN 9
#define DIRECTION_PIN 10
#define ENDSTOP_BACK_PIN 11

#define SWITCH_ON 0
#define SWITCH_OFF 1

void setup(){
  Serial.begin(115200);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIRECTION_PIN, OUTPUT);
  pinMode(ENDSTOP_BACK_PIN, INPUT);
}

void loop(){
  if(Serial.available()){ 												                  // checks if there's a Serial message
    String message = Serial.readString();								            // Serial message will be in format "steps/delay"
    Serial.print("Kevin: "); Serial.println(message); 				      // prints out message
    // reference: https://gist.github.com/mattfelsen/9467420
    float num_steps, delay_microseconds; 									          // num_steps must be an integer, delay_microseconds must be an integer > 3
    for (int i = 0; i < message.length(); i++) {
      if (message.substring(i, i+1) == "/") {   							      // splits message string into num_steps, delay_microseconds
        num_steps = message.substring(0, i).toFloat();
        delay_microseconds = message.substring(i+1).toFloat();
        Serial.print("Number of steps: "); Serial.println(num_steps);
        Serial.print("Delay: "); Serial.println(delay_microseconds);
        steps(num_steps,delay_microseconds);							          // passes in steps num_steps and delay_microseconds 
        break;
      }
    }
  }
}

void step(bool forward, float delayMs){
  // does a step
  if(forward){ digitalWrite(DIRECTION_PIN, HIGH); } 					      // changes direction of motor depending if steps negative or positive
  else{ digitalWrite(DIRECTION_PIN, LOW); }								
  digitalWrite(STEP_PIN, HIGH);											                // starts pulse by setting step pin to high 
  delayMicroseconds(delayMs);											                  // delay must be greater than 3 us according to Arduino docs
  digitalWrite(STEP_PIN, LOW);											                // finishes pulse by going to low
}

void steps(float numSteps, float delayMs){
  bool isForward = true;
  unsigned long start_time, end_time;
  start_time = millis();
  if(delayMs < 5){ delayMs = 5; }										                // makes any delay less than 5 us to 5 us
  if(numSteps < 0){ isForward = false; numSteps = -numSteps; }			// checks whether to step forward or backwards
  for(float i = 0; i < numSteps; i++){									            // does the steps
    if(!isForward && digitalRead(ENDSTOP_BACK_PIN) == SWITCH_OFF){  // if the motor moves backwards and the switch isn't on, do the steps
      step(isForward, delayMs);
      delayMicroseconds(100);
    }
    else if(isForward){
      step(isForward, delayMs);
      delayMicroseconds(100);
    }
    else if(digitalRead(ENDSTOP_BACK_PIN) == SWITCH_ON){
      Serial.println("ENDSTOP ACTIVATED");  
    }
  }
  end_time = millis();
  Serial.print("Number of steps: "); Serial.println(numSteps);
  Serial.print("Delay: "); Serial.println(delayMs);
  float elapsed_time = (end_time - start_time)*0.001;
  float est_time = (numSteps*(delayMs+100))/1000000;
  Serial.print("Elapsed Time: "); Serial.println(elapsed_time);
  Serial.print("Estimated Time: "); Serial.println(est_time);
  Serial.print("Error: "); Serial.println(elapsed_time - est_time);
  
}
