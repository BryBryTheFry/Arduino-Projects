#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
 
// Set AP credentials
#define AP_SSID "TheOtherESP"
#define AP_PASS "flashmeifyoucan"
 
// UDP
WiFiUDP UDP;
IPAddress local_IP(192,168,4,1);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);
#define UDP_PORT 4210
 
// UDP Buffer
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
 
void setup() {
 
  // Setup LED pin
  pinMode(2, OUTPUT);
   
  // Setup serial port
  Serial.begin(115200);
  Serial.println();
 
  // Begin Access Point
  Serial.println("Starting access point...");
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP(AP_SSID, AP_PASS);
  Serial.println(WiFi.localIP());
 
  // Begin listening to UDP port
  UDP.begin(UDP_PORT);
  Serial.print("Listening on UDP port ");
  Serial.println(UDP_PORT);
 
}
 
void loop() {
 
  // Receive packet
  UDP.parsePacket();
  UDP.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
  if (packetBuffer[0]){
    digitalWrite(2, HIGH);
  } else {
    digitalWrite(2, LOW);
  }      
 
} 
