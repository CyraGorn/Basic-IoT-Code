#define LED_PIN D2
#define SEN_PIN D4


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(SEN_PIN, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int state = digitalRead(SEN_PIN);
  if(state == HIGH) {
    
  } else {
    Serial.println("OBSTACLE!!!!!!!");
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
  }
}
