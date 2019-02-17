#include "ps2_Keyboard.h"
#include "ps2_AnsiTranslator.h"
#include "ps2_SimpleDiagnostics.h"

typedef ps2::SimpleDiagnostics<254> Diagnostics_;
static Diagnostics_ diagnostics;
static ps2::AnsiTranslator<Diagnostics_> keyMapping(diagnostics);
static ps2::Keyboard<4,3,1, Diagnostics_> ps2Keyboard(diagnostics);
static ps2::KeyboardLeds lastLedSent = ps2::KeyboardLeds::none;

bool data = true;
bool isExtended = false;
bool wasAKey = false;
char dataBuf[1];

void sendKeyEvent(char keyDown, char c) {
//    Serial.print(keyDown, DEC);
//    Serial.print(" ");
//    Serial.println(c, DEC);
//    Serial.write(keyDown);
//    Serial.write(c);
//    return;
    char x = c / 8;
    char y = c % 8;
    char bit0 = x & 0b00000001;
    char bit1 = (x & 0b00000010) >> 1;
    char bit2 = (x & 0b00000100) >> 2;
    char bit3 = y & 0b00000001;
    char bit4 = (y & 0b00000010) >> 1;
    char bit5 = (y & 0b00000100) >> 2;

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
    Serial.begin(115200);
    Serial.setTimeout(100);
//    while (Serial.read() >= 0);
    ps2Keyboard.begin();
    ps2Keyboard.setScanCodeSet(ps2::ScanCodeSet::pcat);
    keyMapping.setNumLock(true);
    ps2Keyboard.awaitStartup();

    // see the docs for awaitStartup - TL;DR <- when we reset the board but not the keyboard, awaitStartup
    //  records an error because it thinks the keyboard didn't power-up correctly.  When debugging, that's
    //  true - but only because it never powered down.
    diagnostics.reset();

    ps2Keyboard.sendLedStatus(ps2::KeyboardLeds::numLock);
    lastLedSent = ps2::KeyboardLeds::numLock;

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

ps2::KeyboardOutput smartRead() {
  if (Serial.available()) {
    char incomingByte = Serial.read();
    return (ps2::KeyboardOutput) incomingByte;
  } else {
    ps2::KeyboardOutput scanCode = ps2Keyboard.readScanCode();
    return scanCode;
  }
}

void codesSwitch(ps2::KeyboardOutput scannedCode) {
  if (((byte) scannedCode) == 0xe0) {
    isExtended = true;
    scannedCode = smartRead();
    if (scannedCode == ps2::KeyboardOutput::none) {
      return;
    }
  }

  if (((byte) scannedCode) == 0xf0) {
    data = false;
    scannedCode = smartRead();
    if (scannedCode == ps2::KeyboardOutput::none) {
      return;
    }
  }
  
  if (!isExtended) {
    switch((byte) scannedCode) {
      case 0x1c: // A
        sendKeyEvent(data, 1);
        break;
      case 0x32: // B
        sendKeyEvent(data, 2);
        break;
      case 0x21: // C
        sendKeyEvent(data, 3);
        break;
      case 0x23: // D
        sendKeyEvent(data, 4);
        break;
      case 0x24: // E
        sendKeyEvent(data, 5);
        break;
      case 0x2b: // F
        sendKeyEvent(data, 6);
        break;
      case 0x34: // G
        sendKeyEvent(data, 7);
        break;
      case 0x33: // H
        sendKeyEvent(data, 8);
        break;
      case 0x43: // I
        sendKeyEvent(data, 9);
        break;
      case 0x3b: // J
        sendKeyEvent(data, 10);
        break;
      case 0x42: // K
        sendKeyEvent(data, 11);
        break;
      case 0x4b: // L
        sendKeyEvent(data, 12);
        break;
      case 0x3a: // M
        sendKeyEvent(data, 13);
        break;
      case 0x31: // N
        sendKeyEvent(data, 14);
        break;
      case 0x44: // O
        sendKeyEvent(data, 15);
        break;
      case 0x4d: // P
        sendKeyEvent(data, 16);
        break;
      case 0x15: // Q
        sendKeyEvent(data, 17);
        break;
      case 0x2d: // R
        sendKeyEvent(data, 18);
        break;
      case 0x1b: // S
        sendKeyEvent(data, 19);
        break;
      case 0x2c: // T
        sendKeyEvent(data, 20);
        break;
      case 0x3c: // U
        sendKeyEvent(data, 21);
        break;
      case 0x2a: // V
        sendKeyEvent(data, 22);
        break;
      case 0x1d: // W
        sendKeyEvent(data, 23);
        break;
      case 0x22: // X
        sendKeyEvent(data, 24);
        break;
      case 0x35: // Y
        sendKeyEvent(data, 25);
        break;
      case 0x1a: // Z
        sendKeyEvent(data, 26);
        break;
      case 0x29: // Space
        sendKeyEvent(data, 31);
        break;
      case 0x45: // 0
      case 0x70: // 0 (num)
        sendKeyEvent(data, 32);
        break;
      case 0x16: // 1
      case 0x69: // 1 (num)
        sendKeyEvent(data, 33);
        break;
      case 0x1e: // 2
      case 0x72: // 2 (num)
        sendKeyEvent(data, 34);
        break;
      case 0x26: // 3
      case 0x7a: // 3 (num)
        sendKeyEvent(data, 35);
        break;
      case 0x25: // 4
      case 0x6b: // 4 (num)
        sendKeyEvent(data, 36);
        break;
      case 0x2e: // 5
      case 0x73: // 5 (num)
        sendKeyEvent(data, 37);
        break;
      case 0x36: // 6
      case 0x74: // 6 (num)
        sendKeyEvent(data, 38);
        break;
      case 0x3d: // 7
      case 0x6c: // 7 (num)
        sendKeyEvent(data, 39);
        break;
      case 0x3e: // 8
      case 0x75: // 8 (num)
        sendKeyEvent(data, 40);
        break;
      case 0x46: // 9
      case 0x7d: // 9 (num)
        sendKeyEvent(data, 41);
        break;
      case 0x4c: // ;
        sendKeyEvent(data, 42);
        break;
      case 0x52: // : (kbd ')
        sendKeyEvent(data, 43);
        break;
      case 0x41: // ,
        sendKeyEvent(data, 44);
        break;
      case 0x55: // =
        sendKeyEvent(data, 45);
        break;
      case 0x49: // .
        sendKeyEvent(data, 46);
        break;
      case 0x4a: // /
        sendKeyEvent(data, 47);
        break;
      case 0x5a: // Enter
        sendKeyEvent(data, 48);
        break;
      case 0x76: // Break (kbd Esc)
        sendKeyEvent(data, 49);
        break;
      case 0x0d: // Repeat (kbd Tab)
        sendKeyEvent(data, 50);
        break;
      case 0x12: // Shift [Left]
      case 0x59: // Shift [Right]
        sendKeyEvent(data, 53);
        break;
      case 0x66: // Backspace
        sendKeyEvent(data, 29);
        break;
      default:
        break;
    }
  } else {
    switch((byte) scannedCode) {
      case 0x72: // Down arrow
        sendKeyEvent(data, 28);
        break;
      case 0x7a: // Page down
        sendKeyEvent(data, 52);
        break;
      case 0x6b: // Left arrow
        sendKeyEvent(data, 29);
        break;
      case 0x74: // Right arrow
        sendKeyEvent(data, 30);
        break;
      case 0x75: // Up arrow
        sendKeyEvent(data, 27);
        break;
      case 0x71: // Delete
        sendKeyEvent(data, 51);
        break;
      default:
        break;
    }
  }

  if (scannedCode != ps2::KeyboardOutput::none) {
    data = true;
    isExtended = false;
  }
}

void loop() {
//  if (!Serial.available()) {
//    diagnostics.setLedIndicator<LED_BUILTIN>();
//    ps2::KeyboardOutput scanCode = smartRead();
//    if (scanCode == ps2::KeyboardOutput::garbled) {
//        keyMapping.reset();
//    }
//    else if (scanCode != ps2::KeyboardOutput::none)
//    {
////        Serial.println((byte) scannedCode, HEX);
////        Serial.println("first read");
////    Serial.println((byte) scanCode, HEX);
//
//        codesSwitch(scanCode);
//        
//        ps2::KeyboardLeds newLeds =
//              (keyMapping.getCapsLock() ? ps2::KeyboardLeds::capsLock : ps2::KeyboardLeds::none)
//            | (keyMapping.getNumLock() ? ps2::KeyboardLeds::numLock : ps2::KeyboardLeds::none);
//        if (newLeds != lastLedSent) {
//            ps2Keyboard.sendLedStatus(newLeds);
//            lastLedSent = newLeds;
//        }
//    }
//  } else {
    ps2::KeyboardOutput incomingByte = smartRead();
    codesSwitch(incomingByte);
//  }
}
