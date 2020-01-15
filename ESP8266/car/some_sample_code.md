超声波测距

  Serial.write("start...\n");
  digitalWrite(6,HIGH);
  delayMicroseconds(1);
  digitalWrite(6,LOW);

  int distance = pulseIn(5,HIGH)/58;
  Serial.print("dis = ");
  Serial.print(distance);
  Serial.println();
  delay(100);
