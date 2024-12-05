#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Pin definitions
#define LEDPIN_D1 D1        // Pin của LED cảnh báo trên IoT Gateway

// WiFi và MQTT broker thông tin
const char* ssid = "mitranek";
const char* password = "12345679";
const char* mqtt_server = "rabb1tflow3r.net";  // Địa chỉ IP của MQTT broker
const char* mqtt_username = "mqtt";          // Tên người dùng MQTT
const char* mqtt_password = "mqtt";          // Mật khẩu MQTT

WiFiClient espClient;
PubSubClient client(espClient);

// Khởi tạo pin LED
void setup() {
  Serial.begin(115200);
  pinMode(LEDPIN_D1, OUTPUT);
  setupWiFi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(mqttCallback);
}

void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();
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
    if (client.connect("ESP8266_Gateway", mqtt_username, mqtt_password)) {
      Serial.println("Connected to MQTT");
      client.subscribe("iot/gateway");  // Nhận thông điệp từ IoT Node
    } else {
      Serial.print("Failed to connect, state: ");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

// Hàm xử lý message nhận được từ IoT Node
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, payload, length);

  if (error) {
    Serial.print("Error parsing JSON: ");
    Serial.println(error.f_str());
    return;
  }

  // Đọc thông tin từ thông điệp
  const char* status = doc["status"];
  float temperature = doc["temperature"];
  float humidity = doc["humidity"];
  int gasLevel = doc["gasLevel"];

  Serial.println("Message received from IoT Node:");
  Serial.print("Status: "); Serial.println(status);
  Serial.print("Temperature: "); Serial.print(temperature); Serial.println(" °C");
  Serial.print("Humidity: "); Serial.print(humidity); Serial.println(" %");
  Serial.print("Gas Level: "); Serial.println(gasLevel);

  // Kiểm tra trạng thái và bật/tắt LED
  if (strcmp(status, "Alert") == 0) {
    digitalWrite(LEDPIN_D1, HIGH);  // Bật LED khi nhận cảnh báo
    Serial.println("Alert detected! LED ON.");

    // Chuyển tiếp thông điệp lên MQTT broker
    sendToMQTTBroker(temperature, humidity, gasLevel, status);

  } else if (strcmp(status, "Safe") == 0) {
    digitalWrite(LEDPIN_D1, LOW);  // Tắt LED khi trạng thái an toàn
    sendToMQTTBroker(temperature, humidity, gasLevel, status);
    Serial.println("Safe status received. LED OFF.");
  }
}

// Hàm gửi thông điệp cảnh báo lên MQTT Broker
void sendToMQTTBroker(float temperature, float humidity, int gasLevel, const char* status) {
  StaticJsonDocument<200> doc;
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["gasLevel"] = gasLevel;
  doc["status"] = status;  // Trạng thái "Alert" hoặc "Safe"

  String payload;
  serializeJson(doc, payload);

  // Gửi thông điệp cảnh báo lên topic "iot/broker"
  client.publish("iot/broker", payload.c_str());
  Serial.print("Message forwarded to MQTT Broker: ");
  Serial.println(payload);
}
