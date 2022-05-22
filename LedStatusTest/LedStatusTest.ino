#include "LedFunction.h"
LedStatus Led;
BlinkResult result;
String ledType[]={"Num lock","Caps lock","Scroll lock"};

void setup() {
  Led.init();
  Serial.begin(115200);
  while(!Serial);
}

void loop() {
  //Led.Update(1000);
  result = Led.WaitLedBlink(ANY, 1000);
  //result = Led.GetLedBlink(NUM_LOCK, 1000);
  if(result.LedType!=None and result.Blink>0) Serial.println(String(result.LedType)+","+String(result.Blink));
}
