unsigned long previousTime =0; // 오르골 연주시 필요한 시간
unsigned long currentTime = 0; // 연주중 필요한 시간
unsigned long tempo = 0; // 동작에 맞는 템포

unsigned long CharsCreatedTime[105];

String tempoString = "";
boolean oneStart = false;
boolean resetSolenoid = true;
boolean tempoGet = true;
boolean realTimePlay = false;
String moveString = ""; // 블루투스로 동작 문자열 한번에 받기
String previousChars = ""; // 이전에 동작한 솔레노이드를 초기화 시키기 위한 char array


void setup ()
{
  Serial.begin(9600); // 시리얼 시작
  Serial1.begin(9600); // 블루투스 시작
  for(int i = 23; i < 54; i++){
    pinMode(i,OUTPUT); 
    digitalWrite(i,HIGH);
  } // 릴레이 포트 지정( 솔레노이드 동작 핀 )
}

void loop ()
{
  if(Serial.available()){
    Serial1.write(Serial.read());
    Serial.write(Serial.read());
    
  }
  
  if(Serial1.available()){ // 블루투스 값 있을시
    
      BTRate(); // 블루투스 값 전부 받는 함수      
      
  }
  if(realTimePlay){ // 실시간 모바일 연주시 일정 시간에 따라서 솔레노이드를 초기화 시켜주는 부분
    
    for(int deletePoint = 0; deletePoint < previousChars.length(); deletePoint++){
      
          if(CharsCreatedTime[(int)previousChars[deletePoint]] + 50 <= millis() ){
            
            stopSolenoidByChar((int)previousChars[deletePoint]);
            CharsCreatedTime[(int)previousChars[deletePoint]] = 0;
            previousChars[deletePoint] = "";
      
          }
       
    }      
  }

  if(oneStart){ // 오르골 연주 시작
    
    digitalWrite(23,LOW); // 모터 동작
    previousChars ="";
    previousTime = millis(); // 시작 시간 나중에 시간을 계속 가져옴

    for(int readPoint = 0;readPoint < moveString.length(); readPoint++ ){  // 문자열 읽기  
      
      
      if(moveString[readPoint] == 'r'){ // 문자중 중간중간 박자를 넣어줄 r문자가 오면 딜레이 발생
        
      resetSolenoid = true;
        
       while(1){
        currentTime = millis(); // 현재 시간 계속 불러옴
        
        if(currentTime - previousTime == (int)tempo/2 && resetSolenoid){ // 현재시각 - 오르골 시작시간 이템포의 1/2가 되면 솔레노이드 초기화 한번만 실행을 위한 resetSolenoid

          for(int deletePoint = 0; deletePoint < previousChars.length(); deletePoint++){
            
            stopSolenoidByChar((int)previousChars[deletePoint]);
            
          }
          
          previousChars ="";
          
          resetSolenoid = false;
       
          }
         
        if(currentTime - previousTime >= tempo){ // 템포에 맞게 솔레노이드 delay
          
          previousTime = currentTime;
          
          break;
           
         }
         
        }
        
      }else{
        
        moveSolenoidByChar((int)moveString[readPoint]); // 솔레노이드 동작
        
      }
      
    }

    digitalWrite(23,HIGH); // 모터 멈춤   
    oneStart = false; // 다시 값 받을 준비 
    moveString = ""; // 문자열 초기화
    tempoGet = true;
    tempo = 0;
    tempoString = "";
    
  }

}

void moveSolenoidByChar(int num){

  previousChars += (char)num; // 초기화를 위한 변수에 넣어줌
  
        switch(num){
    case 65: digitalWrite(24,LOW);
    break;
    case 67: digitalWrite(25,LOW);
    break;
    case 72: digitalWrite(26,LOW);
    break;
    case 74: digitalWrite(27,LOW);
    break;
    case 76: digitalWrite(28,LOW);
    break;
    case 77: digitalWrite(29,LOW);
    break;
    case 79: digitalWrite(30,LOW);
    break;
    case 103: digitalWrite(52,LOW);
    break;
    case 105: digitalWrite(53,LOW);
    break;
    default: digitalWrite(num-50,LOW);
    break;
  }
  
}
void stopSolenoidByChar(int num){
  
   switch(num){
    case 65: digitalWrite(24,HIGH);
    break;
    case 67: digitalWrite(25,HIGH);
    break;
    case 72: digitalWrite(26,HIGH);
    break;
    case 74: digitalWrite(27,HIGH);
    break;
    case 76: digitalWrite(28,HIGH);
    break;
    case 77: digitalWrite(29,HIGH);
    break;
    case 79: digitalWrite(30,HIGH);
    break;
    case 103: digitalWrite(52,HIGH);
    break;
    case 105: digitalWrite(53,HIGH);
    break;
    default: digitalWrite(num-50,HIGH);
    break;
  }
  
}


void BTRate(){

  char data = (char)Serial1.read(); // 데이터를 char 형식으로 받는다.
  

  if(data == '('){
    
    realTimePlay = true;
    digitalWrite(23,LOW); // 모터 동작

    
  }
  
  
  if(realTimePlay){ // 모바일 리얼타임 연주 라면 아래의 코드 아니라면 else 코드가 실행
    
    if( (int)data <=105 &&(int)data >= 65){
      
      CharsCreatedTime[(int)data] = millis();
      
      moveSolenoidByChar((int)data);
          
    }
   

    }else{
      
      
        if(tempoGet){ // 템포를 받음
                
          if(data == ';'){
                        
            tempoGet = false;
                       
            if(tempoString.toInt() < 60){
              
              tempoString = 120;
              
            }

            tempo = 60000/tempoString.toInt(); // 템포 60BPM = 1분에 60음 60/템포 * 1000;
            
            }else{
              
            tempoString += data;
          
          }
          
        }else{ // 문자열을 받음
      
             if (data == 'n'){
          
               oneStart = true; 
          
             }else{
          
               moveString += data; // moveString 에 받은 데이터 저장
          
             }
       
    }
    
    
  }

    if(data == ')'){
    
    realTimePlay = false;
    digitalWrite(23,HIGH); // 모터 멈추기
    
  }





  
  
  
}


