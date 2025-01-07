#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <Servo.h>

// WiFi bilgileri
const char* ssid = "VOLKAN";
const char* password = "Q2mt9hjbfq";

// Servo nesneleri
Servo servo1;  // Taban
Servo servo2;  // Omuz
Servo servo3;  // Dirsek
Servo servo4;  // Kıskaç

// Servo pinleri
const int SERVO1_PIN = D1;  // GPIO5
const int SERVO2_PIN = D2;  // GPIO4
const int SERVO3_PIN = D3;  // GPIO0
const int SERVO4_PIN = D4;  // GPIO2

// WiFi sunucusu
WiFiServer server(80);

// Global değişkenler
int currentPos1 = 90;
int currentPos2 = 90;
int currentPos3 = 90;
int currentPos4 = 90;

void setup() {
  Serial.begin(115200);
  Serial.println("\nRobot Kol Başlatılıyor...");
  
  // Servo pinlerini ayarla
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);
  servo4.attach(SERVO4_PIN);
  
  // Başlangıç pozisyonları
  servo1.write(90);
  servo2.write(90);
  servo3.write(90);
  servo4.write(90);
  
  // WiFi'ye bağlan
  WiFi.begin(ssid, password);
  
  Serial.print("WiFi'ye bağlanılıyor");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.print("Bağlandı! IP adresi: ");
  Serial.println(WiFi.localIP());
  
  // Sunucuyu başlat
  server.begin();
  Serial.println("Server başlatıldı");
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  while (client.connected()) {
    if (client.available()) {
      String json = client.readStringUntil('\n');
      Serial.print("Alınan JSON: ");
      Serial.println(json);
      handleJson(json);
    }
  }
}

void handleJson(String json) {
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, json);
  
  if (error) {
    Serial.print("JSON parse hatası: ");
    Serial.println(error.c_str());
    return;
  }
  
  // JSON'dan açı değerlerini al
  int targetPos1 = doc["s1"] | 90;  // Varsayılan 90
  int targetPos2 = doc["s2"] | 90;
  int targetPos3 = doc["s3"] | 90;
  int targetPos4 = doc["s4"] | 90;
  
  // Debug çıktısı
  Serial.printf("Hedef açılar: %d,%d,%d,%d\n", targetPos1, targetPos2, targetPos3, targetPos4);
  
  // Servo açılarını direkt güncelle
  servo1.write(targetPos1);
  servo2.write(targetPos2);
  servo3.write(targetPos3);
  servo4.write(targetPos4);
  
  // Mevcut pozisyonları güncelle
  currentPos1 = targetPos1;
  currentPos2 = targetPos2;
  currentPos3 = targetPos3;
  currentPos4 = targetPos4;
  
  // Debug çıktısı
  Serial.printf("Servolar güncellendi: %d,%d,%d,%d\n", currentPos1, currentPos2, currentPos3, currentPos4);
}
