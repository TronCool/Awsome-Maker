
void print_tempExpect(int leg)
{
    String s = "print_tempExpect leg["+ String(leg) +"]";
    Serial.print(s);
    for(int j=0;j<3;j++){
      String s3 = " ["+String(j)+"]= "+String(angleExpectTemp[leg][j]);
      Serial.print(s3);
    } 
    Serial.println();
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
      String s2 = " exp["+String(j)+"]= "+String(angleExpect[i][j]);
      Serial.print(s2);
    }   
    Serial.println();
    for(int j=0;j<3;j++){
      String s3 = " temp["+String(j)+"]= "+String(angleExpectTemp[i][j]);
      Serial.print(s3);
    } 
    Serial.println();
  }
  Serial.println();
}
