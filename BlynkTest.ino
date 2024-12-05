#define LED1_PIN D5
#define LED2_PIN D7
#define OBS_PIN D1
#define GAS_STATE_PIN D3
#define GAS_VALUE_PIN A0


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(OBS_PIN, INPUT);
  pinMode(GAS_STATE_PIN, INPUT);
  pinMode(GAS_VALUE_PIN, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int OBS_state = digitalRead(OBS_PIN);
  if(OBS_state == HIGH) {
    
  } else {
    Serial.println("OBSTACLE!!!!!!!");
    digitalWrite(LED1_PIN, HIGH);
    delay(1000);
    digitalWrite(LED1_PIN, LOW);
  }


  int gasState = digitalRead(GAS_STATE_PIN);
  int gasValue = analogRead(GAS_VALUE_PIN);

  if (gasState == HIGH) {
    
  } else {
    Serial.println("The gas is present");
    digitalWrite(LED2_PIN, HIGH);
    Serial.print("Sensor AO value: ");
    Serial.println(gasValue);
    digitalWrite(LED2_PIN, LOW);
  }
}
