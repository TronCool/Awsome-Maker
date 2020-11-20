
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>


int tyreL1 = D5;//14;
int tyreL2 = D6;//12;

int tyreR1 = D7;//13;
int tyreR2 = D8;//15;

void go(int x);
void back(int x);
void left(int x);
void right(int x);
void stopHere();
void test_tyre();

int verifyValue(int x);
void recvCmd(int x,int y);

static const char ssid[] = "blues car";
static const char password[] = "12345678";
WiFiUDP Udp;
unsigned int localUdpPort = 80;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets
int MotorA1,MotorA2,MotorB1,MotorB2;
int MotorA,MotorB;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  pinMode(tyreL1,OUTPUT);
  pinMode(tyreL2,OUTPUT);

  pinMode(tyreR1,OUTPUT);
  pinMode(tyreR2,OUTPUT);

  digitalWrite(tyreL1,LOW);
  digitalWrite(tyreL2,LOW);
  digitalWrite(tyreR1,LOW);
  digitalWrite(tyreR2,LOW);

  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  Serial.print("localIP: ");
  Serial.println(WiFi.localIP());
  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
  digitalWrite(2,HIGH);//led
}


void loop() {

//  int a,b,c,d;
//  a = digitalRead(tyreL1);
//  b = digitalRead(tyreL2);
//  c = digitalRead(tyreR1);
//  c = digitalRead(tyreR2);
//  Serial.printf("tyre %d,%d,%d,%d\n");
//  test_tyre();
//  return;
  int packetSize = Udp.parsePacket();  
  if (packetSize)
  {
    // receive incoming UDP packets
//    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }

    String str = String(incomingPacket);
    int index = str.indexOf(",");
    int x = (int)str.substring(0,index).toInt();
    int y = (int)str.substring(index+1,str.length()).toInt();

    recvCmd(x,y);

  }
  

}

void test_tyre()
{
  int speed = 100;
  go(speed);
  delay(1000);

  back(speed);
  delay(1000);

  left(speed);
  delay(1000);

  right(speed); 
  delay(1000);

  stopHere();
  delay(3000);  
}

void go(int x)
{
  analogWrite(tyreL1,x);
  analogWrite(tyreL2,0);

  analogWrite(tyreR1,x);
  analogWrite(tyreR2,0); 
  
}

void back(int x)
{
  analogWrite(tyreL1,0);
  analogWrite(tyreL2,x);

  analogWrite(tyreR1,0);
  analogWrite(tyreR2,x);
}

void left(int x)
{
  analogWrite(tyreL1,0);
  analogWrite(tyreL2,x); 
  
  analogWrite(tyreR1,x);
  analogWrite(tyreR2,0);

}

void right(int x)
{
  analogWrite(tyreL1,x);
  analogWrite(tyreL2,0);

  analogWrite(tyreR1,0);
  analogWrite(tyreR2,x);     
}

void stopHere()
{
  analogWrite(tyreL1,0);
  analogWrite(tyreL2,0);

  analogWrite(tyreR1,0);
  analogWrite(tyreR2,0); 
}

int verifyValue(int x)
{
  if(x < 0)
   {
    x = 0;
   }
   else if(x > 255)
   {
    x = 255;
   }

   int y = 0;
   if(x != 0)
      y = map(x, 0, 255, 0, 1023);
   return y;
}

void recvCmd(int x,int y)
{
  Serial.printf("cmd x = %d , y = %d\n", x,y);
  // 0 < x < 255 , 0 < y < 255

  MotorA = MotorB = y;
  MotorA += x;
  MotorB -= x;

  if(MotorA > 0){
    MotorA1 = MotorA;
    MotorA2 = 0;
  }
  else
  {
    MotorA1 = 0;
    MotorA2 = fabs(MotorA);
  }

  if(MotorB > 0){
    MotorB1 = MotorB;
    MotorB2 = 0;
  }
  else
  {
    MotorB1 = 0;
    MotorB2 = fabs(MotorB);
  }
  MotorA1 = verifyValue(MotorA1);
  MotorA2 = verifyValue(MotorA2);
  MotorB1 = verifyValue(MotorB1);
  MotorB2 = verifyValue(MotorB2);

//   Serial.printf("A1 = %d , A2 = %d , B1 = %d , B2 = %d\n", MotorA1,MotorA2,MotorB1,MotorB2);
  analogWrite(tyreL1,MotorA1);
  analogWrite(tyreL2,MotorA2);

  analogWrite(tyreR1,MotorB1);
  analogWrite(tyreR2,MotorB2); 
   
}
