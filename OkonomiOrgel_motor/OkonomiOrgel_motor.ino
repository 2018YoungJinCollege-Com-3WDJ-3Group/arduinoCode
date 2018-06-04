#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "FastLED.h"

Adafruit_PWMServoDriver oddServoModule= Adafruit_PWMServoDriver(0x41);  // odd servo
Adafruit_PWMServoDriver evenServoModule = Adafruit_PWMServoDriver();  // even servo

#define MIN_PULSE_WIDTH       650
#define MAX_PULSE_WIDTH       2350
#define DEFAULT_PULSE_WIDTH   1500 
#define FREQUENCY             50 // pwm setting
#define topAngle              110 // servos top angle
#define bottomAngle           70 // servos bottom angle
#define NUM_LEDS              20
#define NUM_STRIPS            8 // leds 8*20 160leds used


unsigned long previousTime = 0 ; // 오르골 연주시 필요한 시간
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

int ledColorSet[60][3] = {
 {255, 0, 0},
 {255, 69, 0},
 {255, 165, 0},
 {178, 34, 34},
 {255, 20, 147},
 {255, 105, 180},
 {255, 192, 203},
 {210, 105, 30},
 {199, 21, 133},
 {238, 232, 170},
 {218, 165, 32},
 {255, 215, 0},
 {255, 255, 224},
 {173, 255, 47},
 {127, 255, 0},
 {152, 251, 152},
 {0, 250, 154},
 {0, 255, 255},
 {224, 255, 255},
 {175, 238, 238},
 {255, 160, 122},
 {127, 255, 212},
 {64, 224, 208},
 {0, 206, 209},
 {100, 149, 237},
 {173, 216, 230},
 {135, 206, 250},
 {240, 248, 255},
 {0, 204, 255},
 {0, 191, 255},
 {30, 144, 255},
 {65, 105, 225},
 {106, 90, 205},
 {123, 104, 238},
 {138, 43, 226},
 {147, 112, 219},
 {148, 0, 211},
 {218, 112, 214},
 {199, 21, 133},
 {219, 112, 147},
 {255, 0, 255},
 {216, 191, 216},
 {238, 130, 238},
 {221, 160, 221},
 {211, 211, 211},
 {255, 250, 240},
 {255, 222, 173},
 {237, 164, 61},
 {255, 250, 205},
 {245, 222, 179},
 {245, 245, 220},
 {178, 34, 34},
 {255, 127, 80},
 {165, 42, 42},
 {255, 102, 0},
 {238, 232, 170},
 {173, 255, 47},
 {0, 250, 154},
 {255, 102, 0},
 {0, 206, 209},
 
};

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
  if(oneStart){ // 오르골 연주
    
    previousTime = millis();

    for(int readPoint = 0; readPoint < moveString.length(); readPoint++){
      if(moveString[readPoint] == 'r'){ // 딜레이 발생(박자)

        
      if(tempo <= 250){  //FastLED.show()라는 함수가 딜레이를 가지고 있음, 조금 더 효율적인 방법을 찾고있음.
        // led set 속도가 템포에 영향을 주지 않도록 조정
        for(int i=0; i<NUM_LEDS; i++){
          for(int j=0; j<NUM_STRIPS; j++){
            if(ledTempArr[j] != NULL){
              ledSetColor(j,i,ledTempArr[j][1],ledTempArr[j][0],ledTempArr[j][2]);
            }
          }
          FastLED.show();
        }
      }else{
        for(int i=0; i<=NUM_LEDS; i+=2){
          for(int j=0; j<NUM_STRIPS; j++){
            if(ledTempArr[j] != NULL){
              ledSetColor(j,i,ledTempArr[j][1],ledTempArr[j][0],ledTempArr[j][2]);
              ledSetColor(j,i+1,ledTempArr[j][1],ledTempArr[j][0],ledTempArr[j][2]);
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
        int ledStripTemp =random(65)%8;
        int ledColorSetTemp = random(60);
        ledTempArr[ledStripTemp][0] = ledColorSet[ledColorSetTemp][0];
        ledTempArr[ledStripTemp][1] = ledColorSet[ledColorSetTemp][1];
        ledTempArr[ledStripTemp][2] = ledColorSet[ledColorSetTemp][2];    
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
          if(i == 19){
            FastLED.show(); 
          }
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



