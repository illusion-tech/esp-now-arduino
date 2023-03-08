#include <esp_now.h>
#include <WiFi.h>

int int_value;
float float_value;
bool bool_value = true;

/*接收端板子的Mac地址，这里为两块板子*/
uint8_t broadcastAddress[] = { 0xF4, 0x12, 0xFA, 0x57, 0x38, 0xC4 };
uint8_t broadcastAddress2[] = { 0xF4, 0x12, 0xFA, 0x56, 0x32, 0x08 };

//用于测试的数据
typedef struct struct_message {
  char a[32];
  int b;
  float c;
  bool d;
} struct_message;

struct_message myData;

esp_now_peer_info_t peerInfo;

// 回调函数接收数据
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  // 打印 Mac 地址
  Serial.print("ESP32 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
  // 初始化 ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // 注册发送回调函数
  esp_now_register_send_cb(OnDataSent);

  // 注册通信频道

  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // 配置接收板的Mac地址
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;   
  }
  memcpy(peerInfo.peer_addr, broadcastAddress2, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // 创建测试数据
  int_value = random(1, 20);
  float_value = 1.3 * int_value;
  bool_value = !bool_value;
  strcpy(myData.a, "Welcome to the Workshop!");
  myData.b = int_value;
  myData.c = float_value;
  myData.d = bool_value;

  // 向指定 MAC 地址发送数据
  esp_err_t result = esp_now_send(0, (uint8_t *)&myData, sizeof(myData));
  //esp_err_t result2 = esp_now_send(broadcastAddress2, (uint8_t *)&myData, sizeof(myData));

  if (result == ESP_OK) {
    Serial.println("Sending confirmed");
    neopixelWrite(48, RGB_BRIGHTNESS, 0, 0);
  } else {
    Serial.println("Sending error");
  }

  delay(2000);
  neopixelWrite(48, 0, 0, 0);
}