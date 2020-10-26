#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include  <Ticker.h>

int stepAngle = 0;
int sep = 1;
int intval = 20;
int actionIntval = 2000;
int checkCount =0;
int checkFreq = 1;
//Servo LegsServo[4][3];
const int Legs_pin[4][3] = { {0, 1, 2}, {4, 5, 6}, {8, 9, 10}, {12, 13, 14} };
const int Joints_pin[3][4]= {{0,4,8,12},{1,5,9,13},{2,6,10,14}};
// leg[0] = Left front
// leg[1] = Left back
// leg[2] = right front
// leg[3] = right back
// leg[n].0 = arm joint
// leg[n].1 = arm
// leg[n].2 = wrist
const int KEEP =255;
int angleCurrent[4][3];
int angleExpect[4][3];
int angleExpectTemp[4][3];
#define SERVO_FREQ 60 // Analog servos run at ~50 Hz updates
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
Ticker threadTimer;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ);
  delay(10);

//  setDefaultAngle();
  if(stepAngle)
    threadTimer.attach_ms(intval,check_reach_expect);
//  delay(actionIntval);
}



void loop() {


  Serial.println("loop......");
  lie_down();
  delay(actionIntval); 
  stand();
  delay(actionIntval);
  return;
  go_forward();
  delay(actionIntval);   
}



void setServoPulse(uint8_t n, double pulse) {
  double pulselength;
  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= SERVO_FREQ;   // Analog servos run at ~60 Hz updates
  pulselength /= 4096;  // 12 bits of resolution
  pulse *= 1000;  // convert input seconds to us
  pulse /= pulselength;
//  Serial.print("pulse = ");
//  Serial.println(pulse);
  pwm.setPWM(n, 0, pulse);
}
void setAngle(int n,int angle)
{
//  Serial.print("Angle = ");
//  Serial.println(angle);
   double pulse = angle;
   pulse = pulse/90 + 0.5;
   setServoPulse(n,pulse);//0到180度映射为0.5到2.5ms
}

void set_angle_expect_to_current()
{
  for(int i=0;i<4;i++)
    for(int j=0;j<3;j++)
      angleCurrent[i][j] = angleExpect[i][j];
}

void check_reach_expect()
{
  checkCount++;
  String s;
//  Serial.println(checkCount);  
  for(int i = 0;i<4;i++)
  {
    for(int j = 0;j<3;j++){
      if(checkCount%checkFreq ==0){

        if(angleExpectTemp[i][j] == -1){//then do expect angle
          if(abs(angleExpect[i][j]-angleCurrent[i][j])>sep-1){
            if(angleExpect[i][j]>angleCurrent[i][j])
              angleCurrent[i][j]+=sep;
            else if(angleExpect[i][j]<angleCurrent[i][j])
              angleCurrent[i][j]-=sep;
          }
          else
            angleCurrent[i][j]= angleExpect[i][j];
        }
        else{//do temp angle first
          if(angleExpectTemp[i][j] <0 ||angleExpectTemp[i][j] >180 )
            angleExpectTemp[i][j]= -1;  
          else if(angleExpectTemp[i][j] != angleCurrent[i][j]){
            if(abs(angleExpectTemp[i][j]-angleCurrent[i][j])>sep-1){
              if(angleExpectTemp[i][j]>angleCurrent[i][j])
                angleCurrent[i][j]+=sep;
              else if(angleExpectTemp[i][j]<angleCurrent[i][j])
                angleCurrent[i][j]-=sep;  
            }
            else
              angleCurrent[i][j] = angleExpectTemp[i][j]; 
          }
          else
            angleExpectTemp[i][j]= -1;          
        }
      }
    }
  }
  drive_servo_work();
}

void drive_servo_work()
{
  if(stepAngle){
    check_angle_current_and_expect();
    for(int i=2;i>=0;i--)
    { 
      for(int j=0;j<4;j++)
       setAngle(Joints_pin[i][j],angleCurrent[j][i]);//drive servo by same level joint
    }  
  }
  else
  {
    for(int i=2;i>=0;i--)
    { 
      for(int j=0;j<4;j++){
       setAngle(Joints_pin[i][j],angleExpect[j][i]);//drive servo by same level joint
       
      }
    }  
//    delay(20);
  }
}

void check_angle_current_and_expect()
{
  for(int i=0;i<4;i++){
    for(int j=0;j<3;j++)
    {
      if(angleCurrent[i][j]<0)angleCurrent[i][j]=0;
      if(angleCurrent[i][j]>180)angleCurrent[i][j]=180;     
      if(angleExpect[i][j]<0)angleExpect[i][j]=0;
      if(angleExpect[i][j]>180)angleExpect[i][j]=180;

    }
  }
}





int wait_leg_reach(int leg)
{
  while (1){
//    print_angle();
      if(abs(angleCurrent[leg][0]- angleExpect[leg][0])<=sep-1){
        if(abs(angleCurrent[leg][1]- angleExpect[leg][1])<=sep-1){
          if(abs(angleCurrent[leg][2]- angleExpect[leg][2])<=sep-1){
            break;
          }
        }
      }
      delay(intval);
  }
}
void wait_all_reach()
{
//  Serial.println("wait_all_reach.......");
//  print_angle();
  if(stepAngle){
    for (int i = 0; i < 4; i++)
      wait_leg_reach(i);
  }
  else
  {
    drive_servo_work();
  }
//  Serial.println("all reached");
}
