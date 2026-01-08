#include "USB.h"
#include "USBHIDKeyboard.h"
USBHIDKeyboard Keyboard;
// 16 total pins
int readPins[] =    {D2, D3, D4, D5, D6, D7, D8, D9,  D10, D11, D12, A0};//,  A1, A2, A3, A4}; // using A5 and beyond seems to create some sort of reset state
int prevVals[] =    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
char pinLetters[] = {'a','b','c','d','e','f','g','h', 'i', 'j', 'k', 'l'};//, 'm','n','o','p'};
int numPins = 16 ;

void setup(){

  Keyboard.begin();
  USB.begin();

  for(int i = 0; i < numPins; i ++){
    pinMode(readPins[i], INPUT_PULLUP);
  }
  Serial.begin(9600);
}


void loop(){

  //Loop through and read all 16 values
  //Reports back Value at channel 6 is: 346
  for(int i = 0; i < numPins; i ++){
    int val = digitalRead(readPins[i]);
    /*
    Serial.print("reading ");
    Serial.print(readPins[i]);
    Serial.print(" : ");
    Serial.println(val);
*/
    if(prevVals[i] != val){
      if(val == 0){
        Keyboard.press(pinLetters[i]);
      }else{
        Keyboard.release(pinLetters[i]);
      }

    }
    prevVals[i] = val;
  }



}

