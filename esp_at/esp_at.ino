#include "SoftwareSerial.h"

String data;

String TLMT_WIFI_ssid = "PECEPHONE F1";
String TLMT_WIFI_pswd = "1sampai5";

String TLMT_server_ip = "192.168.43.254";
String TLMT_server_port = "80";
String TLMT_server_uri = "/accel";

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  
  // reset();
  connectWifi();
}

//reset the esp8266 module
void reset(){
  Serial1.println("AT+RST");
  delay(1000);
  if(Serial1.find("OK")) Serial.println("Module Reset");
}

void connectWifi(){
  bool _search = true;
  while(_search){
    String cmd = "AT+CWJAP=\"" +TLMT_WIFI_ssid+"\",\"" + TLMT_WIFI_pswd + "\"";
    Serial1.println(cmd);
    delay(4000);
    if(Serial1.find("OK")){
      Serial.println("Connected!");
      _search = false;
    }else{
      Serial.println("Cannot connect to wifi");
    }
  }
}

int TMLT_mode = 0;
String TMLT_POST_raw;
int MEMS_x = 0;

void loop () {
  MEMS_x = analogRead(A0);
  if(TMLT_mode == 0){
    Serial1.println("AT+CIPSTART=\"TCP\",\"" + TLMT_server_ip + "\"," + TLMT_server_port);//start a TCP connection.
    TMLT_mode = 1;
  }else if(TMLT_mode == 1){
    if(Serial1.find("OK")) {
      Serial.println("TCP connection ready");
      String data = "x=" + String(MEMS_x);
      TMLT_POST_raw =
        "POST " + TLMT_server_uri + " HTTP/1.1\r\n" +
        "Host: " + TLMT_server_ip + "\r\n" +
        "Accept: *" + "/" + "*\r\n" +
        "Content-Length: " + data.length() + "\r\n" +
        "Content-Type: application/x-www-form-urlencoded\r\n" +
        "\r\n" + data;
        
      Serial1.print("AT+CIPSEND=");
      Serial1.println(TMLT_POST_raw.length());
      TMLT_mode = 2;
    }else{
      TMLT_mode = 1;
    }
  }else if(TMLT_mode == 2){
    if(Serial1.find(">")){
      Serial.println("Sending..");
      Serial1.print(TMLT_POST_raw);
      if(Serial1.find("SEND OK")){
        Serial.println("Packet sent");
        while (Serial1.available()) {
          String tmpResp = Serial1.readString();
          Serial.println(tmpResp);
        }
        // close the connection
        Serial1.println("AT+CIPCLOSE");
      }
    }
    TMLT_mode = 0;
  }
}
