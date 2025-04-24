#include <WiFi.h>
#include <PubSubClient.h>

#define RXp2 16
#define TXp2 17

// GPIO Pins on esp 32

const char* ssid = "lapi"; // connect esp-32 
const char* password = "27092005"; // using ssid

const char* mqttServer = "172.20.10.5";
const int mqttPort = 1883; // setting up mqtt broker

WiFiClient wifiClient;
PubSubClient client(wifiClient);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on topic: ");
  Serial.print(topic);
  Serial.print(" with payload: ");
  String payloadStr = "";

  for (unsigned int i = 0; i < length; i++) {
    payloadStr += (char)payload[i];
    Serial.print((char)payload[i]);
  }
  Serial.println();
  
  Serial2.println(payloadStr); 
  Serial.println("Sent: " + payloadStr);
}

void setup() {
  Serial.begin(115200); // debugging  
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);   // connect adruino
  Serial.println("ESP32 Ready");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) { // re-connected to mqtt if disconnected
    while (!client.connected()) {
      Serial.println("Connecting to MQTT...");
      if (client.connect("ESP32Client")) {
        Serial.println("Connected to MQTT broker");
        client.subscribe("mpcaProj/comm");
      } else {
        Serial.print("Failed to connect to MQTT, error code: ");
        Serial.println(client.state());
        delay(5000);
      }
    }
  }

  if (Serial2.available()) {
    String message = Serial2.readStringUntil('\n'); // receive from adruino 
    Serial.print("Received from Arduino: ");
    Serial.println(message);
    client.publish("mpcaProj/response", message.c_str());
  }

  client.loop(); // maintain mqtt connection
}




