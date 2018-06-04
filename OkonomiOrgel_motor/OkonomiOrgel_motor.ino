#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "FastLED.h"

Adafruit_PWMServoDriver oddServoModule= Adafruit_PWMServoDriver(0x41); 
Adafruit_PWMServoDriver evenServoModule = Adafruit_PWMServoDriver(); 

#define MIN_PULSE_WIDTH       650
#define MAX_PULSE_WIDTH       2350
#define DEFAULT_PULSE_WIDTH   1500
#define FREQUENCY             50
#define topAngle              110
#define bottomAngle           70
#define NUM_LEDS              20
#define NUM_STRIPS            8


unsigned long previousTime =0; // 오르골 연주시 필요한 시간
unsigned long currentTime = 0; // 연주중 필요한 시간
float tempo = 0; // 동작에 맞는 템포
String tempoString = "";
boolean oneStart = false;
boolean tempoGet = true;
boolean realTimePlay = false;
String moveString = ""; // 블루투스로 동작 문자열 한번에 받기
int servoCount[30]; //서보모터 위치 초기화
CRGB leds[NUM_STRIPS][NUM_LEDS];
int ledTempArr[8][3];

void setup() 
{
  Serial.begin(9600);
  Serial1.begin(9600);
  oddServoModule.begin();
  oddServoModule.setPWMFreq(FREQUENCY); 
  evenServoModule.begin();
  evenServoModule.setPWMFreq(FREQUENCY); // 모터 초기화
  FastLED.addLeds<WS2813,3 >(leds[0], NUM_LEDS);
  FastLED.addLeds<WS2813,4 >(leds[1], NUM_LEDS);
  FastLED.addLeds<WS2813,5 >(leds[2], NUM_LEDS);
  FastLED.addLeds<WS2813,6 >(leds[3], NUM_LEDS);
  FastLED.addLeds<WS2813,7 >(leds[4], NUM_LEDS);
  FastLED.addLeds<WS2813,8 >(leds[5], NUM_LEDS);
  FastLED.addLeds<WS2813,9 >(leds[6], NUM_LEDS);
  FastLED.addLeds<WS2813,10 >(leds[7], NUM_LEDS); // LED 초기화
  
  for(int i = 0; i< 15; i++){ // 모터위치 초기화
    oddServoModule.setPWM(i,0,pulseToAngle(110));
    evenServoModule.setPWM(i,0,pulseToAngle(110));
    delay(100);
  }
  for(int i =0; i<30; i++){ // 각 서보모터의 빈도수 초기화
    servoCount[i] = 0;
  }
  for(int i=0; i<NUM_STRIPS;i++){ // LED 불 전부 끄기
    for(int j=0; j<NUM_LEDS;j++){
        ledSetColor(i,j,0,0,0);
         FastLED.show();
    }
  }
  
}
void loop() {
  if(Serial1.available()){ // 블루투스 값 있을시
      BTRate(); // 블루투스 값 전부 받는 함수      
  }
  if(oneStart){                                 // 오르골 연주
    
    previousTime = millis();

    for(int readPoint = 0; readPoint < moveString.length(); readPoint++){
      if(moveString[readPoint] == 'r'){ // 딜레이

        
      if(tempo <= 250){
        for(int i=0; i<NUM_LEDS; i++){
          for(int j=0; j<NUM_STRIPS; j++){
            if(ledTempArr[j] != NULL){
              ledSetColor(j,i,ledTempArr[j][0],ledTempArr[j][1],ledTempArr[j][2]);
            }
          }
          FastLED.show();
        }
      }else{
        for(int i=0; i<=NUM_LEDS; i+=2){
          for(int j=0; j<NUM_STRIPS; j++){
            if(ledTempArr[j] != NULL){
              ledSetColor(j,i,ledTempArr[j][0],ledTempArr[j][1],ledTempArr[j][2]);
              ledSetColor(j,i+1,ledTempArr[j][0],ledTempArr[j][1],ledTempArr[j][2]);
            }
          }
          FastLED.show();
        }
        
      }
      
        while(1){
          currentTime = millis();

          if(currentTime - previousTime >=tempo){
            previousTime = currentTime;
            int ledTempArr[8][3];
            break;
          }
        }
        
        
      }else{ // 모터로 오르골 연주
        moveServoMotor((int)moveString[readPoint]);
        int ledStripTemp =random(64)%8;
        ledTempArr[ledStripTemp][0] = random(16)*random(16);
        ledTempArr[ledStripTemp][1] = random(16)*random(16);
        ledTempArr[ledStripTemp][2] = random(16)*random(16);        
      }

      
    }
  oneStart = false;
  moveString = ""; // 문자열 초기화
  tempoGet = true;
  tempo = 0;
  tempoString = "";
  for(int i=0; i<NUM_STRIPS;i++){ // LED 불 전부 끄기
    for(int j=0; j<NUM_LEDS;j++){
        ledSetColor(i,j,0,0,0);
         FastLED.show();
    }
  }
}
}


int pulseToAngle(int angle)
{
  int pulse_wide, analog_value;
  pulse_wide   = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
  return analog_value;
}


void moveServoMotor(int moveChar){ // 모터 동작
  int moveServoPwmPin;
  switch(moveChar){
    case 65: moveServoPwmPin = 0;
    break;
    case 67: moveServoPwmPin = 1;
    break;
    case 72: moveServoPwmPin = 2;
    break;
    case 74: moveServoPwmPin = 3;
    break;
    case 76: moveServoPwmPin = 4;
    break;
    case 77: moveServoPwmPin = 5;
    break;
    case 79: moveServoPwmPin = 6;
    break;
    case 103: moveServoPwmPin = 28;
    break;
    case 105: moveServoPwmPin = 29;
    break;
    default: moveServoPwmPin = moveChar - 74;
    break;
  }
  if(moveServoPwmPin%2 == 1){
    
    if(servoCount[moveServoPwmPin] % 2 == 0 ){
      oddServoModule.setPWM((int)(moveServoPwmPin/2),0,pulseToAngle(bottomAngle));
    }else{
      oddServoModule.setPWM((int)(moveServoPwmPin/2),0,pulseToAngle(topAngle));
    }
    servoCount[moveServoPwmPin]++;
    
  }else{
    
    if(servoCount[moveServoPwmPin] % 2 == 0 ){
      evenServoModule.setPWM((int)((moveServoPwmPin)/2),0,pulseToAngle(bottomAngle));
    }else{
      evenServoModule.setPWM((int)((moveServoPwmPin)/2),0,pulseToAngle(topAngle));
    }
    servoCount[moveServoPwmPin]++;
  }
}


void BTRate(){
  char data = (char)Serial1.read(); // 데이터를 char 형식으로 받는다.

  if(data == '('){
    
    realTimePlay = true;
    
  }

  
  if(realTimePlay){
  
      if(data == ')'){
        
        realTimePlay = false;
        
      }else{
        moveServoMotor((int)data);

        int ledStripTemp =random(64)%8;
        ledTempArr[ledStripTemp][0] = random(16)*random(16);
        ledTempArr[ledStripTemp][1] = random(16)*random(16);
        ledTempArr[ledStripTemp][2] = random(16)*random(16);
        for(int i=0; i<NUM_LEDS; i++){
          for(int j=0; j<NUM_STRIPS; j++){
            if(ledTempArr[j] != NULL){
              ledSetColor(j,i,ledTempArr[j][0],ledTempArr[j][1],ledTempArr[j][2]);
            }
          }
          FastLED.show();
        }
      }
    
  }else{                                                                               

    
       if(tempoGet){
        if(data == ';'){
          tempoGet = false;
          if(tempoString.toInt() < 60 || tempoString.toInt() > 400){ //템포의 최소값과 최대값 지정
            tempoString = 120;
          }
          tempo = 30000/tempoString.toInt();
        }else{ 
          tempoString += data;
        }  
      }else{
        if(data == 'n'){
          oneStart = true;
        }else{
          moveString += data;
        }
      }  
    
  }
}
void ledSetColor(int strip,int led, int red, int green, int blue){
  leds[strip][led].r = red;
  leds[strip][led].g = green;
  leds[strip][led].b = blue;
} 



