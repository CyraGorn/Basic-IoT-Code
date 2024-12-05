#define REL D2

void setup() {
  // put your setup code here, to run once:
  pinMode(REL, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(REL, LOW);
  delay(1000);
  digitalWrite(REL, HIGH);
  delay(1000);
}
