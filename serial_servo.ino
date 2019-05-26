// 날씨 API에 의해 완성된 신호가 라즈베리에서 넘어옴. -> 이제 블루투스로 받아 우산이나 마스크함 작동


#include <Servo.h> //서보 라이브러리를 불러옵니다.
#include <SoftwareSerial.h>
 
Servo myservo;  // 서보를 제어할 서보 오브젝트를 만듭니다.
Servo myservo2;  // 서보를 제어할 서보 오브젝트를 만듭니다.
SoftwareSerial mySerial(2,3); //블루투스의 Tx, Rx핀을 2번 3번핀으로 설


void setup() {

Serial.begin(9600);
myservo.attach(9);  // 핀 9의 서보를 서보 오브젝트에 연결합니다.
myservo2.attach(10);  // 핀 9의 서보를 서보 오브젝트에 연결합니다.
Serial.println("Hello");

mySerial.begin(9600);

}

int pos = 0;     // 서보 위치를 저장할 변수를 선언합니다.
char key;
int endangle = 90;
int endangle2 = 90;

void loop() { 

  myservo.write(0);
  myservo2.write(180);

  if( mySerial.available() ){
    key = mySerial.read();
    if( key == 'b'){
      for(pos = 0; pos < endangle; pos += 1) 
      {                                  // 이동할때 각도는 1도씩 이동합니다. 
        myservo.write(pos);              // 'pos'변수의 위치로 서보를 이동시킵니다.
        delay(15);                       
      }

      delay(2000);
      
      for(pos = endangle; pos>=0; pos-=1)    
      {                                
        myservo.write(pos);              // 서보를 반대방향으로 이동합니다.
        delay(15);                     
      } 
    }

    

    else if( key == 'c'){
      for(pos = 180; pos >= endangle2; pos -= 1)
      {                                  // 이동할때 각도는 1도씩 이동합니다. 
        myservo2.write(pos);              // 'pos'변수의 위치로 서보를 이동시킵니다.
        delay(15);                   
      }

      delay(5000);
      
      for(pos = endangle2; pos<=180; pos+=1)    
      {                                
        myservo2.write(pos);              // 서보를 반대방향으로 이동합니다.
        delay(15);                      
      } 
    }
   
  }
}



//출처: https://studymake.tistory.com/77?category=645699 [스터디메이크]
