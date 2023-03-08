#include <esp_now.h>
#include <WiFi.h>

// 定义用于测试的数据
typedef struct struct_message {
  char a[32];
  int b;
  float c;
  bool d;
} struct_message;

struct_message myData;


// 回调函数接收数据
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  neopixelWrite(48, 0, RGB_BRIGHTNESS, 0);
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Data received: ");
  Serial.println(len);
  Serial.print("Character Value: ");
  Serial.println(myData.a);
  Serial.print("Integer Value: ");
  Serial.println(myData.b);
  Serial.print("Float Value: ");
  Serial.println(myData.c);
  Serial.print("Boolean Value: ");
  Serial.println(myData.d);
  Serial.println();
  delay(100);
  neopixelWrite(48, 0, 0, 0);
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  Serial.print("ESP32 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());

  // 初始化 ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // 注册接收回调函数
  esp_now_register_recv_cb(OnDataRecv);
  
}

void loop() {
}