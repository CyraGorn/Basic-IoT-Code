#include <DHT.h>
#include <DHT_U.h>

#define LED_PIN D2
#define DHT_SENSOR_PIN  D7
#define DHT_SENSOR_TYPE DHT11

DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht_sensor.begin();
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  float humidity = dht_sensor.readHumidity();
  float temperature_C = dht_sensor.readTemperature();
  float temperature_F = dht_sensor.readTemperature(true);
  if ( isnan(temperature_C) || isnan(temperature_F) || isnan(humidity)) {
    
  } else {
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print("%");

    Serial.print("  |  ");

    Serial.print("Temperature: ");
    Serial.print(temperature_C);
    Serial.print("°C  ~  ");
    Serial.print(temperature_F);
    Serial.println("°F");
  }

  delay(2000);
}
