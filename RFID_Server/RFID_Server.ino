#include <SPI.h>
#include <SPI.h>
#include <Keypad.h>
#include <Ethernet.h>
#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <LCD5110_Basic.h>
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


#define W5200_CS  10
#define SDCARD_CS 4

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
//Page 1:Commandchp244CFF69
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
#define Reserved11 0x1Achp244CFF69
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
// Set the LCD address to 0x27 for a 16 chars and 2 line display

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
unsigned long lastConnectionTime = 0;             // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 10L * 1000L; // delay between updates, in milliseconds
// the "L" is needed to use long type numbers
LCD5110 myGLCD(25, 27, 29, 31, 33); //The LCD of which is showing the balance of choihong pass

LCD5110 myGLCD2(24, 26, 28, 32, 30); //The LCD of which is showing the Card Type of choihong pass
//Number Pad Proclaiming
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3F, 20, 4);
byte rowPins[ROWS] = {17, 16, 15, 14}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {19, 22, 18}; //connect to the column pinouts of the keypad
extern uint8_t SmallFont[];
extern uint8_t arduino_logo[];
extern uint8_t CYCHP[];
extern uint8_t ELCHP[];
extern uint8_t NOT_REGISTERED[];
extern uint8_t NOT_SENSORED[];
extern uint8_t NOT_SENSORED2[];
extern uint8_t NOT_SENSORED3[];
extern uint8_t NOT_SENSORED4[];
extern uint8_t ADCHP[];
int x = 2;
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
//4 bytes Serial number of card, the 5 bytes is verfiy bytes
uchar serNum[5];
int chpCA31AB85 = 0;//16

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 0, 177);
EthernetClient client;
EthernetClient cardNum;
EthernetClient Update;
EthernetClient cardType;
EthernetClient cardBal;
EthernetClient Edit;

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):

int red = 37;
int green = 35;
int blue = 34;
int CY = 4;
int EL = 0;
int AD = 1;
int bal = 0;
int a = 0, b = 0, z = 0, v, c = 0;
void setup()

{

  int c = EEPROM.read(1);
  int a = EEPROM.read(2);
  int b = EEPROM.read(3);
  int z = EEPROM.read(4);
  pinMode(AD, INPUT);
  pinMode(EL, INPUT);
  pinMode(CY, INPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(SDCARD_CS, OUTPUT);
  digitalWrite(SDCARD_CS, HIGH); //Deselect the SD card

  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();

  chpCA31AB85 = EEPROM.read(24);
  x = EEPROM.read(25);
  bal = EEPROM.read(26);
  myGLCD.InitLCD();


  myGLCD.setFont(SmallFont);
  myGLCD2.InitLCD();


  myGLCD2.setFont(SmallFont);
  //Updating the LCD
  Serial.begin(57600);
  myGLCD.drawBitmap(0, 0, arduino_logo, 84, 48);
   myGLCD2.drawBitmap(0, 0, arduino_logo, 84, 48);
  delay(1000);
  myGLCD.clrScr();myGLCD2.clrScr();
  SPI.begin();
  pinMode(chipSelectPin, OUTPUT); // Set digital pin 10 as OUTPUT to connect it to the RFID /ENABLE pin
  digitalWrite(chipSelectPin, LOW); // Activate the RFID reader
  pinMode(NRSTPD, OUTPUT); // Set digital pin 5 , Not Reset and Power-down
  MFRC522_Init();
  Ethernet.begin(mac,ip);

}
void(* resetFunc) (void) = 0;
String cn1;
String ct1;
int AD_Button = 0;
int EL_Button = 0;
int CY_Button = 0;
void httpRequest() {
  // close any connection before send a new request.
  // This will free the socket on the WiFi shield
  cardNum.stop();

  // if there's a successful connection:
  if (cardNum.connect("120.126.84.78", 80)) {
    //Serial.println("connected");
    // Making HTTP request
    cardNum.println("GET /RFID/CardNumber.txt ");
    cardNum.println("Host: 120.126.84.78");

    // note the time that the connection was made:
    lastConnectionTime = millis();
  }
  
}
void httpRequest1() {
  // close any connection before send a new request.
  // This will free the socket on the WiFi shield
  cardBal.stop();

  // if there's a successful connection:
  if (cardBal.connect("120.126.84.78", 80)) {
    //Serial.println("connected");
    // Making HTTP request
    cardBal.println("GET /RFID/CardBalance.txt ");
    cardBal.println("Host: 120.126.84.78");

    // note the time that the connection was made:
    lastConnectionTime = millis();
  }
  
}
void httpRequest2() {
  // close any connection before send a new request.
  // This will free the socket on the WiFi shield
  cardType.stop();

  // if there's a successful connection:
  if (cardType.connect("120.126.84.78", 80)) {
    //Serial.println("connected");
    // Making HTTP request
    cardType.println("GET /RFID/CardType.txt ");
    cardType.println("Host: 120.126.84.78");

    // note the time that the connection was made:
    lastConnectionTime = millis();
  }
  
}
  char key;
void loop()
{

 
     key = keypad.getKey();
  AD_Button = digitalRead(AD);
  CY_Button = digitalRead(CY);
  EL_Button = digitalRead(EL);
  uchar status;
  uchar str[MAX_LEN];

  // Search card, return card types
  status = MFRC522_Request(PICC_REQIDL, str);
  status = MFRC522_Anticoll(str);
  if (status != MI_OK)
  {

    digitalWrite(blue, LOW);
    myGLCD2.drawBitmap(0, 0, NOT_SENSORED2, 84, 48);

lcd.clear();
    myGLCD.clrScr();
    myGLCD.print("Please Sensor", LEFT, 0);
    myGLCD.print("the card first!", LEFT, 10);
  
    return;
  }

  // Show card type


  //Prevent conflict, return the 4 bytes Serial number of the card

  
  // str[0..3]: serial number of the cardhpD58EB308 = EEPROM.read(1);//1

  // str[4]: XOR checksum of the SN.
  else {

     int y = 1;
  if (cardNum.available()) {

    c++; EEPROM.write(1, c);
    char cn = cardNum.read();

    char cn2[8];
    if (cn != '$') {


      int z = y - 1;
      int Min = 9 * z;
      int Max = 9 * y;

      if (Min < c < Max) {
        cn1 = cn1 + String(cn);

        y++;
      }
    }
    else if (cn == '$') {
      x = 0;
      cn1 = "";
    }
  }

  if (millis() - lastConnectionTime > postingInterval) {
    httpRequest();
  }
    uchar* id = serNum;
    memcpy(serNum, str, 5);

    
    int card = 0x0F & (id[0] >> 4);
    int card2 = 0x0F & id[0];
    int card3 = 0x0F & (id[1] >> 4);
    int card4 = 0x0F & id[1];
    int card5 = 0x0F & (id[2] >> 4);
    int card6 = 0x0F & id[2];
    int card7 = 0x0F & (id[3] >> 4);
    int card8 = 0x0F & id[3];
    String CardNumber = String(card, HEX) + String(card2, HEX) + String(card3, HEX) + String(card4, HEX) + String(card5, HEX) + String(card6, HEX) + String(card7, HEX) + String(card8, HEX);

lcd.setCursor(0,0);
lcd.print(CardNumber);


    // Check people associated with card ID



    //   if ( id[0] == 0xCA && id[1] == 0x31 && id[2] == 0xAB && id[3] == 0x85 ) {
    //
    //      myGLCD.print("(*)for charging", LEFT, 40);
    //      myGLCD.clrScr();
    //      myGLCD.print("The card's ", LEFT, 0);
    //      myGLCD.print(" balance:", LEFT, 10);
    //      myGLCD.printNumI(bal, LEFT, 20);
    //      myGLCD.print("(*)for charging", LEFT, 40);
    //      client.println(bal);
    //      myGLCD2.drawBitmap(0, 0, ELCHP, 84, 48);
    //

    //斜線匡起來的區域拷貝在任何有else if陳敘式的大刮弧裡面
    // }/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//
    if (CardNumber == cn1) {

      myGLCD2.clrScr();}
//      myGLCD2.clrScr();
//     if(ct1=="CYCHP"){
//      myGLCD2.drawBitmap(0, 0, CYCHP, 84, 48);
//     }else if(ct1=="ELCHP"){
//      myGLCD2.drawBitmap(0, 0, ELCHP, 84, 48);
//     }else if(ct1=="ADCHP"){
//      myGLCD2.drawBitmap(0, 0, ADCHP, 84, 48);
//     }
String Bal;
     

      EEPROM.write(25, 2);
     if (key != NO_KEY) {
        
 int a = -1000;
      int b = -100;
      int c = -10;
      int d = -1;
        EEPROM.write(25, 2);
        EEPROM.write(26, bal);
        if (key == '1' && x == 2) {
          bal = a;
        } else if (key == '2' && x == 2) {
          bal = 2 * a;
        } else if (key == '3' && x == 2) {
          bal = 3 * a;
        } else if (key == '4' && x == 2) {
          bal = 4 * a;
        } else if (key == '5' && x == 2) {
          bal = 5 * a;
        } else if (key == '6' && x == 2) {
          bal = 6 * a;
        } else if (key == '7' && x == 2) {
          bal = 7 * a;
        } else if (key == '8' && x == 2) {
          bal = 8 * a;
        } else if (key == '9' && x == 2) {
          bal = 9 * a;
        } else if (key == '1' && x == 3) {
          bal = bal + b;
        } else if (key == '2' && x == 3) {
          bal = bal + 2 * b;
        } else if (key == '3' && x == 3) {
          bal = bal + 3 * b;
        } else if (key == '4' && x == 3) {
          bal = bal + 4 * b;
        } else if (key == '5' && x == 3) {
          bal = bal + 5 * b;
        } else if (key == '6' && x == 3) {
          bal = bal + 6 * b;
        } else if (key == '7' && x == 3) {
          bal = bal + 7 * b;
        } else if (key == '8' && x == 3) {
          bal = bal + 8 * b;
        } else if (key == '9' && x == 3) {
          bal = bal + 9 * b;
        } else if (key == '1' && x == 4) {
          bal = bal + c;
        } else if (key == '2' && x == 4) {
          bal = bal + 2 * c;
        } else if (key == '3' && x == 4) {
          bal = bal + 3 * c;
        } else if (key == '4' && x == 4) {
          bal = bal + 4 * c;
        } else if (key == '5' && x == 4) {
          bal = bal + 5 * c;
        } else if (key == '6' && x == 4) {
          bal = bal + 6 * c;
        } else if (key == '7' && x == 4) {
          bal = bal + 7 * c;
        } else if (key == '8' && x == 4) {
          bal = bal + 8 * c;
        } else if (key == '0' && x == 4) {
          bal = 0;
        } else if (key == '0' && x == 3) {
          bal = 0;
        } else if (key == '0' && x == 2) {
          bal = 0;
        } else if (key == '0' && x == 5) {
          bal = 0;
        } else if (key == '1' && x == 5) {
          bal = bal + d;
        } else if (key == '2' && x == 5) {
          bal = bal + 2 * d;
        } else if (key == '3' && x == 5) {
          bal = bal + 3 * d;
        } else if (key == '4' && x == 5) {
          bal = bal + 4 * d;
        } else if (key == '5' && x == 5) {
          bal = bal + 5 * d;
        } else if (key == '6' && x == 5) {
          bal = bal + 6 * d;
        } else if (key == '7' && x == 5) {
          bal = bal + 7 * d;
        } else if (key == '8' && x == 5) {
          bal = bal + 8 * d;
        } else if (key == '9' && x == 5) {
          bal = bal + 9 * d;
        }
        else if (key == '1' && x == 6) {
          bal = bal; Serial.print("You still remain "); Serial.print(bal, DEC); Serial.println(" dollars"); x = 1; EEPROM.write(26, bal);
         

          
          Change(CardNumber,ct1,bal);

        }

        else if (key == '0' && x == 6) {
          bal = (-1) * bal; Serial.print("You still remain "); Serial.print(bal, DEC); Serial.println(" dollars"); x = 1; EEPROM.write(26, bal);
     Change(CardNumber,ct1,bal);
          
        } else if (key != '0' && x == 6) {

        }
        x++;
         
          
            
      }else{}


//      else if(CardNumber!=cn1){
//        digitalWrite(red, LOW);
//        myGLCD2.drawBitmap(0, 0, NOT_REGISTERED, 84, 48);
//        myGLCD.clrScr();
//        myGLCD.print("The card's", LEFT, 0);
//        myGLCD.print(" number isn't", LEFT, 10);
//        myGLCD.print(" registered!", LEFT, 20);
//        int x = 0;
//        int y = 0;
//        int z = 0;
        if (CY_Button == LOW) {
          CardRegisterAsCY(serNum); delay(1000);
        } else if (EL_Button == LOW) {
          CardRegisterAsEL(serNum); delay(1000);
        } else if (AD_Button == LOW) {
          CardRegisterAsAD(serNum); delay(1000);
        }
//        //
//      }

   // }
  }
  //
  //    EEPROM.write(24, chpCA31AB85);


  MFRC522_Halt(); //command the card into sleep mode
  //把這邊的卡號重複一遍（在最上面）第一個數字就等差數列d=1的形式繼續往下排列
  // Check people associated with card ID

}

/*
   Function：ShowCardID
   Description：Show Card ID
   Input parameter：ID string
   Return：Null
*/
void ShowCardID(uchar *id)
{ lcd.clear();
  int IDlen = 4;
  for (int i = 0; i < IDlen; i++) {

    lcd.print(0x0F & (id[i] >> 4), HEX);
    lcd.print(0x0F & id[i], HEX);
  }
}
void proclaimCardNumber(uchar *id) {
  int card = 0x0F & (id[0] >> 4);
  int card2 = 0x0F & id[0];
  int card3 = 0x0F & (id[1] >> 4);
  int card4 = 0x0F & id[1];
  int card5 = 0x0F & (id[2] >> 4);
  int card6 = 0x0F & id[2];
  int card7 = 0x0F & (id[3] >> 4);
  int card8 = 0x0F & id[3];
  String CardNumber = String(card, HEX) + String(card2, HEX) + String(card3, HEX) + String(card4, HEX) + String(card5, HEX) + String(card6, HEX) + String(card7, HEX) + String(card8, HEX);
}
void CardRegisterAsCY(uchar *id) {
  int card = 0x0F & (id[0] >> 4);
  int card2 = 0x0F & id[0];
  int card3 = 0x0F & (id[1] >> 4);
  int card4 = 0x0F & id[1];
  int card5 = 0x0F & (id[2] >> 4);
  int card6 = 0x0F & id[2];
  int card7 = 0x0F & (id[3] >> 4);
  int card8 = 0x0F & id[3];
  Register(String(card, HEX), String(card2, HEX), String(card3, HEX), String(card4, HEX), String(card5, HEX), String(card6, HEX), String(card7, HEX), String(card8, HEX), "CYCHP", "0");

} void CardRegisterAsAD(uchar *id) {
  int card = 0x0F & (id[0] >> 4);
  int card2 = 0x0F & id[0];
  int card3 = 0x0F & (id[1] >> 4);
  int card4 = 0x0F & id[1];
  int card5 = 0x0F & (id[2] >> 4);
  int card6 = 0x0F & id[2];
  int card7 = 0x0F & (id[3] >> 4);
  int card8 = 0x0F & id[3];
  Register(String(card, HEX), String(card2, HEX), String(card3, HEX), String(card4, HEX), String(card5, HEX), String(card6, HEX), String(card7, HEX), String(card8, HEX), "ADCHP", "0");

}

void CardRegisterAsEL(uchar *id) {
  int card = 0x0F & (id[0] >> 4);
  int card2 = 0x0F & id[0];
  int card3 = 0x0F & (id[1] >> 4);
  int card4 = 0x0F & id[1];
  int card5 = 0x0F & (id[2] >> 4);
  int card6 = 0x0F & id[2];
  int card7 = 0x0F & (id[3] >> 4);
  int card8 = 0x0F & id[3];
  Register(String(card, HEX), String(card2, HEX), String(card3, HEX), String(card4, HEX), String(card5, HEX), String(card6, HEX), String(card7, HEX), String(card8, HEX), "ELCHP", "0");

}
/*
   Function：ShowCardType
   Description：Show Card type
   Input parameter：Type string
   Return：Null
*/
void ShowCardType(uchar* type)
{
  //    Serial.print("Card type: ");
  //    if(type[0]==0x04&&type[1]==0x00)
  //        Serial.println("MFOne-S50");
  //    else if(type[0]==0x02&&type[1]==0x00)
  //        Serial.println("MFOne-S70");
  //    else if(type[0]==0x44&&type[1]==0x00)
  //        Serial.println("MF-UltraLight");
  //    else if(type[0]==0x08&&type[1]==0x00)
  //        Serial.println("MF-Pro");
  //    else if(type[0]==0x44&&type[1]==0x03)
  //        Serial.println("MF Desire");
  //    else
  //        Serial.println("Unknown");
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
void getCardType(int j) {

  if (cardType.connect("120.126.84.78", 80)) {
    Serial.println("connected");
    // Making HTTP request
    cardType.println("GET /RFID/CardType.txt ");
    cardType.println("Host: 120.126.84.78");
  }
  if (cardType.available()) {
    a++; EEPROM.write(2, a);
    char ct = cardType.read(); char ct1;
    if (ct != '$') {


      int k = j - 1;
      int Min = 6 * k;
      int Max = 6 * j;
      if (Min < a < Max) {
        Serial.print(ct);
        ct1 = ct;
        j++;
      }
    } else if (ct == '$') {
      Serial.print("\n");

    }

  }
}
void getCardNumber(int y) {
  if (cardNum.connect("120.126.84.78", 80)) {
    //Serial.println("connected");
    // Making HTTP request
    cardNum.println("GET /RFID/CardNumber.txt ");
    cardNum.println("Host: 120.126.84.78");
  }
  if (cardNum.available()) {
    x++; EEPROM.write(1, x);
    char cn = cardNum.read();
    String cn1;
    char cn2[8];
    if (cn != '$') {


      int z = y - 1;
      int Min = 9 * z;
      int Max = 9 * y;

      if (Min < x < Max) {

        cn1 += cn;

        y++;
      }

    }
    else if (cn == '$') {

      x--;
    }
  }
}
void getCardBalance(int h) {

}
void Change(String CardNumber, String Type, int Bal)
{
 
   
  if (client.connect("120.126.84.78", 80)) {

    client.println("GET /RFID/change_data.php?CardNumber=" + CardNumber + "&CardBalance=" + Bal + "&CardType=" + String(Type) + " HTTP/1.1");
    client.println("Host: 120.126.84.78");
    client.println("Connection: close");
    client.println();
  }

  if (Update.connect("120.126.84.78", 80)) {
    //Serial.println("connected");
    // Making HTTP request
    Update.println("GET /RFID/review_data.php ");
    Update.println("Host: 120.126.84.78");
    lcd.setCursor(0, 1);
    lcd.print("*Edited*");
  }
  client.stop();
}
void Register(String x, String y, String z, String a, String b, String c, String d, String e, String Type, String Balance)
{
  if (client.connect("120.126.84.78", 80)) {

    client.println("GET /RFID/add_data.php?CardNumber=" + String(x) + String(y) + String(z) + String(a) + String(b) + String(c) + String(d) + String(e) + "&CardBalance=" + Balance + "&CardType=" + String(Type) + " HTTP/1.1");
    client.println("Host: 120.126.84.78");
    client.println("Connection: close");
    client.println();

    if (Update.connect("120.126.84.78", 80)) {
      //Serial.println("connected");
      // Making HTTP request
      Update.println("GET /RFID/review_data.php ");
      Update.println("Host: 120.126.84.78");
    }
    lcd.setCursor(0, 1);
    lcd.print("*Registered*");
  }
  else {
    lcd.setCursor(0, 1);
    lcd.print("*Connection failed*");
  }
  client.stop();
}
