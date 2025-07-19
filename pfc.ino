#include <DHT.h>

#define DHTPIN 5
#define DHTTYPE DHT22
#define RAIN_SENSOR_AO A0
#define RAIN_SENSOR_DO 3
#define SOIL_SENSOR_AO A1
#define SOIL_SENSOR_DO 4
#define waterFlow_sensor 2
#define waterPump 6
#define waterSensorPin A2

DHT dht(DHTPIN, DHTTYPE);
volatile int flowPulseCount= 0;
float flowRate = 0;
unsigned long lastFlowTime = 0;
unsigned long lastSensorTime = 0;

bool auto_mode_enabled = false;

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(RAIN_SENSOR_DO, INPUT);
  pinMode(SOIL_SENSOR_DO, INPUT);
  pinMode(waterFlow_sensor, INPUT);
  pinMode(waterPump, OUTPUT);
  digitalWrite(waterPump, HIGH);
  attachInterrupt(digitalPinToInterrupt(waterFlow_sensor), countPulse, RISING);
}

void loop() {
  sensorsData();
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim(); // clean newline or spaces
    if (cmd == "pump_on") {
      auto_mode_enabled = 0;
      pump_on();
    } else if (cmd == "pump_off") {
      auto_mode_enabled = 0;
      pump_off();
    } else if (cmd == "auto_mode") {
      auto_mode_enabled = true;
    }
  }

  if(auto_mode_enabled) {
    autoMode();
  }
}

void autoMode() {
  bool is_raining = isRaining();
  float temp = dht.readTemperature();
  int moisture = analogRead(SOIL_SENSOR_AO);
  moisture = map(moisture, 0, 1023, 100, 0);

  if (is_raining == 1) {
    Serial.println("it is raining!");
    pump_off();
  } else {
    if (moisture < 40 && temp < 30.0) {
      pump_on();
    } else if (temp >= 30.0 && moisture < 50) {
      pump_on();
    } else {
      pump_off();
    }
  }
}

void sensorsData() {
  unsigned long now = millis();
  if(now - lastSensorTime >=2000) {
    lastSensorTime = now;

    float humidity = dht.readHumidity();
    float temp = dht.readTemperature();
    if (isnan(humidity) || isnan(temp)) {
      Serial.println("Failed to read DHT");
      return;
    }

    int waterLevel = readWaterLevel();

    int moisture = analogRead(SOIL_SENSOR_AO);
    moisture = map(moisture, 0, 1023, 100, 0);

    float debit = waterFlow();

    //envoyer tous les données dans une ligne
    Serial.print("humidity:" + String(humidity, 1) + ";");
    Serial.print("temp:" + String(temp, 1) + ";");
    Serial.print("moisture:" + String(moisture) + ";");
    Serial.print("flow:" + String(debit, 1) + ";");
    Serial.println("level:" + String(waterLevel) + ";");

    bool is_raining = isRaining();
    if (is_raining) {
      Serial.println("rain:il pleut;");
    } else {
      Serial.println("rain:pas de pluie;");
    }
  }
}

bool isRaining() {
  int rainState = digitalRead(RAIN_SENSOR_DO);

  if (rainState == LOW) {
    return true; // raining
  } else {
    return false; // not raining
  }
}

int readWaterLevel() {
  int total = 0;
  const int samples = 10;
  for (int i = 0; i < samples; i++) {
    total += analogRead(waterSensorPin);
    delay(10); // small delay to allow stabilization
  }
  int average = total / samples;

  // Adjust map range based on your calibration
  int level = map(average, 450, 700, 0, 100);
  level = constrain(level, 0, 100); // to avoid out-of-range values
  return level;
}

