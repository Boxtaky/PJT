
#include <SoftwareSerial.h>
#include <Servo.h>
//#include <AFMotor.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>



// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #1 (M1 and M2)
Adafruit_StepperMotor *steph = AFMS.getStepper(200, 1);

//HM10 module works as a Serial
SoftwareSerial HM10(2, 3); //HM10(Receive Pin, Transmit Pin)
String msg = "";


Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

//creation of servos variable
Servo servo_oeuil_x;
Servo servo_oeuil_y;
Servo servo_bouche;
Servo serrvo_paupierre_sup;
Servo serrvo_paupierre_inf;


int new_step_pos =0;
int last_step_pos=0;
int pos = 0; 
float pos_x = 0.0;
float pos_y = 0.0;

String x ="";

String y ="";


attachInterrupt(digitalPinToInterrupt(2), ISR, CHANGE);


void setup() {
  Serial.begin(9600);
  HM10.begin(9600);
  // all servos are not attached yet because we dont know witch pins will be availibles
  servo_oeuil_x.attach(9);
  servo_oeuil_y.attach(10);
  
  pos=0;
  steph->setSpeed(1000);
  steph->release();//release put the stepper at his pos 0
  steph->step(1000, FORWARD, DOUBLE); //just for testing
  //myservo.write(pos);
}
void command(String msg){ //command is the function that will decide the actions to take
  char command_info = msg.charAt(0);
  switch (command_info) {//depending on the first character, we select the action
    case 'y' : //movement of the eyes
      Serial.println(msg+"===========");
      msg.remove(0,1);
      //exctraction of the data
      x = msg.substring(0,msg.indexOf(" "));
      y = msg.substring(msg.indexOf(" ")+1,msg.length());
      
      //calculating the servos rotation
      pos_x = ((x.toFloat()*157./200.)  + 117);
      pos_y = ((y.toFloat()*157./200.)  + 117);
      
      
      //set the position of the servos
      servo_oeuil_x.write(pos_x);
      servo_oeuil_y.write(pos_y);
      break;
      
    case 'b' : // b and p are the commands for the eyelids et the mouth, not finished because the transmission system is not fully known
      Serial.println("BBBBB");// debug ligne
      break;
    case 'p' :
      Serial.println("PPPPP");// debug ligne
      break;
    case 't' ://movement of the head
      Serial.println("TTTTT");// debug ligne
      msg.remove(0,1);
      
      new_step_pos=int((msg.toInt()-67)/1.8);//extracting and convertingthe data

      //knowing the last position and the new position, we know if the stepper needs to go forward or backward
      if (new_step_pos < last_step_pos) {
        pos=last_step_pos - new_step_pos;
        if (pos==0) {
          break;
        }
        Serial.println(String(pos)+" pos "+String(last_step_pos)+" last pos "+String(new_step_pos)+" new pos        "+"avance");
        steph->step(100*pos, FORWARD, DOUBLE); 
      }
      else {
        pos=new_step_pos-last_step_pos;
        if (pos==0){
          break;
        }
        Serial.println(String(pos)+" pos "+String(last_step_pos)+" last pos "+String(new_step_pos)+" new pos        "+"recule");
        steph->step(100*pos, BACKWARD, DOUBLE);
      }
      last_step_pos=new_step_pos;
      //myservo.write(pos);
      break;
    default:
      // if nothing else matches, do the default
      Serial.println("invalid command : "+msg);
      break;
  }
}
void ISR { //command that reads the message from the HM10 Module
  if ( HM10.available() ) {
    msg = "";
//unsigned long StartTime = millis();


    while (HM10.available()){
      char lu = HM10.read();
      if (lu == '$') { //the $ signifies the end of a message, 
        command(msg);
        break;
      }
      else {//while it doesnt encounter the £, it builds the command msg
      msg=msg+lu;
      }
      delay(1);
    }
    
//    unsigned long CurrentTime = millis();
//    unsigned long ElapsedTime = CurrentTime - StartTime;
    Serial.println(msg);
    
  } 
  delay(10);
}
void loop() {
  
}
