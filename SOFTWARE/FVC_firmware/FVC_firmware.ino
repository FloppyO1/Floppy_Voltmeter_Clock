/*
  This code was written by Floppy Lab for the Floppy Voltmeter Clock
  This code is under license (check it on the github folder)
  https://github.com/FloppyO1/Floppy_Voltmeter_Clock
*/

#include <Wire.h>
#include <DS3231.h>

// FLOPPY BOARD 3.1
#define V_ORE PB0
#define V_MINUTE PA6
#define A_SECOND PA7
#define SDA PB9
#define SCL PB8
#define BUTTON_HOURS PA4
#define BUTTON_MINUTES PA3


DS3231 myRTC;
bool h12Flag;
bool pmFlag;

void setup() {
  pinMode(V_ORE, OUTPUT);
  pinMode(V_MINUTE, OUTPUT);
  pinMode(A_SECOND, OUTPUT);
  pinMode(BUTTON_MINUTES, INPUT_PULLUP);
  pinMode(BUTTON_HOURS, INPUT_PULLUP);

  Serial.begin(9600);
  Wire.setSDA(SDA);
  Wire.setSCL(SCL);
  Wire.begin();
  delay(500);
  analogWriteResolution(8);

  myRTC.setClockMode(true);  // set to 12h
}

uint32_t timeDebouce = 0;

void loop() {
  setHourHand(myRTC.getHour(h12Flag, pmFlag));
  setMinuteHand(myRTC.getMinute());
  setSecondsHand(myRTC.getSecond());

  if (digitalRead(BUTTON_HOURS) == LOW) {
    if (millis() - timeDebouce >= 300) {
      uint8_t hour = myRTC.getHour(h12Flag, pmFlag);
      if (hour + 1 > 12) {
        hour = 1;
      } else {
        hour++;
      }
      myRTC.setHour(hour);
      timeDebouce = millis();
    }
  }
  if (digitalRead(BUTTON_MINUTES) == LOW) {
    if (millis() - timeDebouce >= 300) {
      uint8_t min = myRTC.getMinute();
      if (min + 1 > 60) {
        min = 1;
      } else {
        min++;
      }
      myRTC.setMinute(min);
      timeDebouce = millis();
    }
  }
}

void setHourHand(int n) {
  if (!n) n = 0;
  float x = 256 / 12;  // 12 number of position (12 hours)
  x = x * (n-1);
  int duty = x;
  analogWrite(V_ORE, duty);
}
void setMinuteHand(int n) {
  if (!n) n = 0;
  float x = 256 / 60;  // 60 number of position (60 minutes)
  x = x * n;
  int duty = x;
  analogWrite(V_MINUTE, duty);
}
void setSecondsHand(int n) {
  if (!n) n = 0;
  float x = 256 / 60;  // 60 number of position (60 seconds)
  x = x * n;
  int duty = x;
  analogWrite(A_SECOND, duty);
}

// void setNumber(int n, int nmax) {
//   if (!n) n = 0;
//   float x = 256 / nmax;
//   x = x * (n - 1);
//   int duty = x;
//   analogWrite(V_ORE, duty);
// }
