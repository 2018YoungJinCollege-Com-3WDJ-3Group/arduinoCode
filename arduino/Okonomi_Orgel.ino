unsigned long previousTime =0; // 오르골 연주시 필요한 시간
unsigned long currentTime = 0; // 연주중 필요한 시간
unsigned long tempo = 1000; // 동작에 맞는 템포 (추후 보강)
boolean oneStart = false;
boolean resetSolenoid = true;
String moveString = ""; // 블루투스로 동작 문자열 한번에 받기
String previousChars = ""; // 이전에 동작한 솔레노이드를 초기화 시키기 위한 char array


void setup ()
{
  Serial.begin(9600); // 시리얼 시작
  Serial1.begin(9600); // 블루투스 시작
  for(int i = 24; i < 54; i++){
    pinMode(i,OUTPUT); 
  } // 릴레이 포트 지정( 솔레노이드 동작 핀 )
}

void loop ()
{
  
  if(Serial1.available()){ // 블루투스 값 있을시
    
    BTRate(); // 블루투스 값 전부 받는 함수
    
  }
  if(oneStart){ // 오르골 연주 시작
    
    previousTime = millis(); // 시작 시간 나중에 시간을 계속 가져옴
    
    for(int i = 0;i < moveString.length();i++ ){  // 문자열 읽기  
      
      
      if(moveString[i] == 'r'){ // 문자중 중간중간 박자를 넣어줄 r문자가 오면 딜레이 발생
        
      resetSolenoid = true;
        
       while(1){
        currentTime = millis(); // 현재 시간 계속 불러옴
        
        if(currentTime - previousTime == tempo/2 && resetSolenoid){ // 현재시각 - 오르골 시작시간 이템포의 1/2가 되면 솔레노이드 초기화 한번만 실행을 위한 resetSolenoid

          for(int j = 0; j < previousChars.length( ); j++){
            
            stopSolenoidByChar( (int)previousChars[j] );
            
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
        
        moveSolenoidByChar((int)moveString[i]);
        
      }
      
    }

    
    oneStart = false; // 다시 값 받을 준비
    moveString = ""; // 문자열 초기화
  }

}

void moveSolenoidByChar(int num){
  Serial.print("동작 솔레노이드 포트: ");
  Serial.println( num-41 );
  previousChars += (char)num; // 초기화를 위한 변수에 넣어줌
  digitalWrite( num-41 ,HIGH); // 솔레노이드 동작
}
void stopSolenoidByChar(int num){
  Serial.print("이전 사용되고 초기화 될 솔레노이드 포트: ");
  Serial.println( num-41 );
  digitalWrite(num-41, LOW);
}


void BTRate(){

  char data = (char)Serial1.read();
  if (data == 'n'){
    oneStart = true; 
  }else{
    moveString +=data; // moveString 에 받은 데이터 저장
  }
 
  
}


