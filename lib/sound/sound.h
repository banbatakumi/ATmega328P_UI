#ifndef sound_h
#define sound_h
#include "Arduino.h"

#define Bt2 1200   // 2分音符
#define Bt4 500   // 4分音符、スタッカート気味
#define Bt8f 450   // 付点8分音符
#define Bt8 300   // 8分音符
#define Bt16 150   // 16分音符

// 音符に対してのDelay Time(msec)を設定
#define Dt2 1100
#define Dt4 500
#define Dt8f 350
#define Dt8 200
#define Dt16 50

// 音程を定義する
#define G4 392   // ソ
#define Ds4 311   // レ＃
#define As4 466   // ラ＃
#define D5 587   // レ
#define Ds5 622   // レ＃
#define Fs4 370   // ファ＃
#define G5 784   // ソ
#define Fs5 740   // ファ＃
#define F5 698   // ファ
#define E5 659   // ミ
#define Gs4 415   // ソ＃
#define Cs5 554   // ド＃
#define C5 523   // ド
#define B4 494   // シ
#define A4 440   // ラ

#define NOTE_C3 131   // ド
#define NOTE_CS3 139
#define NOTE_D3 147   // レ
#define NOTE_DS3 156
#define NOTE_E3 165   // ミ
#define NOTE_F3 175   // ファ
#define NOTE_FS3 185
#define NOTE_G3 196   // ソ
#define NOTE_GS3 208
#define NOTE_A3 220   // ラ
#define NOTE_AS3 233
#define NOTE_B3 247   // シ
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_DS5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GS5 831
#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988
#define NOTE_C6 1047
#define NOTE_CS6 1109
#define NOTE_D6 1175
#define NOTE_DS6 1245
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_FS6 1480
#define NOTE_G6 1568
#define NOTE_GS6 1661
#define NOTE_A6 1760
#define NOTE_AS6 1865
#define NOTE_B6 1976

class sound {
     public:
      sound(int pin_);
      void DarthVader();
      void Disney();

     private:
      int pin;
};

#endif