
//Christopher Ho
//500840595
//christopher.j.ho@ryerson.ca

#include <Servo.h>  //servo library
Servo myservo;      // create servo object to control servo

//Status checkpoints
bool find_parking = true;
bool adjust_spacing = true;
bool final_position = false;

//Ultrasonic sensor variables
int Echo = A4;
int Trig = A5;
int distance = 100;

//Line Tracking IO define
#define LT_R !digitalRead(10)
#define LT_M !digitalRead(4)
#define LT_L !digitalRead(2)

//motor controller pins
#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11
#define carSpeed 100
#define carSpeed2 100

//Global variable for angle of servo
int angle = 0; //0 is default

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

/* Sub-function that completes full parallel parking sequence */
void parallel_park() {
  left();
  delay(1900); //works between 1200, 1300, 1400 

  back(); //reverse into parking at 60 degree angle
  delay(1200);

  while (!LT_M) { //adjust so its facing straight
    right();
  }

  angle = 90;
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
  myservo.write(angle);
  distance = Distance_test();

  if (find_parking) {
    if (distance < 20 && millis() > 1000) { //Check if car is parked
      stop();
      delay(100);

      find_parking = false;
      parallel_park(); // calls sub-function
    }
  }
  
  if (adjust_spacing) { //Move forward/backward
    if (distance > 5 && distance < 10) {
      stop();
      delay(100);
      adjust_spacing = false;
      final_position = true;
    }
    else if (distance >= 10) {
      forward();
    }
    else if (distance <= 5) {
      back();
    }
  }
  
  if (final_position) { //Finished parallel parking
    stop();
  }
  else{
    forward();
  }
}//Loop ends
