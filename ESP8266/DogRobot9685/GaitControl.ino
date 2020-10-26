//void set_expect_default_angle()
//{
//  set_expect_angle(0,100,70,0);
//  set_expect_angle(1,50,150,20);    
//  set_expect_angle(2,100,95,180);
//  set_expect_angle(3,65,40,170);  
//}

void set_expect_default_angle()
{
  set_expect_angle(0,95,90,95);
  set_expect_angle(1,95,100,95);    
  set_expect_angle(2,90,90,90);
  set_expect_angle(3,90,90,90);  
}

void setDefaultAngle()
{
  Serial.println("setDefaultAngle");
  set_expect_default_angle();
  set_angle_expect_to_current();
  wait_all_reach();
}

void set_current_angle(int leg, int angleJoint, int angleArm, int angleWrist)
{
  if(leg<0 || leg>=4)return; 
  
  if(angleJoint != KEEP)
  {
    angleCurrent[leg][0]= angleJoint;
  }

  if(angleArm != KEEP){  
    angleCurrent[leg][1]= angleArm;
  }
  
  if(angleWrist != KEEP){   
    angleCurrent[leg][2]= angleWrist;  
  }
    
}

void set_expect_angle(int leg, int angleJoint, int angleArm, int angleWrist)
{
  if(leg<0 || leg>=4)return;  
    
  if(angleJoint != KEEP){
    angleExpect[leg][0]= angleJoint;
  }
  
  if(angleArm != KEEP){     
    angleExpect[leg][1]= angleArm;
  }
  
  if(angleWrist != KEEP){      
    angleExpect[leg][2]= angleWrist;  
  }
  
  angleExpectTemp[leg][0] = -1;
  angleExpectTemp[leg][1] = -1;
  angleExpectTemp[leg][2] = -1;
}

void set_expect_angle_offset(int leg, int angleJointOffset, int angleArmOffset, int angleWristOffset,int angleWristOffsetTemp)
{
  if(leg<0 || leg>=4)return;   
//  String s = "leg = "+String(leg)+",angleJointOffset = "+String(angleJointOffset)+",angleArmOffset = "+String(angleArmOffset)+",angleWristOffset = "+String(angleWristOffset)+",angleWristOffsetTemp = "+String(angleWristOffsetTemp);
//  Serial.println(s);

  if(leg ==2 || leg == 3)//left offset is normal , right offset is reverse
  {
    angleJointOffset =-angleJointOffset;
    angleArmOffset = -angleArmOffset;
    angleWristOffset = -angleWristOffset;
    if(angleWristOffsetTemp != 0)
      angleWristOffsetTemp = -angleWristOffsetTemp;
  }

//angleExpectTemp  需要针对当前角度设过度角度
  if(angleWristOffsetTemp == 0)
    angleExpectTemp[leg][2] = -1;
  else
    angleExpectTemp[leg][2] = angleCurrent[leg][2]+ angleWristOffsetTemp; 
    
  if(angleJointOffset != KEEP && angleJointOffset != -KEEP){
    angleExpect[leg][0]+= angleJointOffset;

  }
  
  if(angleArmOffset != KEEP && angleArmOffset != -KEEP){
    angleExpect[leg][1]+= angleArmOffset;
  }
  
  if(angleWristOffset != KEEP && angleWristOffset != -KEEP){  
    angleExpect[leg][2]+= angleWristOffset; 
  }


//  print_angle();
//  String s1 = "leg = "+String(leg)+",angleJoint = "+String(angleExpect[leg][0])+",angleArm = "+String(angleExpect[leg][1])+",angleWrist = "+String(angleExpect[leg][2])+",angleWristTemp = "+String(angleExpectTemp[leg][2]);
//  Serial.println(s1);
//  print_tempExpect(leg);
}


void set_expect_angle_offset_left(int angleJointOffset, int angleArmOffset, int angleWristOffset,int angleWristOffsetTemp)
{
  set_expect_angle_offset(0,angleJointOffset,angleArmOffset,angleWristOffset, angleWristOffsetTemp);
  set_expect_angle_offset(1,angleJointOffset,angleArmOffset,angleWristOffset, angleWristOffsetTemp);  
}

void set_expect_angle_offset_right(int angleJointOffset, int angleArmOffset, int angleWristOffset,int angleWristOffsetTemp)
{
  set_expect_angle_offset(2,angleJointOffset,angleArmOffset,angleWristOffset, angleWristOffsetTemp);
  set_expect_angle_offset(3,angleJointOffset,angleArmOffset,angleWristOffset, angleWristOffsetTemp);  
}

void set_expect_angle_offset_rear(int angleJointOffset, int angleArmOffset, int angleWristOffset,int angleWristOffsetTemp)
{
  set_expect_angle_offset(1,angleJointOffset,angleArmOffset,angleWristOffset, angleWristOffsetTemp);
  set_expect_angle_offset(3,angleJointOffset,angleArmOffset,angleWristOffset, angleWristOffsetTemp);  
}

void set_expect_angle_offset_front(int angleJointOffset, int angleArmOffset, int angleWristOffset,int angleWristOffsetTemp)
{
  set_expect_angle_offset(0,angleJointOffset,angleArmOffset,angleWristOffset, angleWristOffsetTemp);
  set_expect_angle_offset(2,angleJointOffset,angleArmOffset,angleWristOffset, angleWristOffsetTemp);  
}

void set_expect_angle_offset_leftfront_cross(int angleJointOffset, int angleArmOffset, int angleWristOffset,int angleWristOffsetTemp)
{
  set_expect_angle_offset(0,angleJointOffset,angleArmOffset,angleWristOffset,angleWristOffsetTemp);
  set_expect_angle_offset(3,angleJointOffset,angleArmOffset,angleWristOffset,angleWristOffsetTemp);  
}

void set_expect_angle_offset_rightfront_cross(int angleJointOffset, int angleArmOffset, int angleWristOffset,int angleWristOffsetTemp)
{
  set_expect_angle_offset(2,angleJointOffset,angleArmOffset,angleWristOffset,angleWristOffsetTemp);
  set_expect_angle_offset(1,angleJointOffset,angleArmOffset,angleWristOffset,angleWristOffsetTemp);  
}

void set_expect_angle_offset_all_legs(int angleJointOffset, int angleArmOffset, int angleWristOffset)
{
  set_expect_angle_offset(0,angleJointOffset,angleArmOffset,angleWristOffset,0);
  set_expect_angle_offset(1,angleJointOffset,angleArmOffset,angleWristOffset,0);   
  set_expect_angle_offset(2,angleJointOffset,angleArmOffset,angleWristOffset,0);
  set_expect_angle_offset(3,angleJointOffset,angleArmOffset,angleWristOffset,0);    
}

//void go_forward_climb()
//{
//  int armAngle = 30;
//  int wristAngle = 30;
//
//  for(int sep=0; sep<5;sep++){
//  set_expect_angle_offset(0,KEEP,KEEP,-wristAngle);
//  wait_all_reach();
//  set_expect_angle_offset(0,KEEP,-armAngle,wristAngle); 
//  wait_all_reach(); 
//
//  set_expect_angle_offset(3,KEEP,KEEP,-wristAngle);
//  wait_all_reach();
//  set_expect_angle_offset(3,KEEP,-armAngle,wristAngle); 
//  wait_all_reach();   
//
//  set_expect_angle_offset(2,KEEP,KEEP,-wristAngle);
//  wait_all_reach();
//  set_expect_angle_offset(2,KEEP,-armAngle,wristAngle); 
//  wait_all_reach();  
//
//  set_expect_angle_offset(1,KEEP,KEEP,-wristAngle);
//  wait_all_reach();
//  set_expect_angle_offset(1,KEEP,-armAngle,wristAngle); 
//  wait_all_reach();  
//
//  for(int i=0;i<4;i++)
//    set_expect_angle_offset(i,KEEP,armAngle,KEEP); 
//    
//  }
//
//}

void go_forward()
{
    int armAngle = 10;
    int wristAngle = 10;
    Serial.println("go_forward");
//status 0 
    //stand
//status 1
    Serial.println("status 1");
    set_expect_angle_offset_leftfront_cross(KEEP,-armAngle,wristAngle,-wristAngle);
    set_expect_angle_offset_rightfront_cross(KEEP,armAngle,wristAngle,0);
    wait_all_reach(); 
//    print_angle();
    delay(5000);
    for(int i=0;i<2;i++){
      
//status 2  
      Serial.println("status 2");
      set_expect_angle_offset_leftfront_cross(KEEP,2*armAngle,KEEP,0);  
      set_expect_angle_offset_rightfront_cross(KEEP,-2*armAngle,KEEP,-wristAngle);
      wait_all_reach();

//      print_angle();
      delay(5000);

//status 3    == stauts 1
      Serial.println("status 3");
      set_expect_angle_offset_leftfront_cross(KEEP,-2*armAngle,KEEP,-wristAngle);
      set_expect_angle_offset_rightfront_cross(KEEP,2*armAngle,KEEP,0);
      wait_all_reach(); 
//      print_angle();
      delay(5000);
    }
    
//status 4
    Serial.println("status 4");
    set_expect_angle_offset_leftfront_cross(KEEP,armAngle,-wristAngle,0);
    set_expect_angle_offset_rightfront_cross(KEEP,-armAngle,-wristAngle,-0); 
//    print_angle();
    delay(5000);   
}   

void stand()
{
  Serial.println("stand");
  checkFreq = 1;
  set_expect_default_angle(); 
  set_expect_angle_offset_all_legs(KEEP,-10,-50); 
//  set_angle_expect_to_current();
  wait_all_reach();
  delay(2000);

  set_expect_default_angle(); 
  set_expect_angle_offset_all_legs(KEEP,-45,KEEP); 
  wait_all_reach();
  delay(2000); 
    
  set_expect_default_angle(); 
  set_expect_angle_offset_all_legs(KEEP,-60,45); 
//  set_angle_expect_to_current();
  wait_all_reach();
  delay(2000);  
  
}



void lie_down()
{
  Serial.println("lie_down");
  checkFreq = 1;
  set_expect_default_angle();
  set_expect_angle_offset_all_legs(KEEP,-40,-50);
  set_angle_expect_to_current();
  wait_all_reach();
  delay(2000);
}

void setAllAngle()
{
    for(int i=0;i<4;i++)
    { 
      for(int j=0;j<3;j++)
       setAngle(Legs_pin[i][j],90);//drive servo by same level joint
    }  
}
