#define LED D4
#define SEN D2

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  pinMode(SEN, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int flame_state = digitalRead(SEN);
  if(flame_state == HIGH) {
    
  } else {
    digitalWrite(LED, HIGH);
    delay(1000);
    digitalWrite(LED, LOW);
  }
}
