


#ifndef TONE_DEFINE
#define TONE_DEFINE
//定义频率表
#define STOP 0     // 休止符
#define T1L 262    // 低音1
#define T2L 294    // 低音2
#define T3L 330    // 低音3
#define T4L 349    // 低音4
#define T5L 392    // 低音5
#define T6L 440    // 低音6
#define T7L 494    // 低音7
#define T1 523     // 中音1
#define T2 578     // 中音2
#define T3 659     // 中音3
#define T4 698     // 中音4
#define T5 784     // 中音5
#define T6 880     // 中音6
#define T7 988     // 中音7
#define T1H 1046   // 高音1
#define T2H 1175   // 高音2
#define T3H 1318   // 高音3
#define T4H 1397   // 高音4
#define T5H 1568   // 高音5
#define T6H 1760   // 高音6
#define T7H 1976   // 高音7
#define T1LP 277   // 低音1#
#define T2LP 311   // 低音2#
#define T3LP 340   // 低音3#
#define T4LP 370   // 低音4#
#define T5LP 415   // 低音5#
#define T6LP 466   // 低音6#
#define T7LP 524   // 低音7#
#define T1P 554    // 中音1#
#define T2P 622    // 中音2#
#define T3P 682    // 中音3#
#define T4P 740    // 中音4#
#define T5P 831    // 中音5#
#define T6P 932    // 中音6#
#define T7P 1046   // 中音7#
#define T1HP 1109  // 高音1#
#define T2HP 1245  // 高音2#
#define T3HP 1356  // 高音3#
#define T4HP 1480  // 高音4#
#define T5HP 1661  // 高音5#
#define T6HP 1865  // 高音6#
#define T7HP 2066  // 高音7#

#define C 1         // 1拍
#define CC 2        // 2拍
#define CCC 3       // 3拍
#define CCCC 4      // 4拍
#define C2 0.5      // 1/2拍
#define C4 0.25     // 1/4拍
#define C8 0.125    // 1/8拍
#define C16 0.0625  // 1/16拍

#endif


int soundPort = 10;
int soundArray1[] =  {T1, T2, T3, T1, T5, T6, T6, T1H, T6, T5, T6, T6, T1H, T5, T6, T3, T6, T5, T3, T5, T3, T1, T2, T3, T1};
float paceArray1[] = {C2, C2, C2, C2, CC, C2, C2, C2,  C2, CC, C2, C2, C,  C2, C2,   C, C2, C2, C2, C2, C2, C2, C2, C2, CC};

int soundArray2[] =  {T5, T5, T3, T2, T1, T5,  T5, T5, T5, T3, T2, T1, T6,   T6, T6, T4, T3, T2, T6,    T6, T5, T5, T4, T2, T3,   T1, T5, T5, T3, T2, T1, T5,    T5, T5, T3, T2, T1, T6};
float paceArray2[] = {C,  C2, C2, C2, C2, C ,  C2, C2, C2, C2, C2, C2 , C,    C,  C2, C2, C2, C2, C ,    C,  C2, C2, C2, C2, C,    C2, C2, C2, C2, C2, C2, C,     C, C2, C2, C2, C2, C};

int count = 0;

void setup() {
  pinMode(soundPort, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  count++;
  Serial.println("------loop-----count");
  Serial.println(sizeof(soundArray1));
  Serial.println(sizeof(soundArray2));
  if (count % 2 == 0) {
    Serial.println("play sound 1----------");
    playSong1();
    //playMusic(soundArray1,paceArray1);
  }
  else {
    Serial.println("play sound 2-----------");
    playSong2();
  }
  delay(2000);
}

void playSong1() {
  int length = sizeof(soundArray1) / 2;
  Serial.print("length=");
  Serial.print(length);
  for (int i = 0; i < length; i = i + 1) {
    play(soundArray1[i], paceArray1[i]);
  }
}


void playSong2() {
  int length = sizeof(soundArray2) / 2;
  Serial.print("length=");
  Serial.print(length);
  for (int i = 0; i < length; i = i + 1) {
    play(soundArray2[i], paceArray2[i]);
  }
}

void play(unsigned int frequency, float pace) {
  unsigned long duration = pace * 1000;
  if (frequency > 0) {
    tone(soundPort, frequency, duration - 10);
  }

  delay(duration);
}
