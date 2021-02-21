#define ESP8266 1
#define ESP32 0

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Ticker.h>
#if ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#else if ESP32
#include <WiFi.h>
#endif
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

int intval = 20;
int intvalUDP = 50;
const int Legs_pin[4][3] = { {0, 1, 2}, {4, 5, 6}, {8, 9, 10}, {12, 13, 14} };
const int Legs_offset[4][3] = { {5, -5, -6},    //0 arm++为下压  wrist--为伸开 joint--为内收
                                {-11, 0, 3},    //1 arm--为下压  wrist--为收缩 joint++为内收
                                {-10,-3, -1},   //2 arm++为下压  wrist--为伸开 joint++为内收
                                {4, 2, 0} };    //3 arm--为下压  wrist--为收缩 joint--为内收
#define SERVO_FREQ 330
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
Ticker threadTimer;
Ticker cmdTimer;
// leg[0] = Left front
// leg[1] = Left back
// leg[2] = right back
// leg[3] = right front
// leg[n].x = arm 
// leg[n].y = wrist
// leg[n].z = joint
const int KEEP =255;
const float stepLength = 100;//这是完整的一步的长度，起步只有一半的长度
float stepLengthLeft = 100;
float stepLengthRight = 100;
const float walkSpeed = 7;
const float middleMoveSpeed = 3;
const float slowMoveSpeed = 1;
static const char ssidzz[] = "ssid";
static const char passwordzz[] = "password";

static const char ssid[] = "Blue Dog";
static const char password[] = "12345678";
WiFiUDP Udp;
unsigned int localUdpPort = 80;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets
char sendingPacket[255];
static int moveFlag = 0;
static int frozenFlag = 0;
int checkPostionOrAngle = 1;//1=postion ,0=angle
String cmdStr;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH );  

  WiFi.softAP(ssid, password);
  Udp.begin(localUdpPort);
  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ); 
  threadTimer.attach_ms(intval,thread_process);
  initMovePosition();
  cmdTimer.attach_ms(intvalUDP,check_cmd);

}
void setServoPulse(uint8_t n, double pulse) {
  double pulselength;
  pulselength = 1000;   // 1,000,000 us per second
  pulselength /= SERVO_FREQ;   // Analog servos run at ~60 Hz updates
  pulselength /= 4096;  // 12 bits of resolution
//  pulse *= 1000;  // convert input seconds to us
  pulse /= pulselength;
  pwm.setPWM(n, 0, pulse);
}
void setAngle(int n,double angle)
{ 
   double pulse = angle;
   pulse = pulse/90 + 0.5;
   setServoPulse(n,pulse);//0到180度映射为0.5到2.5ms
}
void setAngle300(int n,double angle)
{
   double pulse = angle;
   pulse = pulse/150 + 0.5;
   setServoPulse(n,pulse);//0到300度映射为0.5到2.5ms
}
void send_packet(char* data)
{
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
#if ESP8266    
    Udp.write(data);
#else if ESP32
    Udp.print(data);
#endif
    Udp.endPacket();
}
void check_cmd()
{
  int packetSize = Udp.parsePacket();  
  if (packetSize)
  {
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {  
      incomingPacket[len] = 0;
      String cmd = String(incomingPacket);
      if(cmd.equals("heartbeat"))
      {
        send_packet(incomingPacket);
      }  
      else if(cmd.equals("stop"))
      {
        stop_move();
      }
      else if(cmd.equals("frozen"))
      {
        frozen_move();
      }             
      else
      {
        Serial.print("check_cmd ");
        Serial.println(cmd);
        cmdStr = cmd;
      }
    }
  }
}
void thread_process()
{
  if(checkPostionOrAngle == 1)
    check_expect_postion();
  else
    check_expect_angle();
}
void loop() {
  ArduinoOTA.handle();
  String cmd = cmdStr;
  cmdStr.clear();
  if(cmd.equals("go"))
  {
    test_walk_step();
  }   
  else if(cmd.equals("ota"))
  {
    startOTA();
  }
  else if(cmd.equals("updown"))
  {
    test_up_down();
  }
  else if(cmd.equals("crosstest"))
  {
    test_walk_cross(); 
  }    
  else if(cmd.equals("lie"))
  {
    test_lie_down();
  } 
  else if(cmd.equals("stand"))
  {
    test_stand();
  }   
  else if(cmd.equals("sit"))
  {
    test_sit();
  }  
  else if(cmd.equals("eat"))
  {
    test_eat();
  }  
  else if(cmd.equals("jump"))
  {
    test_jump();
  }
  else if(cmd.equals("init"))
  {
    initLegs();
  }
  else if(cmd.equals("300"))
  {
    test300serve();
  }
  else if(cmd.equals("hello"))
  {
    sayHello();
  }  
  else if(cmd.equals("reset"))
  {
    stepLengthLeft = stepLength;
    stepLengthRight = stepLength;
  }
  else if(cmd.equals("tl"))
  {
    moveStep(false,true,10); 
  }
  else if(cmd.equals("tr"))
  {
   moveStep(true,false,10); 
  }  
  else if(cmd.equals("back"))
  {
   moveStep(false,false,10); 
  }
  else if(cmd.equals("climb"))
  {
    moveClimb(10);
  }
  else if(cmd.equals("bye"))
  {
    sayBye();
  }  
  else if(cmd.equals("play"))
  {
    sayHello();
    moveInitPostion(slowMoveSpeed); 
    moveClimb(5);
    moveStandPos();
    moveStep(false,true,5); 
    moveStep(true,false,5); 
    moveStandPos();
    moveStep(false,false,5); 
    moveStandPos();
    moveStep(true,true,5); 
    moveStandPosLow();
    for(int i=0;i<5;i++)
    {
      twistHortizonal(true);
      twistHortizonal(false);
      twistVertical(true);
      twistVertical(false);
      twistCircle(true);
      twistCircle(false);
    }
    sayBye();
    moveInitPostion(slowMoveSpeed); 
  }
  else if(cmd.equals("start")) 
  {
    for(int i=0;i<3;i++)
    {
      stepLengthLeft = i*50;
      stepLengthRight = i*50;
      moveStep(true,true,5); 
      delay(2000);
    }
  }
  else if(cmd.equals("rotate")) 
  {
    moveStandPosLow();
    delay(2000);
    rotateYaw(false,30);
    delay(2000);    
    rotateYaw(true,30);
    delay(2000);

    moveStandPosLow();
    rotatePitch(true,20);
    delay(2000);
    rotatePitch(false,20);
    delay(2000);
    
    moveStandPosLow();
    rotateRoll(true,20);
    delay(2000);
    rotateRoll(false,20);
    delay(2000);
        
    moveStandPosLow();
    delay(2000);
    rotateSlope(true,30);
    delay(2000);
    rotateSlope(false,30);
    delay(2000);  
    moveStandPosLow();  
    moveInitPostion(slowMoveSpeed);
  }
  else if(cmd.indexOf(",")>0)
  {
    int index = cmd.indexOf(",");
    int x = (int)cmd.substring(0,index).toInt();
    int y = (int)cmd.substring(index+1,cmd.length()).toInt();
    if(y>0 && y>abs(x))//go
    {
      moveStep(true,true,10); 
    }
    else if(y<0 && abs(y)>abs(x))//back
    {
      moveStep(false,false,10); 
    } 
    else if(x>0 && x>abs(y))//turn right
    {
      moveStep(true,false,10);   
    }
    else if(x<0 && abs(x)>abs(y))//turn left
    {
      moveStep(false,true,10);   
    }     
  }
}

void test_walk_cross()
{
    for(int i=0;i<3;i++){
    moveVerticalStart();
    delay(2000);
    moveVerticalEnd();
    delay(2000);   
    }
    moveCenter();
    delay(2000); 
    for(int i=0;i<3;i++){
    moveHorizontalStart();
    delay(2000); 
    moveHorizontalEnd();
    delay(2000);     
    }
    moveCenter();
    delay(2000); 
}

void test_walk_step()
{
  Serial.println("test_walk_step");  
  cmdStr.clear();
  
//  moveStandPos();
  moveStep(true,true,10); 
  return;
  moveStepOne();
  while(moveFlag == 1)
  {
    if(is_stopped())
      return;
    moveStepTwo();
    if(is_stopped())
      return;   
    moveStepThree();
  }
  moveStepTwo();
  moveStepFour();
  stopMove();
  moveStandPos();

}

void test_up_down()
{
    for(int i=0;i<3;i++){
    moveVerticalStart();
    delay(2000);

    moveCenter();
    delay(2000);
    
    moveVerticalEnd();
    delay(2000);  

    moveCenter();
    delay(2000);    
    }
}

void test_lie_down()
{
  cmdStr.clear();
  Serial.println("test_lie_down");
  moveInitPostion(slowMoveSpeed); 
}

void test_stand()
{
  cmdStr.clear();
  Serial.println("test_stand");
  moveStandPos();
}

void test_sit()
{
  cmdStr.clear();
  Serial.println("test_sit");
  moveSitPos(); 
}
void test_eat()
{
  cmdStr.clear();
  Serial.println("test_eat");
  moveEatPos(); 
}
void test_jump()
{
  cmdStr.clear();
  Serial.println("test_jump");
  moveJump();   
}
void stop_move()
{
  Serial.println("stop_move");
  moveFlag = 0;
  cmdStr.clear();
}

bool is_stopped()
{
  if(moveFlag == 0)
    return true;
  return false;
}

void frozen_move()
{
  Serial.println("stop_move");
  frozenFlag = 1;
  cmdStr.clear();  
}
bool is_frozen()
{
  if(frozenFlag == 1)
    return true;
  return false;
}
void startOTA()
{
  digitalWrite(LED_BUILTIN, LOW);  
  cmdStr.clear();
  Serial.println("startOTA");
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssidzz, passwordzz);  
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }  
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  
}
