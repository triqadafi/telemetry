#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

const char* TLMT_WIFI_ssid = "PECEPHONE F1";
const char* TLMT_WIFI_pswd = "1sampai5";

void setup(void){
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');

  TLMT_wifi_init();

  // DNS Responder
  if (MDNS.begin("esp8266")) {
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }

  server.on("/", TLMT_directory_root); // root
  server.onNotFound(TLMT_directory_404);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient(); // listen
}

void TLMT_directory_root() {
  String _analog = String(analogRead(A0));
  server.send(200, "text/plain", _analog);
  //server.send(200, "text/json", "{\"x\":100}");
}

void TLMT_directory_404(){
  server.send(404, "text/plain", "404: Not found");
}

void TLMT_wifi_init(){
  WiFi.begin(TLMT_WIFI_ssid, TLMT_WIFI_pswd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());
}
