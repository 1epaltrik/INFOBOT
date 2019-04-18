#include <NewPing.h>//ultrasonic library
#define trig 9
#define echo 10
#define MAX_DISTANCE 400
NewPing sonar(trig, echo, MAX_DISTANCE);

boolean irLeft;
boolean irRight;
boolean irMid;
int PWML=0;
int PWMR=0;
boolean MR1;
boolean MR2;
boolean ML1;
boolean ML2;
int speedMax=170;
int speedMid=130;
int speedMin=120;
int err=0;
int lasterror=0;

void setup() {
  // setup:ir led
  pinMode(8,INPUT);
  pinMode(11,INPUT);
  pinMode(12,INPUT);
  pinMode(13,INPUT);
  // setup motor 
  pinMode(3,OUTPUT);//ENL
  pinMode(2,OUTPUT);//ML1
  pinMode(4,OUTPUT);//ML2
  pinMode(6,OUTPUT);//ENR
  pinMode(5,OUTPUT);//MR1
  pinMode(7,OUTPUT);//MR2
  //RGB
  pinMode(A0,OUTPUT);//B
  pinMode(A1,OUTPUT);//R
  
   MotorStop();
  delay(2000);
}

void loop() {
    CheckIr();
    if(err == 2){MotorStop(); MotorAlt();}
    else if(err == 1){MotorLeft();}
    else if(err == -1){MotorRight();}
    else if(err == 0){MotorForward();}
    else if(err == 3){MotorTurn();}
}

void CheckIr(){
  irLeft=digitalRead(8);
  irMid=digitalRead(11);
  irRight=digitalRead(12);
  if(irLeft == 0 and irRight == 0 and irMid == 1){
    err=0;
    lasterror=0;
  }else if(irLeft == 1 and irRight == 0){
    err=-1;
    lasterror=-1;
  }else if(irLeft == 0 and irRight == 1){
    err=1;
    lasterror=1;
  }else if(irLeft == 0 and irRight == 0 and irMid == 0){
    err=3;
  }
  if(sonar.ping_cm() < 50 and sonar.ping_cm() !=0){
    err=2;
  }
  delay(20);
}

void MotorTurn(){
  analogWrite(A0,1023);
  analogWrite(A1,1023);
  delay(500);
  if(lasterror<1){
  MR1=HIGH;
  MR2=LOW;
  ML1=HIGH;
  ML2=LOW;
  }else if(lasterror=1){
  MR1=LOW;
  MR2=HIGH;
  ML1=LOW;
  ML2=HIGH;
  }
  PWMR=speedMax;
  PWML=speedMax;    
  MotorSet();
  do{
    //CheckIr();
  }while(digitalRead(irMid) != 1);
  PWMR=speedMid;
  PWML=speedMax;    
  MotorSet();
  do{
     CheckIr();
  }while(err != 0);
}

void MotorForward(){
  analogWrite(A0,1023);
  analogWrite(A1,0);
  MR1=LOW;
  MR2=HIGH;
  ML1=HIGH;
  ML2=LOW;
  PWMR=speedMid;
  PWML=speedMax;
  MotorSet();
  do{
    CheckIr();
  }while(err == 0);
}

void MotorLeft(){
  analogWrite(A0,1023);
  analogWrite(A1,1023);
  PWMR=speedMid;
  PWML=0;   
  MotorSet();
  do{
    CheckIr();
  }while(err == 1);
  delay(200);
}

void MotorRight(){
  analogWrite(A0,1023);
  analogWrite(A1,1023);
  PWMR=0;
  PWML=speedMax;    
  MotorSet();
  do{
    CheckIr();
  }while(err == -1);
  delay(200);
}

void MotorSet(){
  MotorStop();
  digitalWrite(2,ML1);
  digitalWrite(4,ML2);
  digitalWrite(5,MR1);
  digitalWrite(7,MR2);
  analogWrite(3,PWML);
  analogWrite(6,PWMR);
}

void MotorStop(){
  analogWrite(3,0);
  analogWrite(6,0);
  digitalWrite(2,LOW);
  digitalWrite(4,LOW);
  digitalWrite(5,LOW);
  digitalWrite(7,LOW);
}

void MotorAlt(){
  analogWrite(A0,0);
  for(int i=0; i<15; i++){
   analogWrite(A1,1023);
   delay(250);
   analogWrite(A1,0);
   delay(250);
  }
}
