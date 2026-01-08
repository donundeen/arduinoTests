#ifndef ARDUINO_USB_MODE
#error This ESP32 SoC has no Native USB interface
#elif ARDUINO_USB_MODE == 1
#warning This sketch should be used when USB is in OTG mode
void setup() {}
void loop() {}
#else
#include "Adafruit_Debounce.h"
#include "USB.h"
#include <Joystick_ESP32S2.h>

/*
Quirks:alignas  
Product ID:	0x0070
  Vendor ID:	0x2341

  cmdline.txt
  usbhid.quirks=0xVID:0xPID:0x040
  usbhid.quirks=0x2341:0x0070:0x040  
  
*/

// Create two joystick instances with different report IDs
// Report ID 1 for Player 1, Report ID 2 for Player 2
Joystick_ Joystick1(JOYSTICK_DEFAULT_REPORT_ID,     // Report ID 1
                    JOYSTICK_TYPE_GAMEPAD,          // Joystick Type: Gamepad
                    2,                              // Button Count (A and B)
                    1,                              // Hat Switch Count (D-pad)
                    true,                           // Include X Axis
                    true,                           // Include Y Axis
                    false,                          // Include Z Axis
                    false,                          // Include Rx Axis
                    false,                          // Include Ry Axis
                    false,                          // Include Rz Axis
                    false,                          // Include Rudder
                    false,                          // Include Throttle
                    false,                          // Include Accelerator
                    false,                          // Include Brake
                    false);                         // Include Steering

Joystick_ Joystick2(JOYSTICK_DEFAULT_REPORT_ID + 1, // Report ID 2
                    JOYSTICK_TYPE_GAMEPAD,          // Joystick Type: Gamepad
                    2,                              // Button Count (A and B)
                    1,                              // Hat Switch Count (D-pad)
                    true,                           // Include X Axis
                    true,                           // Include Y Axis
                    false,                          // Include Z Axis
                    false,                          // Include Rx Axis
                    false,                          // Include Ry Axis
                    false,                          // Include Rz Axis
                    false,                          // Include Rudder
                    false,                          // Include Throttle
                    false,                          // Include Accelerator
                    false,                          // Include Brake
                    false);                         // Include Steering

// 12 total pins
int readPins[] =    {D2, D3, D4, D5, D6, D7, D8, D9,  D10, D11, D12, A0};
int prevVals[] =    {0,0,0,0,0,0,0,0,0,0,0,0};

/*Controls map like this:
Player 1:
D2 : Player 1 UP
D3 : Player 1 DOWN
D4 : Player 1 LEFT
D5 : Player 1 RIGHT
D6 : Player 1 Button B
D7 : Player 1 Button A

Player 2:
D8 : Player 2 UP
D9 : Player 2 DOWN
D10 : Player 2 LEFT
D11 : Player 2 RIGHT
D12 : Player 2 Button B
A0 : Player 2 Button A
*/

#define NUM_PINS 12
int numPins = NUM_PINS;

Adafruit_Debounce* buttons[NUM_PINS];            // Array of button objects

// Track direction button states for each player
bool player1Up = false;
bool player1Down = false;
bool player1Left = false;
bool player1Right = false;
bool player1ButtonA = false;
bool player1ButtonB = false;

bool player2Up = false;
bool player2Down = false;
bool player2Left = false;
bool player2Right = false;
bool player2ButtonA = false;
bool player2ButtonB = false;

// Function to update Player 1 gamepad based on current direction states
void updatePlayer1Gamepad() {
  int x = 0;
  int y = 0;
  int hat = JOYSTICK_HATSWITCH_RELEASE;
  
  // Calculate X axis (-127 to 127)
  if (player1Left && !player1Right) {
    x = -127;
  } else if (player1Right && !player1Left) {
    x = 127;
  }
  
  // Calculate Y axis (-127 to 127)
  if (player1Up && !player1Down) {
    y = -127;
  } else if (player1Down && !player1Up) {
    y = 127;
  }
  
  // Set hat direction based on combination
  // Joystick library uses degrees: 0=Up, 45=UpRight, 90=Right, etc.
  if (player1Up && player1Left) {
    hat = 315;  // Up-Left
  } else if (player1Up && player1Right) {
    hat = 45;   // Up-Right
  } else if (player1Down && player1Left) {
    hat = 225;  // Down-Left
  } else if (player1Down && player1Right) {
    hat = 135;  // Down-Right
  } else if (player1Up) {
    hat = 0;    // Up
  } else if (player1Down) {
    hat = 180;  // Down
  } else if (player1Left) {
    hat = 270;  // Left
  } else if (player1Right) {
    hat = 90;   // Right
  }
  
  // Set axes
  Joystick1.setXAxis(x);
  Joystick1.setYAxis(y);
  
  // Set hat switch
  Joystick1.setHatSwitch(0, hat);
  
  // Set buttons
  Joystick1.setButton(0, player1ButtonB ? 1 : 0);  // Button 0 = B
  Joystick1.setButton(1, player1ButtonA ? 1 : 0);  // Button 1 = A
  
  // Print direction change
  Serial.print("Player 1 Direction: ");
  if (hat == JOYSTICK_HATSWITCH_RELEASE) {
    Serial.println("CENTER");
  } else if (hat == 315) {
    Serial.println("UP-LEFT (diagonal)");
  } else if (hat == 0) {
    Serial.println("UP");
  } else if (hat == 45) {
    Serial.println("UP-RIGHT (diagonal)");
  } else if (hat == 90) {
    Serial.println("RIGHT");
  } else if (hat == 135) {
    Serial.println("DOWN-RIGHT (diagonal)");
  } else if (hat == 180) {
    Serial.println("DOWN");
  } else if (hat == 225) {
    Serial.println("DOWN-LEFT (diagonal)");
  } else if (hat == 270) {
    Serial.println("LEFT");
  }
  Serial.print("  LeftStick: X=");
  Serial.print(x);
  Serial.print(", Y=");
  Serial.println(y);
}

// Function to update Player 2 gamepad based on current direction states
void updatePlayer2Gamepad() {
  int x = 0;
  int y = 0;
  int hat = JOYSTICK_HATSWITCH_RELEASE;
  
  // Calculate X axis (-127 to 127)
  if (player2Left && !player2Right) {
    x = -127;
  } else if (player2Right && !player2Left) {
    x = 127;
  }
  
  // Calculate Y axis (-127 to 127)
  if (player2Up && !player2Down) {
    y = -127;
  } else if (player2Down && !player2Up) {
    y = 127;
  }
  
  // Set hat direction based on combination
  if (player2Up && player2Left) {
    hat = 315;  // Up-Left
  } else if (player2Up && player2Right) {
    hat = 45;   // Up-Right
  } else if (player2Down && player2Left) {
    hat = 225;  // Down-Left
  } else if (player2Down && player2Right) {
    hat = 135;  // Down-Right
  } else if (player2Up) {
    hat = 0;    // Up
  } else if (player2Down) {
    hat = 180;  // Down
  } else if (player2Left) {
    hat = 270;  // Left
  } else if (player2Right) {
    hat = 90;   // Right
  }
  
  // Set axes
  Joystick2.setXAxis(x);
  Joystick2.setYAxis(y);
  
  // Set hat switch
  Joystick2.setHatSwitch(0, hat);
  
  // Set buttons
  Joystick2.setButton(0, player2ButtonB ? 1 : 0);  // Button 0 = B
  Joystick2.setButton(1, player2ButtonA ? 1 : 0);  // Button 1 = A
  
  // Print direction change
  Serial.print("Player 2 Direction: ");
  if (hat == JOYSTICK_HATSWITCH_RELEASE) {
    Serial.println("CENTER");
  } else if (hat == 315) {
    Serial.println("UP-LEFT (diagonal)");
  } else if (hat == 0) {
    Serial.println("UP");
  } else if (hat == 45) {
    Serial.println("UP-RIGHT (diagonal)");
  } else if (hat == 90) {
    Serial.println("RIGHT");
  } else if (hat == 135) {
    Serial.println("DOWN-RIGHT (diagonal)");
  } else if (hat == 180) {
    Serial.println("DOWN");
  } else if (hat == 225) {
    Serial.println("DOWN-LEFT (diagonal)");
  } else if (hat == 270) {
    Serial.println("LEFT");
  }
  Serial.print("  LeftStick: X=");
  Serial.print(x);
  Serial.print(", Y=");
  Serial.println(y);
}

void setup(){
  Serial.begin(115200);

  // Set USB device name (this is what shows up in your computer)
  // Note: setDeviceName may not be available on all ESP32 variants
  // USB.setDeviceName("MockyMocky Game Controller");
  
  // Initialize USB
  USB.begin();
  
  // Initialize both joysticks
  Joystick1.begin();
  Joystick2.begin();
  
  // Set axis ranges for both joysticks (-127 to 127)
  Joystick1.setXAxisRange(-127, 127);
  Joystick1.setYAxisRange(-127, 127);
  Joystick2.setXAxisRange(-127, 127);
  Joystick2.setYAxisRange(-127, 127);
  
  // Wait for USB to be ready
  delay(1000);

  // Initialize debounce buttons
  for (uint8_t i = 0; i < numPins; i++){
    buttons[i] = new Adafruit_Debounce(readPins[i], LOW);
    buttons[i]->begin();
  }
  
  Serial.println("MockyMocky Game Controllers initialized");
  Serial.println("Player 1: Report ID 1");
  Serial.println("Player 2: Report ID 2");
}

void loop() {
  for (uint8_t i = 0; i < numPins; i++){
    buttons[i]->update();
    
    // Player 1 controls (pins 0-5: D2-D7)
    if (i < 6) {
      // Handle direction buttons (0-3)
      if (i < 4) {
        bool stateChanged = false;
        
        if (buttons[i]->justPressed()) {
          switch(i) {
            case 0: // D2 - UP
              Serial.println("Player 1 - UP pressed");
              player1Up = true;
              break;
            case 1: // D3 - DOWN
              Serial.println("Player 1 - DOWN pressed");
              player1Down = true;
              break;
            case 2: // D4 - LEFT
              Serial.println("Player 1 - LEFT pressed");
              player1Left = true;
              break;
            case 3: // D5 - RIGHT
              Serial.println("Player 1 - RIGHT pressed");
              player1Right = true;
              break;
          }
          stateChanged = true;
        }
        
        if (buttons[i]->justReleased()) {
          switch(i) {
            case 0: // D2 - UP
              Serial.println("Player 1 - UP released");
              player1Up = false;
              break;
            case 1: // D3 - DOWN
              Serial.println("Player 1 - DOWN released");
              player1Down = false;
              break;
            case 2: // D4 - LEFT
              Serial.println("Player 1 - LEFT released");
              player1Left = false;
              break;
            case 3: // D5 - RIGHT
              Serial.println("Player 1 - RIGHT released");
              player1Right = false;
              break;
          }
          stateChanged = true;
        }
        
        if (stateChanged) {
          updatePlayer1Gamepad();
        }
      }
      // Handle action buttons (4-5)
      else {
        bool stateChanged = false;
        
        if (buttons[i]->justPressed()) {
          switch(i) {
            case 4: // D6 - Button B
              Serial.println("Player 1 - Button B pressed");
              player1ButtonB = true;
              stateChanged = true;
              break;
            case 5: // D7 - Button A
              Serial.println("Player 1 - Button A pressed");
              player1ButtonA = true;
              stateChanged = true;
              break;
          }
        }
        
        if (buttons[i]->justReleased()) {
          switch(i) {
            case 4: // D6 - Button B
              Serial.println("Player 1 - Button B released");
              player1ButtonB = false;
              stateChanged = true;
              break;
            case 5: // D7 - Button A
              Serial.println("Player 1 - Button A released");
              player1ButtonA = false;
              stateChanged = true;
              break;
          }
        }
        
        if (stateChanged) {
          updatePlayer1Gamepad();
        }
      }
    }
    // Player 2 controls (pins 6-11: D8-A0)
    else {
      // Handle direction buttons (6-9)
      if (i < 10) {
        bool stateChanged = false;
        
        if (buttons[i]->justPressed()) {
          switch(i) {
            case 6: // D8 - UP
              Serial.println("Player 2 - UP pressed");
              player2Up = true;
              break;
            case 7: // D9 - DOWN
              Serial.println("Player 2 - DOWN pressed");
              player2Down = true;
              break;
            case 8: // D10 - LEFT
              Serial.println("Player 2 - LEFT pressed");
              player2Left = true;
              break;
            case 9: // D11 - RIGHT
              Serial.println("Player 2 - RIGHT pressed");
              player2Right = true;
              break;
          }
          stateChanged = true;
        }
        
        if (buttons[i]->justReleased()) {
          switch(i) {
            case 6: // D8 - UP
              Serial.println("Player 2 - UP released");
              player2Up = false;
              break;
            case 7: // D9 - DOWN
              Serial.println("Player 2 - DOWN released");
              player2Down = false;
              break;
            case 8: // D10 - LEFT
              Serial.println("Player 2 - LEFT released");
              player2Left = false;
              break;
            case 9: // D11 - RIGHT
              Serial.println("Player 2 - RIGHT released");
              player2Right = false;
              break;
          }
          stateChanged = true;
        }
        
        if (stateChanged) {
          updatePlayer2Gamepad();
        }
      }
      // Handle action buttons (10-11)
      else {
        bool stateChanged = false;
        
        if (buttons[i]->justPressed()) {
          switch(i) {
            case 10: // D12 - Button B
              Serial.println("Player 2 - Button B pressed");
              player2ButtonB = true;
              stateChanged = true;
              break;
            case 11: // A0 - Button A
              Serial.println("Player 2 - Button A pressed");
              player2ButtonA = true;
              stateChanged = true;
              break;
          }
        }
        
        if (buttons[i]->justReleased()) {
          switch(i) {
            case 10: // D12 - Button B
              Serial.println("Player 2 - Button B released");
              player2ButtonB = false;
              stateChanged = true;
              break;
            case 11: // A0 - Button A
              Serial.println("Player 2 - Button A released");
              player2ButtonA = false;
              stateChanged = true;
              break;
          }
        }
        
        if (stateChanged) {
          updatePlayer2Gamepad();
        }
      }
    }
    delay(10);
  }
}
#endif /* ARDUINO_USB_MODE */
