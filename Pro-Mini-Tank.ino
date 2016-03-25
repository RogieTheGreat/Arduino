#define FORWARD 8
#define BACKWARD 2
#define STOP 5

//Library
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"


//For Radio
uint8_t msg[6] = {0, 0, 0, 0, 0, 0};
uint8_t num_received_data = sizeof(msg);
RF24 radio(9, 10); //(CE,CSN)
const uint64_t pipe = 0xE8E8F0F0E1LL;


int Motor_speed[2] = {2, 7}; // {Left & Right}
int Motor[2][2] = //two dimensional array
{
  {3 , 4},   //input pin to control Motor1--> Motor[0][0]=4, Motor[0][1]=5
  {5 , 6},   //input pin to control Motor2--> Motor[1][0]=6, Motor[1][1]=7
};

int tester = 8;

void setup() {
  Serial.begin(9600);
  radio_start();
  initialize();

  analogWrite(Motor_speed[0], 125);
  analogWrite(Motor_speed[1], 125);
}


void loop() {

  radio_read();
  speed_control();
  motor_move();


}
void motor_move() { //y-axis

if(msg[1]>=170){
    motor_run(0, FORWARD); //motor1 forward
    motor_run(1, FORWARD);
}
else if (msg[1]<=75){
    motor_run(0, BACKWARD);
    motor_run(1, BACKWARD); //motor2 backward
}
 else{ 
    motor_run(0, STOP);
    motor_run(1, STOP);
 }


}


void speed_control () { //X-axis
  if (msg[0] >= 200) {
    analogWrite(Motor_speed[1], 0);
  }
  else if (msg[0] <= 70) {
    analogWrite(Motor_speed[0], 0);
  }
  else {
    analogWrite(Motor_speed[0], 150);
    analogWrite(Motor_speed[1], 150);
  }
}

void motor_run(int motor, int movement) {
  switch (movement) {
    case FORWARD:
      digitalWrite(Motor[motor][0], HIGH);
      digitalWrite(Motor[motor][1], LOW);
      break;
      /*
    case BACKWARD:
      digitalWrite(Motor[motor][0], LOW);
      digitalWrite(Motor[motor][1], HIGH);
      break;
      */
    case STOP:
      digitalWrite(Motor[motor][0], LOW);
      digitalWrite(Motor[motor][1], LOW);
      break;
  }
}

//Functions
void radio_start() {
  radio.begin();
  radio.openReadingPipe(1, pipe);
  radio.startListening();
}

void initialize() {
  for (int i = 0; i < 2; i++) {
    pinMode(Motor_speed[i], OUTPUT);
    for (int j = 0; j < 2; j++) {
      pinMode(Motor[i][j], OUTPUT);
      //digitalWrite(Motor[i][j], LOW);
    }
  }
  pinMode(tester, OUTPUT);
}

void checker(){
       Serial.print("msg[] = {");
    for (int i=0;i<6;i++){
      Serial.print(" ");
      Serial.print(msg[i]);
    }
    Serial.println(" }");

}
void radio_read() {
  
 
 
  checker();
  if (radio.available()) {//if it read something, then it will open
    bool done = false; //to check for new
    //digitalWrite(tester, HIGH);
    while (!done) {
      done = radio.read(msg, num_received_data); //It has value
      //digitalWrite(tester, LOW);
    }
  }
  else {
    Serial.println("No radio available");
  }
}

