
// 라즈베리 파이에서 시리얼 통신으로 들어온 신호를 블루투스로 다른 아두이노로 보내는 코드


#include <SoftwareSerial.h>

SoftwareSerial mySerial(4,5); //블루투스의 Tx, Rx핀을 2번 3번핀으로 설정 우산 마스크
SoftwareSerial mySerial2(2,3); //블루투스의 Tx, Rx핀을 2번 3번핀으로 설정 도어락

char key;

void setup() {
  // 시리얼 통신의 속도를 9600으로 설정
  Serial.begin(9600);
  while (!Serial) {
    ; //시리얼통신이 연결되지 않았다면 코드 실행을 멈추고 무한 반복
  }


  Serial.println("Hello");

  //블루투스와 아두이노의 통신속도를 9600으로 설정
  mySerial.begin(9600);
  mySerial2.begin(9600);
}

void loop() { //코드를 무한반복합니다.
  
  if (Serial.available()) {    //시리얼모니터에 입력된 데이터가 있다면
    key = Serial.read();
    if(key == 'a'){
    mySerial2.write(key);  //블루투스를 통해 입력된 데이터 전달
  }
  if(key == 'b'){
    mySerial.write(key);  //블루투스를 통해 입력된 데이터 전달
  }
  if(key == 'c'){
    mySerial.write(key);  //블루투스를 통해 입력된 데이터 전달
  }
}
}
