/*Demo Code for NanoBeam Dragoon based on PWMServoDriver
Copyright 2015 Steve Fairbanks*/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)

int servos = 12; //Number of servos
/*these define the the three positions of each servo in degrees.  These worked well
 for my setup, but could be modified if one of your legs does not move correctly.*/
int initpos[] = {
  220, 360, 320, 520, 460, 340, 520, 400, 460, 400, 480, 300};
int midstep[] = {
  170, 425, 275, 570, 420, 300, 560, 350, 520, 260, 530, 260};
int walk[]    = {
  300, 280, 470, 380, 520, 500, 400, 480, 300, 580, 320, 500};

int mycount = 0;
uint8_t servonum = 0;

/*example 32 step sequence*/
int myleg[] = {
  0, 1, 4, 6, 2, 6, 5, 1, 9, 5, 6, 2, 10, 7, 3, 7, 9, 8, 0, 11, 5, 1, 3, 2, 5, 2, 5, 11, 4, 8, 6, 10};
int mypos[] = {
  3, 3, 2, 3, 3, 1, 3, 2, 2, 1, 2, 1, 2,  3, 3, 1, 1, 2, 1, 2,  2, 1, 1, 3, 3, 1, 1, 1,  1, 1, 1, 1};

void setup() {
  /*setup serial port for optional viewing*/
  Serial.begin(9600);
  Serial.println("NanoBeam Dragoon 12 channel Servo test!");
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  /*setup pin 12 as a test mode pin.  if you jumper 12 to GND then Dragoon will stay in initial
   position and not start walking.*/
  pinMode(12, INPUT_PULLUP);
  /*move servos to initial position*/
  for(int i=0; i<servos; i++){
    pwm.setPWM(i, 0, initpos[i]);
    Serial.print("servo: ");
    Serial.print(i);
    Serial.print(" pos: ");
    Serial.println(initpos[i]);
  }
  /*after power on, go to initial position and wait 5 seconds before start walking.*/
  delay(5000);
}

void setServoPulse(uint8_t n, double pulse) {
  double pulselength;
  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= 60;   // 60 Hz
  Serial.print(pulselength); 
  Serial.println(" us per period"); 
  pulselength /= 4096;  // 12 bits of resolution
  Serial.print(pulselength); 
  Serial.println(" us per bit"); 
  pulse *= 1000;
  pulse /= pulselength;
  Serial.println(pulse);
  pwm.setPWM(n, 0, pulse);
}

void loop() {
  if(digitalRead(12) == HIGH)  { //check if 12 is tied low then don't move
    /*check if there is any serial input, if not walk through the series in setup*/
    /*if you get serial input then parse leg and position.*/
    /*position can be 1, 2, 3 for the preset positions or between 150-600 to set servo to exact position*/
    while (Serial.available() > 0) {
      int leg = Serial.parseInt(); 
      int pos = Serial.parseInt(); 
      Serial.print("servo: ");
      Serial.print(leg);
      Serial.print(" pos: ");
      if(pos <= SERVOMAX && pos >= SERVOMIN) {
        pwm.setPWM(leg, 0, pos);
        Serial.print("+");
      }
      if(pos==1) {
        pwm.setPWM(leg, 0, initpos[leg]);
        Serial.print("*");
      }
      if(pos==2) {
        pwm.setPWM(leg, 0, midstep[leg]);
        Serial.print("*");
      }
      if(pos==3) {
        pwm.setPWM(leg, 0, walk[leg]);
        Serial.print("*");
      }
   
    Serial.println(pos);
    mycount = 32;
  }

  /*walk through demo step sequence*/
  if(mycount < 32) {
    int leg = myleg[mycount]; 
    int pos = mypos[mycount];  
    Serial.print("servo: ");
    Serial.print(leg);
    Serial.print(" pos: ");
    if(pos==1) {
      pwm.setPWM(leg, 0, initpos[leg]);
      Serial.print("*");
    }
    if(pos==2) {
      pwm.setPWM(leg, 0, midstep[leg]);
      Serial.print("*");
    }
    if(pos==3) {
      pwm.setPWM(leg, 0, walk[leg]);
      Serial.print("*");
    }
    Serial.println(pos);
    if(mycount==31) mycount = -1; //restart
  } 
  mycount++;
  delay(200);
}
else {
  delay(5000);
  //build
}
}

