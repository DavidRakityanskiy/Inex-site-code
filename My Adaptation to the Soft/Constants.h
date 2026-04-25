#ifndef CONSTANTS_H
#define CONSTANTS_H

const char* prescripts[] = {
  "SYSTEM ONLINE",
  "YOU MUST MOVE",
  "DO NOT STOP",
  "WAIT FOR SIGNAL",
  "THE SYSTEM KNOWS",
  "FOLLOW THE LIGHT",
  "STAY ALERT",
  "LISTEN CAREFULLY",
  "ACCESS GRANTED",
  "REBOOTING..."
};

const int totalTexts = sizeof(prescripts) / sizeof(prescripts[0]);
const char* scrambleChars = "0123456789!#%&*ABCDEF";

#endif