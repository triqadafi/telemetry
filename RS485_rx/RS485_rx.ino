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
  Serial.println("RS485 RX");

  pinMode(TLMT_LED, OUTPUT); digitalWrite(TLMT_LED, HIGH);
  pinMode(TLMT_RS485_DE, OUTPUT); digitalWrite(TLMT_RS485_DE, LOW);
  pinMode(TLMT_RS485_RE, OUTPUT); digitalWrite(TLMT_RS485_RE, LOW);

  RS485Serial.begin(9600);
}

void loop(){
  while (RS485Serial.available()){
//    digitalWrite(TLMT_LED, HIGH);
//    Serial.print("HEAD: 0x"); Serial.println(RS485Serial.read(), HEX);
//    Serial.print("HEAD: 0x"); Serial.println(RS485Serial.read(), HEX);
//    Serial.print("ID: 0x"); Serial.println(RS485Serial.read(), HEX);
//    Serial.print("LENGHT: 0x"); Serial.println(RS485Serial.read(), HEX);
//    Serial.print("INS: 0x"); Serial.println(RS485Serial.read(), HEX);
//      
//    RS485Serial.readBytes(analogX.bytes, sizeof(analogX.bytes));
//    Serial.println(analogX.value);
//    
//    Serial.print("Checksum "); Serial.println(RS485Serial.read());

    TLMT_receive();

    digitalWrite(TLMT_LED, LOW);
  }

//    beginTransmission();
//    RS485Serial.write('A');
//    endTransmission();
}

int TLMT_PACKET_index = 0;
int TLMT_PACKET_checksum = 0;

unsigned char TLMT_PACKET_id = 0;           // id of this frame
unsigned char TLMT_PACKET_length = 0;       // length of this frame
unsigned char TLMT_PACKET_ins = 0;          // instruction of this frame
unsigned char TLMT_PACKET[143];      // parameters (match RX-64 buffer size)
unsigned char TLMT_PACKET_i = 0;        // index in param buffer

unsigned long TLMT_millis = 0;
int TLMT_PACKET_count = 0;

void TLMT_receive(){
  if(TLMT_PACKET_index == 0){
    if(RS485Serial.read() == 0xff){
      TLMT_PACKET_index = 1;
    }
  }else if(TLMT_PACKET_index == 1){   // another start byte
      if(RS485Serial.read() == 0xff)
          TLMT_PACKET_index = 2;
      else
          TLMT_PACKET_index = 0;
  }else if(TLMT_PACKET_index == 2){   // next byte is length
      analogX.bytes[0] = RS485Serial.read();
      TLMT_PACKET_index = 3;
  }else if(TLMT_PACKET_index == 3){   // next byte is instruction
      analogX.bytes[1] = RS485Serial.read();
      Serial.println(analogX.value);
      TLMT_PACKET_count++;
      TLMT_PACKET_index = 0;
  }
  if(millis() - TLMT_millis > 1000){
    Serial.println("DPS: " + String(TLMT_PACKET_count));
    TLMT_millis = millis();
    TLMT_PACKET_count = 0;
  }
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
