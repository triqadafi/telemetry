#include <SoftwareSerial.h>

#define TLMT_RS485_RO 12 //RX
#define TLMT_RS485_DI 9 //TX

#define TLMT_RS485_DE 11  //RS485 Direction control
#define TLMT_RS485_RE 10  //RS485 Direction control

#define TLMT_RS485_TRANSMIT HIGH
#define TLMT_RS485_RECEIVE LOW

#define TLMT_LED  13

SoftwareSerial RS485Serial(TLMT_RS485_RO, TLMT_RS485_DI); // RX, TX

int byteReceived;
int byteSend;

union ub_uint16{uint16_t value; byte bytes[2];};
ub_uint16 analogX;


void setup(){
  // Start the built-in serial port, probably to Serial Monitor
  Serial.begin(115200);
  Serial.println("RS485 TX");

  pinMode(TLMT_LED, OUTPUT); digitalWrite(TLMT_LED, HIGH);
  pinMode(TLMT_RS485_DE, OUTPUT); digitalWrite(TLMT_RS485_DE, LOW);
  pinMode(TLMT_RS485_RE, OUTPUT); digitalWrite(TLMT_RS485_RE, LOW);

  RS485Serial.begin(9600);

  analogX.value = 234;
}

void loop(){
    beginTransmission();
    RS485Serial.write(0xFF); 
    RS485Serial.write(0xFF); 
    RS485Serial.write(analogX.bytes, sizeof(analogX.bytes));

//    int checksum = 255 - ((0x01 + 0x04 + 0x00 + analogX.bytes[0] + analogX.bytes[1])%256);
//    RS485Serial.write(checksum); //INS

    endTransmission();

//  while (RS485Serial.available()){
//    digitalWrite(TLMT_LED, HIGH);
//    
//    char _test = RS485Serial.read();
//    Serial.print(_test);
//    
//    digitalWrite(TLMT_LED, LOW);
//  }
}

void beginTransmission(){
  digitalWrite(TLMT_LED, HIGH);
  digitalWrite(TLMT_RS485_DE, TLMT_RS485_TRANSMIT);
  digitalWrite(TLMT_RS485_RE, TLMT_RS485_TRANSMIT);
}
void endTransmission(){
  digitalWrite(TLMT_RS485_DE, TLMT_RS485_RECEIVE);
  digitalWrite(TLMT_RS485_RE, TLMT_RS485_RECEIVE);
  digitalWrite(TLMT_LED, LOW);
}
