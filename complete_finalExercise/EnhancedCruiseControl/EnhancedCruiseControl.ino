//Christopher Ho
//500840595
//christopher.j.ho@ryerson.ca

#include <Servo.h>  //servo library
Servo myservo;      // create servo object to control servo

//Ultrasonic sensor variables
int Echo = A4;
int Trig = A5;
int distance = 0;

//Line Tracking IO define
#define LT_R !digitalRead(10)
#define LT_M !digitalRead(4)
#define LT_L !digitalRead(2)

//Motor controller pins
#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11
int carSpeed = 100;
#define carSpeed2 100

// True/False depending on which lane its starting in
int current_lane = 0; // 0=right 1=left


void forward() {
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("Forward");
}

void back() {
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Back");
}

void left() {
  analogWrite(ENA, carSpeed2);
  analogWrite(ENB, carSpeed2);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("Left");
}

void right() {
  analogWrite(ENA, carSpeed2);
  analogWrite(ENB, carSpeed2);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Right");
}

void stop() {
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  Serial.println("Stop!");
}

/* Ultrasonic distance measurement method */
int Distance_test() {
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(20);
  digitalWrite(Trig, LOW);
  float Fdistance = pulseIn(Echo, HIGH);
  Fdistance = Fdistance / 58;
  return (int)Fdistance;
}

/* Functions to change lanes */
void switch_to_left() {
  left();
  delay(1200);

  while (!LT_L) {
    forward();
  }

  right();
  delay(1300);

  left();
  delay(800);

  while (!LT_L) {
    forward();
  }

  right();
  delay(1300);
}

void switch_to_right() {
  right();
  delay(1300);

  while (!LT_R) {
    forward();
  }

  left();
  delay(1300);
}

void setup() {
  myservo.attach(3, 500, 2500);  // attach servo on pin 3 to servo object (new servo range is 500-2500)
  Serial.begin(9600);
  pinMode(10, INPUT);
  pinMode(4, INPUT);
  pinMode(2, INPUT);

  pinMode(Echo, INPUT);
  pinMode(Trig, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  stop();
}

/* Updates frequently */
void loop() {
  myservo.write(90);
  distance = Distance_test();

  if (distance < 30) { //change condition to switch lanes
    //Switch lanes
    if (current_lane == 0) {
      switch_to_left();
      current_lane = 1;
    }
    else if (current_lane == 1) {
      switch_to_right();
      switch_to_right();
      current_lane = 0;
    }
  }
  else if(distance < 40){
    carSpeed = 80;
  }
  else if(distance < 50){
    carSpeed = 100;
  }
  else if(distance > 50){
    carSpeed = 130; 
  }
  
  if (LT_R) {
    left();
    while (LT_R);
  }
  else if (LT_L) {
    right();
    while (LT_L);
  }
  else if (!LT_L && !LT_M && !LT_R) {
    forward();
    delay(1);
  }
}//Loop ends
