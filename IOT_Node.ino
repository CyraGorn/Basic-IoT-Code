#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <ArduinoJson.h>

// Pin definitions
#define DHTPIN D2           // Pin của DHT11
#define LEDPIN D4           // Pin của đèn LED (cho khí gas)
#define OBSTACLE_LED_PIN D6  // Pin của đèn LED (cho vật cản)
#define MQPIN A0            // Pin của MQ sensor
#define OBSTACLE_PIN D5     // Pin của cảm biến vật cản HW201

// WiFi và MQTT broker thông tin
const char* ssid = "mitranek";
const char* password = "12345679";
const char* mqtt_server = "rabb1tflow3r.net";  // Địa chỉ IP của MQTT broker
const char* mqtt_username = "mqtt";          // Tên người dùng MQTT
const char* mqtt_password = "mqtt";          // Mật khẩu MQTT

WiFiClient espClient;
PubSubClient client(espClient);

// DHT11 sensor
DHT dht(DHTPIN, DHT11);    // Khởi tạo đối tượng DHT

// Cài đặt ngưỡng
const int gasThreshold = 400;    // Ngưỡng khí gas
const float tempThreshold = 50.0; // Ngưỡng nhiệt độ

unsigned long lastGasRead = 0;
unsigned long gasReadInterval = 2000;  // Đọc khí gas mỗi 2 giây

void setup() {
  // Khởi tạo Serial Monitor
  Serial.begin(115200);

  // Khởi tạo các cảm biến
  dht.begin();
  pinMode(LEDPIN, OUTPUT);
  pinMode(OBSTACLE_LED_PIN, OUTPUT);
  pinMode(MQPIN, INPUT);
  pinMode(OBSTACLE_PIN, INPUT);

  // Kết nối WiFi
  setupWiFi();

  // Kết nối tới MQTT broker
  client.setServer(mqtt_server, 1883);
}

void loop() {
  // Kiểm tra kết nối MQTT và kết nối lại nếu bị mất kết nối
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();

  unsigned long currentMillis = millis();

  // Kiểm tra khí gas mỗi 2 giây
  if (currentMillis - lastGasRead >= gasReadInterval) {
    lastGasRead = currentMillis;
    float temperature = readTemperature();
    float humidity = readHumidity();
    int gasLevel = readGasLevel();

    // Hiển thị dữ liệu ra Serial Monitor
    printSensorData(temperature, humidity, gasLevel);

    // Gửi thông điệp dựa trên trạng thái khí gas
    if (gasLevel > gasThreshold || temperature > tempThreshold) {
      sendAlertMessage(temperature, humidity, gasLevel, "Alert");  // Gửi cảnh báo
      digitalWrite(LEDPIN, HIGH);  // Bật LED khí gas
    } else {
      sendAlertMessage(temperature, humidity, gasLevel, "Safe");   // Gửi trạng thái an toàn
      digitalWrite(LEDPIN, LOW);  // Tắt LED khí gas
    }
  }
}

// Hàm kết nối WiFi
void setupWiFi() {
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000); // Chờ 1 giây giữa các lần kiểm tra
    Serial.print("."); // In dấu chấm thay vì dòng mới
  }
  Serial.println("\nWiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// Hàm kết nối lại MQTT nếu mất kết nối
void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("ESP8266_Node", mqtt_username, mqtt_password)) {
      Serial.println("Connected to MQTT");
    } else {
      Serial.print("Failed to connect, state: ");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

// Hàm gửi thông điệp cảnh báo đến IoT Gateway
void sendAlertMessage(float temperature, float humidity, int gasLevel, const char* status) {
  StaticJsonDocument<200> doc;
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["gasLevel"] = gasLevel;
  doc["status"] = status;  // Trạng thái "Alert" hoặc "Safe"

  String payload;
  serializeJson(doc, payload);

  // Gửi message đến IoT Gateway qua topic "iot/gateway"
  client.publish("iot/gateway", payload.c_str());
  Serial.print("Message sent to IoT Gateway: ");
  Serial.println(payload);
}

// Hàm đọc dữ liệu cảm biến
float readTemperature() { return dht.readTemperature(); }
float readHumidity() { return dht.readHumidity(); }
int readGasLevel() { return analogRead(MQPIN); }
void printSensorData(float temperature, float humidity, int gasLevel) {
  Serial.print("Temp: "); Serial.print(temperature);
  Serial.print(" °C, Humidity: "); Serial.print(humidity);
  Serial.print(" %, Gas: "); Serial.println(gasLevel);
}
