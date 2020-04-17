
  #include <pgmspace.h>

const uint8_t arduino_logo[] PROGMEM={
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xF8, 0xFC, 0xFC,   // 0x0010 (16) pixels
0xFE, 0xFE, 0x7F, 0x7F, 0x7F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x7F, 0x7F, 0x7F, 0xFE, 0xFE, 0xFE,   // 0x0020 (32) pixels
0xFC, 0xFC, 0xF8, 0xF0, 0xF0, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0,   // 0x0030 (48) pixels
0xF0, 0xF0, 0xF8, 0xFC, 0xFC, 0xFE, 0xFE, 0xFE, 0x7F, 0x7F, 0x7F, 0x3F, 0x3F, 0x3F, 0x3F, 0x7F,   // 0x0040 (64) pixels
0x7F, 0x7F, 0x7F, 0xFE, 0xFE, 0xFC, 0xFC, 0xF8, 0xF8, 0xF0, 0xE0, 0xC0, 0x86, 0x06, 0x06, 0x00,   // 0x0050 (80) pixels
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x0060 (96) pixels
0x1F, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,   // 0x0070 (112) pixels
0x80, 0x80, 0x00, 0x00, 0x01, 0x01, 0x03, 0x0F, 0x1F, 0x3F, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xFE,   // 0x0080 (128) pixels
0xFF, 0xFF, 0xFF, 0x3F, 0x1F, 0x0F, 0x03, 0x01, 0x01, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0xF8,   // 0x0090 (144) pixels
0xF8, 0xF8, 0xF8, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x1F, 0xFF, 0xFF, 0xFF,   // 0x00A0 (160) pixels
0xFF, 0xFF, 0xFC, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x7F, 0xFF,   // 0x00B0 (176) pixels
0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x07, 0x07,   // 0x00C0 (192) pixels
0x07, 0x07, 0x07, 0x07, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xF0, 0xF8, 0xFE, 0xFF,   // 0x00D0 (208) pixels
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xF8, 0xF0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x07,   // 0x00E0 (224) pixels
0x07, 0x07, 0x07, 0x3F, 0x3F, 0x3F, 0x3F, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0,   // 0x00F0 (240) pixels
0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // 0x0100 (256) pixels
0x00, 0x00, 0x00, 0x00, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0x7F, 0xFF, 0xFE, 0xFE, 0xFC, 0xFC,   // 0x0110 (272) pixels
0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xFC, 0xFC, 0xFC, 0xFE, 0xFF, 0x7F, 0x7F, 0x3F, 0x3F,   // 0x0120 (288) pixels
0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x3F, 0x7F,   // 0x0130 (304) pixels
0x7F, 0xFF, 0xFE, 0xFC, 0xFC, 0xFC, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xFC, 0xFC, 0xFE,   // 0x0140 (320) pixels
0xFE, 0xFF, 0x7F, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // 0x0150 (336) pixels
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0x80, 0x00, 0x00,   // 0x0160 (352) pixels
0x00, 0x00, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0x81, 0x81, 0x01, 0x01, 0xC1, 0xC1, 0xC0, 0xC0,   // 0x0170 (368) pixels
0xC0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0x80, 0x00,   // 0x0180 (384) pixels
0x00, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC1, 0xC1, 0x01, 0x01, 0xC1, 0xC1, 0xC1, 0x01, 0x01,   // 0x0190 (400) pixels
0x01, 0xC1, 0xC1, 0x01, 0x00, 0x00, 0x80, 0xC0, 0xC0, 0xC0, 0xC0, 0x80, 0x80, 0x00, 0x00, 0x00,   // 0x01A0 (416) pixels
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xF8, 0xFF, 0x3F, 0x3B,   // 0x01B0 (432) pixels
0x3F, 0x7F, 0xFE, 0xF0, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x1C, 0x3C, 0xFF, 0xFF, 0xC7, 0x00, 0x00,   // 0x01C0 (448) pixels
0xFF, 0xFF, 0xFF, 0xE0, 0xE0, 0xF1, 0x7F, 0x3F, 0x1F, 0x00, 0x3F, 0x7F, 0xFF, 0xE0, 0xC0, 0xE0,   // 0x01D0 (464) pixels
0xFF, 0x7F, 0x3F, 0x00, 0x00, 0xE0, 0xE0, 0xFF, 0xFF, 0xFF, 0xE0, 0xE0, 0xC0, 0x00, 0x00, 0xFF,   // 0x01E0 (480) pixels
0xFF, 0x07, 0x0F, 0x3E, 0xFC, 0xFF, 0xFF, 0x00, 0x00, 0x3F, 0x7F, 0xFF, 0xE0, 0xC0, 0xE1, 0x7F,   // 0x01F0 (496) pixels
0x7F, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

const uint8_t The_End[] PROGMEM={
0x00, 0x80, 0x80, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xE0, 0x60, 0x00, 0x00,   // 0x0010 (16) pixels
0x80, 0xC0, 0xC0, 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x80, 0xC0, 0xC0, 0xC0, 0xC0, 0xE0,   // 0x0020 (32) pixels
0xC0, 0xC0, 0xC0, 0xC0, 0xE0, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0xC0, 0xE0,   // 0x0030 (48) pixels
0xE0, 0xC0, 0xC0, 0xC0, 0xE0, 0xE0, 0x00, 0x00, 0x00, 0x80, 0x80, 0xC0, 0xC0, 0xC0, 0x00, 0x00,   // 0x0040 (64) pixels
0x00, 0x00, 0x80, 0xE0, 0xF0, 0xF0, 0x60, 0x40, 0xF0, 0xF0, 0xF0, 0xF0, 0xE0, 0xE0, 0xC0, 0xC0,   // 0x0050 (80) pixels
0x80, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x03, 0x81, 0xFC, 0xFF, 0x0F, 0x03, 0x00, 0x00, 0x00,   // 0x0060 (96) pixels
0xEE, 0x6F, 0x67, 0xFF, 0xFF, 0x7F, 0x71, 0x30, 0xF0, 0xFF, 0x3F, 0x39, 0x38, 0x18, 0x00, 0x01,   // 0x0070 (112) pixels
0x00, 0xF8, 0xFF, 0x1F, 0x0F, 0x0C, 0x0D, 0x8D, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,   // 0x0080 (128) pixels
0x00, 0xC0, 0xFF, 0x7F, 0x0F, 0x0C, 0x0D, 0x0D, 0x84, 0x80, 0x80, 0x07, 0x07, 0x83, 0xFF, 0xFF,   // 0x0090 (144) pixels
0x1F, 0x3F, 0xFE, 0xF8, 0xF8, 0xFE, 0xDF, 0x03, 0x00, 0x00, 0x00, 0x03, 0xFF, 0xFF, 0x1F, 0x00,   // 0x00A0 (160) pixels
0x00, 0x80, 0x81, 0xC3, 0xE7, 0x7F, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x06, 0x07, 0x07, 0x03, 0x00,   // 0x00B0 (176) pixels
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x0F, 0x07, 0x03, 0x00, 0x00, 0x0F, 0x0F, 0x07, 0x00,   // 0x00C0 (192) pixels
0x00, 0x06, 0x06, 0x3F, 0x1F, 0x0F, 0x0F, 0x0E, 0x06, 0x06, 0x06, 0x07, 0x07, 0x03, 0x00, 0x00,   // 0x00D0 (208) pixels
0x00, 0x00, 0x06, 0x1F, 0x3F, 0x1F, 0x0F, 0x0E, 0x06, 0x06, 0x06, 0x07, 0x07, 0x07, 0x03, 0x00,   // 0x00E0 (224) pixels
0x06, 0x0F, 0x07, 0x01, 0x00, 0x00, 0x00, 0x01, 0x0F, 0x07, 0x03, 0x00, 0x00, 0x18, 0x1C, 0x1F,   // 0x00F0 (240) pixels
0x0F, 0x07, 0x06, 0x07, 0x03, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 
};

const uint8_t pacman1[] PROGMEM={
0x80, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7E, 0x3E, 0x1C,   // 0x0010 (16) pixels
0x0C, 0x00, 0x00, 0x00, 0x1F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF9,   // 0x0020 (32) pixels
0xF0, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x07, 0x0F,   // 0x0030 (48) pixels
0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x07, 0x07, 0x03, 0x03, 0x00, 0x00, 0x00, 
};

const uint8_t pacman2[] PROGMEM={
0x80, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0x7C,   // 0x0010 (16) pixels
0x7C, 0x38, 0x20, 0x00, 0x1F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF9,   // 0x0020 (32) pixels
0xF9, 0xF0, 0xF0, 0xE0, 0xE0, 0xC0, 0x40, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x07, 0x0F,   // 0x0030 (48) pixels
0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x07, 0x07, 0x03, 0x03, 0x01, 0x00, 0x00, 
};

const uint8_t pacman3[] PROGMEM={
0x80, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFC,   // 0x0010 (16) pixels
0xF8, 0xF0, 0xE0, 0x80, 0x1F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x0020 (32) pixels
0xFF, 0xFF, 0xFF, 0xFF, 0xFB, 0xF9, 0x79, 0x19, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x07, 0x0F,   // 0x0030 (48) pixels
0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x07, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 
};

const uint8_t pill[] PROGMEM={
0x0E, 0x1F, 0x1F, 0x1F, 0x0E, 
};

