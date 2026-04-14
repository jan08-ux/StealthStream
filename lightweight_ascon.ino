#include <WiFi.h>
#include <PubSubClient.h>
#include <string.h>

extern "C" {
  #include "api.h"
}
// 🔹 WiFi
const char* ssid = "your ssid";
const char* password = "your password";

// 🔹 MQTT
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

// 🔹 ASCON KEY + NONCE
uint8_t key[16] = {0};      // SAME in Python
uint8_t nonce[16] = {0};    // MUST sync

void setup_wifi() {
  Serial.print("Connecting WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting MQTT...");
    if (client.connect("ESP32_ASCON_S3")) {
      Serial.println("Connected!");
    } else {
      Serial.print("Failed, rc=");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  delay(2000);
  Serial.println("ESP32 STARTED");
}


void loop() {

  if (!client.connected()) reconnect();
  client.loop();

  // 🔹 STEP 1: Generate Random Data
  int value = random(0, 100);
  char plaintext[32];
  sprintf(plaintext, "VALUE=%d", value);

  Serial.println("\n====================");
  Serial.print("Original: ");
  Serial.println(plaintext);

  // 🔹 STEP 2: Encrypt using ASCON
  uint8_t ciphertext[128];
  unsigned long long clen;

  crypto_aead_encrypt(
    ciphertext, &clen,
    (uint8_t*)plaintext, strlen(plaintext),
    NULL, 0,
    NULL,
    nonce,
    key
  );

  // 🔹 STEP 3: Convert to HEX
  // Convert nonce + ciphertext to HEX
char hexOutput[300];

// First add nonce (16 bytes)
for (int i = 0; i < 16; i++) {
  sprintf(&hexOutput[i*2], "%02X", nonce[i]);
}

// Then add ciphertext
for (int i = 0; i < clen; i++) {
  sprintf(&hexOutput[32 + i*2], "%02X", ciphertext[i]);
}

Serial.print("Encrypted HEX: ");
Serial.println(hexOutput);

// Send
client.publish("ascon/s3", hexOutput);

// Increment nonce AFTER sending
nonce[0]++;

  // 🔹 STEP 4: Send via MQTT
  client.publish("ascon/s3", hexOutput);

  // 🔹 STEP 5: Update Nonce
  nonce[0]++;

  delay(5000);
}
