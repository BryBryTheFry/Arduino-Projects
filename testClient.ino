#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include<Wire.h>
#include <Servo.h>

// Set WiFi credentials
#define WIFI_SSID "TheOtherESP"
#define WIFI_PASS "flashmeifyoucan"

// UDP
WiFiUDP UDP;
IPAddress remote_IP(192, 168, 4, 1);
#define UDP_PORT 4210

const int MPU = 0x68;
int16_t AcX, AcY;
int AccX, AccY;

void setup() {

  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  // Setup IO
  pinMode(2, INPUT);

  // Setup serial port
  Serial.begin(115200);
  Serial.println();

  // Begin WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  WiFi.mode(WIFI_STA);

  // Connecting to WiFi...
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  // Loop continuously while WiFi is not connected
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  }

  // Connected to WiFi
  Serial.println();
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  // Begin UDP port
  UDP.begin(UDP_PORT);
  Serial.print("Opening UDP port ");
  Serial.println(UDP_PORT);

}

void loop() {

  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 12, true);
  AcX = Wire.read() << 8 | Wire.read();
  AcY = Wire.read() << 8 | Wire.read();
  AccX = AcX / -20;
  AccY = AcY / -20;

  Serial.print("Accelerometer: ");
  Serial.print("X = "); Serial.print(AccX);
  Serial.print("     ");
  Serial.print("Y = "); Serial.print(AccY);
  Serial.println(" ");

  // Read button
  char buttonState = digitalRead(2);

  if (-150 < AccY && AccY < 150) {
    delay(0);
  }

  else {
    // Send Packet
    UDP.beginPacket(remote_IP, UDP_PORT);
    UDP.write(buttonState);
    UDP.endPacket();
  }

  delay(100);
}
