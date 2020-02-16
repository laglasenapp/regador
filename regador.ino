#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <ArduinoJson.h>


//  char* wifiCredentialsUser[connectionSize] = { "NET_2GE7D83B", "Glasenapp's home"};
//  char* wifiCredentialsPw[connectionSize] = { "65E7D83B", "anschliessend"};
const char* ssid     = "NET_2GE7D83B";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "65E7D83B";     // The password of the Wi-Fi network
const char* server = "www.smarthomecontrol.com.br";
const byte port = 80;
const size_t capacity = JSON_OBJECT_SIZE(8) + 75;

byte mac[6];

unsigned int array1[8];
unsigned int array2[8];
unsigned int array3[8];
unsigned int array4[8];
unsigned int array5[8];
unsigned int array6[8];
unsigned int array7[8];
unsigned int array8[8];
unsigned int array9[8];
unsigned int array10[8];


byte array1_idx = 0;
byte array2_idx = 0;
byte array3_idx = 0;
byte array4_idx = 0;
byte array5_idx = 0;
byte array6_idx = 0;
byte array7_idx = 0;
byte array8_idx = 0;
byte array9_idx = 0;
byte array10_idx = 0;

bool wifiInitiated = false;

bool conectWifi() {
  if (wifiInitiated && WiFi.status() == WL_CONNECTED) {
    return true;
  }

  WiFi.begin(ssid, password);             // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.println(" ...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED && i < 100) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i);
    Serial.print(' ');
  }
  WiFi.macAddress(mac);
  Serial.println('\n');
  Serial.println("Connection established!");
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
  if (WiFi.status() == WL_CONNECTED) {
    wifiInitiated = true;
    delay(1000);
    return true;
  } else {
    delay(5 * 60 * 1000);
    return false;
  }
}

void setup() {
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);

  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, HIGH);
  digitalWrite(D5, HIGH);
  digitalWrite(D6, HIGH);
  digitalWrite(D7, HIGH);
  digitalWrite(D8, HIGH);

  Serial.begin(115200);
  delay(10);
  Serial.println('\n');

  WiFi.mode(WIFI_STA);

}



void loop() {
  while (conectWifi()) {
    break;
  }

  Serial.print("connecting to ");
  Serial.print(server);
  Serial.print(':');
  Serial.println(port);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(server, port)) {
    Serial.println("connection failed");
    delay(5000);
    return;
  }

  // This will send a string to the server
  Serial.println("sending data to server");
  if (client.connected()) {
    client.println("GET http://www.smarthomecontrol.com.br/regador/ HTTP/1.1");
    client.println("Host: smarthomecontrol.com.br");
    client.println("Authorization: Basic cmVnMDpyZWcwcHc=");
    client.println("Connection: close");
    client.println();
  }

  // wait for data to be available
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      delay(60000);
      return;
    }
  }

  Serial.println("receiving from remote server");
  char status[32] = {0};
  client.readBytesUntil('\r', status, sizeof(status));
  if (strcmp(status, "HTTP/1.1 200 OK") != 0) {
    Serial.print(F("Unexpected response: "));
    Serial.println(status);
    return;
  }

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    Serial.println(F("Invalid response"));
    return;
  }

  // Close the connection
  Serial.println();
  Serial.println("closing connection");
  client.stop();

  DynamicJsonDocument jsonBuffer(capacity);
  DeserializationError error = deserializeJson(jsonBuffer, client);
  if (error) {
    Serial.println("Error: ");
    Serial.print(error.c_str());
    delay(5000);
    return;
  }

  byte enabled = jsonBuffer["enabled"]; //enabled
  int s1 = jsonBuffer["s1"]; //s1
  int s2 = jsonBuffer["s2"]; //s2
  int s3 = jsonBuffer["s3"]; //s3
  int s4 = jsonBuffer["s4"]; //s4
  int s5 = jsonBuffer["s5"]; //s5
  int s6 = jsonBuffer["s6"]; //s6
  int s7 = jsonBuffer["s7"]; //s7
  int s8 = jsonBuffer["s8"]; //s8

  if (enabled == 1) {
    if (s1 > 0) {
      digitalWrite(D1, LOW);
      delay(s1);
      digitalWrite(D1, HIGH);
    } //s1

    if (s2 > 0) {
      digitalWrite(D2, LOW);
      delay(s2);
      digitalWrite(D2, HIGH);
    } //s2

    if (s3 > 0) {
      digitalWrite(D3, LOW);
      delay(s3);
      digitalWrite(D3, HIGH);
    } //s3

    if (s4 > 0) {
      digitalWrite(D4, LOW);
      delay(s4);
      digitalWrite(D4, HIGH);
    } //s4

    if (s5 > 0) {
      digitalWrite(D5, LOW);
      delay(s5);
      digitalWrite(D5, HIGH);
    } //s5

    if (s6 > 0) {
      digitalWrite(D6, LOW);
      delay(s6);
      digitalWrite(D6, HIGH);
    } //s6

    if (s7 > 0) {
      digitalWrite(D7, LOW);
      delay(s7);
      digitalWrite(D7, HIGH);
    } //s7

    if (s8 > 0) {
      digitalWrite(D8, LOW);
      delay(s8);
      digitalWrite(D8, HIGH);
    } //s8

  } //if enabled
  Serial.println("Delay of 1 hour");
  delay(300000); // execute once every 5 minutes, don't flood remote service
}
