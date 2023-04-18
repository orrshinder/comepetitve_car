#include <AFMotor.h>  // DC Motors library
#include <IRremote.h> // IR library 

#define Engle_360_in_time = 2400;  ///change

//------------------------------------------------------------------------------ Sensors pins and State machin
int IRFront = 31;     // IRFront pin description ///0 deg   //// number 1
int IRRight = 53;     // IRRight pin description ///-45 deg //// number 2
int IRLeft = 35;      // IRLeft pin description  ///45 deg  //// number 3
int signal_in0 = 36;    // -45deg
int signal_in1 = 40;    // 0deg
int signal_in2 = 44;    // 45deg

int angle = 0;
//{delay(650) = 90 deg}
//{delay(325) = 45 deg}
int flag_calibrait = 0;
int flag_state_R_or_L = 0;
int Right_meger = 0;
int Left_meger = 0;

unsigned long duration1;
unsigned long duration2;
unsigned long duration3;

int trig_front = 38;  // SUPERFront pin description // 
int echo_front = 39;                                // 
int trig_right = 40;  // SUPERRight pin description // perpul
int echo_right = 41;                                // blue
int trig_left = 42;   // SUPERLeft pin description  // perpul
int echo_left = 43;                                 // blue

//-------------------------------------------------------------------------------- AF_Motor:
AF_DCMotor Right_front_motor(1); //defind pins for motors
AF_DCMotor Left_front_motor(4);
AF_DCMotor Right_back_motor(2);
AF_DCMotor Left_back_motor(3);

// put your setup code here, to run once:
void setup() { //------------------------------------------------------------------- Definding inputs and outputs 
  Serial.begin(9600);           // Define baud rate for serial communication 
  pinMode(IRFront,INPUT);       
  pinMode(IRRight,INPUT);       
  pinMode(IRLeft,INPUT);        
  pinMode(trig_front,OUTPUT); 
  pinMode(echo_front,INPUT);
  pinMode(trig_right,OUTPUT); 
  pinMode(echo_right,INPUT);
  pinMode(trig_left,OUTPUT);  
  pinMode(echo_left,INPUT);
 
}
// --------------------------------------------------------------------------------- state
// direction = 1 == start
// direction = 2 == Left_Tern
// direction = 3 == Right_Tern
// direction = 4 == Forward
// direction = 5 == Avoid
// direction = 6 == Blind
// direction = 7 == Destination_found

int direction = 1;   // defind state machin /// start

/////////////////////////////////////////////////////////////////////////////////////////////////////////////// Main Program
// put your setup code here, to run contly:
void loop() { //-------------------------------------------------------------------- State Machin

  switch(direction){
    
    case 1:       // -----------------------------------------------------------  START
      
      if(IRsensor_Front() > 1000)
      {
        direction = 4;          // case go to forward 
      }
      if(IRsensor_Left() > 1060)
      {
        direction = 2;         // case go to right
      }
      if(IRsensor_Right() > 1060)
      {
        direction = 3;        // case go to left
      }
      if((ultra_front() < 20)&&((IRsensor_Right() < 1060)&&(IRsensor_Left() < 1060)&&(IRsensor_Front() < 1060)))
      {
        direction = 5;        // case go to Avoid
      }
      else{
        direction = 6;       // case go to Blind
      }
      break;
    
    case 2:   // --------------------------------------------------------------- Short tern Right
    
      if(IRsensor_Front() > 1050){
        Stop_Motor();
        delayMicroseconds(200);
        direction = 4;
      }
      else{
        Short_tern_Right();
      }
      flag_state_R_or_L = 1;
      break;
    
    case 3:  // ---------------------------------------------------------------- Short tern Left

   if(IRsensor_Front() > 1050){
        Stop_Motor();
        delayMicroseconds(200);
        direction = 4; //go to forword
      }
      else
      {
        Short_tern_Left();
      }
      flag_state_R_or_L = 2;
      break;
    
    case 4:     // ------------------------------------------------------------ Forward

     if((IRsensor_Right() > 1040) || (IRsensor_Left() > 1040)){
        Stop_Motor();
        delayMicroseconds(100);
        direction = 1; // go to start
      }
      if((ultra_front() < 50) && (IRsensor_Front() < 1000)){
        direction = 5; // go to Avoid
      }
      else{
          Forward_motors();
          if((IRsensor_Front() < 1040)&&(flag_state_R_or_L == 1)){ ///// fix motion to the left
             Short_tern_Left();
          }
          if((IRsensor_Front() < 1040)&&(flag_state_R_or_L == 2)){ ///// fix motion to the right
             Short_tern_Right();
          }
          if((ultra_front() < 50)){
            Stop_Motor();
            delayMicroseconds(250);
            direction = 7; //go to Destination found
           } 
      }
      break;
         
    case 5:    // ------------------------------------------------------------ Avoid /// avoid objects
      
      ////////////////////////////////////////////////////////////////////////////////////serching
      direction = 1;
      
     
      break;

    case 6:   //-------------------------------------------------------------- Blind /// search somethin

      break;
      

    case 7:    // ------------------------------------------------------------ Destination_found

        Stop_Motor();
        if(ultra_front() > 30 ){
           direction = 1;
        }
      break;  
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Fuction
void Searching_Signal(){  /// that fun is for searching IR reciving // the car don't see nothing
   //--------------------------------------------------------------------------------- Right Search
   if((IRsensor_Front() > 1020)||(IRsensor_Right() > 1020)||(IRsensor_Left() > 1020)){ ///Finding IR
    Stop_Motor();
    delayMicroseconds(250);
    direction = 1;
   }
   while((IRsensor_Front() < 1020)||(Right_meger > 5)){ ///searching right side //// Right_meger is for cunte how mach to tern the car 45 deg
     Short_tern_Right();
     Right_meger = Right_meger +1;
     flag_calibrait = 1;
     if((IRsensor_Front() > 1020)){ //IR recive signal when checking serauding
      direction = 4;
      Stop_Motor();
      delayMicroseconds(250);
      break;
     }
   }
   Right_meger = 0;
   //-------------------------------------------------------------------------------- Left Search
   while((IRsensor_Front() < 1020)||(Left_meger > 5)){ ///searching left side //// Left_meger is for cunte how mach to tern the car 45 deg
     Short_tern_Left();
     Left_meger = Left_meger +1;
     flag_calibrait = 2;
     if((IRsensor_Front() > 1020)){ //IR recive signal when checking serauding
      direction = 4;
      Stop_Motor();
      delayMicroseconds(250);
      break;
     }
   }
   Left_meger = 0;
//-------------------------------------------------------------------------------- calibrait the center of the car
   if(flag_calibrait == 1){
    Short_tern_Left(); 
    delay(325); // 45 degrees left
   }
   if(flag_calibrait == 2){
    Short_tern_Right(); 
    delay(325); // 45 degrees right
   }
//--------------------------------------------------------------------------------- forcing forword
   flag_calibrait = 0;
   if(direction != 4){ // the IR don't see 
     direction = 4;  ///force forword///
     if(ultra_front < 50){ ///if findig object 
      Searching_Objects();
     }
   }
}

void Searching_Objects(){  ///that function is to find the size of the object that we want to avoid from // go to this function if the front ultras < 50
  
     if(ultra_right() > 50){
      Short_tern_Left();
      while(ultra_right() < 50){
        Forward_motors();
      }
      Forward_motors();
      delay(500);
      while(ultra_right() > 50){
        Short_tern_Right();
      }
      while(ultra_right() < 50){
        Forward_motors();
      }
      //----------------------------------- We passed the object from the Left side
     }
     else{
      Short_tern_Right();
      while(ultra_left() < 50){
        Forward_motors();
      }
      Forward_motors();
      delay(500);
      while(ultra_left() > 50){
        Short_tern_Left();
      }
      while(ultra_left() < 50){
        Forward_motors();
      }
    }
     //----------------------------------- We passed the object from the Right side
}

int Angle(){ ///finding the angle of the car from the center difult
  //arccos(B/C) = A //degrees 
}

void seing_sensors(){
      Serial.print("distans in santimeter right :");
      Serial.println(ultra_right());                /////to see signals
      Serial.print("distans in santimeter front :");
      Serial.println(ultra_front());
      Serial.print("distans in santimeter left :");
      Serial.println(ultra_left());
      Serial.print("IR_sessor_Front :");
      Serial.println(IRsensor_Front());
      Serial.print("IR_sessor_Right :");
      Serial.println(IRsensor_Right());
      Serial.print("IR_sessor_Letf :");
      Serial.println(IRsensor_Left());
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

void Forward_motors() //------------------------------------------------ moving forword 
{
  Right_front_motor.setSpeed(speed(80)); //motor 80% tern
  Left_front_motor.setSpeed(speed(80));
  Left_back_motor.setSpeed(speed(80));   
  Right_back_motor.setSpeed(speed(80));
  
  Right_front_motor.run(FORWARD);
  Right_back_motor.run(FORWARD);
  Left_front_motor.run(FORWARD);
  Left_back_motor.run(FORWARD);
}

void Short_tern_Right() // ---------------------------------------------- tern right
{
  Right_front_motor.setSpeed(speed(70)); //motor 70% tern
  Right_front_motor.run(FORWARD);
  
  Right_back_motor.setSpeed(speed(70));
  Right_back_motor.run(FORWARD);
  
  Left_front_motor.setSpeed(speed(0));  //motor 0% tern
  Left_front_motor.run(FORWARD);
  
  Left_back_motor.setSpeed(speed(0));
  Left_back_motor.run(FORWARD);
}

void Short_tern_Left()//------------------------------------------------ tern left
{
  Right_front_motor.setSpeed(speed(0)); //motor 0% tern
  Right_front_motor.run(FORWARD);
  
  Right_back_motor.setSpeed(speed(0));
  Right_back_motor.run(FORWARD);
  
  Left_front_motor.setSpeed(speed(70));  //motor 70% tern
  Left_front_motor.run(FORWARD);
  
  Left_back_motor.setSpeed(speed(70));
  Left_back_motor.run(FORWARD);
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
  delayMicroseconds(100);
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
