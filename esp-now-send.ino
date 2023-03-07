#include <WiFi.h>
#include <esp_now.h>

// 设置掌控板声音传感器与光线传感器引脚编号
const int soundPin = 36;
const int lightPin = 39;

// 设置数据结构体
typedef struct struct_message {
  String board_name;
  double light;
  double sound;
} struct_message;

struct_message myData;

// 接收设备的 MAC 地址
//F4:12:FA:57:38:C4
uint8_t broadcastAddress[] = { 0xF4, 0x12, 0xFA, 0x57, 0x38, 0xC4 };

// 数据发送回调函数
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  Serial.print("Packet to: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  Serial.print("Send status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  Serial.println();
}

void setup() {
  Serial.begin(115200);

  // 初始化 ESP-NOW
  WiFi.mode(WIFI_STA);
  attachInterrupt(digitalPinToInterrupt(0), pin_ISR, RISING);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // 设置发送数据回调函数
  esp_now_register_send_cb(OnDataSent);

  // 绑定数据接收端
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // 检查设备是否配对成功
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  
}

void pin_ISR() {
  Serial.println("Interrupt!");
  WiFi.mode(WIFI_MODE_STA);
  Serial.print("ESP32 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
}

void loop() {
  // 设置要发送的数据
  myData.board_name = "mPython_#1";
  myData.light = 1.0;
  myData.sound = 1.0;

  // 发送数据
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
  neopixelWrite(48, RGB_BRIGHTNESS, 0, 0);
  // 检查数据是否发送成功
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }
  delay(1000);
  neopixelWrite(48, 0, 0, 0);
}