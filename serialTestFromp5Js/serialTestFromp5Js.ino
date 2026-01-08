#include <AsyncTimer.h>

int timing = 500;
int ledPin = LED_BUILTIN;

// see Example 5 for more sophisticated serial data sending/receiving
// https://forum.arduino.cc/t/serial-input-basics-updated/382007/3#p-2780919-example-5-receiving-and-parsing-several-pieces-of-data-3



AsyncTimer t;
unsigned short intervalId;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("starting");
  intervalId = t.setInterval(toggleLed, timing);

}

void loop() {
  t.handle();

  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    // Use parseInt() to look for the next integer in the buffer
    long receivedValue = Serial.parseInt(); 
    // Optional: read the trailing newline/carriage return characters
    if (Serial.read() == '\n') {
      // Process the received value here
      // ... 
      Serial.println(receivedValue);
      timing = receivedValue;
      t.changeDelay(intervalId, timing);
    }  
  }
}

int ledState = LOW;
void toggleLed(){
  digitalWrite(ledPin, !digitalRead(ledPin));
}
