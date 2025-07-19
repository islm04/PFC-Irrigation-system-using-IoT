#define BLYNK_TEMPLATE_ID YOUR_TEMPLATE_ID
#define BLYNK_TEMPLATE_NAME "irrigation system"
#define BLYNK_AUTH_TOKEN YOUR_TOKEN
#define BLYNK_PRINT Serial

#include <HardwareSerial.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <WiFiClient.h>

#define RX_PIN 16
#define TX_PIN 17
#define BLYNK_PRINT Serial

//connexion au wifi
char ssid[] = "OPPO A7";
char pass[] = "WelcomeFr";

HardwareSerial ArduinoSerial(1);
BlynkTimer timer;

int autoMode = 0;

//virtual pin de la pompe
BLYNK_WRITE(V0) {
  int stateV0 = param.asInt();
  if (autoMode == 0) {
    if (stateV0 == 1) {
      ArduinoSerial.println("pump_on");
    } else {
      ArduinoSerial.println("pump_off");
    }
  } else {
    Serial.println("Wait for conditions");
    delay(1000);
  }
}

// virtual pin de mode de fonctionnement
BLYNK_WRITE(V1) {
  int selectedMode = param.asInt();
  if (selectedMode == 1) {
    autoMode = 1;
    ArduinoSerial.println("auto_mode");
    Blynk.virtualWrite(V0, 0);
  } else {
    autoMode = 0;
  }
}

void setup() {
  Serial.begin(115200); 
  delay(100);
  connectWifi();
  ArduinoSerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN); 
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();
  readFromArduino();
}

void readFromArduino() {
  while (ArduinoSerial.available()) {
    String line = ArduinoSerial.readStringUntil('\n');
    line.trim();
    Serial.println("Received: " + line);

    // Split by ';'
    int start = 0;
    while (start < line.length()) {
      int sep = line.indexOf(';', start);
      if (sep == -1) break;

      String pair = line.substring(start, sep);
      int colon = pair.indexOf(':');
      if (colon != -1) {
        String key = pair.substring(0, colon);
        String val = pair.substring(colon + 1);

        float fval = val.toFloat(); // convert to float

        //declaration les variables pour blynk
        if (key == "humidity") {
          Blynk.virtualWrite(V4, fval); 
        } else if (key == "temp") {
          Blynk.virtualWrite(V5, fval);
        } else if (key == "moisture") {
          Blynk.virtualWrite(V2, fval); 
        } else if (key == "flow") {
          Blynk.virtualWrite(V3, fval); 
        } else if (key == "rain") {
          Blynk.virtualWrite(V6, val);
        } else if (key == "level") {
          Blynk.virtualWrite(V7, fval);
        }
      }
      start = sep + 1;
    }
  }
}

void connectWifi() {
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");
  Serial.println("IP Address: " + WiFi.localIP().toString());
}