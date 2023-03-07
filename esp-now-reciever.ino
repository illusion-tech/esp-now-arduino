#include <esp_now.h>
#include <WiFi.h>

// Define a data structure
typedef struct struct_message {
  char a[32];
  int b;
  float c;
  bool d;
} struct_message;

struct_message myData;


// Callback function executed when data is received
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

  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  Serial.print("ESP32 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());

  // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register callback function
  esp_now_register_recv_cb(OnDataRecv);
  
}

void loop() {
}