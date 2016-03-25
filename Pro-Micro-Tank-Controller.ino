//    Code for Transmitter

//    Rogie Bernabe
//    R/C Remote
//    v1.0
//    Pro Micro


//Library
#include  <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"


//For Radio
uint8_t msg[6]= {0,0,0,0,0,0};
const uint8_t buffer_size = sizeof(msg);
RF24 radio(9, 10); //(CE,CSN)
const uint64_t pipe = 0xE8E8F0F0E1LL;
//

//For Joystick & Buttons

int joystick_analog[3] = {A0, A1, A2}; // [x-axis,y-axis,z-button]
int joystick[3] = {0, 0, 0}; //Initializer Value

int Button_Analog[4] = {A3, 4, 5, 6};
int Button[4] = {0, 0, 0, 0};//Initializer Value


/*
 * Note
 * msg[] = [joystick[3], Button[4]];
 */
//Main Code
void setup() {

  Serial.begin(9600);
  radio_start();
  initialize();
Serial.println("Done Setup!");
}

void loop() {
  analog_button();
  save();
//checker();
//radio_send();
  transmit();
  bool ok = radio.write(msg, buffer_size);
}

// End

//Functions
void radio_start() {
  radio.begin();
  radio.openWritingPipe(pipe);
}

void initialize() {
  for (int i = 0; i < 3; i++) {
    pinMode(joystick_analog[i], INPUT);
    delay(2);
  }
  for (int i = 0; i <= 3; i++) {
    pinMode(Button_Analog[i], INPUT);
    delay(2);
  }
}

void radio_send() {//test
  msg[0] = 0;//value to be annalog read
  msg[1] = 1;//value

  for (int i = 0; i < 10; i++) {
    bool ok = radio.write(msg, buffer_size);
  }
  delay(1000);
  Serial.println("Msg 1 Done");

  msg[0] = 1;//value
  msg[1] = 0;//value

  for (int i = 0; i < 10; i++) {
    bool ok = radio.write(msg, buffer_size);
  }
  Serial.println("Msg 2 Done");
  delay(1000);
}

void analog_button() { //Try to debug

  for (int i = 0; i < 3; i++) {
    joystick[i] = map(analogRead(joystick_analog[i]), 0, 1023, 0, 255);
    
  }
  /*
   * Analog[3]
   * x-axis
   * y-axis
   * z-button - Function 3
   */
  for (int i = 0; i <= 3 ; i++) {
    Button[i] = digitalRead(Button_Analog[i]);
  }
  /*
   * Button 0 - Go
   * Button 1 - Back
   * Button 2 - Function 1
   * Button 3 - Function 2
   */
//Checker
/*
   Serial.print("joystick 1: ");
   Serial.println(joystick[0]);
   
   Serial.print("joystick 2: ");
   Serial.println(joystick[1]);
   //delay(500);
   */
}

void save() {
  int i = 0;
    for (int j = 0; j < 3; j++) {
      msg[i] = joystick[j];
      i++;
    }
    
    for (int j = 0; j <= 3; j++) {
      msg[i] = Button[j];
      i++;
      
    }
    
  }

  void checker(){
    
     Serial.print("msg[] = {");
    for (int i=0;i<6;i++){
      Serial.print(" ");
      Serial.print(msg[i]);
    }
    Serial.println(" }");
  }

void transmit() {

  bool ok = false;
  while (!ok) {
    //digitalWrite(LED,HIGH);
    ok = radio.write(msg, buffer_size);
    //If send LED light up
    //digitalWrite(LED,LOW);
    Serial.println("Transmiting");
    //delay(2);
  }
  Serial.println("Transmited!");
}




