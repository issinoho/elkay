// LK201 / LK401 protocol constants. See docs/protocol.md.
#pragma once

#include <stdint.h>

namespace lk {

const uint32_t kBaud = 4800;

// Responses from the keyboard (0xB3-0xBB are never keycodes).
const uint8_t kAllUps = 0xB3;
const uint8_t kMetronome = 0xB4;
const uint8_t kOutputError = 0xB5;
const uint8_t kInputError = 0xB6;
const uint8_t kKbdLocked = 0xB7;
const uint8_t kTestModeAck = 0xB8;
const uint8_t kPrefixKeysDown = 0xB9;
const uint8_t kModeChangeAck = 0xBA;
const uint8_t kReserved = 0xBB;

// Power-up report.
const uint8_t kPowerUpId = 0x01;
const uint8_t kStuckKey = 0x3D;
const uint8_t kSelfTestFailed = 0x3E;

// Commands to the keyboard.
const uint8_t kLedOff = 0x11;
const uint8_t kLedOn = 0x13;
const uint8_t kEnableKeyclick = 0x1B;
const uint8_t kEnableBell = 0x23;
const uint8_t kSoundBell = 0xA7;
const uint8_t kDisableKeyclick = 0x99;
const uint8_t kDisableBell = 0xA1;
const uint8_t kRequestId = 0xAB;
const uint8_t kDisableCtrlClick = 0xB9;
const uint8_t kEnableCtrlClick = 0xBB;
const uint8_t kSetDefaults = 0xD3;
const uint8_t kEnableLk401 = 0xE9;
const uint8_t kPowerUpReset = 0xFD;

// LED parameter bits, sent as 0x80 | mask after kLedOn / kLedOff.
const uint8_t kLedWait = 0x01;
const uint8_t kLedCompose = 0x02;
const uint8_t kLedLock = 0x04;
const uint8_t kLedHold = 0x08;
const uint8_t kLedAll = 0x0F;

// Division modes, and the mode-set command byte: 1 DDDD MM 0.
const uint8_t kModeDown = 0x00;
const uint8_t kModeAutoRepeat = 0x02;
const uint8_t kModeUpDown = 0x06;
const uint8_t kDivisions = 14;

inline uint8_t setMode(uint8_t division, uint8_t mode) {
  return 0x80 | (division << 3) | mode;
}

inline uint8_t param(uint8_t value) { return 0x80 | value; }

// The name of a response byte, or nullptr for a keycode.
inline const char *responseName(uint8_t b) {
  switch (b) {
    case kAllUps: return "ALL UPS";
    case kMetronome: return "METRONOME";
    case kOutputError: return "OUTPUT ERROR";
    case kInputError: return "INPUT ERROR";
    case kKbdLocked: return "KBD LOCKED";
    case kTestModeAck: return "TEST MODE ACK";
    case kPrefixKeysDown: return "PREFIX KEYS DOWN";
    case kModeChangeAck: return "MODE CHANGE ACK";
    case kReserved: return "RESERVED";
    default: return nullptr;
  }
}

}  // namespace lk
