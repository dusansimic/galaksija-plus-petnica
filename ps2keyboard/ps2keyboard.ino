/*  PS2Keyboard library example
  
  PS2Keyboard now requries both pins specified for begin()

  keyboard.begin(data_pin, irq_pin);
  
  Valid irq pins:
     Arduino Uno:  2, 3
     Arduino Due:  All pins, except 13 (LED)
     Arduino Mega: 2, 3, 18, 19, 20, 21
     Teensy 2.0:   All pins, except 13 (LED)
     Teensy 2.0:   5, 6, 7, 8
     Teensy 1.0:   0, 1, 2, 3, 4, 6, 7, 16
     Teensy++ 2.0: 0, 1, 2, 3, 18, 19, 36, 37
     Teensy++ 1.0: 0, 1, 2, 3, 18, 19, 36, 37
     Sanguino:     2, 10, 11
  
  for more information you can read the original wiki in arduino.cc
  at http://www.arduino.cc/playground/Main/PS2Keyboard
  or http://www.pjrc.com/teensy/td_libs_PS2Keyboard.html
  
  Like the Original library and example this is under LGPL license.
  
  Modified by Cuninganreset@gmail.com on 2010-03-22
  Modified by Paul Stoffregen <paul@pjrc.com> June 2010
*/
   
#include <PS2Keyboard.h>

const int DataPin = 4;
const int IRQpin =  3;

PS2Keyboard keyboard;

//enum UGLY_CHARS {
//  UP_ARROW = 11,
//  DOWN_ARROW = 10,
//  LEFT_ARROW = 8,
//  RIGHT_ARROW = 21
//};

//char ASCII_ARR[127] = {
//  0, 0, 0, 0, 0, 0, 0, 0,
//  "backspace", "tab", "down arrow", "up arrow", 0, "return", 0, 0,
//  0, 0, "list", 0, 0, "right arrow", 0, 0,
//  ' ', 0, 0, "escape", 0, 0, 0, 0,
//  0, 0, 0, 0, 0, 0, 0, 0, 0,
//  0, 0, 0, ',', 0, '.', '/',
//  '0', '1', '2', '3', '4', '5', '6', '7',
//  '8', '9', ':', ';', 0, '=', 0, 0,
//  0, 'A', 'B', 'C', 'D', 'E', 'F', 'G',
//  'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
//  'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
//  'X', 'Y', 'Z', 0, "shift", 0, 0, 0,
//  0, 0, 0, 0, 0, 0, 0, 0,
//  0, 0, 0, 0, 0, 0, 0, 0,
//  0, 0, 0, 0, 0, 0, 0, 0,
//  0, 0, 0, 0, 0, 0, 0, "delete"
//}

char ASCII_ARR[128] = {
   0,  0,  0,  0,  0,  0,  0,  0,
  29, 50, 28, 27,  0, 48,  0,  0,
   0,  0, 52,  0,  0, 30,  0,  0,
  31,  0,  0, 49,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0, 44,  0, 46, 47,
  32, 33, 34, 35, 36, 37, 38, 39,
  40, 41, 43, 42,  0, 45,  0,  0,
   0,  1,  2,  3,  4,  5,  6,  7,
   8,  9, 10, 11, 12, 13, 14, 15,
  16, 17, 18, 19, 20, 21, 22, 23,
  24, 25, 26,  0, 53,  0,  0,  0,
   0,  1,  2,  3,  4,  5,  6,  7,
   8,  9, 10, 11, 12, 13, 14, 15,
  16, 17, 18, 19, 20, 21, 22, 23,
  24, 25, 26,  0,  0,  0,  0, 51
};

char lastStateKey = 0;
char lateLastStateKey = 0;
bool alreadyReleased = true;

void sendKeyEvent(char keyDown, char c) {
    char x = ASCII_ARR[c] / 8;
    char y = ASCII_ARR[c] % 8;
    char bit0 = x & 0b00000001;
    char bit1 = (x & 0b00000010) >> 1;
    char bit2 = (x & 0b00000100) >> 2;
    char bit3 = y & 0b00000001;
    char bit4 = (y & 0b00000010) >> 1;
    char bit5 = (y & 0b00000100) >> 2;

    Serial.print(keyDown, DEC);
    Serial.print(" ");
    Serial.print(bit2, DEC);
    Serial.print(" ");
    Serial.print(bit1, DEC);
    Serial.print(" ");
    Serial.print(bit0, DEC);
    Serial.print(" ");
    Serial.print(bit5, DEC);
    Serial.print(" ");
    Serial.print(bit4, DEC);
    Serial.print(" ");
    Serial.println(bit3, DEC);

    digitalWrite(5, bit0);      // x0
    digitalWrite(6, bit1);      // x1
    digitalWrite(7, bit2);      // x2
    digitalWrite(8, bit3);      // y0
    digitalWrite(9, bit4);      // y1
    digitalWrite(10, bit5);     // y2
    digitalWrite(13, keyDown);  // data
    digitalWrite(12, HIGH);     // strobe in
    digitalWrite(12, LOW);      // strobe out hehe
}

void setup() {
  delay(1000);
  keyboard.begin(DataPin, IRQpin);
  Serial.begin(9600);
  Serial.println("Keyboard Test:");

  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

  digitalWrite(HIGH, 11); // reset
  digitalWrite(LOW, 11);
}

void loop() {
  if (keyboard.available()) {
    
    // read the next key
    char c = keyboard.read();
    lateLastStateKey = lastStateKey;
    lastStateKey = c;

    sendKeyEvent(1, c);
    if (lastStateKey != lateLastStateKey) {
      alreadyReleased = false;
    }

//    Serial.println(c, DEC);
//    Serial.println(ASCII_ARR[c], DEC);
  } else if (!alreadyReleased) {
    sendKeyEvent(0, lastStateKey);
    alreadyReleased = true;
  }
}
