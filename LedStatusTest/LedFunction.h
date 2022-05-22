/*
 * @Author: Mm1KEE 
 * @Date: 2021-10-07 00:20:41 
 * @Last Modified by:   Mm1KEE 
 * @Last Modified time: 2022-05-22 12:20:34 
 */

#include"KeyboardMod.h"
//#include <HID-Project.h>
int LedA = 9;
int LedB = 10;
enum BlinkType {
  NUM_LOCK,
  CAPS_LOCK,
  SCROLL_LOCK,
  COMPOSE,
  KANA,
  ANY,
  None
};
bool CapsLock = false;
bool NumLock = false;
bool ScrollLock = false;

struct BlinkResult {
  BlinkType LedType;
  int Blink;
  bool TimeOut;
};
struct LedStatus {
  int leds;
  bool NumLockPrev, CapsLockPrev, ScrollLockPrev;
  int Blinks[5] = {0, 0, 0, 0, 0}; //NUM_LOCK,CAPS_LOCK,SCROLL_LOCK,COMPOSE,KANA,
  long StartTime;
  long CurrentTime;
  long WaitTime;
  bool Started = false;

  void init() {
    Keyboard.begin();
    //AbsoluteMouse.begin();
    pinMode(LedB, OUTPUT);
    pinMode(LedA, OUTPUT);
  }

  bool Update(long WaitTime) {
    bool TimeOut;
    //leds = BootKeyboard.getLeds();
    leds = Keyboard.getLeds();
    //Serial.println(leds);
    NumLockPrev = NumLock;
    CapsLockPrev = CapsLock;
    ScrollLockPrev = ScrollLock;
    CurrentTime = millis();
    //Serial.println(" started:"+String(Started));
    if (!Started) {
      ClearBlinks();
      Started = true;
      StartTime = millis();
      TimeOut = false;
      //Serial.println("start");
      //Serial.println("start:"+String(StartTime)+" started:"+String(Started));
    }
    else {
      if (CurrentTime >= StartTime + WaitTime) {
        TimeOut = true;
        //Serial.println("stop at "+String(CurrentTime));
      }
      else TimeOut = false;
    }
    if (leds & (1 << NUM_LOCK)) NumLock = true;
    else NumLock = false;
    if (leds & (1 << CAPS_LOCK)) CapsLock = true;
    else CapsLock = false;
    if (leds & (1 << SCROLL_LOCK)) ScrollLock = true;
    else ScrollLock = false;
    //Serial.println(String(NumLockPrev)+"  "+String(NumLock));
    if (NumLockPrev != NumLock) Blinks[NUM_LOCK]++;
    if (CapsLockPrev != CapsLock) Blinks[CAPS_LOCK]++;
    if (ScrollLockPrev != ScrollLock) Blinks[SCROLL_LOCK]++;
    digitalWrite(LedA, NumLock);
    digitalWrite(LedB, CapsLock);
    //for(int i=0;i<4;i++) Serial.print(String(Blinks[i])+",");Serial.println();
    //Serial.println(String(StartTime) + " " + String(CurrentTime) + " " + String(Started));
    return TimeOut;
  }
  bool GetLockStatus(BlinkType Type) {
    switch (Type) {
      case CAPS_LOCK:
        return CapsLock;
      case NUM_LOCK:
        return NumLock;
      case SCROLL_LOCK:
        return ScrollLock;
    }
  }

  BlinkResult GetLedBlink(BlinkType Type, long WaitTime) {
    BlinkResult result;
    result.TimeOut = Update(WaitTime);
    if (result.TimeOut) Started = false;
    switch (Type) {
      case CAPS_LOCK:
        result.Blink = Blinks[CAPS_LOCK];
        result.LedType = CAPS_LOCK;
        //if (Blinks[CAPS_LOCK]>=1) Serial.println("Caps lock blink: " + String(Blinks[CAPS_LOCK]));
        break;
      case NUM_LOCK:
        result.Blink = Blinks[NUM_LOCK];
        result.LedType = NUM_LOCK;
        //Serial.println("Num lock blink: " + String(Blink));
        break;
      case SCROLL_LOCK:
        result.Blink = Blinks[SCROLL_LOCK];
        result.LedType = SCROLL_LOCK;
        break;
      default:
        result.Blink = -1;
        result.LedType = None;
        break;
    }
    //Serial.println(String(TimeIndex) + "  " + String(Blink));
    return result;
  }

  BlinkResult WaitLedBlink(BlinkType Type, long WaitTime) {
    int Blink;
    BlinkResult result;
    result.TimeOut = Update(WaitTime);
    //Serial.println(String(Blinks[NUM_LOCK])+" "+String(Blinks[CAPS_LOCK])+" "+String(StartTime + WaitTime));
    switch (Type) {
      case CAPS_LOCK:
        if (Blinks[CAPS_LOCK] > 0) {
          result.Blink = Blinks[CAPS_LOCK];
          result.LedType = CAPS_LOCK;
          Started = false;
        }
        else result.LedType = None;
        break;
      case NUM_LOCK:
        if (Blinks[NUM_LOCK] > 0) {
          result.Blink = Blinks[NUM_LOCK];
          result.LedType = NUM_LOCK;
          Started = false;
        }
        else result.LedType = None;
        break;
      case SCROLL_LOCK:
        if (Blinks[SCROLL_LOCK] > 0) {
          result.Blink = Blinks[SCROLL_LOCK];
          result.LedType = SCROLL_LOCK;
          Started = false;
        }
        else result.LedType = None;
        break;
      case ANY:
        /*if (Blinks[NUM_LOCK] > 0 || Blinks[CAPS_LOCK] > 0 || Blinks[SCROLL_LOCK] > 0) {
          if (Blinks[NUM_LOCK] >= Blinks[CAPS_LOCK]) {
            result.LedType = NUM_LOCK;
            Started = false;
          }
          else {
            result.LedType = CAPS_LOCK;
            Started = false;
          }
          }*/
        if (Blinks[NUM_LOCK] > 0) {
          result.Blink = Blinks[NUM_LOCK];
          result.LedType = NUM_LOCK;
          Started = false;
          break;
        }
        else if (Blinks[CAPS_LOCK] > 0) {
          result.Blink = Blinks[CAPS_LOCK];
          result.LedType = CAPS_LOCK;
          Started = false;
          break;
        }
        else if (Blinks[SCROLL_LOCK] > 0) {
          result.Blink = Blinks[SCROLL_LOCK];
          result.LedType = SCROLL_LOCK;
          Started = false;
          break;
        }
        else result.LedType = None;;
        break;
      default:
        result.LedType = None;
        break;

    }
    //Serial.println(String(TimeIndex) + "  " + String(Blink));
    //Serial.println(String(result.TimeOut));
    return result;
  }

  void ClearBlinks() {
    for (int i = 0; i < 5; i++) Blinks[i] = 0;
  }
};
