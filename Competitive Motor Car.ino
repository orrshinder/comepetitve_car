#include <AFMotor.h>  // DC Motors library
#include <IRremote.h> // IR library 

#define Engle_360_in_time = 2200;  ///change

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
//-------------------------------------------------------------------------------- AF_Motor:
AF_DCMotor Right_front_motor(1); //defind pins for motors
AF_DCMotor Left_front_motor(4);
AF_DCMotor Right_back_motor(2);
AF_DCMotor Left_back_motor(3);

// put your setup code here, to run once:
void setup() { //------------------------------------------------------------------- Definding inputs and outputs 
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
  
  //pinMode(IRFront, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(IRFront),IRsensor_Front() , FALLING);
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
int couting = 0;
int count = 0; 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////// Main Program
// put your setup code here, to run contly:
void loop() { //-------------------------------------------------------------------- State Machin

  switch(direction){
    
    case 1:       // -----------------------------------------------------------  START
      //seing_sensors();
      Serial.print("case 1");
      while((IRsensor_Front() > 1060)&&(count < 1) ){ //////Forword
        Serial.print("while 1");
        direction = 4;
        count = count + 1;
      }
      count = 0;
      while((IRsensor_Left() > 1020)&&(count < 1) ){ ///////Left
        Serial.print("while 2");
        direction = 2;
        count = count + 1;
      }
      count = 0;
      while((IRsensor_Right() > 1020)&&(count < 1) ){ //////Right
        Serial.print("while 3");
        direction = 3;
        count = count + 1;
      }
      count = 0;
      while(((ultra_front() < 40)&&((IRsensor_Right() < 1000)&&(IRsensor_Left() < 1000)&&(IRsensor_Front() < 1000)))&&(count < 1)){ /////Avoid
        Serial.print("while 4");
        Serial.print("count:");
        Serial.println(count);
        direction = 5;
        count = count + 1;
      }
      count = 0;
      while(((IRsensor_Right() < 1000)&&(IRsensor_Left() < 1000)&&(IRsensor_Front() < 1000))&&(count < 1)){ /////Blind
        Serial.print("while 5");
        seing_sensors();
        direction = 6;
        count = count + 1;
      }
      count = 0;
      break;
    
    case 2:   // --------------------------------------------------------------- Short tern Right
      Serial.print("case 2");
      Serial.println();
      while(IRsensor_Front() < 1040){
          Short_tern_Right(60,00);
          //delay(100);
      }
      Stop_Motor();
      delay(200);
      direction = 4;
      flag_state_R_or_L = 1;
      break;
    
    case 3:  // ---------------------------------------------------------------- Short tern Left
      Serial.print("case 3");
      Serial.println();
      while(IRsensor_Front() < 1040)
      {
        Short_tern_Left(0,60);
        //delay(250);
        //seing_sensors();
      }
      Stop_Motor();
      delay(200);
      direction = 4;
      flag_state_R_or_L = 2;
      break;
    
    case 4:     // ------------------------------------------------------------ Forward
      Serial.print("case 4");
      Serial.println();
      //seing_sensors();
      while((IRsensor_Right() > 1030) || (IRsensor_Left() > 1030)){
        //flag_state_R_or_L = 0;
        Stop_Motor();
        delay(100);
        direction = 1; // go to start
        break;
      }
      while((IRsensor_Front() < 1000)){ ////to check if the if is working after the while above it ??????
        Stop_Motor();
        delay(100);
        while((ultra_front() < 20)&&(ultra_right() < 10)){ //onject from the right
           Stop_Motor();
           delay(25);
        Serial.print("while  1");
        Serial.println();
        seing_sensors();
        Short_tern_Right(60,0);
        delay(100);
        }
        while((ultra_front() < 20)&&(ultra_left() < 10)){//object from the left
         Stop_Motor();
           delay(25);
        Serial.print("while  2");
        Serial.println();
        Short_tern_Left(0,60);
        delay(100);
        }
        while((ultra_front() > 40)&&(ultra_right() > 30)&&(ultra_left() > 30)){//object from the front
        Serial.print("while  3");
        Serial.println();
        Forward_motors(85,94);
        delay(100);
        }
        while((ultra_front() < 40)&&(ultra_right() > 30)&&(ultra_left() > 30)){//object from the front
        Serial.print("while  4");
        Serial.println();
        Stop_Motor();

        //Forward_motors(85,94);
        //delay(100);
        direction = 5; // go to Avoid
        break;
        }
      }
        while((ultra_front() > 30)){
         Forward_motors(90,98);
          delay(100);
        }
        while((ultra_front() < 30)&&(IRsensor_Front() > 1000)){//// 30 sentimeter from the transmiter
           Stop_Motor();
           delay(250);
           direction = 7; //------------------------ Destination found
           Serial.print("while aziraaaa to 7");
           Serial.println();
           break;
          }
      break;
         
    case 5:    // ------------------------------------------------------------ Avoid /// avoid objects
        Serial.print("case 5");
        Serial.println();
        if(IRsensor_Front() > 1020){ /// can see IR front 
          Stop_Motor();
          delay(200);
          direction = 4; // go to forword
          break;
        }
        while((ultra_front() > 40)){ // find object in front of the car
          Forward_motors(60,68); 
          Serial.print("forword case 5");
          Serial.println();
          delay(100);
        }
        Stop_Motor();
        delay(200);
        /*while((ultra_front() > 30)){ // 20 sentimeters from the object
          Stop_Motor();
          Serial.print("stop case 5");
          Serial.println();
          delay(200);
        }*/
 ///////////////////////////////////////////////////////// serching
        Searching_Objects(1); // 0 or other number will cheing the derection of the avoid object 
        Serial.print("end function search object");
        Serial.println();
        while((IRsensor_Front() > 1000)&&(count < 20)){  //count = 20 is for mejer the amount of tern we want
          Short_tern_Left(80,0);
          count = count + 1;    ///the to limit the tern of the car to 90 deg 
        }
        count = 0;
        //Short_tern_Left(80,0); //90 deg tern
        //delay(600);            //90 deg tern
        Stop_Motor();
        delay(250);
        direction = 1;
      break;

    case 6:   //-------------------------------------------------------------- Blind /// search somethin
        Serial.print("case 6");
        Serial.println();
        //seing_sensors();
        Searching_Signal();
        if(direction == 4){
          break;
        }
        else{
          while((IRsensor_Right() < 1000)||(IRsensor_Left() < 1000)||(IRsensor_Front() < 1000)){
            Forward_motors(80,80);
            delay(100);
            if(couting > 2500){break;} //running to mach time 
          }
          couting = 0;
          //Searching_Signal();
        }
      break;

    case 7:    // ------------------------------------------------------------ Destination_found
        Serial.print("case 7 Destination found");
        Serial.println();
        Stop_Motor();
        delay(500);
        if(ultra_front() > 50 ){
           direction = 1;
        }
      break;  
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Fuction
void Searching_Signal(){  /// that fun is for searching IR reciving // the car don't see nothing
   //------------------------------------------------------------------------------------------------ Right Search
   if((IRsensor_Front() > 1020)||(IRsensor_Right() > 1020)||(IRsensor_Left() > 1020)){ /// Finding IR
    Stop_Motor();
    delay(250);
    direction = 1;
    return;
   }
   //---------------------------------------
   unsigned long startMillis = millis();
   unsigned long currentMillis = millis();
   int count_time=0;
   while(count_time<3||(IRsensor_Front() < 1020)){ ///problme in millis()- not counting the current time 
      /*currentMillis = millis(); 
      Serial.print("currentMillis time: ");
      Serial.println(currentMillis - startMillis);
      Serial.print("IRfront size: ");
      Serial.println(IRsensor_Front());*/
      count_time++;
      Serial.print("count_time");
      Serial.println(count_time);
      if(count_time > 3)
      {
        Stop_Motor();
        delay(50);
        direction = 4;
        break;
      }
      if((IRsensor_Front() > 1000))
      {
        Stop_Motor();
        delay(50);
        break;
      }
      Short_tern_Right_check(57,60);
      //delay(125);
      //Stop_Motor();
        //delay(1);

   }
   Serial.print("currentMillis time after wile ");
   Serial.println(millis()-currentMillis);
   previusmilis=currentMillis;
   direction = 4;
}

void Searching_Objects(int x){  ///that function is to find the size of the object that we want to avoid from // go to this function if the front ultras < 50

    if(x!=0){ //------------------------------ We passed the object from the Left side
      //Stop_Motor();
      //delay(200);
      while(ultra_right() > 20){//------------------------------- left tern
        Serial.print("left tern");
        Serial.println();
        Short_tern_Left(80,0);
      }
      Stop_Motor();
      delay(100);
      while(ultra_right() < 20){//---------------------------- forword_1
        Serial.print("forword 1");
        Serial.println();
        //Serial.println(ultra_front());
        Forward_motors(30,30);
        //delay(100);
      }
      Stop_Motor();
      delay(50);
      while(ultra_right() > 35){//------------------- short tern right_1
        Serial.print("long tern right_1");
        Serial.println();
        //Serial.println(ultra_front());
        //delay(100);
        //seing_sensors();
        Short_tern_Right(0,80);
        //Long_tern_Right(10,70);
        //delay(350);
      }
      Stop_Motor();
      delay(50);
      while(ultra_right() < 35){ //---------------- forword_2
        Serial.print("forword_2");
        Serial.println();
        //Serial.println(ultra_front());
        //delay(100);
        Forward_motors(30,30);
        //delay(100);
      }
      Stop_Motor();
      delay(50);
      while(ultra_right() > 10){//------------------ short tern right _2 //the object is behaind you + serching for IR front
        if(IRsensor_Front() > 1020){
          direction = 4;
          return;
        }
        Serial.print("long tern right _2");
        Serial.println();
        //Serial.println(ultra_front());
        Short_tern_Right(0,80);
        //Long_tern_Right(20,70);
      }
      Stop_Motor();
      delay(50);
      while(IRsensor_Left() < 1020){ // if not seing IR from the left
        if(ultra_right() < 10){ //we still seing object from the right but not IR from the left
          Stop_Motor();
          delay(100);
          Forward_motors(50,50);  //stedy forword
          delay(200);
        }
        else{    /// after we pass the object tern 90 deg left
          Short_tern_Left(80,0);
          delay(600);
          break;
        }
      }
    }
}

void seing_sensors(){
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

void Forward_motors(int Right_speed,int left_speed) //------------------------------------------------ moving forword 
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

void Short_tern_Right(int Right_speed,int left_speed) // ---------------------------------------------- tern right
{
  Right_front_motor.setSpeed(speed(Right_speed)); //motor 0% tern
  Right_front_motor.run(FORWARD);
  
  Right_back_motor.setSpeed(speed(Right_speed));
  Right_back_motor.run(FORWARD);
  
  Left_front_motor.setSpeed(speed(left_speed));  //motor 80% tern
  Left_front_motor.run(FORWARD);
  
  Left_back_motor.setSpeed(speed(left_speed));
  Left_back_motor.run(FORWARD);
}

void Short_tern_Right_check(int Right_speed,int left_speed) // ---------------------------------------------- tern right
{
  Right_front_motor.setSpeed(speed(Right_speed)); //motor 20% tern
  Right_front_motor.run(BACKWARD);
  
  Right_back_motor.setSpeed(speed(Right_speed));
  Right_back_motor.run(BACKWARD);
  
  Left_front_motor.setSpeed(speed(left_speed));  //motor 80% tern
  Left_front_motor.run(FORWARD);
  
  Left_back_motor.setSpeed(speed(left_speed));
  Left_back_motor.run(FORWARD);
}

void Short_tern_Left(int Right_speed,int left_speed)//------------------------------------------------ tern left
{
  Right_front_motor.setSpeed(speed(Right_speed)); //motor 80% tern
  Right_front_motor.run(FORWARD);
  
  Right_back_motor.setSpeed(speed(Right_speed));
  Right_back_motor.run(FORWARD);
  
  Left_front_motor.setSpeed(speed(left_speed));  //motor 0% tern
  Left_front_motor.run(FORWARD);
  
  Left_back_motor.setSpeed(speed(left_speed));
  Left_back_motor.run(FORWARD);
} 

void Long_tern_Left(int Right_speed,int left_speed)//------------------------------------------------ tern left
{
  Right_front_motor.setSpeed(speed(Right_speed)); //motor 80% tern
  Right_front_motor.run(FORWARD);
  
  Right_back_motor.setSpeed(speed(Right_speed));
  Right_back_motor.run(FORWARD);
  
  Left_front_motor.setSpeed(speed(left_speed));  //motor 20% tern
  Left_front_motor.run(FORWARD);
  
  Left_back_motor.setSpeed(speed(left_speed));
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
