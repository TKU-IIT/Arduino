#include <SPI.h>
#include <Keypad.h>
#include <LCD5110_Graph.h>
#include <EEPROM.h>

#include <avr/sleep.h>

#define uchar unsigned char
#define uint unsigned int

//data array maxium length
#define MAX_LEN 16


/////////////////////////////////////////////////////////////////////
//set the pin
/////////////////////////////////////////////////////////////////////
const int chipSelectPin = 53;
//mega2560 use 53 port , uno use 10 port.

const int NRSTPD = 5;
int i = 0;
LCD5110 myGLCD(12, 11, 10, 9, 8);
//Number Pad Proclaiming
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {17, 16, 15, 14}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {20, 19, 18}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
//Number Pad Proclaiming
//MF522 command bits
#define PCD_IDLE 0x00 //NO action; cancel current commands
#define PCD_AUTHENT 0x0E //verify password key
#define PCD_RECEIVE 0x08 //receive data
#define PCD_TRANSMIT 0x04 //send data
#define PCD_TRANSCEIVE 0x0C //send and receive data
#define PCD_RESETPHASE 0x0F //reset
#define PCD_CALCCRC 0x03 //CRC check and caculation

//Mifare_One card command bits
#define PICC_REQIDL 0x26 //Search the cards that not into sleep mode in the antenna area
#define PICC_REQALL 0x52 //Search all the cards in the antenna area
#define PICC_ANTICOLL 0x93 //prevent conflict
#define PICC_SElECTTAG 0x93 //select card
#define PICC_AUTHENT1A 0x60 //verify A password key
#define PICC_AUTHENT1B 0x61 //verify B password key
#define PICC_READ 0x30 //read
#define PICC_WRITE 0xA0 //write
#define PICC_DECREMENT 0xC0 //deduct value
#define PICC_INCREMENT 0xC1 //charge up value
#define PICC_RESTORE 0xC2 //Restore data into buffer
#define PICC_TRANSFER 0xB0 //Save data into buffer
#define PICC_HALT 0x50 //sleep mode

//THe mistake code that return when communicate with MF522
#define MI_OK 0
#define MI_NOTAGERR 1
#define MI_ERR 2

//------------------MFRC522 register ---------------
//Page 0:Command and Status
#define Reserved00 0x00
#define CommandReg 0x01
#define CommIEnReg 0x02
#define DivlEnReg 0x03
#define CommIrqReg 0x04
#define DivIrqReg 0x05
#define ErrorReg 0x06
#define Status1Reg 0x07
#define Status2Reg 0x08
#define FIFODataReg 0x09
#define FIFOLevelReg 0x0A
#define WaterLevelReg 0x0B
#define ControlReg 0x0C
#define BitFramingReg 0x0D
#define CollReg 0x0E
#define Reserved01 0x0F
//Page 1:Command
#define Reserved10 0x10
#define ModeReg 0x11
#define TxModeReg 0x12
#define RxModeReg 0x13
#define TxControlReg 0x14
#define TxAutoReg 0x15
#define TxSelReg 0x16
#define RxSelReg 0x17
#define RxThresholdReg 0x18
#define DemodReg 0x19
#define Reserved11 0x1A
#define Reserved12 0x1B
#define MifareReg 0x1C
#define Reserved13 0x1D
#define Reserved14 0x1E
#define SerialSpeedReg 0x1F
//Page 2:CFG
#define Reserved20 0x20
#define CRCResultRegM 0x21
#define CRCResultRegL 0x22
#define Reserved21 0x23
#define ModWidthReg 0x24
#define Reserved22 0x25
#define RFCfgReg 0x26
#define GsNReg 0x27
#define CWGsPReg 0x28
#define ModGsPReg 0x29
#define TModeReg 0x2A
#define TPrescalerReg 0x2B
#define TReloadRegH 0x2C
#define TReloadRegL 0x2D
#define TCounterValueRegH 0x2E
#define TCounterValueRegL 0x2F
//Page 3:TestRegister
#define Reserved30 0x30
#define TestSel1Reg 0x31
#define TestSel2Reg 0x32
#define TestPinEnReg 0x33
#define TestPinValueReg 0x34
#define TestBusReg 0x35
#define AutoTestReg 0x36
#define VersionReg 0x37
#define AnalogTestReg 0x38
#define TestDAC1Reg 0x39
#define TestDAC2Reg 0x3A
#define TestADCReg 0x3B
#define Reserved31 0x3C
#define Reserved32 0x3D
#define Reserved33 0x3E
#define Reserved34 0x3F
//-----------------------------------------------
#define PIN_SCE   8
#define PIN_RESET 9
#define PIN_DC    10
#define PIN_SDIN  11
#define PIN_SCLK  12

#define LCD_C     LOW
#define LCD_D     HIGH

#define LCD_X     84
#define LCD_Y     48

extern uint8_t SmallFont[];
extern uint8_t arduino_logo[];
extern unsigned char TinyFont[];
extern uint8_t The_End[];
extern uint8_t pacman1[];
extern uint8_t pacman2[];
extern uint8_t pacman3[];
extern uint8_t pill[];

float y;
uint8_t* bm;
int pacy;

static const byte ASCII[][5] =
{
  {0x00, 0x00, 0x00, 0x00, 0x00} // 20
  , {0x00, 0x00, 0x5f, 0x00, 0x00} // 21 !
  , {0x00, 0x07, 0x00, 0x07, 0x00} // 22 "
  , {0x14, 0x7f, 0x14, 0x7f, 0x14} // 23 #
  , {0x24, 0x2a, 0x7f, 0x2a, 0x12} // 24 $
  , {0x23, 0x13, 0x08, 0x64, 0x62} // 25 %
  , {0x36, 0x49, 0x55, 0x22, 0x50} // 26 &
  , {0x00, 0x05, 0x03, 0x00, 0x00} // 27 '
  , {0x00, 0x1c, 0x22, 0x41, 0x00} // 28 (
  , {0x00, 0x41, 0x22, 0x1c, 0x00} // 29 )
  , {0x14, 0x08, 0x3e, 0x08, 0x14} // 2a *
  , {0x08, 0x08, 0x3e, 0x08, 0x08} // 2b +
  , {0x00, 0x50, 0x30, 0x00, 0x00} // 2c ,
  , {0x08, 0x08, 0x08, 0x08, 0x08} // 2d -
  , {0x00, 0x60, 0x60, 0x00, 0x00} // 2e .
  , {0x20, 0x10, 0x08, 0x04, 0x02} // 2f /
  , {0x3e, 0x51, 0x49, 0x45, 0x3e} // 30 0
  , {0x00, 0x42, 0x7f, 0x40, 0x00} // 31 1
  , {0x42, 0x61, 0x51, 0x49, 0x46} // 32 2
  , {0x21, 0x41, 0x45, 0x4b, 0x31} // 33 3
  , {0x18, 0x14, 0x12, 0x7f, 0x10} // 34 4
  , {0x27, 0x45, 0x45, 0x45, 0x39} // 35 5
  , {0x3c, 0x4a, 0x49, 0x49, 0x30} // 36 6
  , {0x01, 0x71, 0x09, 0x05, 0x03} // 37 7
  , {0x36, 0x49, 0x49, 0x49, 0x36} // 38 8
  , {0x06, 0x49, 0x49, 0x29, 0x1e} // 39 9
  , {0x00, 0x36, 0x36, 0x00, 0x00} // 3a :
  , {0x00, 0x56, 0x36, 0x00, 0x00} // 3b ;
  , {0x08, 0x14, 0x22, 0x41, 0x00} // 3c <
  , {0x14, 0x14, 0x14, 0x14, 0x14} // 3d =
  , {0x00, 0x41, 0x22, 0x14, 0x08} // 3e >
  , {0x02, 0x01, 0x51, 0x09, 0x06} // 3f ?
  , {0x32, 0x49, 0x79, 0x41, 0x3e} // 40 @
  , {0x7e, 0x11, 0x11, 0x11, 0x7e} // 41 A
  , {0x7f, 0x49, 0x49, 0x49, 0x36} // 42 B
  , {0x3e, 0x41, 0x41, 0x41, 0x22} // 43 C
  , {0x7f, 0x41, 0x41, 0x22, 0x1c} // 44 D
  , {0x7f, 0x49, 0x49, 0x49, 0x41} // 45 E
  , {0x7f, 0x09, 0x09, 0x09, 0x01} // 46 F
  , {0x3e, 0x41, 0x49, 0x49, 0x7a} // 47 G
  , {0x7f, 0x08, 0x08, 0x08, 0x7f} // 48 H
  , {0x00, 0x41, 0x7f, 0x41, 0x00} // 49 I
  , {0x20, 0x40, 0x41, 0x3f, 0x01} // 4a J
  , {0x7f, 0x08, 0x14, 0x22, 0x41} // 4b K
  , {0x7f, 0x40, 0x40, 0x40, 0x40} // 4c L
  , {0x7f, 0x02, 0x0c, 0x02, 0x7f} // 4d M
  , {0x7f, 0x04, 0x08, 0x10, 0x7f} // 4e N
  , {0x3e, 0x41, 0x41, 0x41, 0x3e} // 4f O
  , {0x7f, 0x09, 0x09, 0x09, 0x06} // 50 P
  , {0x3e, 0x41, 0x51, 0x21, 0x5e} // 51 Q
  , {0x7f, 0x09, 0x19, 0x29, 0x46} // 52 R
  , {0x46, 0x49, 0x49, 0x49, 0x31} // 53 S
  , {0x01, 0x01, 0x7f, 0x01, 0x01} // 54 T
  , {0x3f, 0x40, 0x40, 0x40, 0x3f} // 55 U
  , {0x1f, 0x20, 0x40, 0x20, 0x1f} // 56 V
  , {0x3f, 0x40, 0x38, 0x40, 0x3f} // 57 W
  , {0x63, 0x14, 0x08, 0x14, 0x63} // 58 X
  , {0x07, 0x08, 0x70, 0x08, 0x07} // 59 Y
  , {0x61, 0x51, 0x49, 0x45, 0x43} // 5a Z
  , {0x00, 0x7f, 0x41, 0x41, 0x00} // 5b [
  , {0x02, 0x04, 0x08, 0x10, 0x20} // 5c ¥
  , {0x00, 0x41, 0x41, 0x7f, 0x00} // 5d ]
  , {0x04, 0x02, 0x01, 0x02, 0x04} // 5e ^
  , {0x40, 0x40, 0x40, 0x40, 0x40} // 5f _
  , {0x00, 0x01, 0x02, 0x04, 0x00} // 60 `
  , {0x20, 0x54, 0x54, 0x54, 0x78} // 61 a
  , {0x7f, 0x48, 0x44, 0x44, 0x38} // 62 b
  , {0x38, 0x44, 0x44, 0x44, 0x20} // 63 c
  , {0x38, 0x44, 0x44, 0x48, 0x7f} // 64 d
  , {0x38, 0x54, 0x54, 0x54, 0x18} // 65 e
  , {0x08, 0x7e, 0x09, 0x01, 0x02} // 66 f
  , {0x0c, 0x52, 0x52, 0x52, 0x3e} // 67 g
  , {0x7f, 0x08, 0x04, 0x04, 0x78} // 68 h
  , {0x00, 0x44, 0x7d, 0x40, 0x00} // 69 i
  , {0x20, 0x40, 0x44, 0x3d, 0x00} // 6a j
  , {0x7f, 0x10, 0x28, 0x44, 0x00} // 6b k
  , {0x00, 0x41, 0x7f, 0x40, 0x00} // 6c l
  , {0x7c, 0x04, 0x18, 0x04, 0x78} // 6d m
  , {0x7c, 0x08, 0x04, 0x04, 0x78} // 6e n
  , {0x38, 0x44, 0x44, 0x44, 0x38} // 6f o
  , {0x7c, 0x14, 0x14, 0x14, 0x08} // 70 p
  , {0x08, 0x14, 0x14, 0x18, 0x7c} // 71 q
  , {0x7c, 0x08, 0x04, 0x04, 0x08} // 72 r
  , {0x48, 0x54, 0x54, 0x54, 0x20} // 73 s
  , {0x04, 0x3f, 0x44, 0x40, 0x20} // 74 t
  , {0x3c, 0x40, 0x40, 0x20, 0x7c} // 75 u
  , {0x1c, 0x20, 0x40, 0x20, 0x1c} // 76 v
  , {0x3c, 0x40, 0x30, 0x40, 0x3c} // 77 w
  , {0x44, 0x28, 0x10, 0x28, 0x44} // 78 x
  , {0x0c, 0x50, 0x50, 0x50, 0x3c} // 79 y
  , {0x44, 0x64, 0x54, 0x4c, 0x44} // 7a z
  , {0x00, 0x08, 0x36, 0x41, 0x00} // 7b {
  , {0x00, 0x00, 0x7f, 0x00, 0x00} // 7c |
  , {0x00, 0x41, 0x36, 0x08, 0x00} // 7d }
  , {0x10, 0x08, 0x08, 0x10, 0x08} // 7e ←
  , {0x78, 0x46, 0x41, 0x46, 0x78} // 7f →
};


//int chpD58EB308a;//重複卡號加a
//int chpA4F88950a;
//int chpF5FEA804a;
//int chp49AFF7E6a;
//int chp460F232Ba;
//int chpD51BC704a;
//int chp244CFF69a;
//int chpA3DF3833a;
//int chpB4B1EE69a;
//int chp74BB3251a;
//int chp9E39232Ba;
//int chpD4563D51a;
//int chp84523751a;
//int chpD029FF01a;
//int chpB29C63A9a;
//int chpA5DBAF08a;
//int chp2572BB04a;
//


//4 bytes Serial number of card, the 5 bytes is verfiy bytes
uchar serNum[5];

void setup()
{
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
  randomSeed(analogRead(7));
  Serial.begin(9600);

  SPI.begin();

  pinMode(chipSelectPin, OUTPUT); // Set digital pin 10 as OUTPUT to connect it to the RFID /ENABLE pin
  digitalWrite(chipSelectPin, LOW); // Activate the RFID reader
  pinMode(NRSTPD, OUTPUT); // Set digital pin 5 , Not Reset and Power-down

 MFRC522_Init();
}

void loop()
{

  uchar status;
  uchar str[MAX_LEN];
status = MFRC522_Anticoll(str);

  if (status != MI_OK)
  {
    myGLCD.clrScr();
    myGLCD.print("Please Sensor", LEFT, 0);
    myGLCD.print("the card first!", LEFT, 10);
    myGLCD.update();
return;
  }





  // str[0..3]: serial number of the card
  // str[4]: XOR checksum of the SN.
  if (status == MI_OK)
  {


memcpy(serNum, str, 5);uchar* id = serNum;
        ShowCardID(serNum);

    /////////////////
    int chpD58EB308 = 0;//1
    int chp2572BB04 = 0;//2
    int chp84523751 = 0;//3
    int chpD4563D51 = 0;//4
    int chp9E39232B = 0;//5
    int chp74BB3251 = 0;//6
    int chpB4B1EE69 = 0;//7
    int chpA3DF3833 = 0;//8
    int chp244CFF67 = 0;//9
    int chpD51BC704 = 0;//A
    int chpD4893751 = 0;//B
    int chp49AFF7E6 = 0;//C
    int chpF5FEA804 = 0;//D
    int chpA4F88950 = 0;//E
    int chpD029FF01 = 0;//F
    int chpB29C63A9 = 0;//10
    int chpA5DBAF08 = 0;//11
    int chp460F232B = 0;//12
//    ////////////////
//     chpD58EB308 = EEPROM.read(1);//1
//     chp2572BB04 = EEPROM.read(2);//2
//     chp84523751 = EEPROM.read(3);//3
//     chpD4563D51 = EEPROM.read(4);//4
//     chp9E39232B = EEPROM.read(5);//5
//     chp74BB3251 = EEPROM.read(6);//6
//     chpB4B1EE69 = EEPROM.read(7);//7
//     chpA3DF3833 = EEPROM.read(8);//8
//     chp244CFF67 = EEPROM.read(9);//9
//     chpD51BC704 = EEPROM.read(10);//A
//     chpD4893751 = EEPROM.read(11);//B
//     chp49AFF7E6 = EEPROM.read(12);//C
//     chpF5FEA804 = EEPROM.read(13);//D
//     chpA4F88950 = EEPROM.read(14);//E
//    chpD029FF01 = EEPROM.read(15);//F
//    chpB29C63A9 = EEPROM.read(16);//10
//    chpA5DBAF08 = EEPROM.read(17);//11
//    chp460F232B = EEPROM.read(18);//12

    
    if ( id[0] == 0xD5 && id[1] == 0x8E && id[2] == 0xB3 && id[3] == 0x08 ) {
      char key = keypad.getKey();
      myGLCD.print("(*)for charging", LEFT, 40);
      myGLCD.clrScr();
      myGLCD.print("The card's ", LEFT, 0);
      myGLCD.print(" balance:", LEFT, 10);
      myGLCD.printNumI(chpD58EB308, LEFT, 20);
      myGLCD.print("(*)for charging", LEFT, 40);
      myGLCD.update();
      if(key='*'){/////////////////////////////////////////////////////////////////////////
      int value1=-1;
      int value2=-10;
      int value3=-100;
      int value4=-1000;
      int value5=-5;
      int value6=-50;
      int value7=-500;  
      int value8=-5000;
        if (key == '1') {chpD58EB308=chpD58EB308+value1;}//1
        if (key == '2') {chpD58EB308=chpD58EB308+value2;}
        if (key == '3') {chpD58EB308=chpD58EB308+value3;}
        if (key == '4') {chpD58EB308=chpD58EB308+value4;}
        if (key == '5') {chpD58EB308=chpD58EB308+value5;}
        if (key == '6') {chpD58EB308=chpD58EB308+value6;}
        if (key == '7') {chpD58EB308=chpD58EB308+value7;}
        if (key == '8') {chpD58EB308=chpD58EB308+value8;}
        if (key == '9') {value1=value1*(-1);value2=value2*(-1);value3=value3*(-1);value4=value4*(-1);value5=value5*(-1);value6=value6*(-1);value7=value7*(-1);value8=value8*(-1);}
        if (key == '0') {}
      //斜線匡起來的區域拷貝在任何有else if陳敘式的大刮弧裡面
    }/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
    else if ( id[0] == 0x25 && id[1] == 0x72 && id[2] == 0xBB && id[3] == 0x04 ) {
      myGLCD.clrScr();
      myGLCD.print("The card's ", LEFT, 0);
      myGLCD.print(" balance:", LEFT, 10);
      myGLCD.printNumI(chp2572BB04, LEFT, 20);
      myGLCD.print("(*)for charging", LEFT, 40);
      myGLCD.update();char key = keypad.getKey();
       if(key='*'){/////////////////////////////////////////////////////////////////////////
      int value1=-1;
      int value2=-10;
      int value3=-100;
      int value4=-1000;
      int value5=-5;
      int value6=-50;
      int value7=-500;  
      int value8=-5000;
        if (key == '1') {chp2572BB04=chp2572BB04+value1;}//2
        if (key == '2') {chp2572BB04=chp2572BB04+value2;}
        if (key == '3') {chp2572BB04=chp2572BB04+value3;}
        if (key == '4') {chp2572BB04=chp2572BB04+value4;}
        if (key == '5') {chp2572BB04=chp2572BB04+value5;}
        if (key == '6') {chp2572BB04=chp2572BB04+value6;}
        if (key == '7') {chp2572BB04=chp2572BB04+value7;}
        if (key == '8') {chp2572BB04=chp2572BB04+value8;}
        if (key == '9') {value1=value1*(-1);value2=value2*(-1);value3=value3*(-1);value4=value4*(-1);value5=value5*(-1);value6=value6*(-1);value7=value7*(-1);value8=value8*(-1);}//改卡號
        if (key == '0') {}
      //斜線匡起來的區域拷貝在任何有else if陳敘式的大刮弧裡面
    }/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
    else if ( id[0] == 0x84 && id[1] == 0x52 && id[2] == 0x37 && id[3] == 0x51 ) {
      myGLCD.clrScr();
      myGLCD.print("The card's ", LEFT, 0);
      myGLCD.print(" balance:", LEFT, 10);
      myGLCD.printNumI(chp84523751, LEFT, 20);
      myGLCD.print("(*)for charging", LEFT, 40);
      myGLCD.update();char key = keypad.getKey();
       if(key='*'){/////////////////////////////////////////////////////////////////////////
      int value1=-1;
      int value2=-10;
      int value3=-100;
      int value4=-1000;
      int value5=-5;
      int value6=-50;
      int value7=-500;  
      int value8=-5000;
        if (key == '1') {chp84523751=chp84523751+value1;}//3
        if (key == '2') {chp84523751=chp84523751+value2;}
        if (key == '3') {chp84523751=chp84523751+value3;}
        if (key == '4') {chp84523751=chp84523751+value4;}
        if (key == '5') {chp84523751=chp84523751+value5;}
        if (key == '6') {chp84523751=chp84523751+value6;}
        if (key == '7') {chp84523751=chp84523751+value7;}
        if (key == '8') {chp84523751=chp84523751+value8;}
        if (key == '9') {value1=value1*(-1);value2=value2*(-1);value3=value3*(-1);value4=value4*(-1);value5=value5*(-1);value6=value6*(-1);value7=value7*(-1);value8=value8*(-1);}//改卡號
        if (key == '0') {}
      //斜線匡起來的區域拷貝在任何有else if陳敘式的大刮弧裡面
    }/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
    else if ( id[0] == 0xD4 && id[1] == 0x56 && id[2] == 0x3D && id[3] == 0x51 ) {
      myGLCD.clrScr();
      myGLCD.print("The card's ", LEFT, 0);
      myGLCD.print(" balance:", LEFT, 10);
      myGLCD.printNumI(chp9E39232B, LEFT, 20);
      myGLCD.print("(*)for charging", LEFT, 40);
      myGLCD.update();char key = keypad.getKey();
       if(key='*'){/////////////////////////////////////////////////////////////////////////
      int value1=-1;
      int value2=-10;
      int value3=-100;
      int value4=-1000;
      int value5=-5;
      int value6=-50;
      int value7=-500;  
      int value8=-5000;
        if (key == '1') {chpD4563D51=chpD4563D51+value1;}//4
        if (key == '2') {chpD4563D51=chpD4563D51+value2;}
        if (key == '3') {chpD4563D51=chpD4563D51+value3;}
        if (key == '4') {chpD4563D51=chpD4563D51+value4;}
        if (key == '5') {chpD4563D51=chpD4563D51+value5;}
        if (key == '6') {chpD4563D51=chpD4563D51+value6;}
        if (key == '7') {chpD4563D51=chpD4563D51+value7;}
        if (key == '8') {chpD4563D51=chpD4563D51+value8;}
        if (key == '9') {value1=value1*(-1);value2=value2*(-1);value3=value3*(-1);value4=value4*(-1);value5=value5*(-1);value6=value6*(-1);value7=value7*(-1);value8=value8*(-1);}
        if (key == '0') {}
      //斜線匡起來的區域拷貝在任何有else if陳敘式的大刮弧裡面
    }/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
    else if ( id[0] == 0x9E && id[1] == 0x39 && id[2] == 0x23 && id[3] == 0x2B ) {
      myGLCD.clrScr();
      myGLCD.print("The card's ", LEFT, 0);
      myGLCD.print(" balance:", LEFT, 10);
      myGLCD.printNumI(chp74BB3251, LEFT, 20);
      myGLCD.print("(*)for charging", LEFT, 40);
      myGLCD.update();char key = keypad.getKey();
       if(key='*'){/////////////////////////////////////////////////////////////////////////
      int value1=-1;
      int value2=-10;
      int value3=-100;
      int value4=-1000;
      int value5=-5;
      int value6=-50;
      int value7=-500;  
      int value8=-5000;
        if (key == '1') {chp9E39232B=chp9E39232B+value1;}//5
        if (key == '2') {chp9E39232B=chp9E39232B+value2;}
        if (key == '3') {chp9E39232B=chp9E39232B+value3;}
        if (key == '4') {chp9E39232B=chp9E39232B+value4;}
        if (key == '5') {chp9E39232B=chp9E39232B+value5;}
        if (key == '6') {chp9E39232B=chp9E39232B+value6;}
        if (key == '7') {chp9E39232B=chp9E39232B+value7;}
        if (key == '8') {chp9E39232B=chp9E39232B+value8;}
        if (key == '9') {value1=value1*(-1);value2=value2*(-1);value3=value3*(-1);value4=value4*(-1);value5=value5*(-1);value6=value6*(-1);value7=value7*(-1);value8=value8*(-1);}//改卡號
        if (key == '0') {}//改卡號
      //斜線匡起來的區域拷貝在任何有else if陳敘式的大刮弧裡面
    }/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
    else if ( id[0] == 0x74 && id[1] == 0xBB && id[2] == 0x32 && id[3] == 0x51 ) {
      myGLCD.clrScr();
      myGLCD.print("The card's ", LEFT, 0);
      myGLCD.print(" balance:", LEFT, 10);
      myGLCD.printNumI(chpB4B1EE69, LEFT, 20);
      myGLCD.print("(*)for charging", LEFT, 40);
      myGLCD.update();char key = keypad.getKey();
       if(key='*'){/////////////////////////////////////////////////////////////////////////
      int value1=-1;
      int value2=-10;
      int value3=-100;
      int value4=-1000;
      int value5=-5;
      int value6=-50;
      int value7=-500;  
      int value8=-5000;
        if (key == '1') {chp74BB3251=chp74BB3251+value1;}//6
        if (key == '2') {chp74BB3251=chp74BB3251+value2;}
        if (key == '3') {chp74BB3251=chp74BB3251+value3;}
        if (key == '4') {chp74BB3251=chp74BB3251+value4;}
        if (key == '5') {chp74BB3251=chp74BB3251+value5;}
        if (key == '6') {chp74BB3251=chp74BB3251+value6;}
        if (key == '7') {chp74BB3251=chp74BB3251+value7;}
        if (key == '8') {chp74BB3251=chp74BB3251+value8;}
        if (key == '9') {value1=value1*(-1);value2=value2*(-1);value3=value3*(-1);value4=value4*(-1);value5=value5*(-1);value6=value6*(-1);value7=value7*(-1);value8=value8*(-1);}//改卡號
        if (key == '0') {}
      //斜線匡起來的區域拷貝在任何有else if陳敘式的大刮弧裡面
    }/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
    else if ( id[0] == 0xB4 && id[1] == 0xB1 && id[2] == 0xEE && id[3] == 0x69 ) {
      myGLCD.clrScr();
      myGLCD.print("The card's ", LEFT, 0);
      myGLCD.print(" balance:", LEFT, 10);
      myGLCD.printNumI(chpD58EB308, LEFT, 20);
      myGLCD.print("(*)for charging", LEFT, 40);
      myGLCD.update();char key = keypad.getKey();
       if(key='*'){/////////////////////////////////////////////////////////////////////////
      int value1=-1;
      int value2=-10;
      int value3=-100;
      int value4=-1000;
      int value5=-5;
      int value6=-50;
      int value7=-500;  
      int value8=-5000;
        if (key == '1') {chpB4B1EE69=chpB4B1EE69+value1;}//7
        if (key == '2') {chpB4B1EE69=chpB4B1EE69+value2;}
        if (key == '3') {chpB4B1EE69=chpB4B1EE69+value3;}
        if (key == '4') {chpB4B1EE69=chpB4B1EE69+value4;}
        if (key == '5') {chpB4B1EE69=chpB4B1EE69+value5;}
        if (key == '6') {chpB4B1EE69=chpB4B1EE69+value6;}
        if (key == '7') {chpB4B1EE69=chpB4B1EE69+value7;}
        if (key == '8') {chpB4B1EE69=chpB4B1EE69+value8;}
        if (key == '9') {value1=value1*(-1);value2=value2*(-1);value3=value3*(-1);value4=value4*(-1);value5=value5*(-1);value6=value6*(-1);value7=value7*(-1);value8=value8*(-1);}//改卡號
        if (key == '0') {}
      //斜線匡起來的區域拷貝在任何有else if陳敘式的大刮弧裡面
    }/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
    else if ( id[0] == 0xA3 && id[1] == 0xDF && id[2] == 0x38 && id[3] == 0x33 ) {
      myGLCD.clrScr();
      myGLCD.print("The card's ", LEFT, 0);
      myGLCD.print(" balance:", LEFT, 10);
      myGLCD.printNumI(chpD58EB308, LEFT, 20);
      myGLCD.print("(*)for charging", LEFT, 40);
      myGLCD.update();char key = keypad.getKey();
       if(key='*'){/////////////////////////////////////////////////////////////////////////
      int value1=-1;
      int value2=-10;
      int value3=-100;
      int value4=-1000;
      int value5=-5;
      int value6=-50;
      int value7=-500;  
      int value8=-5000;
        if (key == '1') {chpD58EB308=chpD58EB308+value1;}//8
        if (key == '2') {chpD58EB308=chpD58EB308+value2;}
        if (key == '3') {chpD58EB308=chpD58EB308+value3;}
        if (key == '4') {chpD58EB308=chpD58EB308+value4;}
        if (key == '5') {chpD58EB308=chpD58EB308+value5;}
        if (key == '6') {chpD58EB308=chpD58EB308+value6;}
        if (key == '7') {chpD58EB308=chpD58EB308+value7;}
        if (key == '8') {chpD58EB308=chpD58EB308+value8;}
        if (key == '9') {value1=value1*(-1);value2=value2*(-1);value3=value3*(-1);value4=value4*(-1);value5=value5*(-1);value6=value6*(-1);value7=value7*(-1);value8=value8*(-1);}//改卡號
        if (key == '0') {}
      //斜線匡起來的區域拷貝在任何有else if陳敘式的大刮弧裡面
    }/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
    else if ( id[0] == 0x24 && id[1] == 0x4C && id[2] == 0xFF && id[3] == 0x67 ) {
      myGLCD.clrScr();
      myGLCD.print("The card's ", LEFT, 0);
      myGLCD.print(" balance:", LEFT, 10);
      myGLCD.printNumI(chp244CFF67, LEFT, 20);
      myGLCD.print("(*)for charging", LEFT, 40);
      myGLCD.update();char key = keypad.getKey();
       if(key='*'){/////////////////////////////////////////////////////////////////////////
      int value1=-1;
      int value2=-10;
      int value3=-100;
      int value4=-1000;
      int value5=-5;
      int value6=-50;
      int value7=-500;  
      int value8=-5000;
        if (key == '1') {chp244CFF67=chp244CFF67+value1;}//9
        if (key == '2') {chp244CFF67=chp244CFF67+value2;}
        if (key == '3') {chp244CFF67=chp244CFF67+value3;}
        if (key == '4') {chp244CFF67=chp244CFF67+value4;}
        if (key == '5') {chp244CFF67=chp244CFF67+value5;}
        if (key == '6') {chp244CFF67=chp244CFF67+value6;}
        if (key == '7') {chp244CFF67=chp244CFF67+value7;}
        if (key == '8') {chp244CFF67=chp244CFF67+value8;}
        if (key == '9') {value1=value1*(-1);value2=value2*(-1);value3=value3*(-1);value4=value4*(-1);value5=value5*(-1);value6=value6*(-1);value7=value7*(-1);value8=value8*(-1);}//改卡號
        if (key == '0') {}
      //斜線匡起來的區域拷貝在任何有else if陳敘式的大刮弧裡面
    }/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
    else if ( id[0] == 0xD5 && id[1] == 0x1B && id[2] == 0xC7 && id[3] == 0x04 ) {
      myGLCD.clrScr();
      myGLCD.print("The card's ", LEFT, 0);
      myGLCD.print(" balance:", LEFT, 10);
      myGLCD.printNumI(chpD51BC704, LEFT, 20);
      myGLCD.print("(*)for charging", LEFT, 40);
      myGLCD.update();char key = keypad.getKey();
       if(key='*'){/////////////////////////////////////////////////////////////////////////
      int value1=-1;
      int value2=-10;
      int value3=-100;
      int value4=-1000;
      int value5=-5;
      int value6=-50;
      int value7=-500;      ////////////////
//     chpD58EB308 = EEPROM.read(1);//1
//     chp2572BB04 = EEPROM.read(2);//2
//     chp84523751 = EEPROM.read(3);//3
//     chpD4563D51 = EEPROM.read(4);//4
//     chp9E39232B = EEPROM.read(5);//5
//     chp74BB3251 = EEPROM.read(6);//6
//     chpB4B1EE69 = EEPROM.read(7);//7
//     chpA3DF3833 = EEPROM.read(8);//8
//     chp244CFF67 = EEPROM.read(9);//9
//     chpD51BC704 = EEPROM.read(10);//A
//     chpD4893751 = EEPROM.read(11);//B
//     chp49AFF7E6 = EEPROM.read(12);//C
//     chpF5FEA804 = EEPROM.read(13);//D
//     chpA4F88950 = EEPROM.read(14);//E
//    chpD029FF01 = EEPROM.read(15);//F
//    chpB29C63A9 = EEPROM.read(16);//10
//    chpA5DBAF08 = EEPROM.read(17);//11
//    chp460F232B = EEPROM.read(18);//
      int value8=-5000;
        if (key == '1') {chpD51BC704=chpD51BC704+value1;}//A
        if (key == '2') {chpD51BC704=chpD51BC704+value2;}
        if (key == '3') {chpD51BC704=chpD51BC704+value3;}
        if (key == '4') {chpD51BC704=chpD51BC704+value4;}
        if (key == '5') {chpD51BC704=chpD51BC704+value5;}
        if (key == '6') {chpD51BC704=chpD51BC704+value6;}
        if (key == '7') {chpD51BC704=chpD51BC704+value7;}
        if (key == '8') {chpD51BC704=chpD51BC704+value8;}
        if (key == '9') {value1=value1*(-1);value2=value2*(-1);value3=value3*(-1);value4=value4*(-1);value5=value5*(-1);value6=value6*(-1);value7=value7*(-1);value8=value8*(-1);}//改卡號
        if (key == '0') {}
      //斜線匡起來的區域拷貝在任何有else if陳敘式的大刮弧裡面
    }/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
    else if ( id[0] == 0xD4 && id[1] == 0x89 && id[2] == 0x37 && id[3] == 0x51 ) {
      myGLCD.clrScr();
      myGLCD.print("The card's ", LEFT, 0);
      myGLCD.print(" balance:", LEFT, 10);
      myGLCD.printNumI(chpD4893751, LEFT, 20);
      myGLCD.print("(*)for charging", LEFT, 40);
      myGLCD.update();char key = keypad.getKey();
       if(key='*'){/////////////////////////////////////////////////////////////////////////
      int value1=-1;
      int value2=-10;status = MFRC522_Anticoll(str);
      int value3=-100;
      int value4=-1000;
      int value5=-5;
      int value6=-50;
      int value7=-500;  
      int value8=-5000;
        if (key == '1') {chpD4893751=chpD4893751+value1;}//B
        if (key == '2') {chpD4893751=chpD4893751+value2;}
        if (key == '3') {chpD4893751=chpD4893751+value3;}
        if (key == '4') {chpD4893751=chpD4893751+value4;}
        if (key == '5') {chpD4893751=chpD4893751+value5;}
        if (key == '6') {chpD4893751=chpD4893751+value6;}
        if (key == '7') {chpD4893751=chpD4893751+value7;}
        if (key == '8') {chpD4893751=chpD4893751+value8;}
        if (key == '9') {value1=value1*(-1);value2=value2*(-1);value3=value3*(-1);value4=value4*(-1);value5=value5*(-1);value6=value6*(-1);value7=value7*(-1);value8=value8*(-1);}//改卡號
        if (key == '0') {}//改卡號
      //斜線匡起來的區域拷貝在任何有else if陳敘式的大刮弧裡面
    }/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
    else if ( id[0] == 0x49 && id[1] == 0xAF && id[2] == 0xF7 && id[3] == 0xE6 ) {
      myGLCD.clrScr();
      myGLCD.print("The card's ", LEFT, 0);
      myGLCD.print(" balance:", LEFT, 10);
      myGLCD.printNumI(chp49AFF7E6, LEFT, 20);
      myGLCD.print("(*)for charging", LEFT, 40);
      myGLCD.update();char key = keypad.getKey();
       if(key='*'){/////////////////////////////////////////////////////////////////////////
      int value1=-1;
      int value2=-10;
      int value3=-100;
      int value4=-1000;
      int value5=-5;
      int value6=-50;
      int value7=-500;  
      int value8=-5000;
        if (key == '1') {chp49AFF7E6=chp49AFF7E6+value1;}//C
        if (key == '2') {chp49AFF7E6=chp49AFF7E6+value2;}
        if (key == '3') {chp49AFF7E6=chp49AFF7E6+value3;}
        if (key == '4') {chp49AFF7E6=chp49AFF7E6+value4;}
        if (key == '5') {chp49AFF7E6=chp49AFF7E6+value5;}
        if (key == '6') {chp49AFF7E6=chp49AFF7E6+value6;}
        if (key == '7') {chp49AFF7E6=chp49AFF7E6+value7;}
        if (key == '8') {chp49AFF7E6=chp49AFF7E6+value8;}
        if (key == '9') {value1=value1*(-1);value2=value2*(-1);value3=value3*(-1);value4=value4*(-1);value5=value5*(-1);value6=value6*(-1);value7=value7*(-1);value8=value8*(-1);}//改卡號
        if (key == '0') {}//改卡號
      //斜線匡起來的區域拷貝在任何有else if陳敘式的大刮弧裡面
    }/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
    else if ( id[0] == 0xF5 && id[1] == 0xFE && id[2] == 0xA8 && id[3] == 0x04 ) {
      myGLCD.clrScr();
      myGLCD.print("The card's ", LEFT, 0);
      myGLCD.print(" balance:", LEFT, 10);
      myGLCD.printNumI(chpF5FEA804, LEFT, 20);
      myGLCD.print("(*)for charging", LEFT, 40);
      myGLCD.update();char key = keypad.getKey();
       if(key='*'){/////////////////////////////////////////////////////////////////////////
      int value1=-1;
      int value2=-10;
      int value3=-100;
      int value4=-1000;
      int value5=-5;
      int value6=-50;
      int value7=-500;  
      int value8=-5000;
        if (key == '1') {chpF5FEA804=chpF5FEA804+value1;}//D
        if (key == '2') {chpF5FEA804=chpF5FEA804+value2;}
        if (key == '3') {chpF5FEA804=chpF5FEA804+value3;}
        if (key == '4') {chpF5FEA804=chpF5FEA804+value4;}
        if (key == '5') {chpF5FEA804=chpF5FEA804+value5;}
        if (key == '6') {chpF5FEA804=chpF5FEA804+value6;}
        if (key == '7') {chpF5FEA804=chpF5FEA804+value7;}
        if (key == '8') {chpF5FEA804=chpF5FEA804+value8;}
        if (key == '9') {value1=value1*(-1);value2=value2*(-1);value3=value3*(-1);value4=value4*(-1);value5=value5*(-1);value6=value6*(-1);value7=value7*(-1);value8=value8*(-1);}//改卡號
        if (key == '0') {}
      //斜線匡起來的區域拷貝在任何有else if陳敘式的大刮弧裡面
    }/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
    else if ( id[0] == 0xA4 && id[1] == 0xF8 && id[2] == 0x89 && id[3] == 0x50 ) {
      myGLCD.clrScr();
      myGLCD.print("The card's ", LEFT, 0);
      myGLCD.print(" balance:", LEFT, 10);
      myGLCD.printNumI(chpA4F88950, LEFT, 20);
      myGLCD.print("(*)for charging", LEFT, 40);
      myGLCD.update();char key = keypad.getKey();
       if(key='*'){/////////////////////////////////////////////////////////////////////////
      int value1=-1;
      int value2=-10;
      int value3=-100;
      int value4=-1000;
      int value5=-5;
      int value6=-50;
      int value7=-500;  
      int value8=-5000;
        if (key == '1') {chpA4F88950=chpA4F88950+value1;}//E
        if (key == '2') {chpA4F88950=chpA4F88950+value2;}
        if (key == '3') {chpA4F88950=chpA4F88950+value3;}
        if (key == '4') {chpA4F88950=chpA4F88950+value4;}
        if (key == '5') {chpA4F88950=chpA4F88950+value5;}
        if (key == '6') {chpA4F88950=chpA4F88950+value6;}
        if (key == '7') {chpA4F88950=chpA4F88950+value7;}
        if (key == '8') {chpA4F88950=chpA4F88950+value8;}
        if (key == '9') {value1=value1*(-1);value2=value2*(-1);value3=value3*(-1);value4=value4*(-1);value5=value5*(-1);value6=value6*(-1);value7=value7*(-1);value8=value8*(-1);}//改卡號
        if (key == '0') {}
      //斜線匡起來的區域拷貝在任何有else if陳敘式的大刮弧裡面
    }/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
    else if ( id[0] == 0xD0 && id[1] == 0x29 && id[2] == 0xFF && id[3] == 0x01 ) {
      myGLCD.clrScr();
      myGLCD.print("The card's ", LEFT, 0);
      myGLCD.print(" balance:", LEFT, 10);
      myGLCD.printNumI(chpD029FF01, LEFT, 20);
      myGLCD.print("(*)for charging", LEFT, 40);
      myGLCD.update();char key = keypad.getKey();
       if(key='*'){/////////////////////////////////////////////////////////////////////////
      int value1=-1;
      int value2=-10;
      int value3=-100;
      int value4=-1000;
      int value5=-5;
      int value6=-50;
      int value7=-500;  
      int value8=-5000;
        if (key == '1') {chpD029FF01=chpD029FF01+value1;}//F
        if (key == '2') {chpD029FF01=chpD029FF01+value2;}
        if (key == '3') {chpD029FF01=chpD029FF01+value3;}
        if (key == '4') {chpD029FF01=chpD029FF01+value4;}
        if (key == '5') {chpD029FF01=chpD029FF01+value5;}
        if (key == '6') {chpD029FF01=chpD029FF01+value6;}
        if (key == '7') {chpD029FF01=chpD029FF01+value7;}
        if (key == '8') {chpD029FF01=chpD029FF01+value8;}
        if (key == '9') {value1=value1*(-1);value2=value2*(-1);value3=value3*(-1);value4=value4*(-1);value5=value5*(-1);value6=value6*(-1);value7=value7*(-1);value8=value8*(-1);}//改卡號
        if (key == '0') {}
      //斜線匡起來的區域拷貝在任何有else if陳敘式的大刮弧裡面
    }/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
    else if ( id[0] == 0xB2 && id[1] == 0x9C && id[2] == 0x63 && id[3] == 0xA9 ) {
      myGLCD.clrScr();
      myGLCD.print("The card's ", LEFT, 0);
      myGLCD.print(" balance:", LEFT, 10);
      myGLCD.printNumI(chpB29C63A9, LEFT, 20);
      myGLCD.print("(*)for charging", LEFT, 40);
      myGLCD.update();char key = keypad.getKey();
       if(key='*'){/////////////////////////////////////////////////////////////////////////
      int value1=-1;
      int value2=-10;
      int value3=-100;
      int value4=-1000;
      int value5=-5;
      int value6=-50;
      int value7=-500;  
      int value8=-5000;
        if (key == '1') {chpB29C63A9=chpB29C63A9+value1;}//10
        if (key == '2') {chpB29C63A9=chpB29C63A9+value2;}
        if (key == '3') {chpB29C63A9=chpB29C63A9+value3;}
        if (key == '4') {chpB29C63A9=chpB29C63A9+value4;}
        if (key == '5') {chpB29C63A9=chpB29C63A9+value5;}
        if (key == '6') {chpB29C63A9=chpB29C63A9+value6;}
        if (key == '7') {chpB29C63A9=chpB29C63A9+value7;}
        if (key == '8') {chpB29C63A9=chpB29C63A9+value8;}
        if (key == '9') {value1=value1*(-1);value2=value2*(-1);value3=value3*(-1);value4=value4*(-1);value5=value5*(-1);value6=value6*(-1);value7=value7*(-1);value8=value8*(-1);}//改卡號
        if (key == '0') {}
      //斜線匡起來的區域拷貝在任何有else if陳敘式的大刮弧裡面
    }/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
    else if ( id[0] == 0xA5 && id[1] == 0xDB && id[2] == 0xAF && id[3] == 0x08 ) {
      myGLCD.clrScr();
      myGLCD.print("The card's ", LEFT, 0);
      myGLCD.print(" balance:", LEFT, 10);
      myGLCD.printNumI(chpA5DBAF08, LEFT, 20);
      myGLCD.print("(*)for charging", LEFT, 40);
      myGLCD.update();char key = keypad.getKey();
       if(key='*'){
        /////////////////////////////////////////////////////////////////////////
      int value1=-1;
      int value2=-10;
      int value3=-100;
      int value4=-1000;
      int value5=-5;
      int value6=-50;
      int value7=-500;  
      int value8=-5000;
        if (key == '1') {chpA5DBAF08=chpA5DBAF08+value1;}//11
        if (key == '2') {chpA5DBAF08=chpA5DBAF08+value2;}
        if (key == '3') {chpA5DBAF08=chpA5DBAF08+value3;}
        if (key == '4') {chpA5DBAF08=chpA5DBAF08+value4;}
        if (key == '5') {chpA5DBAF08=chpA5DBAF08+value5;}
        if (key == '6') {chpA5DBAF08=chpA5DBAF08+value6;}
        if (key == '7') {chpA5DBAF08=chpA5DBAF08+value7;}
        if (key == '8') {chpA5DBAF08=chpA5DBAF08+value8;}
        if (key == '9') {value1=value1*(-1);value2=value2*(-1);value3=value3*(-1);value4=value4*(-1);value5=value5*(-1);value6=value6*(-1);value7=value7*(-1);value8=value8*(-1);}//改卡號
        if (key == '0') {}
      //斜線匡起來的區域拷貝在任何有else if陳敘式的大刮弧裡面
    }/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
    else if ( id[0] == 0xA5 && id[1] == 0xDB && id[2] == 0xAF && id[3] == 0x08 ) {
      myGLCD.clrScr();
      myGLCD.print("The card's ", LEFT, 0);
      myGLCD.print(" balance:", LEFT, 10);
      myGLCD.printNumI(chpA5DBAF08, LEFT, 20);
      myGLCD.print("(*)for charging", LEFT, 40);
      myGLCD.update();char key = keypad.getKey();
       if(key='*'){
        /////////////////////////////////////////////////////////////////////////
      int value1=-1;
      int value2=-10;
      int value3=-100;
      int value4=-1000;
      int value5=-5;
      int value6=-50;
      int value7=-500;  
      int value8=-5000;
        if (key == '1') {chp460F232B=chp460F232B+value1;}//12
        if (key == '2') {chp460F232B=chp460F232B+value2;}
        if (key == '3') {chp460F232B=chp460F232B+value3;}
        if (key == '4') {chp460F232B=chp460F232B+value4;}
        if (key == '5') {chp460F232B=chp460F232B+value5;}
        if (key == '6') {chp460F232B=chp460F232B+value6;}
        if (key == '7') {chp460F232B=chp460F232B+value7;}
        if (key == '8') {chp460F232B=chp460F232B+value8;}
        if (key == '9') {value1=value1*(-1);value2=value2*(-1);value3=value3*(-1);value4=value4*(-1);value5=value5*(-1);value6=value6*(-1);value7=value7*(-1);value8=value8*(-1);}//改卡號
        if (key == '0') {}
      //斜線匡起來的區域拷貝在任何有else if陳敘式的大刮弧裡面
    }/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
    else {
      myGLCD.clrScr();
      myGLCD.print("The card's", LEFT, 0);
      myGLCD.print(" number isn't", LEFT, 10);
      myGLCD.print(" registered!", LEFT, 20);
      myGLCD.update();
    }


    
//EEPROM.write(1, chpD58EB308);
//EEPROM.write(2, chp2572BB04);
//EEPROM.write(3, chp84523751);
//EEPROM.write(4, chpD4563D51);
//EEPROM.write(5, chp9E39232B);
//EEPROM.write(6, chp74BB3251);
//EEPROM.write(7, chpB4B1EE69);
//EEPROM.write(8, chpA3DF3833);
//EEPROM.write(9, chp244CFF67);
//EEPROM.write(10, chpD51BC704);
//EEPROM.write(11, chp49AFF7E6);
//EEPROM.write(12, chpF5FEA804);
//EEPROM.write(13, chpA4F88950);
//EEPROM.write(14, chpD029FF01);
//EEPROM.write(15, chpB29C63A9);
//EEPROM.write(16, chpB29C63A9);
//EEPROM.write(17, chpD4893751);//把這邊的卡號重複一遍（在最上面）第一個數字就等差數列d=1的形式繼續往下排列
    // Check people associated with card ID  
    
  
  }
MFRC522_Halt();

  delay(200);

}

/*
   Function：ShowCardID
   Description：Show Card ID
   Input parameter：ID string
   Return：Null
*/
void ShowCardID(uchar *id){


  int IDlen = 4;

  for (int i = 0; i < IDlen; i++) {


    // Write a piece of text on the first line...



    Serial.print(0x0F & (id[i] >> 4), HEX);
    Serial.print(0x0F & id[i], HEX);
    //        myGLCD.printNumI(0x0F & (id[i]>>4), HEX);
    //        myGLCD.printNumI(0x0F & id[i], HEX);
  }
  Serial.println("");
}

/*
   Function：ShowCardType
   Description：Show Card type
   Input parameter：Type string
   Return：Null
*/
void ShowCardType(uchar* type)
{
  Serial.print("Card type: ");
  if (type[0] == 0x04 && type[1] == 0x00)
    Serial.println("MFOne-S50");
  else if (type[0] == 0x02 && type[1] == 0x00)
    Serial.println("MFOne-S70");
  else if (type[0] == 0x44 && type[1] == 0x00)
    Serial.println("MF-UltraLight");
  else if (type[0] == 0x08 && type[1] == 0x00)
    Serial.println("MF-Pro");
  else if (type[0] == 0x44 && type[1] == 0x03)
    Serial.println("MF Desire");
  else
    Serial.println("Unknown");
}

/*
   Function：Write_MFRC5200
   Description：write a byte data into one register of MR RC522
   Input parameter：addr--register address；val--the value that need to write in
   Return：Null
*/
void Write_MFRC522(uchar addr, uchar val)
{
  digitalWrite(chipSelectPin, LOW);

  //address format：0XXXXXX0
  SPI.transfer((addr << 1) & 0x7E);
  SPI.transfer(val);

  digitalWrite(chipSelectPin, HIGH);
}

/*
   Function：Read_MFRC522
   Description：read a byte data into one register of MR RC522
   Input parameter：addr--register address
   Return：return the read value
*/
uchar Read_MFRC522(uchar addr)
{
  uchar val;

  digitalWrite(chipSelectPin, LOW);

  //address format：1XXXXXX0
  SPI.transfer(((addr << 1) & 0x7E) | 0x80);
  val = SPI.transfer(0x00);

  digitalWrite(chipSelectPin, HIGH);

  return val;
}

/*
   Function：SetBitMask
   Description：set RC522 register bit
   Input parameter：reg--register address;mask--value
   Return：null
*/
void SetBitMask(uchar reg, uchar mask)
{
  uchar tmp;
  tmp = Read_MFRC522(reg);
  Write_MFRC522(reg, tmp | mask); // set bit mask
}

/*
   Function：ClearBitMask
   Description：clear RC522 register bit
   Input parameter：reg--register address;mask--value
   Return：null
*/
void ClearBitMask(uchar reg, uchar mask)
{
  uchar tmp;
  tmp = Read_MFRC522(reg);
  Write_MFRC522(reg, tmp & (~mask)); // clear bit mask
}

/*
   Function：AntennaOn
   Description：Turn on antenna, every time turn on or shut down antenna need at least 1ms delay
   Input parameter：null
   Return：null
*/
void AntennaOn(void)
{
  uchar temp;

  temp = Read_MFRC522(TxControlReg);
  if (!(temp & 0x03))
  {
    SetBitMask(TxControlReg, 0x03);
  }
}

/*
   Function：AntennaOff
   Description：Turn off antenna, every time turn on or shut down antenna need at least 1ms delay
   Input parameter：null
   Return：null
*/
void AntennaOff(void)
{
  ClearBitMask(TxControlReg, 0x03);
}

/*
   Function：ResetMFRC522
   Description： reset RC522
   Input parameter：null
   Return：null
*/
void MFRC522_Reset(void)
{
  Write_MFRC522(CommandReg, PCD_RESETPHASE);
}

/*
   Function：InitMFRC522
   Description：initilize RC522
   Input parameter：null
   Return：null
*/
void MFRC522_Init(void)
{
  digitalWrite(NRSTPD, HIGH);

  MFRC522_Reset();

  //Timer: TPrescaler*TreloadVal/6.78MHz = 24ms
  Write_MFRC522(TModeReg, 0x8D); //Tauto=1; f(Timer) = 6.78MHz/TPreScaler
  Write_MFRC522(TPrescalerReg, 0x3E); //TModeReg[3..0] + TPrescalerReg
  Write_MFRC522(TReloadRegL, 30);
  Write_MFRC522(TReloadRegH, 0);

  Write_MFRC522(TxAutoReg, 0x40); //100%ASK
  Write_MFRC522(ModeReg, 0x3D); //CRC initilizate value 0x6363 ???

  //ClearBitMask(Status2Reg, 0x08); //MFCrypto1On=0
  //Write_MFRC522(RxSelReg, 0x86); //RxWait = RxSelReg[5..0]
  //Write_MFRC522(RFCfgReg, 0x7F); //RxGain = 48dB

  AntennaOn(); //turn on antenna
}

/*
   Function：MFRC522_Request
   Description：Searching card, read card type
   Input parameter：reqMode--search methods，
   TagType--return card types
   0x4400 = Mifare_UltraLight
   0x0400 = Mifare_One(S50)
   0x0200 = Mifare_One(S70)
   0x0800 = Mifare_Pro(X)
   0x4403 = Mifare_DESFire
   return：return MI_OK if successed
*/
uchar MFRC522_Request(uchar reqMode, uchar *TagType)
{
  uchar status;
  uint backBits; //the data bits that received

  Write_MFRC522(BitFramingReg, 0x07); //TxLastBists = BitFramingReg[2..0] ???

  TagType[0] = reqMode;
  status = MFRC522_ToCard(PCD_TRANSCEIVE, TagType, 1, TagType, &backBits);

  if ((status != MI_OK) || (backBits != 0x10))
  {
    status = MI_ERR;
  }

  return status;
}

/*
   Function：MFRC522_ToCard
   Description：communicate between RC522 and ISO14443
   Input parameter：command--MF522 command bits
   sendData--send data to card via rc522
   sendLen--send data length
   backData--the return data from card
   backLen--the length of return data
   return：return MI_OK if successed
*/
uchar MFRC522_ToCard(uchar command, uchar *sendData, uchar sendLen, uchar *backData, uint *backLen)
{
  uchar status = MI_ERR;
  uchar irqEn = 0x00;
  uchar waitIRq = 0x00;
  uchar lastBits;
  uchar n;
  uint i;

  switch (command)
  {
    case PCD_AUTHENT: //verify card password
      {
        irqEn = 0x12;
        waitIRq = 0x10;
        break;
      }
    case PCD_TRANSCEIVE: //send data in the FIFO
      {
        irqEn = 0x77;
        waitIRq = 0x30;
        break;
      }
    default:
      break;
  }

  Write_MFRC522(CommIEnReg, irqEn | 0x80); //Allow interruption
  ClearBitMask(CommIrqReg, 0x80); //Clear all the interrupt bits
  SetBitMask(FIFOLevelReg, 0x80); //FlushBuffer=1, FIFO initilizate

  Write_MFRC522(CommandReg, PCD_IDLE); //NO action;cancel current command ???

  //write data into FIFO
  for (i = 0; i < sendLen; i++)
  {
    Write_MFRC522(FIFODataReg, sendData[i]);
  }

  //procceed it
  Write_MFRC522(CommandReg, command);
  if (command == PCD_TRANSCEIVE)
  {
    SetBitMask(BitFramingReg, 0x80); //StartSend=1,transmission of data starts
  }

  //waite receive data is finished
  i = 2000; //i should adjust according the clock, the maxium the waiting time should be 25 ms???
  do
  {
    //CommIrqReg[7..0]
    //Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
    n = Read_MFRC522(CommIrqReg);
    i--;
  }
  while ((i != 0) && !(n & 0x01) && !(n & waitIRq));

  ClearBitMask(BitFramingReg, 0x80); //StartSend=0

  if (i != 0)
  {
    if (!(Read_MFRC522(ErrorReg) & 0x1B)) //BufferOvfl Collerr CRCErr ProtecolErr
    {
      status = MI_OK;
      if (n & irqEn & 0x01)
      {
        status = MI_NOTAGERR; //??
      }

      if (command == PCD_TRANSCEIVE)
      {
        n = Read_MFRC522(FIFOLevelReg);
        lastBits = Read_MFRC522(ControlReg) & 0x07;
        if (lastBits)
        {
          *backLen = (n - 1) * 8 + lastBits;
        }
        else
        {
          *backLen = n * 8;
        }

        if (n == 0)
        {
          n = 1;
        }
        if (n > MAX_LEN)
        {
          n = MAX_LEN;
        }

        //read the data from FIFO
        for (i = 0; i < n; i++)
        {
          backData[i] = Read_MFRC522(FIFODataReg);
        }
      }
    }
    else
    {
      status = MI_ERR;
    }

  }

  //SetBitMask(ControlReg,0x80); //timer stops
  //Write_MFRC522(CommandReg, PCD_IDLE);

  return status;
}

/*
   Function：MFRC522_Anticoll
   Description：Prevent conflict, read the card serial number
   Input parameter：serNum--return the 4 bytes card serial number, the 5th byte is recheck byte
   return：return MI_OK if successed
*/
uchar MFRC522_Anticoll(uchar *serNum)
{
  uchar status;
  uchar i;
  uchar serNumCheck = 0;
  uint unLen;

  //ClearBitMask(Status2Reg, 0x08); //strSensclear
  //ClearBitMask(CollReg,0x80); //ValuesAfterColl
  Write_MFRC522(BitFramingReg, 0x00); //TxLastBists = BitFramingReg[2..0]

  serNum[0] = PICC_ANTICOLL;
  serNum[1] = 0x20;
  status = MFRC522_ToCard(PCD_TRANSCEIVE, serNum, 2, serNum, &unLen);

  if (status == MI_OK)
  {
    //Verify card serial number
    for (i = 0; i < 4; i++)
    {
      serNumCheck ^= serNum[i];
    }
    if (serNumCheck != serNum[i])
    {
      status = MI_ERR;
    }
  }

  //SetBitMask(CollReg, 0x80); //ValuesAfterColl=1

  return status;
}

/*
   Function：CalulateCRC
   Description：Use MF522 to caculate CRC
   Input parameter：pIndata--the CRC data need to be read，len--data length，pOutData-- the caculated result of CRC
   return：Null
*/
void CalulateCRC(uchar *pIndata, uchar len, uchar *pOutData)
{
  uchar i, n;

  ClearBitMask(DivIrqReg, 0x04); //CRCIrq = 0
  SetBitMask(FIFOLevelReg, 0x80); //Clear FIFO pointer
  //Write_MFRC522(CommandReg, PCD_IDLE);

  //Write data into FIFO
  for (i = 0; i < len; i++)
  {
    Write_MFRC522(FIFODataReg, *(pIndata + i));
  }
  Write_MFRC522(CommandReg, PCD_CALCCRC);

  //waite CRC caculation to finish
  i = 0xFF;
  do
  {
    n = Read_MFRC522(DivIrqReg);
    i--;
  }
  while ((i != 0) && !(n & 0x04)); //CRCIrq = 1

  //read CRC caculation result
  pOutData[0] = Read_MFRC522(CRCResultRegL);
  pOutData[1] = Read_MFRC522(CRCResultRegM);
}

/*
   Function：MFRC522_Write
   Description：write block data
   Input parameters：blockAddr--block address;writeData--Write 16 bytes data into block
   return：return MI_OK if successed
*/
uchar MFRC522_Write(uchar blockAddr, uchar *writeData)
{
  uchar status;
  uint recvBits;
  uchar i;
  uchar buff[18];

  buff[0] = PICC_WRITE;
  buff[1] = blockAddr;
  CalulateCRC(buff, 2, &buff[2]);
  status = MFRC522_ToCard(PCD_TRANSCEIVE, buff, 4, buff, &recvBits);

  if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A))
  {
    status = MI_ERR;
  }

  if (status == MI_OK)
  {
    for (i = 0; i < 16; i++) //Write 16 bytes data into FIFO
    {
      buff[i] = *(writeData + i);
    }
    CalulateCRC(buff, 16, &buff[16]);
    status = MFRC522_ToCard(PCD_TRANSCEIVE, buff, 18, buff, &recvBits);

    if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A))
    {
      status = MI_ERR;
    }
  }

  return status;
}

/*
   Function：MFRC522_Halt
   Description：Command the cards into sleep mode
   Input parameters：null
   return：null
*/
void MFRC522_Halt(void)
{
  uchar status;
  uint unLen;
  uchar buff[4];

  buff[0] = PICC_HALT;
  buff[1] = 0;
  CalulateCRC(buff, 2, &buff[2]);

  status = MFRC522_ToCard(PCD_TRANSCEIVE, buff, 4, buff, &unLen);
}