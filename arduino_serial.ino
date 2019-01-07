#include <SoftwareSerial.h>
#include <Servo.h>


SoftwareSerial HM10(2, 3); //HM10(Receive Pin, Transmit Pin)
String msg = "";


Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0; 

void setup() {
  Serial.begin(9600);
  HM10.begin(9600);
  myservo.attach(10);
  pos=0;
  myservo.write(pos);
}

void loop() {
  if ( HM10.available() ) {
    msg = "";
//unsigned long StartTime = millis();


    while (HM10.available()){
      char lu = HM10.read();
      if (lu == '$') {
        break;
      }
      else {
      msg=msg+lu;
      }
      delay(1);
    }
    pos=msg.toInt();
    myservo.write(pos);
//    unsigned long CurrentTime = millis();
//    unsigned long ElapsedTime = CurrentTime - StartTime;
    Serial.println(msg);
    
  } 
  delay(10);
}
