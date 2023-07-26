#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

int pHSense = A0;
int samples = 10;
int turbidity;
float adc_resolution = 1024.0;

const char* ssid = "Kost Iskandar";
const char* password = "setiawan";

//Web/Server address to read/write from
const char* host = "http://192.168.1.101/api/send";  //your IP/web server address

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);


void setup() {
  Serial.begin(9600);
  wificheck(ssid, password);

  lcd.begin();
}

void wificheck(const char* ssid, const char* password) {
  WiFi.mode(WIFI_OFF);  //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);  //This line hides the viewing of ESP as wifi hotspot

  WiFi.begin(ssid, password);  //Connect to your WiFi router
  Serial.println("");

  Serial.print("Connecting");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting");
  lcd.setCursor(0, 1);
  lcd.print("Wifi ...");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connected to ");
  lcd.setCursor(0, 1);
  lcd.print("ssid");
}

void sendData(float ph, int turbidity) {
  HTTPClient http;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ph " + String(ph) + " NTU " + String(turbidity));
  delay(2000);
  //Prepare data
  String postData;

  //prepare request
  postData = "ph=" + String(ph) + "&turbidity=" + String(turbidity);
  http.begin(host);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode = http.POST(postData);
  String payload = http.getString();

  Serial.print("HTTP Response code: ");
  if (httpCode == 200) {
    lcd.setCursor(0, 1);
    lcd.print("Data disimpan :)");
    Serial.println("Terkirim");
  } else {
    Serial.println("Koneksi Terputus");
    lcd.setCursor(0, 1);
    lcd.print("Terputus :(");
  }

  // Free resources
  http.end();
}

float round_to_dp(float in_value, int decimal_place) {
  float multiplier = powf(10.0f, decimal_place);
  in_value = roundf(in_value * multiplier) / multiplier;
  return in_value;
}

void loop() {

  int sensorValue = analogRead(D3);
  // Serial.println(sensorValue);
  // int turbidity = map(sensorValue, 0, 750, 100, 0);
  turbidity = map(sensorValue, 0, 750, 100, 0);
  delay(100);


  int measurings = 0;

  for (int i = 0; i < samples; i++) {
    measurings += analogRead(pHSense);
    delay(10);
  }

  float voltage = 3.3 / adc_resolution * measurings / samples;
  float ph = (7 + ((2.5 - voltage) / 0.18));
  Serial.print("pH= ");
  Serial.println(ph);
  Serial.print("turbidity= ");
  Serial.println(turbidity);
  sendData(ph, turbidity);


  delay(1000);  // Wait for 1 second before the next reading.
}
