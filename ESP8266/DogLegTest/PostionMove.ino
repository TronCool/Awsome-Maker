// leg[0] = Left front
// leg[1] = right front
// leg[2] = Left back
// leg[3] = right back
// leg[n].0 = arm 
// leg[n].1 = wrist
// leg[n].2 = joint
const float La = 110;//arm length
const float Lb = 125;//wrist length
const float Lc = 60;//joint length
const float Ld = 185; //前后joint的间距
const float Le = 200; //左右joint的间距

const int minAlpha = 0;
const int maxAlpha = 180;
const int minBeta = 0;
const int maxBeta = 150;
int tempSpeed[4][3];
float curPostion[4][3];
float expPostion[4][3];
float postionReady[4][3];

#define pi 3.14
const float standX_Front = 0;
const float standX_Rear = 0;
const float standX_Rear_For_Move = 50;
//const float standY = 200;
const float standY_Front = 200;
const float standY_Rear = 200;
const float standY_Front_Low = 150;
const float standY_Rear_Low = 150;
float stepSpeed = 6;//步速
//const float walkSpeed = 7;
//const float middleMoveSpeed = 3;
//const float slowMoveSpeed = 1;
float highOfLegLift = 40; 
float stepLengthSlow = 30;//这个是半步的长度
float moveStepSlowX[4];

const int stepCount = 8;//完整步长的采样点数， 
void set_current_postion(int leg, float x, float y, float z)
{
  curPostion[leg][0] = x;
  curPostion[leg][1] = y;
  curPostion[leg][2] = z;    
}

void set_expect_postion(int leg, float x, float y, float z)
{
  if(x != KEEP)
    expPostion[leg][0] = x;
  if(y != KEEP)  
    expPostion[leg][1] = y;
  if(z != KEEP)   
    expPostion[leg][2] = z;     
}

void check_expect_postion()
{
  static float alpha, beta, gama;
  int nReady=0;
  for(int i=0;i<4;i++)
  {
    nReady = 0;
    for(int j=0;j<3;j++)
    {
      if(curPostion[i][j]!= expPostion[i][j])
      {
        if(abs(curPostion[i][j] - expPostion[i][j])<stepSpeed)
        {
//          if(stepSpeed>slowMoveSpeed)
//          {
//            if(curPostion[i][j] < expPostion[i][j])
//              curPostion[i][j] += slowMoveSpeed;
//            else
//            curPostion[i][j] -= slowMoveSpeed;            
//          }
//          else
            curPostion[i][j] = expPostion[i][j];
        }
        else{
         if(curPostion[i][j] < expPostion[i][j])
           curPostion[i][j] += stepSpeed;
         else
           curPostion[i][j] -= stepSpeed;
        }
        postionReady[i][j] = 0; 
      }
      else{
        postionReady[i][j] = KEEP;
        continue;
      }
    }
    for(int j=0;j<3;j++)
    {
      nReady += postionReady[i][j];
    }
    if(nReady == 3*KEEP)//如果1条腿的3个节点已经驱动到位，就不要再计算舵机的角度了
      continue;
    postion_to_angle(i,alpha,beta,gama);
    angle_to_servo(i,alpha,beta,gama);
  }
}

void direct_reach_all_postion()
{
  static float alpha, beta, gama;
  for(int i=0;i<4;i++)
  {
    postion_to_angle(i,alpha,beta,gama);
    angle_to_servo(i,alpha,beta,gama);      
  }
}
void wait_reach_postion(int leg)
{
  while (1){
    if (curPostion[leg][0] == expPostion[leg][0])
      if (curPostion[leg][1] == expPostion[leg][1])
        if (curPostion[leg][2] == expPostion[leg][2])
          break;
    delay(10);      
  }
}

void wait_all_reach_postion(void)
{
  checkPostionOrAngle = 1;
  for (int i = 0; i < 4; i++)
    wait_reach_postion(i);
}

void postion_to_angle(int leg,volatile float& alpha,volatile float& beta,volatile float& gama)
{
  float x = curPostion[leg][0];
  float y = curPostion[leg][1];
  float z = curPostion[leg][2];  
  if(z != 0)//对于0脚， z>0是内收，
  {//需要变成三维坐标，y需要重新计算
    float L4 = abs(z-Lc);
    float L5 = sqrt(pow(y,2)+pow(L4,2));
    float A = 0;//z == Lc , L4==0;
    if(z==Lc)
      A = 0;
    else if(z>0 && z < Lc)
      A = atan(L4/y);
    else //z<0
      A = atan(y/L4);
    
    float B = acos(Lc/L5);
    float ya = sqrt(pow(y,2)+pow(L4,2)-pow(Lc,2));
    if(z<0)
      gama = (A-B)/pi*180;
    else if(z<=Lc)
      gama = 90-(A+B)/pi*180;
    else
      gama = 180-(A+B)/pi*180;

//    if(leg == 0)
////      Serial.println(" z="+String(z)+" A="+String(A)+" B="+String(B)+" ya="+String(ya)+" gama="+String(gama));
//      Serial.println(" gama="+String(gama));
    y = ya;
  }
  else
      gama = 0;
  float a1 = (pow(La,2)-pow(Lb,2)+pow(x,2)+pow(y,2))/(2*La*(sqrt(pow(x,2)+pow(y,2))));
  float a2 = (pow(La,2)+pow(Lb,2)-pow(x,2)-pow(y,2))/(2*La*Lb);

  alpha = acos(a1)+atan((x/y));

  beta = acos(a2);

//  gama = z;//0&3 joint 内收的夹角 >0 内收

  if(leg == 0 || leg == 2){
    alpha = 300 -60 - alpha/ pi * 180;
    beta = 180 - beta/ pi * 180;
  }
  else //if(leg == 1 || leg == 3)
  {
    alpha = 60 + alpha/ pi * 180;
    beta = 0 + beta/ pi * 180;
  }
  
  if(leg == 0 || leg == 3)
  {
    gama = 90 - gama;
  }
  else
  {
    gama = 90 + gama;
  }
  
//  gama = 90;
}

void angle_to_servo(int leg ,float alpha,float beta,float gama)
{
//  if(alpha<=minAlpha)alpha = minAlpha;
//  if(alpha>=maxAlpha)alpha = maxAlpha;
//  if(beta<=minBeta)beta = minBeta;
//  if(beta>=maxBeta)beta = maxBeta; 
  alpha = alpha+Legs_offset[leg][0];
  beta = beta+Legs_offset[leg][1];
  gama = gama+Legs_offset[leg][2]; 
  setAngle300(Legs_pin[leg][0],alpha);
  setAngle(Legs_pin[leg][1],beta); 
  setAngle(Legs_pin[leg][2],gama);
  angleCurrent[leg][0] = alpha;
  angleCurrent[leg][1] = beta;
  angleCurrent[leg][2] = gama;   
  angleExpert[leg][0] = alpha;
  angleExpert[leg][1] = beta;
  angleExpert[leg][2] = gama;    
}

void initMovePosition()
{
  Serial.println("initMove");
  int initX = -50;
  int initY = 70;

  for(int i = 0;i<4;i++)
  {
    set_current_postion(i,initX,initY,0);
    set_expect_postion(i,initX,initY,0);
  }  
  wait_all_reach_postion();
}

void moveInitPostion(float s)
{
  Serial.println("moveInitPostion");
  stepSpeed = s;
  int initX = -50;
  int initY = 70;

  for(int i = 0;i<4;i++)
  {
    set_expect_postion(i,initX,initY,0);
  }  
  wait_all_reach_postion();  
}

void moveInitJumpPostion(float s)
{
  Serial.println("moveInitPostion");
  stepSpeed = s;
  int initX = 0;
  int initY = 70;

  for(int i = 0;i<4;i++)
  {
    if(i==0||i==1)
      set_expect_postion(i,initX,initY,0);
    else
      set_expect_postion(i,initX,initY,0);
  }  
  wait_all_reach_postion();  
}

void moveVerticalEnd()
{
  Serial.println("moveVerticalEnd");

  set_expect_postion(0,0,200,0);  
  set_expect_postion(1,0,200,0);  
  set_expect_postion(2,0,200,0);  
  set_expect_postion(3,0,200,0);    
  wait_all_reach_postion();
}

void moveVerticalStart()
{
  Serial.println("moveVerticalStart");

  set_expect_postion(0,0,100,0);  
  set_expect_postion(1,0,100,0);    
  set_expect_postion(2,0,100,0);  
  set_expect_postion(3,0,100,0);   
  wait_all_reach_postion();
}

void moveCenter()
{
  Serial.println("moveCenter");
  set_expect_postion(0,0,150,0);  
  set_expect_postion(1,0,150,0);  
  set_expect_postion(2,0,150,0);  
  set_expect_postion(3,0,150,0);   
  wait_all_reach_postion();  
}
void moveHorizontalEnd()
{
  Serial.println("moveHorizontalEnd");
  set_expect_postion(0,100,150,0);  
  set_expect_postion(1,100,150,0);  
  set_expect_postion(2,100,150,0);  
  set_expect_postion(3,100,150,0);    
  wait_all_reach_postion();
}

void moveHorizontalStart()
{
  Serial.println("moveHorizontalStart"); 
  set_expect_postion(0,-100,150,0);  
  set_expect_postion(1,-100,150,0); 
  set_expect_postion(2,-100,150,0);  
  set_expect_postion(3,-100,150,0);   
  wait_all_reach_postion();
}


void moveStandPos()
{
  Serial.println("moveStandPos");
  stepSpeed = slowMoveSpeed;
  set_expect_postion(0,standX_Front,standY_Front,0);  
  set_expect_postion(1,standX_Front,standY_Front,0);  
  set_expect_postion(2,standX_Rear,standY_Rear,0);  
  set_expect_postion(3,standX_Rear,standY_Rear,0);    
  wait_all_reach_postion();
}
void moveStandPosLow()
{
  Serial.println("moveStandPos");
  stepSpeed = slowMoveSpeed;
  set_expect_postion(0,standX_Front,standY_Front_Low,0);  
  set_expect_postion(1,standX_Front,standY_Front_Low,0);  
  set_expect_postion(2,standX_Rear,standY_Rear_Low,0);  
  set_expect_postion(3,standX_Rear,standY_Rear_Low,0);    
  wait_all_reach_postion();
      
}
void moveStandPosForMoveLow()
{
  Serial.println("moveStandPos");
  stepSpeed = slowMoveSpeed;
  set_expect_postion(0,standX_Front,standY_Front_Low,0);  
  set_expect_postion(1,standX_Front,standY_Front_Low,0);  
  set_expect_postion(2,standX_Rear_For_Move,standY_Rear_Low,0);  
  set_expect_postion(3,standX_Rear_For_Move,standY_Rear_Low,0);    
  wait_all_reach_postion();
      
}
void startMove()
{//站立是 前后脚的X位置都为standX_Front，start时需要把后脚的X 调整到standX_Rear
  Serial.println("startMove");
  stepSpeed = slowMoveSpeed;

//  set_expect_postion(2,KEEP,standY_Rear-highOfLegLift,0);   
//  wait_all_reach_postion();
  set_expect_postion(2,standX_Rear_For_Move,standY_Rear,0);  
//  wait_all_reach_postion();
//  set_expect_postion(3,KEEP,standY_Rear-highOfLegLift,0);  
//  wait_all_reach_postion(); 
  set_expect_postion(3,standX_Rear_For_Move,standY_Rear,0); 
  wait_all_reach_postion();   
 
}
void stopMove()
{//移动结束后要把后脚的X复位到 standX_Front
  Serial.println("stopMove");
  stepSpeed = slowMoveSpeed;

  set_expect_postion(2,KEEP,standY_Rear-highOfLegLift,0);   
  wait_all_reach_postion();
  set_expect_postion(2,standX_Rear,standY_Rear,0);  
  wait_all_reach_postion();
  set_expect_postion(3,KEEP,standY_Rear-highOfLegLift,0);   
  wait_all_reach_postion();   
  set_expect_postion(3,standX_Rear,standY_Rear,0);  
  wait_all_reach_postion();   
}
void moveStepOne()
{
  Serial.println("moveStepOne");
  stepSpeed = walkSpeed;
  int steps = stepCount/2;
  float stepOneLengthLeft = stepLengthLeft/2;
  float stepOneLengthRight = stepLengthRight/2;  
  float posX[steps];
  float posY[steps];
  float stepXL,stepYL;
  float stepXR,stepYR;
  for(int i = 0;i<steps;i++)
  {
    if(is_stopped())
      return;
    stepXL = (i+1)*(stepOneLengthLeft/steps);
    stepYL = sin((i+1)*(pi/steps))* (stepOneLengthLeft/2);
    stepXR = (i+1)*(stepOneLengthRight/steps);
    stepYR = sin((i+1)*(pi/steps))* (stepOneLengthRight/2);
    
    posX[i] = standX_Front - stepXL;//(i+1)*(stepOneLength/steps);
    posY[i] = standY_Front - stepYL;//sin((i+1)*(pi/steps))* (stepOneLength/2);
    set_expect_postion(0,posX[i],posY[i],0);  


    posX[i] = standX_Rear - stepXR;
    posY[i] = standY_Rear - stepYR;
    set_expect_postion(3,posX[i],posY[i],0); 


    posX[i] = standX_Front+stepXR;//(i+1)*(stepOneLength/steps) ;
    set_expect_postion(1,posX[i],standY_Front,0); 
    posX[i] = standX_Rear+stepXL; 
    set_expect_postion(2,posX[i],standY_Rear,0);    
    wait_all_reach_postion();   
  }
}

void moveStepTwo()
{
  Serial.println("moveStepTwo");
  stepSpeed = walkSpeed;
  int steps = stepCount;
//  float stepOneLength = stepLength/2;
  float stepOneLengthLeft = stepLengthLeft/2;
  float stepOneLengthRight = stepLengthRight/2;  
  float posX[steps];
  float posY[steps];
  float stepXL,stepYL;
  float stepXR,stepYR;
  for(int i = 0;i<steps;i++)
  {
    if(is_stopped())
      return;
    stepXL = stepOneLengthLeft - (stepLengthLeft/steps)*(i+1);
    stepYL = sin((i+1)*(pi/steps))* (stepLengthLeft/2); 
    stepXR = stepOneLengthRight - (stepLengthRight/steps)*(i+1);
    stepYR = sin((i+1)*(pi/steps))* (stepLengthRight/2); 
    
    posX[i] = standX_Front - stepXL;//- stepOneLength + (stepLength/steps)*(i+1);
    set_expect_postion(0,posX[i],standY_Front,0); 
    posX[i] = standX_Rear - stepXR; 
    set_expect_postion(3,posX[i],standY_Rear,0);     

    posX[i] = standX_Front + stepXR;//stepOneLength - (i+1)*(stepLength/steps);
    posY[i] = standY_Front - stepYR;//sin((i+1)*(pi/steps))* (stepLength/2); 
    set_expect_postion(1,posX[i],posY[i],0);  
    posX[i] = standX_Rear + stepXL;
    posY[i] = standY_Rear - stepYL;
    set_expect_postion(2,posX[i],posY[i],0); 
    wait_all_reach_postion();  
  }
}

void moveStepThree()
{
  Serial.println("moveStepThree");
  stepSpeed = walkSpeed;
  int steps = stepCount;
//  float stepOneLength = stepLength/2;
  float stepOneLengthLeft = stepLengthLeft/2;
  float stepOneLengthRight = stepLengthRight/2;  
  float posX[steps];
  float posY[steps];
  float stepXL,stepYL;
  float stepXR,stepYR;
  for(int i = 0;i<steps;i++)
  {
    if(is_stopped())
      return;
    stepXL = stepOneLengthLeft - (i+1)*(stepLengthLeft/steps);
    stepYL = sin((i+1)*(pi/steps))* (stepLengthLeft/2); 
    stepXR = stepOneLengthRight - (i+1)*(stepLengthRight/steps);
    stepYR = sin((i+1)*(pi/steps))* (stepLengthRight/2);    
    
    posX[i] = standX_Front + stepXL;//stepOneLength - (i+1)*(stepLength/steps);
    posY[i] = standY_Front - stepYL;//sin((i+1)*(pi/steps))* (stepLength/2);   
    set_expect_postion(0,posX[i],posY[i],0); 
     
    posX[i] = standX_Rear + stepXR;
    posY[i] = standY_Rear - stepYR;
    set_expect_postion(3,posX[i],posY[i],0);  
    
    posX[i] = standX_Front - stepXR;//- stepOneLength + (stepLength/steps)*(i+1);
    set_expect_postion(1,posX[i],standY_Front,0);  
    posX[i] = standX_Rear - stepXL;
    set_expect_postion(2,posX[i],standY_Rear,0);      
    wait_all_reach_postion();   
  }
}

void moveStepFour()
{
  Serial.println("moveStepFour");
  stepSpeed = walkSpeed;
  int steps = stepCount/2;
//  float stepOneLength = stepLength/2;
  float stepOneLengthLeft = stepLengthLeft/2;
  float stepOneLengthRight = stepLengthRight/2;  
  float posX[steps];
  float posY[steps];
  float stepXL,stepYL;
  float stepXR,stepYR;
  for(int i = 0;i<steps;i++)
  {
    if(is_stopped())
      return; 
    stepXL = stepOneLengthLeft - (i+1)*(stepOneLengthLeft/steps);
    stepYL = sin((i+1)*(pi/steps))* (stepOneLengthLeft/2);
    stepXR = stepOneLengthRight - (i+1)*(stepOneLengthRight/steps);
    stepYR = sin((i+1)*(pi/steps))* (stepOneLengthRight/2);
    
    
    posX[i] = standX_Front + stepXL;//stepOneLength - (i+1)*(stepOneLength/steps);
    posY[i] = standY_Front - stepYL;//sin((i+1)*(pi/steps))* (stepOneLength/2);
    set_expect_postion(0,posX[i],posY[i],0); 
    
    posX[i] = standX_Rear + stepXR;
    posY[i] = standY_Rear - stepYR; 
    set_expect_postion(3,posX[i],posY[i],0); 

    posX[i] = standX_Front - stepXR;//- stepOneLength + (i+1)*(stepOneLength/steps);
    set_expect_postion(1,posX[i],standY_Front,0);  
    posX[i] = standX_Rear - stepXL;
    set_expect_postion(2,posX[i],standY_Rear,0);    
    wait_all_reach_postion();   
  }  
}

void moveSitPos()
{
  Serial.println("moveSitPos");
  stepSpeed = slowMoveSpeed; 
  set_expect_postion(0,80,220,0); 
  set_expect_postion(1,80,220,0); 
  set_expect_postion(2,0,110,0); 
  set_expect_postion(3,0,110,0); 
  wait_all_reach_postion();    
}
void moveEatPos()
{
  Serial.println("moveEatPos");
  stepSpeed = slowMoveSpeed; 
  set_expect_postion(0,-60,30,0); 
  set_expect_postion(1,-60,30,0); 
  set_expect_postion(2,0,150,0); 
  set_expect_postion(3,0,150,0); 
  wait_all_reach_postion();    
}

void moveJump()
{
  Serial.println("moveJump");
  moveInitPostion(slowMoveSpeed);
  moveInitJumpPostion(slowMoveSpeed);
  delay(1000);
  int jumpX = 100;
  int jumpY = 220;
   
  for(int i = 0;i<4;i++)
  {
    if(i==0||i==1){
      set_expect_postion(i,jumpX,jumpY,0);
      set_current_postion(i,jumpX,jumpY,0);       

    }
    else{
      set_expect_postion(i,jumpX,jumpY,0);   
      set_current_postion(i,jumpX,jumpY,0);     
    }
    
  }  
  direct_reach_all_postion(); 
  delay(50); 
  moveInitPostion(10);
  return;

//  initX = 0;
//  initY = 220;
//  stepSpeed = walkSpeed;
//  for(int i = 0;i<4;i++)
//  {
//    set_expect_postion(i,initX,initY,0);
//    if(i==2||i==3)
//      set_expect_postion(i,initX+standX_Rear,initY,0);
//  } 
//  wait_all_reach_postion();  
//
//  initX = 0;
//  initY = 150;
//  stepSpeed = middleMoveSpeed; 
//  for(int i = 0;i<4;i++)
//  {
//    set_expect_postion(i,initX,initY,0);
//    if(i==2||i==3)
//      set_expect_postion(i,initX+standX_Rear,initY,0);
//  }  
//  wait_all_reach_postion();  
}

//======== new walk step ========//
void moveStep(bool leftForward ,bool rightForward,int steps)
{
  Serial.println("moveStep");
  moveFlag = 1;
  if(!isWalkPos()){
    moveStandPos();
    delay(1000);
  }
  while(steps-->0){
    Serial.println("moveStep...");
    if(is_stopped())
      break;
    legLiftMove(true,leftForward,rightForward);
    legLiftMove(false,leftForward,rightForward);
  }
  return;
}

void moveClimb(int steps)
{
  moveFlag = 1;
  if(!isClimbPos()){
    moveStandPosForMoveLow();
    delay(1000);
  }
  int legSeq[]= {0,3,1,2};
  while(steps-->0){
    Serial.println("moveSlow..."+String(steps));
    if(is_stopped())
      break;

    for(int i= 0;i<4;i++)
    {
      if(is_stopped())break;
      stepSpeed = walkSpeed;//middleMoveSpeed;
      makeLegstepSlow(legSeq[i]);
      if(is_stopped())break;
      stepSpeed = middleMoveSpeed;
      bodyMoveSlow();     
    }

  }
}

void bodyMoveSlow()
{
    Serial.println("bodyMoveSlow...");
//    String str;
    float offsetZ = 5;
    for(int i=0;i<4;i++)
    {
      moveStepSlowX[i]+= stepLengthSlow; 
//      str = String(moveStepSlowX[i]);
//      Serial.println("bodyMoveSlow "+str);      
    }
    set_expect_postion(0,getStandX(0)+moveStepSlowX[0],standY_Front_Low,offsetZ); 
    set_expect_postion(1,getStandX(1)+moveStepSlowX[1],standY_Front_Low,offsetZ); 
    set_expect_postion(2,getStandX(2)+moveStepSlowX[2],standY_Rear_Low,offsetZ);    
    set_expect_postion(3,getStandX(3)+moveStepSlowX[3],standY_Rear_Low,offsetZ);  
    wait_all_reach_postion();
}

void makeLegstepSlow(int i)
{
    int n = 2;
    float offsetZ = 5;
    moveStepSlowX[i]= -n*stepLengthSlow;
    float xv,yv;
    int otherLeg,otherRowLeg1,otherRowLeg2;
    float gravityOffset = 10;//抬腿的另一条腿顶起的高度，为了重心偏移
    if(is_stopped())
      return;
    if(i==0||i==1){
      xv = standX_Front;
      yv = standY_Front_Low;
      if(i == 0){
        otherLeg = 1;
        otherRowLeg1 = 2;//同侧
        otherRowLeg2 = 3;//交叉侧
      }
      else{//i==1
        otherLeg = 0;
        otherRowLeg1 = 3;//同侧
        otherRowLeg2 = 2;//交叉侧
      }

     
    }
    else{
      xv = getStandX(i);
      yv = standY_Rear_Low;    
      if(i == 2){
        otherLeg = 3;
        otherRowLeg1 = 0;//同侧
        otherRowLeg2 = 1;//交叉侧   
      }
      else{//i == 3
        otherLeg = 2; 
        otherRowLeg1 = 1;//同侧
        otherRowLeg2 = 0;//交叉侧    
      }            
    }

    set_expect_postion(otherLeg,KEEP,yv+gravityOffset,offsetZ); //同排的另一条腿顶起，使重心偏前排或后排的腿，有助于抬起的腿保持悬空
    set_expect_postion(otherRowLeg1,KEEP,yv-gravityOffset,offsetZ);//另一排的两条腿下蹲，同样使重心偏前排或后排的腿，有助于抬起的腿保持悬空
    set_expect_postion(otherRowLeg2,KEEP,yv-2*gravityOffset,offsetZ);//交叉侧的腿要更加下蹲一点，有助于抬起的腿保持悬空
    wait_all_reach_postion(); 
    set_expect_postion(i,xv,yv-highOfLegLift,offsetZ); //腿抬起
    wait_all_reach_postion(); 
    set_expect_postion(i,xv+moveStepSlowX[i],yv-highOfLegLift,offsetZ); //腿平移
    wait_all_reach_postion();     
    set_expect_postion(i,xv+moveStepSlowX[i],yv+gravityOffset,offsetZ); //腿放下

    wait_all_reach_postion(); 
}


void legLiftMove(bool bLeftFrontCross,bool leftForward ,bool rightForward)
{
  float stepXL,stepXR;
  int nLeft,nRight;
  int otherLeft,otherRight;  
  float offsetZ = 5;
  stepSpeed = walkSpeed;
  stepXL = stepLengthLeft/3;
  stepXR = stepLengthRight/3;  
  if(!leftForward)
    stepXL = -stepXL;
  if(!rightForward) 
    stepXR = -stepXR;

  if(is_stopped())
    return;
  if(bLeftFrontCross){
    nLeft = 0;
    nRight = 3;
    otherLeft = 2;
    otherRight = 1;    
  }
  else{
    nLeft = 2;
    nRight = 1;
    otherLeft = 0;
    otherRight = 3;    
  }
  
  set_expect_postion(nLeft,getStandX(nLeft),getStandY(nLeft)-highOfLegLift,offsetZ); 
  set_expect_postion(nRight,getStandX(nRight),getStandY(nRight)-highOfLegLift,offsetZ);   
  set_expect_postion(otherLeft,getStandX(otherLeft),getStandY(otherLeft),offsetZ); 
  set_expect_postion(otherRight,getStandX(otherRight),getStandY(otherRight),offsetZ);   
  wait_all_reach_postion();

  set_expect_postion(nLeft,getStandX(nLeft)-stepXL,getStandY(nLeft),offsetZ); 
  set_expect_postion(nRight,getStandX(nRight)-stepXR,getStandY(nRight),offsetZ);  
  set_expect_postion(otherLeft,getStandX(otherLeft)+stepXL,getStandY(otherLeft),offsetZ); 
  set_expect_postion(otherRight,getStandX(otherRight)+stepXR,getStandY(otherRight),offsetZ);  
  wait_all_reach_postion(); 

}
float getStandX(int i)
{
  float x;
  switch(i)
  {
    case 0:
    case 1:
      x = standX_Front;
      break;
    case 2:
    case 3:
      x = standX_Rear_For_Move;
      break;
  }
  return x;
}
float getStandY(int i)
{
  float y;
  switch(i)
  {
    case 0:
    case 1:
      y = standY_Front;
      break;
    case 2:
    case 3:
      y = standY_Rear;
      break;
  }
  return y;
}
void twistHortizonal(bool bLeft)
{

  float turnValue = 30;
  if(!bLeft)
    turnValue = -turnValue;
  
  set_expect_postion(0,KEEP,KEEP,turnValue); 
  set_expect_postion(3,KEEP,KEEP,turnValue);  
  
  set_expect_postion(1,KEEP,KEEP,-turnValue); 
  set_expect_postion(2,KEEP,KEEP,-turnValue);   
  wait_all_reach_postion();
}

void twistVertical(bool bHeadUp)
{
  float turnValue = 20;
  if(!bHeadUp)
    turnValue = -turnValue;  
    
  set_expect_postion(0,KEEP,standY_Front_Low+turnValue,KEEP); 
  set_expect_postion(1,KEEP,standY_Front_Low+turnValue,KEEP);  
  
  set_expect_postion(2,KEEP,standY_Rear_Low-turnValue,KEEP); 
  set_expect_postion(3,KEEP,standY_Rear_Low-turnValue,KEEP);   
  wait_all_reach_postion();    
}

void twistCircle(bool bClockwise)
{
  twistVertical(bClockwise);
  twistHortizonal(bClockwise);
  twistVertical(!bClockwise);
  twistHortizonal(!bClockwise);    
}

void sayHello()
{
  Serial.println("sayHello");
  moveSitPos();

  for(int i=0;i<3;i++){
    set_expect_postion(1,-50,50,0); 
    wait_all_reach_postion(); 
    set_expect_postion(1,50,100,0); 
    wait_all_reach_postion();   
  }
  moveSitPos();
}

void sayBye()
{
  Serial.println("sayBye");
  moveSitPos();

  for(int i=0;i<3;i++){
    set_expect_postion(1,-50,150,-100); 
    wait_all_reach_postion(); 
    set_expect_postion(1,-50,150,100); 
    wait_all_reach_postion();   
  }
  moveSitPos();
}

void rotateYaw(bool bRight,float rAngle)
{//水平转动
  moveStandPosLow();
  float x,y,x1,x2,z1,z2,theta;
  x = Le;
  y = Ld-standX_Front+standX_Rear;
  theta = rAngle*pi/180;
  float maxTheta = atan(x/y);
  if(theta>maxTheta)
    theta = maxTheta;

  float a = x/2*tan(theta/2);

  if(bRight)
  {
    z1=(y/2-a)*sin(theta);
    x1=(y/2+a)-(y/2-a)*cos(theta);
    z2=(y/2+a)*sin(theta);
    x2=(y/2+a)*cos(theta)-(y/2-a);
    set_expect_postion(0,standX_Front+x1,standY_Front_Low,-z1); 
    set_expect_postion(1,standX_Front-x2,standY_Front_Low,z2);  
    set_expect_postion(2,standX_Rear+x2,standY_Rear_Low,z2); 
    set_expect_postion(3,standX_Rear-x1,standY_Rear_Low,-z1);      
  }
  else
  {
    z1=(y/2+a)*sin(theta);
    x1=(y/2+a)*cos(theta)-(y/2-a);
    z2=(y/2-a)*sin(theta);
    x2=x*sin(theta)-(y/2+a)*cos(theta)+(y/2-a);
    set_expect_postion(0,standX_Front-x1,standY_Front_Low,z1); 
    set_expect_postion(1,standX_Front+x2,standY_Front_Low,-z2);  
    set_expect_postion(2,standX_Rear-x2,standY_Rear_Low,-z2); 
    set_expect_postion(3,standX_Rear+x1,standY_Rear_Low,z1);        
  }
  wait_all_reach_postion();  
   
}

void rotatePitch(bool bUp,float rAngle)
{//前后翻动
  moveStandPosLow();
  float delta = rAngle*pi/180;
  float x,y,dx,dy;
  x = Ld-standX_Front+standX_Rear;
  y = standY_Front;
  dx = x/2*(1-cos(delta));
  dy = y/2*sin(delta);
  if(bUp)
  {
    set_expect_postion(0,standX_Front+dx,standY_Front_Low+dy,0); 
    set_expect_postion(1,standX_Front+dx,standY_Front_Low+dy,0);  
    set_expect_postion(2,standX_Rear-dx,standY_Rear_Low-dy,0); 
    set_expect_postion(3,standX_Rear-dx,standY_Rear_Low-dy,0);      
  }
  else
  {
    set_expect_postion(0,standX_Front+dx,standY_Front_Low-dy,0); 
    set_expect_postion(1,standX_Front+dx,standY_Front_Low-dy,0);  
    set_expect_postion(2,standX_Rear-dx,standY_Rear_Low+dy,0); 
    set_expect_postion(3,standX_Rear-dx,standY_Rear_Low+dy,0);      
  }
  wait_all_reach_postion(); 
}

void rotateRoll(bool bRight,float rAngle)
{//左右侧翻
  moveStandPosLow();
  float delta = rAngle*pi/180;
  float w,y,dz,dy;
  w = Le;
  y = standY_Front_Low;  
  dz = w/2*(1-cos(delta));
  dy = w/2*sin(delta);
  if(bRight)
  {
    set_expect_postion(0,standX_Front,standY_Front_Low+dy,-dz); 
    set_expect_postion(1,standX_Front,standY_Front_Low-dy,-dz);  
    set_expect_postion(2,standX_Rear,standY_Rear_Low+dy,-dz); 
    set_expect_postion(3,standX_Rear,standY_Rear_Low-dy,-dz);      
  }
  else
  {
    set_expect_postion(0,standX_Front,standY_Front_Low-dy,-dz); 
    set_expect_postion(1,standX_Front,standY_Front_Low+dy,-dz);  
    set_expect_postion(2,standX_Rear,standY_Rear_Low-dy,-dz); 
    set_expect_postion(3,standX_Rear,standY_Rear_Low+dy,-dz);      
  }
  wait_all_reach_postion();   
}

void rotateSlope(bool bRight,float rAngle)
{//侧向转动
  moveStandPosLow();
  float x,y,y1,y2,z1,z2,theta;
  x = Le;    
  y = standY_Front;
  float maxAngle = 30;
  if(rAngle>maxAngle)
    rAngle = maxAngle;  
  theta = rAngle*pi/180;
  float a;
  a = x/2*tan(theta/2);
  y1 = (y-a)*cos(theta)-a;
  z1 = (y-a)*sin(theta);
  y2 = (y+a)*cos(theta)+a;
  z2 = (y+a)*sin(theta);

  if(bRight){
    set_expect_postion(0,standX_Front,y1,-z1); 
    set_expect_postion(2,standX_Rear,y1,-z1);   
    set_expect_postion(1,standX_Front,y2,z2);  
    set_expect_postion(3,standX_Rear,y2,z2);   
  }
  else
  {
    set_expect_postion(0,standX_Front,y2,z2); 
    set_expect_postion(2,standX_Rear,y2,z2);   
    set_expect_postion(1,standX_Front,y1,-z1);  
    set_expect_postion(3,standX_Rear,y1,-z1);     
  }
  wait_all_reach_postion(); 
}

bool isClimbPos()
{
  for(int i=0;i<4;i++)
  {
    if(abs(curPostion[i][1]-standY_Front_Low)>10)
      return false;
  }
  return true;
}
bool isWalkPos()
{
  for(int i=0;i<4;i++)
  {
    if(abs(curPostion[i][1]-standY_Front)>10)
      return false;
  }
  return true;  
}
