#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver oddServoModule= Adafruit_PWMServoDriver(); 
Adafruit_PWMServoDriver evenServoModule = Adafruit_PWMServoDriver(0x41); 

#define MIN_PULSE_WIDTH       650
#define MAX_PULSE_WIDTH       2350
#define DEFAULT_PULSE_WIDTH   1500
#define FREQUENCY             50

unsigned long previousTime =0; // 오르골 연주시 필요한 시간
unsigned long currentTime = 0; // 연주중 필요한 시간
unsigned long tempo = 0; // 동작에 맞는 템포
String tempoString = "";
boolean oneStart = false;
boolean tempoGet = true;
String moveString = ""; // 블루투스로 동작 문자열 한번에 받기
int servoCount[30]; //서보모터 위치 초기화

void setup() 
{
  oddServoModule.begin();
  oddServoModule.setPWMFreq(FREQUENCY);
  evenServoModule.begin();
  evenServoModule.setPWMFreq(FREQUENCY);
  for(int i = 0; i< 15; i++){ // 모터위치 초기화
    oddServoModule.setPWM(i,0,pulseToAngle(110));
    evenServoModule.setPWM(i,0,pulseToAngle(110)); 
  }
  for(int i =0; i<30; i++){ // 각 서보모터의 빈도수 초기화
    servoCount[i] = 0;
  }
}
void loop() { 
  
  if(Serial1.available()){ // 블루투스 값 있을시
      BTRate(); // 블루투스 값 전부 받는 함수      
  }
  if(oneStart){ // 오르골 연주
    
    previousTime = millis();

    for(int readPoint = 0; readPoint < moveString.length(); readPoint++){
      if(moveString[readPoint] == 'r'){ // 딜레이
        
        while(1){   
          
          currentTime = millis();
          
          if(currentTime - previousTime >= tempo){
            previousTime = currentTime;
            break;
          }
          
        }
        
      }else{ // 모터로 오르골 연주
        moveServoMotor(mxoveString[readPoint].toInt());
  
      }

      
    }
    
  }
  oneStart = false;
  moveString = ""; // 문자열 초기화
  tempoGet = true;
  tempo = 0;
  tempoString = "";
  
}


int pulseToAngle(int angle)
{
  int pulse_wide, analog_value;
  pulse_wide   = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
  return analog_value;
}


void moveServoMotor(int moveChar){
  int moveServoPwmPin;
  switch(moveChar){
    case 65: moveServoPwmPin = 1;
    break;
    case 67: moveServoPwmPin = 2;
    break;
    case 72: moveServoPwmPin = 3;
    break;
    case 74: moveServoPwmPin = 4;
    break;
    case 76: moveServoPwmPin = 5;
    break;
    case 77: moveServoPwmPin = 6;
    break;
    case 79: moveServoPwmPin = 7;
    break;
    case 103: moveServoPwmPin = 29;
    break;
    case 105: moveServoPwmPin = 30;
    break;
    default: moveServoPwmPin = moveChar - 73;
    break;
  }
  if(moveServoPwmPin%2 == 1){
    
    if(servoCount[moveServoPwmPin] % 2 == 0 ){
      oddServoModule.setPWM(((moveServoPwmPin+1)/2).toInt(),0,pulseToAngle(70));
    }else{
      oddServoModule.setPWM(((moveServoPwmPin+1)/2).toInt(),0,pulseToAngle(110));
    }
    servoCount[moveServoPwmPin]++;
    
  }else{
    
    if(servoCount[moveServoPwmPin] % 2 == 0 ){
      evenServoModule.setPWM(((moveServoPwmPin)/2).toInt(),0,pulseToAngle(70));
    }else{
      evenServoModule.setPWM(((moveServoPwmPin)/2).toInt(),0,pulseToAngle(110));
    }
    servoCount[moveServoPwmPin]++;
    
  }
  
}


void BTRate(){
  char data = (char)Serial1.read(); // 데이터를 char 형식으로 받는다.

  if(tempoGet){
    if(data == ';'){
      tempoGet = false;
      if(tempoString.toInt() < 60 || tempoString.toInt() > 400){ //템포의 최소값과 최대값 지정
        tempoString = 120;
      }
      tempo = 60000/tempoString.toInt();
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

