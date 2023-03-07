#include <esp_now.h>
#include <WiFi.h>

int int_value;
float float_value;
bool bool_value = true;

// MAC Address of responder
uint8_t broadcastAddress[] = { 0xF4, 0x12, 0xFA, 0x57, 0x38, 0xC4 };
uint8_t broadcastAddress2[] = { 0xF4, 0x12, 0xFA, 0x56, 0x32, 0x08 };

// Define a data structure
typedef struct struct_message {
  char a[32];
  int b;
  float c;
  bool d;
} struct_message;

struct_message myData;

// Peer info
esp_now_peer_info_t peerInfo;
esp_now_peer_info_t peerInfo2;

// Callback function called when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);
  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  Serial.print("ESP32 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
  // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the send callback
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  memcpy(peerInfo2.peer_addr, broadcastAddress2, 6);
  peerInfo2.channel = 0;
  peerInfo2.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  if (esp_now_add_peer(&peerInfo2) != ESP_OK) {
    Serial.println("Failed to add peer2");
    return;
  }
  neopixelWrite(48, RGB_BRIGHTNESS, 0, 0);
}

void loop() {

  // Create test data
  int_value = random(1, 20);
  float_value = 1.3 * int_value;
  bool_value = !bool_value;

  // Format structured data
  strcpy(myData.a, "Welcome to the Workshop!");
  myData.b = int_value;
  myData.c = float_value;
  myData.d = bool_value;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
  esp_err_t result2 = esp_now_send(broadcastAddress2, (uint8_t *)&myData, sizeof(myData));

  if (result == ESP_OK) {
    Serial.println("Sending confirmed");
  } else {
    Serial.println("Sending error");
  }

  if (result2 == ESP_OK) {
    Serial.println("Sending confirmed");
  } else {
    Serial.println("Sending error");
  }

  delay(2000);
}