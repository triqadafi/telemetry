#include <ESP8266WiFi.h>
#include <WebSocketClient.h>

WiFiClient client;
WebSocketClient webSocketClient;

const char* TLMT_WIFI_ssid = "PECEPHONE F1";
const char* TLMT_WIFI_pswd = "1sampai5";

char TLMT_WEBSOCKET_host[] = "192.168.43.254";
char TLMT_WEBSOCKET_path[] = "/";
int TLMT_WEBSOCKET_port = 3000;

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println('\n');

  TLMT_wifi_init();


  delay(5000);

  TLMT_wsConnect();
}
void loop() {
  String data;
  if (client.connected()) {
//    webSocketClient.getData(data);
//    if (data.length() > 0) {
//      Serial.print("Received data: ");
//      Serial.println(data);
//    }
    webSocketClient.sendData(String(analogRead(A0)));
  } else {
    Serial.println("Client disconnected.");
    TLMT_wsConnect();
  }
}
void TLMT_wsConnect(){
  // Connect to the websocket server
  if (client.connect(TLMT_WEBSOCKET_host, TLMT_WEBSOCKET_port)) {
    Serial.println("Connected");
  } else {
    Serial.println("Connection failed.");
    ESP.restart();
  }

  // Handshake with the server
  webSocketClient.host = TLMT_WEBSOCKET_host;
  webSocketClient.path = TLMT_WEBSOCKET_path;
  if (webSocketClient.handshake(client)) {
    Serial.println("Handshake successful");
  } else {
    Serial.println("Handshake failed.");
    ESP.restart();
  }
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
