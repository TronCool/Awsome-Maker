#include <Servo.h>

#define  Btn1  A0
#define  JY1   A1
#define  JX1   A2

#define  Btn2   A3
#define  JY2   A4
#define  JX2   A5

int JoyX1,JoyY1,JoyBtn1,JoyX2,JoyY2,JoyBtn2;
int BaseLock,HandLock;
Servo myservo[4]; //0 是底下转盘，1 是小臂 。2是大臂 ， 3 是爪子
int servoOffset[4]={0,-5,-15,0};
int curAngle[4];
int attachFlag;
int servoRange[4][2]={{0,180},{20,120},{30,180},{60,150}};//角度范围 {min,max}
void setup() {
  // put your setup code here, to run once:
  attachServo();
  initServo();
  pinMode(Btn1,INPUT_PULLUP);
  pinMode(JY1,INPUT);
  pinMode(JX1,INPUT);
  pinMode(Btn2,INPUT_PULLUP);
  pinMode(JY2,INPUT);
  pinMode(JX2,INPUT);
  Serial.begin(115200);
  JoyX1=JoyY1=JoyBtn1=JoyX2=JoyY2=JoyBtn2=0;
  BaseLock = HandLock = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  detectJoyStick();
  moveArm();
  delay(20);//这个delay 也可以用来调节速度，数值大速度慢，建议20-100之间
}

void attachServo()
{
  Serial.println("attachServo");
  for(int i=0;i<4;i++)
  {
    myservo[i].attach(i+2);
  }
  attachFlag = 1;
  
}

void detachServo()
{
  for(int i = 90 ;i<150;i++){
    servoWrite(2,i);
    delay(20);
  }
  for(int i=0;i<4;i++)
  {
    myservo[i].detach();
  }
  attachFlag = 0;
  Serial.println("detachServo");
}
void servoWrite(int i,float a)
{
  if(a > servoRange[i][1])
    a = servoRange[i][1];
  if(a < servoRange[i][0])
    a = servoRange[i][0];   
  curAngle[i] = a;
  a = a+servoOffset[i];
  myservo[i].write(a);
}

void initServo()
{
  for(int i=0;i<4;i++)
  {
    servoWrite(i,90);
  }
  Serial.println("initServo");
}

void detectJoyStick()
{
  static int JoyValueX1,JoyValueY1,JoyValueBtn1,JoyValueX2,JoyValueY2,JoyValueBtn2;
  static int pressCount = 0;
  static int pressBtn1Count = 0;
  static int pressBtn2Count = 0;

  //需要把X,Y轴对换，方向再反向，方便接线处朝上
  JoyValueX1 = 1023 - analogRead(JY1);
  JoyValueY1 = 1023 - analogRead(JX1);
  JoyValueBtn1 = analogRead(Btn1);

  JoyValueX2 = 1023 - analogRead(JY2);
  JoyValueY2 = 1023 - analogRead(JX2);
  JoyValueBtn2 = analogRead(Btn2); 

  int showFlag = 0;
  if(abs(JoyValueX1-JoyX1)>10)
  {
    JoyX1 = JoyValueX1;
    showFlag = 1;
  }
  if(abs(JoyValueX2-JoyX2)>10)
  {
    JoyX2 = JoyValueX2;
    showFlag = 1;
  }  
  if(abs(JoyValueY1-JoyY1)>10)
  {
    JoyY1 = JoyValueY1;
    showFlag = 1;
  }
  if(abs(JoyValueY2-JoyY2)>10)
  {
    JoyY2 = JoyValueY2;
    showFlag = 1;
  }  

}

void moveArm()
{
  if(!attachFlag)
    return;
  float X1,Y1,X2,Y2;
  int range = 5;// 这个值可以调节速度，数值大速度快
  X1 = map(JoyX1,0,1023,-range,range);
  Y1 = map(JoyY1,0,1023,-range,range);
  X2 = map(JoyX2,0,1023,-range,range);
  Y2 = map(JoyY2,0,1023,-range,range);

  X1 =  - X1;
  Y2 =  - Y2;

  if(JoyX1>=400 && JoyX1<= 600)
    X1 = 0;
  if(JoyX2>=400 && JoyX2<= 600)
    X2 = 0;   
  if(JoyY1>=400 && JoyY1<= 600)
    Y1 = 0;
  if(JoyY2>=400 && JoyY2<= 600)
    Y2 = 0;        



  String s = "JoyX1 = "+String(X1);
  Serial.print(s);
  s= " JoyY1 = "+String(Y1) ;
  Serial.print(s);
  s= " JoyX2 = "+String(X2);
  Serial.print(s);
  s= " JoyY2 = "+String(Y2);
  Serial.println(s);
  servoWrite(0,curAngle[0]+X1);//底盘
  servoWrite(3,curAngle[3]+X2);//爪子
  servoWrite(2,curAngle[2]+Y1);//大臂
  servoWrite(1,curAngle[1]+Y2);//小臂；
}
