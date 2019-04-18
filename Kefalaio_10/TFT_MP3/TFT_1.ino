// UTFT_ViewFont 
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/


#include <UTFT.h>
#include <avr/pgmspace.h>
#include <DHT.h>
#include <DS3231.h>
#include <BY8001.h>
#include <SoftwareSerial.h>
// Arduino Mega:
// -------------------
// Standard Arduino Mega/Due shield            : <display model>,38,39,40,41
// CTE TFT LCD/SD Shield for Arduino Mega      : <display model>,38,39,40,41
//
UTFT myGLCD(CTE40,38,39,40,41);
DHT dht(2, DHT22); //Temp-Humidity Sensor
DS3231  rtc(SDA, SCL); //Arduino Pins
#define trigPin 4 //Ultrasonic
#define echoPin 3 //Ultrasonic
SoftwareSerial mp3Serial(6, 5);  // RX, TX

//Read external font files
extern uint8_t Calibri32x64GR[]; 
extern uint8_t Calibri24x32GR[];
extern unsigned int logosmall[0x400];

int flag=0;
int flagU=0;
double setUpdate=0;
String CurDate;
String OldDate;
String DayName;
String DayDate;
String MonthDate;
String YearDate;
float duration, distance;
Time  t;
BY8001 mp3; // creating an instance of class BY8001 and call it 'mp3'

void setup()
{
  myGLCD.InitLCD(); 
  myGLCD.clrScr();
  myGLCD.fillScr(255,255,255); 
  dht.begin();
  rtc.begin();
  
  rtc.setDOW(MONDAY);     // Set Day-of-Week to SUNDAY
  rtc.setTime(23, 58, 0);     // Set the time to 12:00:00 (24hr format)
  rtc.setDate(31, 12, 2019);   // Set the date to January 1st, 2014

  Serial.begin(9600);  // set serial monitor baud rate to Arduino IDE
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  mp3Serial.begin(9600);  // BY8001 set to 9600 baud (required) 
  mp3.setup(mp3Serial); // tell BY8001 library which serial port to use. 
  delay(2000);  // allow time for BY8001 cold boot; may adjust depending on flash storage size
  mp3.setVolume(30);
  //int by8001_VolumeSetting = 15; //  Set volume here to 15 (0-30 range) if not already set
  //if ((int)mp3.getVolumeSetting() != by8001_VolumeSetting) mp3.setVolume(by8001_VolumeSetting);
  myGLCD.setFont(Calibri32x64GR);
}

void loop()
{
  if(flag == 0){
    setBackground();
    checkChanges();
    setUpdate=millis();
    }
 if(setUpdate+5000 < millis()){
   checkChanges();
   TempHumSet();
   setUpdate=millis();
 }
  Clock();
  if(flagU == 0){  
    Ultrasonic();
    }
  else if(flagU == 1){
      mp3.play();
      delay(50);
      flagU=0;
    }
}

void Ultrasonic(){
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) * 0.0344;
  if (distance <= 20 and distance !=0){flagU=1;}
}

void checkChanges(){
  CurDate=rtc.getDateStr();
  if(CurDate != OldDate){
    DateSet();
    OldDate=CurDate;
  }
  
  String dowDate=rtc.getDOWStr();
  if(dowDate != DayName){
    DayName=dowDate;
    flag=2;
    }
}

void setBackground(){
  myGLCD.setBackColor(255, 255, 255);
  myGLCD.setFont(Calibri32x64GR);
  myGLCD.setColor(255, 0, 0);
  myGLCD.print("@",10, 170);
  myGLCD.setColor(0, 100, 255);
  myGLCD.print("Q", 435, 170);
  myGLCD.drawBitmap (140, 250,200 , 67, logosmall);
  flag=1;
}
void DateSet(){
  DayDate=CurDate.substring(0,2);
  MonthDate=CurDate.substring(3,5);
  YearDate=CurDate.substring(6,10);
  DayName=rtc.getDOWStr();
  if(DayName == "Monday"){DayName="DEYTERA";}
  else if(DayName == "Tuesday"){DayName="TRITH";}
  else if(DayName == "Wednesday"){DayName="TETARTH";}
  else if(DayName == "Thursday"){DayName="PEMPTH";}
  else if(DayName == "Friday"){DayName="PARASKEYH";}
  else if(DayName == "Saturday"){DayName="SABBATO";}
  else if(DayName == "Sunday"){DayName="KYRIAKH";}
 
  if(MonthDate == "01"){MonthDate="IANOYARIOY";}
  else if(MonthDate == "02"){MonthDate="FEBROYARIOY";}
  else if(MonthDate == "03"){MonthDate="MARTIOY";}
  else if(MonthDate == "04"){MonthDate="APRILIOY";}
  else if(MonthDate == "05"){MonthDate="MAIOY";}
  else if(MonthDate == "06"){MonthDate="IOYNIOY";}
  else if(MonthDate == "07"){MonthDate="IOYLIOY";} 
  else if(MonthDate == "08"){MonthDate="AYGOYSTOY";} 
  else if(MonthDate == "09"){MonthDate="SEPTEMBRIOY";} 
  else if(MonthDate == "10"){MonthDate="OKTVBRIOY";} 
  else if(MonthDate == "11"){MonthDate="NOEMBRIOY";} 
  else if(MonthDate == "12"){MonthDate="DEKEMBRIOY";} 
  
  myGLCD.setFont(Calibri24x32GR);
  
  myGLCD.setColor(0, 0, 0);
  myGLCD.print(" >"+DayName+"< ", CENTER, 10);
  
  myGLCD.setColor(0, 0, 0);
  myGLCD.print("                  ", CENTER, 130); 
  myGLCD.print(MonthDate+" "+YearDate, CENTER, 130); 
    
  myGLCD.setFont(Calibri32x64GR);
  myGLCD.setColor(0, 0, 255);
  myGLCD.print("  "+DayDate+"  ", CENTER, 65); 
}

void Clock(){
  String timeDate=rtc.getTimeStr();
  myGLCD.setFont(Calibri32x64GR);
  myGLCD.setColor(133, 23, 120);
  myGLCD.print("  "+timeDate+"  ", CENTER, 180);
}

void TempHumSet(){
  delay(200);
  int h = dht.readHumidity();
  int t = dht.readTemperature();
  myGLCD.setFont(Calibri32x64GR);
  myGLCD.setColor(255, 0, 0);
  myGLCD.print(" "+String(t)+"W ", -20, 240);
  myGLCD.setColor(0, 100, 255);
  myGLCD.print(" ^"+String(h)+" ", 340, 240);
}
