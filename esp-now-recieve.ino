#include <WiFi.h>
#include <esp_now.h>

// 设置数据结构体
typedef struct struct_message {
  String board_name;
  double light;
  double sound;
} struct_message;

struct_message myData;

// 数据接收回调函数
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("board_name: ");
  Serial.println(myData.board_name);
  Serial.print("light: ");
  Serial.println(myData.light);
  Serial.print("sound:");
  Serial.println(myData.sound);
  Serial.println();
  neopixelWrite(48, 0, RGB_BRIGHTNESS, 0);
}

void setup() {
  Serial.begin(115200);

  // 初始化 ESP-NOW
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // 设置接收数据回调函数
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  neopixelWrite(48, 0, RGB_BRIGHTNESS, 0);
  delay(1000);
  neopixelWrite(48, 0, 0, 0);
}