#include <DHT.h>

#define DHTPIN 2         
#define DHTTYPE DHT11    
DHT dht(DHTPIN, DHTTYPE);

const int soundSensorPin = A0;      
const int vibrationSensorPin = A1;
const float bias = 1.65;        
const float Vref = 0.00631;     

void handleReceivedCommand(String payload) {
  payload.trim();

  if (payload.equalsIgnoreCase("sound")) {
    const int numSamples = 1000;
    float sumSquares = 0.0;

    for (int i = 0; i < numSamples; i++) {
      int sensorValue = analogRead(soundSensorPin);
      float voltage = sensorValue * (3.3 / 1023.0);
      float amplitude = voltage - bias;
      sumSquares += amplitude * amplitude;
      delayMicroseconds(100);
    }

    float rms = sqrt(sumSquares / numSamples);
    float dB = 20 * log10(rms / Vref);
    Serial.println("Noise Level: " + String(dB, 2) + " dB");
  } 
  else if (payload.equalsIgnoreCase("temperature")) {
    float temp = dht.readTemperature();
    if (isnan(temp)) {
      Serial.println("DHT11 error: Failed to read temperature");
    } else {
      Serial.println("Temperature: " + String(temp, 2) + " °C");
    }
  }
  else if (payload.equalsIgnoreCase("humidity")) {
    float hum = dht.readHumidity();
    if (isnan(hum)) {
      Serial.println("DHT11 error: Failed to read humidity");
    } else {
      Serial.println("Humidity: " + String(hum, 2) + " %");
    }
  }
  else if (payload.equalsIgnoreCase("vibration")) {
    int sensorValue = analogRead(vibrationSensorPin);
    float voltage = sensorValue * (3.3 / 1023.0);
    float amplitude = fabs(voltage - bias);  
    Serial.println("Vibration Amplitude: " + String(amplitude, 3) + " V");
  }
  else {
    Serial.println("Unknown Command");
  }
}

void setup() {
  Serial.begin(9600);
  delay(1000);
  dht.begin();
}

void loop() {
  if (Serial.available()) {
    String msg = Serial.readStringUntil('\n');
    handleReceivedCommand(msg);
  }
}
