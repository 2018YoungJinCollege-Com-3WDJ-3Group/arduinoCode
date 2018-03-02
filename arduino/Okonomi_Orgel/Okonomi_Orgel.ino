unsigned long previousTime =0; // 오르골 연주시 필요한 시간
unsigned long currentTime = 0; // 연주중 필요한 시간
unsigned long tempo = 0; // 동작에 맞는 템포

String tempoString = "";
boolean oneStart = false;
boolean resetSolenoid = true;
boolean tempoGet = true;
String moveString = ""; // 블루투스로 동작 문자열 한번에 받기
String previousChars = ""; // 이전에 동작한 솔레노이드를 초기화 시키기 위한 char array


void setup ()
{
  Serial.begin(9600); // 시리얼 시작
  Serial1.begin(9600); // 블루투스 시작
  for(int i = 23; i < 54; i++){
    pinMode(i,OUTPUT); 
  } // 릴레이 포트 지정( 솔레노이드 동작 핀 )
}

void loop ()
{
  
  if(Serial1.available()){ // 블루투스 값 있을시
    
      BTRate(); // 블루투스 값 전부 받는 함수      
      
  }
  
  if(oneStart){ // 오르골 연주 시작
    digitalWrite(23,HIGH); // 모터 동작
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
        
        moveSolenoidByChar((int)moveString[readPoint]);
        
      }
      
    }

    digitalWrite(23,LOW); // 모터 멈춤
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
void stopSolenoidByChar(int num){
  
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


void BTRate(){

  char data = (char)Serial1.read();
  if(tempoGet){

    if(data == ';'){
      tempoGet = false;
      if(tempoString.toInt() == 0){
        tempoString = 1000;
      }
      tempo = 60000/tempoString.toInt(); // 템포 60BPM = 1분에 60음 60/템포 * 1000;
      }else{
      tempoString += data;
    }
    
  }else{

       if (data == 'n'){
    
         oneStart = true; 
    
       }else{
    
         moveString +=data; // moveString 에 받은 데이터 저장
    
       }
 
  }
  
  
}


