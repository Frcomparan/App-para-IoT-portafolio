#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

#define DHTPIN 18
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

const char *ssid = "W_Aula_WB11";
const char *password = "itcolima6";

// const char *ssid = "TP-Link_BEF1_2.4";
// const char *password = "Th3B00k07";

String serverName = "http://0144-187-190-35-202.ngrok-free.app";

unsigned long lastTime = 0;
unsigned long timerDelay = 3000;

const int buttonPinAsc = 2;
const int buttonPinDesc = 4;
const int ledPin = 5;

bool ascRequested = false;
bool descRequested = false;

// Function to set up the WiFi connection
void setupWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

// Function to set up pins and initializations
void setup() {
  Serial.begin(115200);
  dht.begin();

  setupWiFi();

  pinMode(buttonPinAsc, INPUT_PULLUP);
  pinMode(buttonPinDesc, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

// Function to handle sending sensor data to the server
void sendDataToServer(String path, String key, float value) {
  HTTPClient http;
  http.begin((serverName + path).c_str());

  DynamicJsonDocument jsonDoc(64);
  jsonDoc[key] = value;

  String jsonString;
  serializeJson(jsonDoc, jsonString);

  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST(jsonString);

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}

// Function to handle LED status
void handleLEDStatus() {
  String serverPathLed = serverName + "/led";
  HTTPClient http;
  http.begin(serverPathLed.c_str());

  int httpResponseCodeGET = http.GET();

  if (httpResponseCodeGET > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCodeGET);
    String payload = http.getString();
    Serial.println(payload);

    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);

    int led = doc["led"];

    if (led == 0) {
      digitalWrite(ledPin, LOW);
    }
    if (led == 1) {
      digitalWrite(ledPin, HIGH);
    }
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCodeGET);
  }

  http.end();
}

// Function to handle the button actions
void handleButtonActions() {
  if (digitalRead(buttonPinAsc) == HIGH && !ascRequested) {
    ascRequested = true;
  }

  if (digitalRead(buttonPinDesc) == HIGH && !descRequested) {
    descRequested = true;
  }
}

// Function to send the action to the server
void sendActionToServer(String action, int quantity) {
  String serverPath = serverName + "/led";
  HTTPClient http;
  http.begin(serverPath.c_str());

  DynamicJsonDocument jsonDoc(64);
  jsonDoc["action"] = action;
  jsonDoc["quantity"] = quantity;

  String jsonStringCounter;
  serializeJson(jsonDoc, jsonStringCounter);

  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST(jsonStringCounter);

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature)) {
    Serial.println("Error reading DHT sensor!");
    return;
  }

  handleButtonActions();

  if (WiFi.status() == WL_CONNECTED) {
    sendDataToServer("/temperature", "temperature", temperature);
    sendDataToServer("/humidity", "humidity", humidity);

    handleLEDStatus();

    if (ascRequested || descRequested) {
      if ((millis() - lastTime) > timerDelay) {
        if (ascRequested) {
          sendActionToServer("asc", 1);
          ascRequested = false;
        } else if (descRequested) {
          sendActionToServer("desc", 1);
          descRequested = false;
        }
        lastTime = millis();
      }
    }
  } else {
    Serial.println("WiFi Disconnected");
  }

  delay(500);
}
