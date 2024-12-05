#define LEDPIN D2
#define BUTPIN D3

void setup() {
  // put your setup code here, to run once:
  pinMode(LEDPIN, OUTPUT);
  pinMode(BUTPIN, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int state = digitalRead(BUTPIN);
  if(state == HIGH) {
    
  } else {
    digitalWrite(LEDPIN, HIGH);
    digitalWrite(LEDPIN, LOW);
  }
}
