
#include <SoftwareSerial.h>
#include <Servo.h>
//#include <AFMotor.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>



// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *steph = AFMS.getStepper(200, 1);

//HM10 module works as a Serial
SoftwareSerial HM10(2, 3); //HM10(Receive Pin, Transmit Pin)
String msg = "";


Servo myservo;  // create servo object to control a servo

Servo servo_oeuil_x;
Servo servo_oeuil_y;
Servo servo_bouche;
Servo serrvo_paupierre_sup;
Servo serrvo_paupierre_inf;

int new_step_pos =0;
int last_step_pos=0;
int pos = 0; 
int Kx=1;
int Ky=1;
float pos_x = 0.0;
float pos_y = 0.0;

String strength ="";

String angle ="";

String x ="";

String y ="";

void setup() {

  
  AFMS.begin();
  //the serial is used as a debuger
  Serial.begin(9600);
  HM10.begin(9600);

  
   // all servos are not attached yet because we dont know witch pins will be availibles
  servo_oeuil_x.attach(9);
  servo_oeuil_y.attach(10);
  servo_oeuil_x.write(67);
  servo_oeuil_y.write(67);
  pos=0;
  //initialise the stepper at step 0
  steph->setSpeed(100);
  steph->release();
  //steph->step(1000, FORWARD, DOUBLE); 
  //myservo.write(pos);
}
void command(String msg){
  //analyse the message and choose the corresponding action
  char command_info = msg.charAt(0);
  switch (command_info) {
    //the first charracter correspond to the action (eyes movement, head ...)
    case 'y' ://movement of the eyes
      Serial.println(msg+"===========");
      msg.remove(0,1);
      //extract the data from the message
      strength = msg.substring(0,msg.indexOf(" "));
      angle = msg.substring(msg.indexOf(" ")+1,msg.length());
      //process the data to calculate the position of the motors
      pos_x = Kx*(strength.toFloat()*cos(2*(PI/360.)*angle.toFloat()) +117);//   ((x.toFloat()*157./200.)  + 117);
      pos_y = Ky*(strength.toFloat()*sin(2*(PI/360.)*angle.toFloat()) +117);//((y.toFloat()*157./200.)  + 117);

      //debug command 
      Serial.println(String(pos_x)+"      "+String(pos_y));
      
      //set the position of the servos
      servo_oeuil_x.write(pos_x);
      servo_oeuil_y.write(pos_y);
      break;
    case 'b' : // b and p are the commands for the eyelids et the mouth, not finished because the transmission system is not fully known
      Serial.println("BBBBB");
      break;
    case 'p' :
      Serial.println("PPPPP");
      break;
    case 't' ://movement of the head
      Serial.println("TTTTT");
      msg.remove(0,1);
      new_step_pos=int((msg.toInt()-67)/1.8); //extracting and converting the data

      //knowing the last position and the new position, we know if the stepper needs to go forward or backward
      
      if (new_step_pos < last_step_pos) {
        pos=last_step_pos - new_step_pos;
        if (pos==0) {
          break;
        }
        Serial.println(String(pos)+" pos "+String(last_step_pos)+" last pos "+String(new_step_pos)+" new pos        "+"avance");
        steph->step(pos, FORWARD, DOUBLE); // tell the stepper how many steps it as to go
      }
      else {
        pos=new_step_pos-last_step_pos;
        if (pos==0){
          break;
        }
        Serial.println(String(pos)+" pos "+String(last_step_pos)+" last pos "+String(new_step_pos)+" new pos        "+"recule");
        steph->step(pos, BACKWARD, DOUBLE);
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
void loop() {
  if ( HM10.available() ) {
    msg = "";
//unsigned long StartTime = millis();


    while (HM10.available()){
      char lu = HM10.read();
      if (lu == '$') {// it reads charracters until it founds a "$" meaning the end of a command
        command(msg);
        break;
      }
      else {
      msg=msg+lu;
      }
      delay(1);//this prevent an issues when the HM10 module is slower than the arduino's loop
    }
    
//    unsigned long CurrentTime = millis();
//    unsigned long ElapsedTime = CurrentTime - StartTime;
    Serial.println(msg);
    
  } 
  delay(10);
}
