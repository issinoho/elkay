// elkay stage 1: LK401 serial sniffer and command console.
//
// Every byte from the keyboard is printed to the USB serial console, and
// commands typed on the console are sent to the keyboard. Used to confirm the
// wiring and record the keycode table (docs/keymap.md) before the HID stage.
//
// Console input, one line at a time:
//   hex bytes    e.g. "13 84" (Lock LED on), sent as typed
//   init         the initialisation sequence from docs/protocol.md
//   reset        power-up reset; the keyboard repeats its self-test report
//   id           request the keyboard ID
//   bell         sound the bell
//   click / noclick
//   help

#include <Arduino.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "lk.h"

static const uint32_t kConsoleBaud = 115200;
static const uint8_t kActivityLed = LED_BUILTIN;
static const unsigned long kActivityMs = 30;

static char line[64];
static uint8_t lineLen;
static unsigned long activityUntil;

static void printHex(uint8_t b) {
  if (b < 0x10) Serial.print('0');
  Serial.print(b, HEX);
}

static void printStamp() {
  Serial.print(millis());
  Serial.print('\t');
}

static void send(uint8_t b) {
  Serial1.write(b);
  printStamp();
  Serial.print(F("-> "));
  printHex(b);
  Serial.println();
}

static void sendInit() {
  send(lk::kRequestId);
  send(lk::kSetDefaults);
  send(lk::kEnableLk401);
  for (uint8_t d = 1; d <= lk::kDivisions; d++) {
    send(lk::setMode(d, lk::kModeUpDown));
  }
  send(lk::kDisableKeyclick);
  send(lk::kDisableCtrlClick);
  send(lk::kLedOff);
  send(lk::param(lk::kLedAll));
}

static void printHelp() {
  Serial.println(F("elkay console: hex bytes | init | reset | id | bell | "
                   "click | noclick | help"));
}

// Sends each hex byte in the line; returns false if any token is not hex.
static bool sendHex(char *s) {
  uint8_t bytes[sizeof(line) / 2];
  uint8_t n = 0;
  for (char *tok = strtok(s, " ,"); tok; tok = strtok(nullptr, " ,")) {
    char *end;
    unsigned long v = strtoul(tok, &end, 16);
    if (*end != '\0' || v > 0xFF || n == sizeof(bytes)) return false;
    bytes[n++] = v;
  }
  for (uint8_t i = 0; i < n; i++) send(bytes[i]);
  return true;
}

static void runCommand(char *cmd) {
  if (!strcmp(cmd, "")) return;
  if (!strcmp(cmd, "help")) {
    printHelp();
  } else if (!strcmp(cmd, "init")) {
    sendInit();
  } else if (!strcmp(cmd, "reset")) {
    send(lk::kPowerUpReset);
  } else if (!strcmp(cmd, "id")) {
    send(lk::kRequestId);
  } else if (!strcmp(cmd, "bell")) {
    send(lk::kSoundBell);
  } else if (!strcmp(cmd, "click")) {
    send(lk::kEnableKeyclick);
    send(lk::param(2));
  } else if (!strcmp(cmd, "noclick")) {
    send(lk::kDisableKeyclick);
  } else if (!sendHex(cmd)) {
    Serial.print(F("? "));
    Serial.println(cmd);
  }
}

static void readConsole() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\r' || c == '\n') {
      line[lineLen] = '\0';
      lineLen = 0;
      runCommand(line);
    } else if (lineLen < sizeof(line) - 1) {
      line[lineLen++] = tolower(c);
    }
  }
}

static void readKeyboard() {
  while (Serial1.available()) {
    uint8_t b = Serial1.read();
    digitalWrite(kActivityLed, HIGH);
    activityUntil = millis() + kActivityMs;

    printStamp();
    Serial.print(F("<- "));
    printHex(b);
    const char *name = lk::responseName(b);
    if (name) {
      Serial.print(' ');
      Serial.print(name);
    }
    Serial.println();
  }
}

void setup() {
  pinMode(kActivityLed, OUTPUT);
  Serial.begin(kConsoleBaud);
  Serial1.begin(lk::kBaud);
  // Hold RX at idle when nothing drives it, so an unconnected input does not
  // read noise as bytes. The UART still owns the pin; this only sets the pull-up.
  pinMode(0, INPUT_PULLUP);
  printHelp();
}

void loop() {
  readKeyboard();
  readConsole();
  if (activityUntil && (long)(millis() - activityUntil) >= 0) {
    digitalWrite(kActivityLed, LOW);
    activityUntil = 0;
  }
}
