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
int flag_turn = 0;

unsigned long duration1;
unsigned long duration2;
unsigned long duration3;

int trig_front = 38;  // SUPERFront pin description // 
int echo_front = 39;                                // 
int trig_right = 40;  // SUPERRight pin description // perpul
int echo_right = 41;                                // blue
int trig_left = 42;   // SUPERLeft pin description  // perpul
int echo_left = 43;                                 // blue
int couting = 0;
  int count = 0; 
  int count_time=0;
  int Dis = 0;
  int flag_state = 0;
  int count_wall = 0;
  int x = 30;
  int y = 100;
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
  int Dis = 0;
  int flag_state = 0;
  int count_wall = 0;
  int x = 30;

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

void loop() {
/*
  while(true){
      Serial.print("distans in santimeter right :");
      Serial.println(ultra_right());               
      //Serial.print("distans in santimeter front :");
      //Serial.println(ultra_front());
  }
  */

   unsigned long start_time = millis();
   while((millis()<(start_time+2078))&&(IRsensor_Front() < 1040))  //first task: look for IR, and spin around
      {
        motors(68,68,BACKWARD,BACKWARD,FORWARD,FORWARD);
      }
      Stop_Motor();
      delay(200);

      while((IRsensor_Front() > 1040) && (ultra_front() > 20)){ // found ir going forword
      Serial.print("found ir going forword");
      Serial.println(); 
        motors(80,80,FORWARD,FORWARD,FORWARD,FORWARD);
      }
      Stop_Motor();
      delay(50);

      while((IRsensor_Front() > 1020) && (ultra_front() < 20)){ // reach the ir stopping
       Serial.print("reach the ir stopping");
      Serial.println(); 
        Stop_Motor();
      }
      Stop_Motor();
      delay(50);
     
      while((ultra_front() > x)&&(ultra_left() > x)&&(ultra_right() > x)){ // not seeing any obsicel moveing forword
      Serial.print(" not seeing any obsicel moveing forword");
      Serial.println(); 
          motors(60,60,FORWARD,FORWARD,FORWARD,FORWARD);
        }
        Stop_Motor();
        delay(50);
      while((ultra_front() > 20)&&(ultra_front() < 30)){
           motors(40,40,FORWARD,FORWARD,FORWARD,FORWARD);
      }
        Stop_Motor();
      delay(50);


      while((ultra_front() < 20) && (ultra_right() > x)&&(ultra_left() > x)){ // seeing obstical in front, rotet left antill ultra right see less then 40 
      Serial.print(" seeing obstical in front, rotet left antill ultra right see less then 40");
      Serial.println(); 
      if(flag_turn == 0)
      {
        motors(80,80,FORWARD,FORWARD,BACKWARD,BACKWARD);
      }
      else
      {
        motors(80,80,BACKWARD,BACKWARD,FORWARD,FORWARD);
      }
      }    
      Stop_Motor();
      delay(50); 
       if(flag_turn == 0)
       {
        flag_turn=1;
       }
       else
       {
        flag_turn=0;
       }
      while((ultra_left() < x) && (count_wall<y) ){ //obstical in left, figerout if this is box or wall 
       Serial.print(" obstical in left, figerout if this is box or wall");
      Serial.println();
        motors(60,60,FORWARD,FORWARD,FORWARD,FORWARD);
        count_wall ++;
      }
      Stop_Motor();
      delay(50);

      while(ultra_left() < x){ // obstical in left is wall
       Serial.print(" obstical in left is wall");
      Serial.println();
        count_wall = 0;
        motors(80,80,BACKWARD,BACKWARD,FORWARD,FORWARD);
      }
      Stop_Motor();
      delay(50);

      while(ultra_left() > x && count_wall>0){ // obsticel in left is box 
          Serial.print(" obsticel in left is box");
      Serial.println();
        count_wall -- ;
        motors(80,80,BACKWARD,BACKWARD,FORWARD,FORWARD);
      }
      Stop_Motor();
      delay(50);
      
      while((ultra_right() < x) && (count_wall<y)){//obstical in right figerout if wall or box 
      Serial.print(" obstical in right figerout if wall or box");
      Serial.println();
        motors(50,58,FORWARD,FORWARD,FORWARD,FORWARD);
        count_wall ++;
      }
      Stop_Motor();
      delay(50);
      
        while(ultra_right() < x){ // obstical in right is wall
        Serial.print("  obstical in right is wall");
        Serial.println();
        count_wall = 0;
        motors(80,80,FORWARD,FORWARD,BACKWARD,BACKWARD);
      }
      Stop_Motor();
      delay(50);

      while(ultra_right() > x && count_wall>0){ // obsticel in right is box
      Serial.print("  obsticel in right is box");
        Serial.println();
        count_wall -- ;
        motors(80,80,BACKWARD,BACKWARD,FORWARD,FORWARD);
      }
      Stop_Motor();
      delay(50);  
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

 int IRsensor_Left(){ //----------------------------------------- colling the sensor that we want to read from /// blinking for now //// with out time delay!!!!!!
  duration2 = pulseIn(IRLeft,LOW);
  delayMicroseconds(100);
  return duration2; 
}

int IRsensor_Right() //----------------------------------------- colling the sensor that we want to read from /// blinking for now //// with out time delay!!!!!!
{
  duration3 = pulseIn(IRRight,LOW); 
  delayMicroseconds(100);
  
  return duration3;
}
