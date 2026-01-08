#ifndef ARDUINO_USB_MODE
#error This ESP32 SoC has no Native USB interface
#elif ARDUINO_USB_MODE == 1
#warning This sketch should be used when USB is in OTG mode
void setup() {}
void loop() {}
#else
#include "USB.h"
#include "USBHIDGamepad.h"

// Create two gamepad instances
USBHIDGamepad Gamepad1;
USBHIDGamepad Gamepad2;

void setup() {
  // Initialize both gamepads
  Gamepad1.begin();
  Gamepad2.begin();
  
  // Initialize USB
  USB.begin();
  
  // Wait for USB to be ready
  delay(1000);
}

void loop() {
  // Example: Press and release buttons on Gamepad 1
  // You can modify this to respond to your inputs (buttons, sensors, etc.)
  
  // Gamepad 1 - Press UP (D-pad up)
  // Using leftStick: negative Y is up
  Gamepad1.leftStick(0, -127);
  Gamepad1.hat(1);  // 1 = up
  delay(100);
  Gamepad1.leftStick(0, 0);
  Gamepad1.hat(HAT_CENTER);
  delay(100);
  
  // Gamepad 1 - Press DOWN (D-pad down)
  Gamepad1.leftStick(0, 127);  // Positive Y is down
  Gamepad1.hat(5);  // 5 = down
  delay(100);
  Gamepad1.leftStick(0, 0);
  Gamepad1.hat(HAT_CENTER);
  delay(100);
  
  // Gamepad 1 - Press LEFT (D-pad left)
  Gamepad1.leftStick(-127, 0);  // Negative X is left
  Gamepad1.hat(7);  // 7 = left
  delay(100);
  Gamepad1.leftStick(0, 0);
  Gamepad1.hat(HAT_CENTER);
  delay(100);
  
  // Gamepad 1 - Press RIGHT (D-pad right)
  Gamepad1.leftStick(127, 0);  // Positive X is right
  Gamepad1.hat(3);  // 3 = right
  delay(100);
  Gamepad1.leftStick(0, 0);
  Gamepad1.hat(HAT_CENTER);
  delay(100);
  
  // Gamepad 1 - Press B button (button 1)
  Gamepad1.pressButton(1);
  delay(100);
  Gamepad1.releaseButton(1);
  delay(100);
  
  // Gamepad 1 - Press A button (button 2)
  Gamepad1.pressButton(2);
  delay(100);
  Gamepad1.releaseButton(2);
  delay(500);
  
  // Gamepad 2 - Press UP (D-pad up)
  Gamepad2.leftStick(0, -127);
  Gamepad2.hat(1);
  delay(100);
  Gamepad2.leftStick(0, 0);
  Gamepad2.hat(HAT_CENTER);
  delay(100);
  
  // Gamepad 2 - Press DOWN (D-pad down)
  Gamepad2.leftStick(0, 127);
  Gamepad2.hat(5);
  delay(100);
  Gamepad2.leftStick(0, 0);
  Gamepad2.hat(HAT_CENTER);
  delay(100);
  
  // Gamepad 2 - Press LEFT (D-pad left)
  Gamepad2.leftStick(-127, 0);
  Gamepad2.hat(7);
  delay(100);
  Gamepad2.leftStick(0, 0);
  Gamepad2.hat(HAT_CENTER);
  delay(100);
  
  // Gamepad 2 - Press RIGHT (D-pad right)
  Gamepad2.leftStick(127, 0);
  Gamepad2.hat(3);
  delay(100);
  Gamepad2.leftStick(0, 0);
  Gamepad2.hat(HAT_CENTER);
  delay(100);
  
  // Gamepad 2 - Press B button (button 1)
  Gamepad2.pressButton(1);
  delay(100);
  Gamepad2.releaseButton(1);
  delay(100);
  
  // Gamepad 2 - Press A button (button 2)
  Gamepad2.pressButton(2);
  delay(100);
  Gamepad2.releaseButton(2);
  delay(1000);
}
#endif /* ARDUINO_USB_MODE */
