int angleCurrent[4][3];
int angleExpert[4][3];
int angleReady[4][3];
int stepAngle = 1;
int actionIntval = 2000;
int checkCount =0;
int checkFreq = 1;
void set_current_angle(int leg, int alpha, int beta, int gama)
{
  if(leg<0 || leg>=4)return; 
  if(alpha != KEEP)
    angleCurrent[leg][0]= alpha;
  if(beta != KEEP)
    angleCurrent[leg][1]= beta;
  if(gama != KEEP)
    angleCurrent[leg][2]= gama;    
}

void set_expect_angle(int leg, int alpha, int beta, int gama)
{
  if(leg<0 || leg>=4)return;  
  if(alpha != KEEP)
    angleExpert[leg][0]= alpha;
  if(beta != KEEP)
    angleExpert[leg][1]= beta;
  if(gama != KEEP)
    angleExpert[leg][2]= gama;  
}

//void set_expect_angle_offset(int leg, int angleJointOffset, int angleArmOffset, int angleWristOffset)
//{
//  if(leg<0 || leg>=4)return;  
//  if(angleJointOffset != KEEP)
//    angleExpert[leg][0]+= angleJointOffset;
//  if(angleArmOffset != KEEP)
//    angleExpert[leg][1]+= angleArmOffset;
//  if(angleWristOffset != KEEP)
//    angleExpert[leg][2]+= angleWristOffset; 
//}

void check_expect_angle()
{
  int nReady=0;
  for(int i=0;i<4;i++)
  {
    nReady = 0;
    for(int j=0;j<3;j++)
    {
      if(angleCurrent[i][j]!= angleExpert[i][j])
      {  
        if(abs(angleCurrent[i][j] - angleExpert[i][j])<stepAngle)
          angleCurrent[i][j] = angleExpert[i][j];
        else{
         if(angleCurrent[i][j] < angleExpert[i][j])
           angleCurrent[i][j] += stepAngle;
         else
           angleCurrent[i][j] -= stepAngle;
        }
        angleReady[i][j] = 0;       
      }
      else{
        angleReady[i][j] = KEEP;
        continue;
      }
    }
    for(int j=0;j<3;j++)
    {
      nReady += angleReady[i][j];
    }
    if(nReady == 3*KEEP)//如果1条腿的3个节点已经驱动到位，就不要再计算舵机的角度了
      continue; 

    drive_to_servo(i);
  }
}
void drive_to_servo(int leg )
{
//  Serial.println("drive_to_servo");
   
  float alpha, beta, gama;
  alpha = angleCurrent[leg][0];
  beta = angleCurrent[leg][1];
  gama = angleCurrent[leg][2];
    if(leg == 0 || leg == 2){
      
    
    }
    else if(leg == 1 || leg == 3){
      alpha = 300 - alpha;
      beta = 180 - beta;  
    }
    gama = 90;
  setAngle300(Legs_pin[leg][0],alpha);
  setAngle(Legs_pin[leg][1],beta); 
  setAngle(Legs_pin[leg][2],gama);    
//  Serial.print(alpha);
//  Serial.print(" ");
//  Serial.println(beta); 
}
int wait_leg_reach(int leg)
{
  while (1){
      if(angleCurrent[leg][0]== angleExpert[leg][0]){
        if(angleCurrent[leg][1]== angleExpert[leg][1]){
          if(angleCurrent[leg][2]== angleExpert[leg][2]){
            break;
          }
        }
      }
      delay(10);   
  }
}
void wait_all_reach()
{
  checkPostionOrAngle = 0;
  for (int i = 0; i < 4; i++)
    wait_leg_reach(i);
}
void print_angle()
{
  Serial.println("print_angle");
  for(int i=0;i<4;i++){
    String s = "print leg["+ String(i) +"]";
    Serial.println(s);
    for(int j=0;j<3;j++){
      String s1 = " cur["+String(j)+"]= "+String(angleCurrent[i][j]);
      Serial.print(s1);
    }
    Serial.println();
    for(int j=0;j<3;j++){
      String s2 = " exp["+String(j)+"]= "+String(angleExpert[i][j]);
      Serial.print(s2);
    }   
    Serial.println();
  }
  Serial.println();
}

//void lie_down()
//{
//  Serial.println("lie_down");
//  checkFreq = 1;
//  set_expect_angle(0,45,150,10);
//  set_expect_angle(1,45,150,10);
//  set_expect_angle(2,60,40,180);
//  set_expect_angle(3,60,40,180);
//  wait_all_reach();
//}
//
//void stand_half()
//{
//  Serial.println("stand_half");
//  checkFreq = 1;
//  set_expect_angle(0,45,140,55);
//  set_expect_angle(1,45,140,55);
//  set_expect_angle(2,60,50,135);
//  set_expect_angle(3,60,50,135);  
//  wait_all_reach();
//}
//
//void stand()
//{
//  Serial.println("stand");
//  checkFreq = 1;
//  set_expect_angle(0,45,130,100);
//  set_expect_angle(1,45,130,100);
//  set_expect_angle(2,60,60,90);
//  set_expect_angle(3,60,60,90);  
//  wait_all_reach();
//}
//
//void go_forward(int steps,int spd)
//{
//  Serial.println("go_forward");
//  stand();
//  wait_all_reach();
//  checkFreq = spd;
//  for(int i = 0;i<steps;i++)
//  {
//    String s = "go_forward step="+String(i);
//    Serial.println(s);
//    set_expect_angle_offset(0,KEEP,KEEP,10);
//    set_expect_angle_offset(3,KEEP,KEEP,10);
//    wait_all_reach();
//    set_expect_angle_offset(0,KEEP,30,-10);
//    set_expect_angle_offset(3,KEEP,30,-10);
//    wait_all_reach();
//
//
//    set_expect_angle_offset(0,KEEP,-30,KEEP);
//    set_expect_angle_offset(3,KEEP,-30,KEEP);
//    set_expect_angle_offset(1,KEEP,KEEP,-10);
//    set_expect_angle_offset(2,KEEP,KEEP,-10); 
//    wait_all_reach(); 
//    set_expect_angle_offset(1,KEEP,30,KEEP);
//    set_expect_angle_offset(2,KEEP,30,KEEP);    
//    wait_all_reach();    
//    set_expect_angle_offset(1,KEEP,30,KEEP);
//    set_expect_angle_offset(2,KEEP,30,KEEP);    
//    wait_all_reach();    
//
//    set_expect_angle_offset(1,KEEP,-30,KEEP);
//    set_expect_angle_offset(2,KEEP,-30,KEEP);    
//    wait_all_reach(); 
//
//  }
//}

void initLegs()
{
  for (int i = 0; i < 4; i++){
    for(int j=0;j<3;j++){
      if(j == 0)
        setAngle300(Legs_pin[i][j],150+Legs_offset[i][j]);
      else
        setAngle(Legs_pin[i][j],90+Legs_offset[i][j]);
      delay(10);
    }
  }
}

void test300serve()
{
  for(int i=0;i<4;i++)
  {
    setAngle300(Legs_pin[3][0],i*90);
    delay(2000);
  }
}
