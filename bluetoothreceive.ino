// 도어락 with 12v 솔레노이드
// 블루투스로 신호를 받으면 문을 연다

#include <SoftwareSerial.h>
int relay_pin = 7;
SoftwareSerial mySerial(2, 3); //블루투스의 Tx, Rx핀을 2번 3번핀으로 설정


char key;

void setup() {
  // 시리얼 통신의 속도를 9600으로 설정
  Serial.begin(9600);
  while (!Serial) {
    ; //시리얼통신이 연결되지 않았다면 코드 실행을 멈추고 무한 반복
  }

  pinMode( relay_pin , OUTPUT);

  Serial.println("Hello World!");

  //블루투스와 아두이노의 통신속도를 9600으로 설정
  mySerial.begin(9600);
}

void loop() { //코드를 무한반복합니다.
  if (mySerial.available()) { //블루투스에서 넘어온 데이터가 있다면
    key = mySerial.read();
    if(key == 'a'){
    digitalWrite( relay_pin , HIGH);
    delay(5000);
    digitalWrite(relay_pin , LOW);
    delay(1000);
  }
}
}
