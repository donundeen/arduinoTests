//abcdefghijkl


#include "Adafruit_Debounce.h"

#include "USB.h"
#include "USBHIDKeyboard.h"
USBHIDKeyboard Keyboard;
// 16 total pins
int readPins[] =    {D2, D3, D4, D5, D6, D7, D8, D9,  D10, D11, D12, A0};//,  A1, A2, A3, A4}; // using A5 and beyond seems to create some sort of reset state
int prevVals[] =    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
char pinLetters[] = {'a','b','c','d','e','f','g','h', 'i', 'j', 'k', 'l'};//, 'm','n','o','p'};
/*Controls should map like this
D2 : Player 1 UP
D3 : Player 1 DOWN
D4 : Player 1 LEFT
D5 : Player 1 RIGHT
D6 : Player 1 Button B
D7 : Player 1 Button A

D8 : Player 1 UP
D9 : Player 1 DOWN
D10 : Player 1 LEFT
D11 : Player 1 RIGHT
D12 : Player 1 Button B
A0 : Player 1 Button A


*/
#define NUM_PINS 12
int numPins = NUM_PINS;

Adafruit_Debounce* buttons[NUM_PINS];            // Array of button objects


void setup(){
  Serial.begin(115200);

  Keyboard.begin();
  USB.begin();

  for (uint8_t i = 0; i < numPins; i++){
    buttons[i] = new Adafruit_Debounce(readPins[i], LOW);
    buttons[i]->begin();
  }
}

void loop() {
  for (uint8_t i = 0; i < numPins; i++){
    buttons[i]->update();
    if (buttons[i]->justPressed()) {
      Serial.println("Button was just pressed!");
      Serial.println(pinLetters[i]);
      Keyboard.press(pinLetters[i]);      
    }

    if (buttons[i]->justReleased()) {
      Serial.println("Button was just released!");
      Serial.println(pinLetters[i]);
      Keyboard.release(pinLetters[i]);      
    }
    delay(10);
  }
}
