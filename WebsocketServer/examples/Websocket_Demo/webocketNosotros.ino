#include <SPI.h>
#include <Ethernet.h>

// Enabe debug tracing to Serial port.
#define DEBUG

// Here we define a maximum framelength to 64 bytes. Default is 256.
#define MAX_FRAME_LENGTH 64

#include <WebSocket.h>

byte mac[] = { 0x52, 0x4F, 0x43, 0x4B, 0x45, 0x54 };
byte ip[] = { 192, 168, 1 , 200 };
int entrada = 22;
int led = 7;
int led2 = 26;
int led3 = 28;
// Create a Websocket server
WebSocket wsServer;

void onConnect(WebSocket &socket) {
  Serial.println("onConnect called");
}


// You must have at least one function with the following signature.
// It will be called by the server when a data frame is received.
void onData(WebSocket &socket, char* dataString, byte frameLength) {
  
#ifdef DEBUG
  Serial.print("Got data: ");
  Serial.write((unsigned char*)dataString, frameLength);
  //Serial.write();
  //Serial.write((char&)dataString, frameLength);
  Serial.println();
#endif
  opcionLed(atoi(dataString));
  // Just echo back data for fun.
  socket.send(dataString, frameLength);
}

void onDisconnect(WebSocket &socket) {
  Serial.println("onDisconnect called");
}



void setup() {
  pinMode(entrada,INPUT);
  pinMode(led,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
#ifdef DEBUG  
  Serial.begin(115200);
#endif
  Ethernet.begin(mac, ip);
  
  wsServer.registerConnectCallback(&onConnect);
  wsServer.registerDataCallback(&onData);
  wsServer.registerDisconnectCallback(&onDisconnect);  
  wsServer.begin();
  
  delay(100); // Give Ethernet time to get ready
}



void opcionLed(int amp)
{
  char* a;
  if(digitalRead(amp)==HIGH)
  {
    digitalWrite(amp,LOW);
    a = "abajo";
    wsServer.send(a,strlen(a));

  }
  else if(digitalRead(amp) == LOW){
    digitalWrite(amp,HIGH);
    a = "Arriba";
    wsServer.send(a,strlen(a));

  }
}


void loop() {
  // Should be called for each loop.
  wsServer.listen();
  

  
if (wsServer.isConnected()) {
    //wsServer.send("abc123", 6);
  //if(digitalRead(entrada)==HIGH)
  /*if(count==0)
  {
    wsServer.send("arriba",6);
    digitalWrite(led, HIGH);
    count ++;
  }
  else
  {
    wsServer.send("Abajo",5);
    digitalWrite(led, LOW);
    count=0;
  }*/
  if(digitalRead(entrada)==HIGH)
  {
    opcionLed(7);
  }
  delay(300);
  }
}