#define DO_PIN D7
#define LED_PIN D2
#define AO_PIN A0

void setup() {
  // Initialize the Serial to communicate with the Serial Monitor.
  Serial.begin(9600);
  // initialize the esp8266's pin as an input
  pinMode(DO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  Serial.println("Warming up the MQ2 sensor");
  delay(10000);  // wait for the MQ2 to warm up
}

void loop() {
  int gasState = digitalRead(DO_PIN);
  int gasValue = analogRead(AO_PIN);

  if (gasState == HIGH) {
    Serial.println("The gas is NOT present");
    digitalWrite(LED_PIN, LOW);
  } else {
    Serial.println("The gas is present");
    digitalWrite(LED_PIN, HIGH);
    Serial.print("MQ2 sensor AO value: ");
    Serial.println(gasValue);
  }
}
