#include <Arduino.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <Countimer.h>

// defined lcd pins
const int rs = 2, en = 3, d4 = 4, d5 = 5,d6 = 6, d7 = 7; 

int i = 0;

// defining button parameters 
const int setBt = 8, upBt = 9, downBt = 10, startBt = 11;
int set = 0;
bool setFlag = 0;

// defining buzzer parameters
int buzzerPin = A0;
int buzzTime = 200;

// defined variables to read button values
int oldSetVal = 1, oldUpVal = 1, oldDownVal = 1, oldStartVal = 1;
int newSetVal, newUpVal, newDownVal, newStartVal; 

//variables for second, minute and hour
int time_s = 0, time_m = 0, time_h = 0;

//initiated liquidCrystal library
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//initializing Countimer as tdown
Countimer tdown; 

// what will happen when the timmer sets 00:00:00
void onComplete(){
  do{
    tone(buzzerPin, 450);
    delay(buzzTime);
    noTone(buzzerPin);
    delay(buzzTime);
    i++;
    }while(i<4); 


  tdown.stop();
  
}

// writing time_s, time_m, time_h into the eeprom memory of the arduino 
void EEPROM_write(){
  EEPROM.write(1, time_s);
  EEPROM.write(2, time_m);
  EEPROM.write(3, time_h);
}

//reading form the eeprom memory of the arduino
void EEPROM_read(){
  time_s = EEPROM.read(1);
  time_m = EEPROM.read(2);
  time_h = EEPROM.read(3);

}

//printing time at one second of interval 
void print_time(){
  time_s = tdown.getCurrentSeconds();
  time_m = tdown.getCurrentMinutes();
  time_h = tdown.getCurrentHours();
}

// code for when the set button is clicked
void onClickSetBt(){
  
  set = set+1;
  if (set>4){set=0; setFlag = 1;}
  
  delay(100);
}

// code for when the up button is clicked
void onClickUpBt(){
  if (set == 1){time_s++; if (time_s>59){time_s=0;}}
  if (set == 2){time_m++; if (time_m>59){time_m=0;}}
  if (set == 3){time_h++; if (time_h>99){time_h=0;}}
  if (set == 4){set = 1;}
}

// code for when the down button is clicked
void onClickDownBt(){
  if (set == 1){time_s--; if (time_s<0){time_s=59;}}
  if (set == 2){time_m--; if (time_m<0){time_m=59;}}
  if (set == 3){time_h--; if (time_h<0){time_h=99;}}
  if (set == 4){set = 2;}
}

// code for waht to print in the first row of the lcd screen
void lcdRowOne(){
  lcd.setCursor(0,0);
  if (set == 0){lcd.print("      Timer     ");}
  if (set == 1){lcd.print("  Set Timer SS  ");}
  if (set == 2){lcd.print("  Set Timer MM  ");}
  if (set == 3){lcd.print("  Set Timer HH  ");}
}

// code for what to print in th second row of the lcd screen
void lcdRowTwo(){
  lcd.setCursor(4,1);
  if(time_h<=9){lcd.print("0");}
  lcd.print(time_h);
  lcd.print(":");
  if(time_m<=9){lcd.print("0");}
  lcd.print(time_m);
  lcd.print(":");
  if(time_s<=9){lcd.print("0");}
  lcd.print(time_s);
  lcd.print("  ");
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //initialize all the buttons
  pinMode(setBt,INPUT);
  pinMode(upBt,INPUT);
  pinMode(downBt,INPUT);
  pinMode(startBt,INPUT);
  // set lcd's columns and rows 


  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" Welcome To The ");
  lcd.setCursor(0,1);
  lcd.print("World Of Arduino");
  delay(1000);

  EEPROM_read();

  // setting up the timer for count_down
  tdown.setCounter(time_h,time_m,time_s, tdown.COUNT_DOWN, onComplete); 

  //seting an interval of 1000 milisecond(1 second)
  tdown.setInterval(print_time,1000);
  
  delay(1000);
  
  lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly:

  //reading the value of button
  newSetVal = digitalRead(setBt);
  newUpVal = digitalRead(upBt);
  newDownVal = digitalRead(downBt);
  newStartVal = digitalRead(startBt);

  tdown.run();
  
  if (oldSetVal == 0 && newSetVal == 1){
    onClickSetBt();
  }
  oldSetVal = newSetVal; 

  if (oldUpVal == 0 && newUpVal == 1){
    onClickUpBt();
  }
  oldUpVal = newUpVal;

  if (oldDownVal == 0 && newDownVal == 1){
    onClickDownBt();
  }
  oldDownVal = newDownVal;

  if (set == 0 && setFlag == 1){
    EEPROM_write();
    tdown.start();
    setFlag = 0;
  }
  Serial.print(time_s);
  lcdRowOne();
  lcdRowTwo();
}