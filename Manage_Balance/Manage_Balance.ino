/* @file CustomKeypad.pde
  || @version 1.0
  || @author Alexander Brevig
  || @contact alexanderbrevig@gmail.com
  ||
  || @description
  || | Demonstrates changing the keypad size and key values.
  || #
*/
#include <EEPROM.h>
#include <Keypad.h>
#include <Password.h> //Password kutuphanesi ekleniyor.

int x=2;


int bal=0;
//define the cymbols on the buttons of the keypads
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {17, 16, 15, 14}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {19,22, 18};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


void setup() {
  Serial.begin(9600);
x = EEPROM.read(1);
bal = EEPROM.read(2);
}
int balance;
void loop() {

  char key = keypad.getKey(); 

  int a=-1000;
  int b=-100;
  int c=-10;
  int d=-1;
  
EEPROM.write(1, 2);
  if (key != NO_KEY) {

Serial.print(x);
EEPROM.write(1, 2);
EEPROM.write(2, bal);
 if (key=='1'&&x==2){
      bal=a;
    }else if (key=='2'&&x==2){
      bal=2*a;
    }else if (key=='3'&&x==2){
      bal=3*a;
    }else if (key=='4'&&x==2){
      bal=4*a;
    }else if (key=='5'&&x==2){
      bal=5*a;
    }else if (key=='6'&&x==2){
      bal=6*a;
    }else if (key=='7'&&x==2){
      bal=7*a;
    }else if (key=='8'&&x==2){
      bal=8*a;
    }else if (key=='9'&&x==2){
      bal=9*a;
    }else if (key=='1'&&x==3){
      bal=bal+b;
    }else if (key=='0'&&x==2){
      bal=bal;
    }else if (key=='0'&&x==3){
      bal=bal;
    }else if (key=='0'&&x==4){
      bal=bal;
    }else if (key=='0'&&x==5){
      bal=bal;
    }else if (key=='2'&&x==3){
      bal=bal+2*b;
    }else if (key=='3'&&x==3){
      bal=bal+3*b;
    }else if (key=='4'&&x==3){
      bal=bal+4*b;
    }else if (key=='5'&&x==3){
      bal=bal+5*b;
    }else if (key=='6'&&x==3){
      bal=bal+6*b;
    }else if (key=='7'&&x==3){
      bal=bal+7*b;
    }else if (key=='8'&&x==3){
      bal=bal+8*b;
    }else if (key=='9'&&x==3){
      bal=bal+9*b;
    }else if (key=='1'&&x==4){
      bal=bal+c;
    }else if (key=='2'&&x==4){
      bal=bal+2*c;
    }else if (key=='3'&&x==4){
      bal=bal+3*c;
    }else if (key=='4'&&x==4){
      bal=bal+4*c;
    }else if (key=='5'&&x==4){
      bal=bal+5*c;
    }else if (key=='6'&&x==4){
      bal=bal+6*c;
    }else if (key=='7'&&x==4){
      bal=bal+7*c;
    }else if (key=='8'&&x==4){
      bal=bal+8*c;
    }else if (key=='9'&&x==4){
      bal=bal+9*c;
    }else if (key=='1'&&x==5){
      bal=bal+d;
    }else if (key=='2'&&x==5){
      bal=bal+2*d;
    }else if (key=='3'&&x==5){
      bal=bal+3*d;
    }else if (key=='4'&&x==5){
      bal=bal+4*d;
    }else if (key=='5'&&x==5){
      bal=bal+5*d;
    }else if (key=='6'&&x==5){
      bal=bal+6*d;
    }else if (key=='7'&&x==5){
      bal=bal+7*d;
    }else if (key=='8'&&x==5){
      bal=bal+8*d;
    }else if (key=='9'&&x==5){
      bal=bal+9*d;
    }
    else if (key == '1' && x == 6) {
bal=bal;Serial.print("You still remain ");Serial.print(bal,DEC);Serial.println(" dollars");x=1;

   
    }
    else if (key == '0' && x == 6) {
bal=(-1)*bal;Serial.print("You still remain ");Serial.print(bal,DEC);Serial.println(" dollars");x=1;
    }else if (key!='0'&&x==2){
      return 0;
    }
    x++;
    }
  }
 











