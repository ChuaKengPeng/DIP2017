//-----------------------------------------------------------------------------------For Declaration----------------------------------------------------------------------------------------------//
#include <SoftwareSerial.h>
SoftwareSerial A2A(8, 9); // Initialize Pin 8 and 9 as RX and TX for BotConSerial

//RDM6300
#define PwrSupply 2

//HC-06
#define GndSupply 4 

//Enable Motors
#define StandBy 10 //  Pin to enable the motors

//Right Motor
#define AnalogSpeedRight 3 // Pin to control the speed of right motor
#define RightMotorIN1 7 // Pin to control the direction of right motor
#define RightMotorIN2 6 // Pin to control the direction of right motor

//Left Motor
#define AnalogSpeedLeft 5 // Pin to control the speed of left motor
#define LeftMotorIN1 11 // Pin to control direction of left motor
#define LeftMotorIN2 12 // Pin to control direction of left motor

//Android Commands to Arduino Micro
#define Command_Available A2A.available() > 0
#define Switch_On_Off_Pressed command == 'W'
#define Car_On TurnOn % 2 == 1
#define Car_Off TurnOn % 2 == 0
#define Disconnect_Pressed command == 'Z'
#define Decrease_Pressed command == 'V'
#define Increase_Pressed command == 'X'
#define Forward_Pressed command == 'F'
#define Reverse_Pressed command == 'B'
#define Forward_Left_Pressed command == 'G'
#define Reverse_Left_Pressed command == 'H'
#define Forward_Right_Pressed command == 'I'
#define Reverse_Right_Pressed command == 'J'
#define Left_Rotation_Pressed command == 'L'
#define Right_Rotation_Pressed command == 'R'
#define Stop_Pressed command == 'S'
#define Auto_Pressed command == 'A'

//LED
#define LightOut 13

//Time Setting
unsigned long timesetter;
unsigned long initialtime;
int timestarter = 0;
int minute;
int second;
int millisecond;
int StationCounter = 1;
char precode[14];

//Speed Control
int SpdLF = 0; // Set between 0 to 255
int SpdLR = 0; // Set between 0 to 255
int SpdRF = 0; // Set between 0 to 255
int SpdRR = 0; // Set between 0 to 255

boolean Turn = false;

char command;
int TurnOn = 0;

//-----------------------------------------------------------------------------Get Commands from Andriod to Arduino-----------------------------------------------------------------------------//

void Get_Command(){
  if (Command_Available){
    command = A2A.read();
  }
}

void Armed_Disarmed(){
  TurnOn++;
}


//------------------------------------------------------------------------------For extraction of data from RFID cards------------------------------------------------------------------------------------//

void Read_RFID() { //  Extract data from RFID card
  char code[14];
  if (Serial1.available() > 0) {  //  When data is received
    Serial1.readBytes(code, 14);  //  store data into array code
    if (code[0] != precode[0] || code[1] != precode[1] || code[2] != precode[2] || code[3] != precode[3] || code[4] != precode[4] || code[5] != precode[5] || code[6] != precode[6] || code[7] != precode[7] || code[8] != precode[8] || code[9] != precode[9] || code[10] != precode[10] || code[11] != precode[11] || code[12] != precode[12] || code[13] != precode[13]){  //  The same RFID card cannot be read consecutively
      digitalWrite(LightOut, HIGH); //  Turn on LED when data is stored
      if (timestarter != 1 && StationCounter == 1) {
        timestarter = 1;
        initialtime = millis();
      }
      Time_Display();
      A2A.print("Station ");
      A2A.print(StationCounter);
      A2A.print(": | ");
      A2A.println(code);
      StationCounter++;
      for (int i=0; i<14; i++){
        precode[i] = code[i];
      }
    }
  }
  digitalWrite(LightOut, LOW);  //  Turn off LED when data is displayed
}

//--------------------------------------------------------------------------------For displaying of running time---------------------------------------------------------------------------------//

void Time_Display() {  //  Display present time in a proper format
  if (timestarter == 1) {
    if (StationCounter == 1) {
      A2A.print("[Time: ");
      A2A.print("0");
      A2A.print(":");
      A2A.print("0");
      A2A.print(":");
      A2A.print("0");
      A2A.print("] | ");
    }
    else {
      timesetter = millis();
      timesetter -= initialtime;
      millisecond = timesetter % 1000;
      second = (timesetter / 1000);
      minute = second / 60;
      second %= 60;
      A2A.print("[Time: ");
      A2A.print(minute);
      A2A.print(":");
      A2A.print(second);
      A2A.print(":");
      A2A.print(millisecond);
      A2A.print("] | ");
    }
  }
}

//-------------------------------------------------------------------------------------For Individual Motors----------------------------------------------------------------------------------------// 

void Left_Forward(int speed) {  //  Turn left motor in forward direction
  digitalWrite(StandBy, HIGH);
  digitalWrite(LeftMotorIN1, LOW);
  digitalWrite(LeftMotorIN2, HIGH);
  if (speed <= 255 && speed >= 0) { //  speed input must only be between 0 to 255
    if (Turn == true) {
      analogWrite(AnalogSpeedLeft, speed);
    }
    if (Turn == false) {
    analogWrite(AnalogSpeedLeft, SpdLF);
    }  
  }
}

void Left_Reverse(int speed) {  //  Turn left motor in reverse direction
  digitalWrite(StandBy, HIGH);
  digitalWrite(LeftMotorIN1, HIGH);
  digitalWrite(LeftMotorIN2, LOW);
  if (speed <= 255 && speed >= 0) { //  speed input must only be between 0 to 255
    if (Turn == true) {
      analogWrite(AnalogSpeedLeft, speed);  
    }
    if (Turn == false) {
      analogWrite(AnalogSpeedLeft, SpdLR);
    }
  }
}

void Right_Forward(int speed) { // Turn right motor in forward direction
  digitalWrite(StandBy, HIGH);
  digitalWrite(RightMotorIN1, LOW);
  digitalWrite(RightMotorIN2, HIGH);
  if (speed <= 255 && speed >= 0) { //  speed input must only be between 0 to 255
    if (Turn == true) {
      analogWrite(AnalogSpeedRight, speed);  
    }
    if (Turn == false) {
      analogWrite(AnalogSpeedRight, SpdRF);
    }
  }
}

void Right_Reverse(int speed) { //  Turn right motor in reverse direction
  digitalWrite(StandBy, HIGH);
  digitalWrite(RightMotorIN1, HIGH);
  digitalWrite(RightMotorIN2, LOW);
  if (speed <= 255 && speed >= 0){ //  speed input must only be between 0 to 255
    if (Turn == true) {
      analogWrite(AnalogSpeedRight, speed); 
    }
    if (Turn == false) {
      analogWrite(AnalogSpeedRight, SpdRR);
    }
  }
}

//----------------------------------------------------------------------------------For car movements-------------------------------------------------------------------------------------//

void Forward() {  //  Car move in forward direction
  Turn = false;
  Left_Forward(SpdLF);
  Right_Forward(SpdRF);
}

void Reverse() {  //  Car move in reverse direction
  Turn = false;
  Left_Reverse(SpdLR);
  Right_Reverse(SpdRR);
}

void Rotate_Left(int speed) { // Car rotate to the left
  if (speed >= 0 && speed <= 255){ //  speed input must only be between 0 to 255
    Turn = true;
    Left_Reverse(speed);
    Right_Forward(speed);
  }
}

void Rotate_Right(int speed) {  //  Car rotate to the right
  if (speed >= 0 && speed <= 255){ //  speed input must only be between 0 to 255
    Turn = true;
    Right_Reverse(speed);
    Left_Forward(speed);
  }
}

void Forward_Turn(int Lspeed, int Rspeed) { // Car do a left/right turn in forward direction
  if (Lspeed >= 0 && Lspeed <= 255 && Rspeed >= 0 && Rspeed <= 255){ //  Lspeed & Rspeed input must only be between 0 to 255
    Turn = true;
    Left_Forward(Lspeed);
    Right_Forward(Rspeed);
  }
}

void Reverse_Turn(int Lspeed, int Rspeed) { //  Car do a left/right turn in reverse direction
  if (Lspeed >= 0 && Lspeed <= 255 && Rspeed >= 0 && Rspeed <= 255){ //  Lspeed & Rspeed input must only be between 0 to 255
    Turn = true;
    Left_Reverse(Lspeed);
    Right_Reverse(Rspeed);
  }
}

void Motor_Stop() { //  Car stop
  digitalWrite(StandBy, LOW);
}

//------------------------------------------------------------------------For acceleration/decceleration of motor speed------------------------------------------------------------------------//

void Speed_Change(int accel) { //  Change the speed for forward and reverse motion
  int speedcheck = 0;
  if ((SpdLF + accel) > 255 || (SpdRF + accel) > 255 || (SpdLR + accel) > 255 || (SpdRR + accel) > 255 || (SpdLF + accel) < 0 || (SpdRF + accel) < 0 || (SpdLR + accel) < 0 || (SpdRR + accel) < 0) { // (present speed) plus (accel input) must be between 0 to 255 
    speedcheck = 1;
  }
  if (speedcheck == 0) {
    SpdLF += accel;
    SpdRF += accel;
    SpdLR += accel;
    SpdRR += accel;
  }
  A2A.print("LF Motor: ");
  A2A.print(SpdLF);
  A2A.print("RF Motor: ");
  A2A.print(SpdRF);
  A2A.print("LR Motor: ");
  A2A.print(SpdLR);
  A2A.print("RR Motor: ");
  A2A.println(SpdRR);
  command = Stop_Pressed;
}

//-----------------------------------------------------------------------------For reset upon disconnection------------------------------------------------------------------------------------//

void Reset() {  //  Reset all the variables and timer when bluetooth is disconnected
  SpdLF = 0;
  SpdLR = 0;
  SpdRF = 0;
  SpdRR = 0;
  timesetter = 0;
  initialtime = 0;
  minute = 0;
  second = 0;
  millisecond = 0;
  StationCounter = 1;
  timestarter = 0;
  command = 'S';
  Turn = false;
  TurnOn = 0;
  for (int i = 0; i < 14; i++){
    precode[i] = 0;
  }
  for (int i = 0; i <= 255; i++){
      analogWrite(LightOut, i);
      delay(10);
  }
  for (int i = 255; i >= 0; i--){
      analogWrite(LightOut, i);
      delay(10);
  }
}

//------------------------------------------------------------------------------For set up upon power up----------------------------------------------------------------------------------------//
void Setup() {  // Setting to be done when arduino is powered up
  pinMode(PwrSupply, OUTPUT);
  digitalWrite(PwrSupply, HIGH);

  pinMode(GndSupply, OUTPUT);
  digitalWrite(GndSupply, LOW);

  pinMode(LightOut, OUTPUT);
  digitalWrite(LightOut, LOW);
  
  pinMode(StandBy, OUTPUT);
  digitalWrite(StandBy, LOW);

  pinMode(AnalogSpeedRight, OUTPUT);
  pinMode(RightMotorIN1, OUTPUT);
  pinMode(RightMotorIN2, OUTPUT);

  pinMode(AnalogSpeedLeft, OUTPUT);
  pinMode(LeftMotorIN1, OUTPUT);
  pinMode(LeftMotorIN2, OUTPUT);

  Serial1.begin(9600);  // initialize serial1 communication at 9600 bits per second
  A2A.begin(115200); // initialize BotConSerial communication at 115200 bits per second
}
