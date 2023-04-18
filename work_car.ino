#include <AFMotor.h>  // DC Motors library
#include <stdio.h>

//------------------------------------------------------------------------------ Sensors pins and State machin
int IRFront = 47;     // IRFront pin description ///0 deg   //// number 1 ///// leg 18 for interapt 
int IRRight = 53;     // IRRight pin description ///-45 deg //// number 2
int IRLeft = 35;      // IRLeft pin description  ///45 deg  //// number 3
int signal_in0 = 36;    // -45deg
int signal_in1 = 40;    // 0deg
int signal_in2 = 44;    // 45deg

int angle = 0;
//{delay(650) = 90 deg} check again
//{delay(325) = 45 deg} check again
int flag_calibrait = 0;
int flag_state_R_or_L = 0;
int Right_count = 0;
int Left_count = 0;

unsigned long duration1;
unsigned long duration2;
unsigned long duration3;

int trig_front = 38;  // SUPERFront pin description // 
int echo_front = 39;                                // 
int trig_right = 40;  // SUPERRight pin description // perpul
int echo_right = 41;                                // blue
int trig_left = 42;   // SUPERLeft pin description  // perpul
int echo_left = 43;                                 // blue
unsigned long previusmilis=0;
int couting = 0;
  int count = 0; 
  int count_time=0;
  int Dis = 0;
  int flag_state = 0;
 // long int start_time = millis();
  int count_wall = 0;
  int x = 30;
  int y = 15;
  int count_around=0;
  int count_turn_right=15;
//-------------------------------------------------------------------------------- AF_Motor:
AF_DCMotor Right_front_motor(1); //defind pins for motors
AF_DCMotor Left_front_motor(4);
AF_DCMotor Right_back_motor(2);
AF_DCMotor Left_back_motor(3);

// put your setup code here, to run once:

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);           // Define baud rate for serial communication 
  pinMode(IRFront,INPUT); //interapt if IRFront = 18       
  pinMode(IRRight,INPUT);       
  pinMode(IRLeft,INPUT);        
  pinMode(trig_front,OUTPUT); 
  pinMode(echo_front,INPUT);
  pinMode(trig_right,OUTPUT); 
  pinMode(echo_right,INPUT);
  pinMode(trig_left,OUTPUT);  
  pinMode(echo_left,INPUT);

  int couting = 0;
  int count = 0; 
  int count_time=0;
  int count_around=0;
  int Dis = 0;
  int flag_state = 0;
  int count_turn_right=15;
  int count_wall = 0;

     /*
      Serial.print("distans in santimeter right :");
      Serial.println(ultra_right());               
      Serial.print("distans in santimeter front :");
      Serial.println(ultra_front());
      Serial.print("distans in santimeter left :");
      Serial.println(ultra_left());
      Serial.print("IR_sessor_Front duration :");
      Serial.println(IRsensor_Front());
      Serial.print("IR_sessor_Right duration :");
      Serial.println(IRsensor_Right());
      Serial.print("IR_sessor_Letf duration :");
      Serial.println(IRsensor_Left());
    */

}

//void motors(int Right_speed,int left_speed,char R_F_dir , char R_B_dir , char L_F_dir , char L_B_dir )

void loop() {////////////////////////////////////////////////////////////////////////////////// main 
/*
     // Serial.print("IR_sessor_Front duration :");
      //Serial.println(IRsensor_Front());
      Serial.print("IR_sessor_Right duration :");
      Serial.println(IRsensor_Right());
      Serial.print("IR_sessor_Letf duration :");
      Serial.println(IRsensor_Left());
*/
/*while(true)
{
      Serial.print("IR_sessor_Front duration :");
      Serial.println(IRsensor_Front());
      Serial.print("distans in santimeter front :");
      Serial.println(ultra_front());
}
*/
  // put your main code here, to run repeatedly:
   unsigned long start_time = millis();
   while((millis()<(start_time+2078))&&(IRsensor_Front() < 1040))  //first task: look for IR, and spin around
      {
        motors(68,68,BACKWARD,BACKWARD,FORWARD,FORWARD);
      }
      Stop_Motor();
      delay(200);

      while((IRsensor_Front() > 1040) && (ultra_front() > 30)){ //secend task: if the IR is in front of as and ther is not an object move forword
        Serial.print("IR_sessor_Front duration :");
        Serial.println(IRsensor_Front());
        motors(85,80,FORWARD,FORWARD,FORWARD,FORWARD);
      }
      Stop_Motor();
      delay(50);

      while((IRsensor_Front() > 1020) && (ultra_front() < 30)){ //therd task: if the IR is in front of as and ther is an object stoop motors
        Stop_Motor();
      }
        while((ultra_front() > x)&&(ultra_left() > x)&&(ultra_right() > x)&&(IRsensor_Front() < 1000)){ // forth task: there is no IR detect and no object detected
           Serial.print("goinng forword");
          Serial.println();
          motors(85,80,FORWARD,FORWARD,FORWARD,FORWARD);
          //seing_sensors();
        }
        Stop_Motor();
        delay(50);
      
      while((ultra_front() < 20)&&(IRsensor_Front() < 1000)||(ultra_right() < 20) && (count_around<10)){
        motors(40,40,FORWARD,FORWARD,BACKWARD,BACKWARD);
        count_around ++;
      }
      count_around=0;
      Stop_Motor();
      delay(100);
      
      while((ultra_left() < x) && (count_wall<y) ){  //fifth task: 
        motors(60,60,FORWARD,FORWARD,FORWARD,FORWARD);
        count_wall ++;
      }
      Stop_Motor();
      delay(50);
      while(ultra_left() < x){
        count_wall = 0;
        motors(60,60,BACKWARD,BACKWARD,FORWARD,FORWARD);
      }
      Stop_Motor();
      delay(50);
      while((ultra_left() > x) && (count_wall>0)){
        count_wall = 0 ;
        motors(60,60,BACKWARD,BACKWARD,FORWARD,FORWARD);
      }
      Stop_Motor();
      delay(50);
      
      while((ultra_right() < x) && (count_wall<y)){
        Serial.print("wall on the right going forword coint_wall = ");
        Serial.println(count_wall);
        motors(50,50,FORWARD,FORWARD,FORWARD,FORWARD);
        count_wall ++;
      }
      Stop_Motor();
      delay(50);
      
       while(ultra_right() < x){
        Serial.print("still wall on the right coint_wall = ");
        Serial.println(count_wall);
        count_wall = 0;
        motors(60,60,FORWARD,FORWARD,BACKWARD,BACKWARD);
      }
      Stop_Motor();
      delay(50);
       
      while(ultra_right() > 30 && count_turn_right>0){
        motors(0,70,FORWARD,FORWARD,FORWARD,FORWARD);
        count_turn_right-- ;
      }
      count_turn_right=15;
      Stop_Motor();
      delay(50);
      /*while((ultra_front() > x)){ // forth task: there is no IR detect and no object detected
          motors(80,80,FORWARD,FORWARD,FORWARD,FORWARD);
        }
        Stop_Motor();
        delay(50);
      
*/
  
}



int speed(int persent) //------------------------------------------------------- maping 0% to 100% motor force 
{
  return map(persent,0,100,0,255);
}

int ultra_front(){   //-----------------------------------------------------------UltraSonic check_front
  long dur;
  int dis;

  digitalWrite(trig_front,LOW);
  delayMicroseconds(2);
  digitalWrite(trig_front,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_front,LOW);

  dur = pulseIn(echo_front,HIGH);
  dis = (dur * (0.034/2))+2;
      
  return dis;
}

int ultra_left(){   //-----------------------------------------------------------UltraSonic check_left
  long dur;
  int dis;

  digitalWrite(trig_left,LOW);
  delayMicroseconds(2);
  digitalWrite(trig_left,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_left,LOW);

  dur = pulseIn(echo_left,HIGH);
  dis = (dur * (0.034/2))+4;

  return dis;
}

int ultra_right(){   //-----------------------------------------------------------UltraSonic check_right
  long dur;
  int dis;

  digitalWrite(trig_right,LOW);
  delayMicroseconds(2);
  digitalWrite(trig_right,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_right,LOW);

  dur = pulseIn(echo_right,HIGH);
  dis = (dur * (0.034/2))+2;

  return dis;
}

/*
void motors(int Right_speed,int left_speed) //------------------------------------------------ moving forword 
{
  Right_front_motor.setSpeed(speed(Right_speed)); 
  Left_front_motor.setSpeed(speed(left_speed));
  Left_back_motor.setSpeed(speed(left_speed));   
  Right_back_motor.setSpeed(speed(Right_speed));
  
  Right_front_motor.run(FORWARD);
  Right_back_motor.run(FORWARD);
  Left_front_motor.run(FORWARD);
  Left_back_motor.run(FORWARD);
}
*/

void motors(int Right_speed,int left_speed,char R_F_dir , char R_B_dir , char L_F_dir , char L_B_dir ) // ---------------------------------------------- tern right
{
  Right_front_motor.setSpeed(speed(Right_speed)); //motor 20% tern
  Right_front_motor.run(R_F_dir);
  
  Right_back_motor.setSpeed(speed(Right_speed));
  Right_back_motor.run( R_B_dir);
  
  Left_front_motor.setSpeed(speed(left_speed));  //motor 80% tern
  Left_front_motor.run(L_F_dir);
  
  Left_back_motor.setSpeed(speed(left_speed));
  Left_back_motor.run(L_B_dir);
}

void Stop_Motor() //------------------------------------------- Shut Down Motors
{
  Right_front_motor.run(RELEASE);
  Right_back_motor.run(RELEASE);
  Left_front_motor.run(RELEASE);
  Left_back_motor.run(RELEASE);
}

int IRsensor_Front() //----------------------------------------- colling the sensor that we want to read from /// blinking for now //// with out time delay!!!!!!
{
  duration1 = pulseIn(IRFront,LOW); 
  delayMicroseconds(1000);
  return duration1;
}

 /*int IRsensor_Left(){ //----------------------------------------- colling the sensor that we want to read from /// blinking for now //// with out time delay!!!!!!
  duration2 = pulseIn(IRLeft,LOW);
  delayMicroseconds(100);
  return duration2; 
}

int IRsensor_Right() //----------------------------------------- colling the sensor that we want to read from /// blinking for now //// with out time delay!!!!!!
{
  duration3 = pulseIn(IRRight,LOW); 
  delayMicroseconds(100);
  
  return duration3;
}*/
