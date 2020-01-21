超声波测距

void setup() {
  // put your setup code here, to run once:
  pinMode(6,OUTPUT);
  pinMode(5,INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.write("start...\n");
  digitalWrite(6,HIGH);
  delayMicroseconds(1);
  digitalWrite(6,LOW);

  int distance = pulseIn(5,HIGH)/58;
  Serial.print("dis = ");
  Serial.print(distance);
  Serial.println();
  delay(100);
}


四路寻迹

#define LED1 A0
#define LED2 A1
#define LED3 A2
#define LED4 A3

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED1,INPUT);
  pinMode(LED2,INPUT);
  pinMode(LED3,INPUT);
  pinMode(LED4,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int L1,L2,L3,L4;
  L1 = analogRead(LED1);//无障碍 高电平1022， 否则低电平 22
  L2 = analogRead(LED2);
  L3 = analogRead(LED3);
  L4 = analogRead(LED4);  
  Serial.print(L1);
  Serial.print("-");  
  Serial.print(L2);
  Serial.print("-");  
  Serial.print(L3);
  Serial.print("-");  
  Serial.print(L4);
  Serial.println("");  
  delay(1000);
}

码盘检测

#define speedPin 5//定义数字3接口
int cntValue=0;
void setup ()
{
  pinMode(speedPin,INPUT);//5号数字口设置为输入状态
  Serial.begin(115200);
  Serial.println("Speed Count\n");
  attachInterrupt(digitalPinToInterrupt(speedPin),IntCallback,FALLING);
}

ICACHE_RAM_ATTR void IntCallback()
{
  cntValue++;//计数增加
  Serial.println(cntValue);
}

void loop()
{

}

