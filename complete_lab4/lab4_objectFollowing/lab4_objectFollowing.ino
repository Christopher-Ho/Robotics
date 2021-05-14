//Christopher Ho
//500840595
//christopher.j.ho@ryerson.ca

#include <Servo.h>  //servo library
Servo myservo;      // create servo object to control servo

//Ultrasonic sensor variables
int Echo = A4;
int Trig = A5;
int rightDistance = 0, leftDistance = 0;

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
int angle = 90;

/* Independent timed event */
const unsigned long eventInterval = 100;
unsigned long previousTime = 0;

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

void setup() {
  myservo.attach(3, 700, 2400);  // attach servo on pin 3 to servo object
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
  unsigned long currentTime = millis();

  /* Search for object every 100ms */
  if (currentTime - previousTime >= eventInterval) {
    myservo.write(90);
    leftDistance = Distance_test();
    rightDistance = Distance_test();
    //    if (angle == 60) {
    //      angle = 120;
    //      myservo.write(120);
    //      rightDistance = Distance_test();
    //    }
    //    else if (angle == 120) {
    //      angle = 60;
    //      myservo.write(60);
    //      leftDistance = Distance_test();
    //    }
    previousTime = currentTime;
  }//end ultrsonic

  /* Line sensor */
  if (LT_M && LT_R && LT_L) { /* if all three sensors triggered */
    back();
    delay(500);

    left();
    delay(500);
    while (LT_R);
  }
  else if (LT_R) { /* right sensor triggered, reverse then turn left */
    back();
    delay(500);

    left();
    delay(500);
    while (LT_R);
  }
  else if (LT_L) { /* left sensor triggered, reverse then turn right */
    back();
    delay(500);

    right();
    delay(500);
    while (LT_L);
  }
  else { /* no sensors triggered, keep moving forward */
    /* follow object */
    if ((rightDistance > 70) && (leftDistance > 70)) {
      stop();
    }
    else if ((rightDistance >= 20) && (leftDistance >= 20)) {
      forward();
    }
    else if ((rightDistance <= 10) && (leftDistance <= 10)) {
      back();
    }
    else if (rightDistance - 3 > leftDistance) {
      left();
    }
    else if (rightDistance + 3 < leftDistance) {
      right();
    }
    else {
      stop();
    }// ultrasonic sensor code
  }//line sensor code

}//Loop ends
