//Christopher Ho
//500840595
//christopher.j.ho@ryerson.ca

#include <Servo.h>  //servo library
Servo myservo;      // create servo object to control servo
int Echo = A4;
int Trig = A5;
int rightDistance = 0, leftDistance = 0, middleDistance = 0;

//Line Tracking IO define
#define LT_R !digitalRead(10)
#define LT_M !digitalRead(4)
#define LT_L !digitalRead(2)

#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11

#define carSpeed 100
long randNumber;

/* Two independent timed events */
const unsigned long eventTime1_Ultrasonic = 100;
const unsigned long eventTime_2_Ultrasonic = 200; //interval in ms
const unsigned long eventTime3_Ultrasonic = 300;

unsigned long previousTime_1 = 0;
unsigned long previousTime_2 = 0;
unsigned long previousTime_3 = 0;

void forward() {
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("go forward!");
}

void back() {
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("go back!");
}

void left() {
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("go left!");
}

void right() {
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("go right!");
}

void stop() {
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  Serial.println("Stop!");
}

/* picks random direction for robot to turn */
void pick_side() {
  randNumber = random(0, 1);
  unsigned long currentTime = millis();
  if (randNumber > 0.5) {
    while (millis() < currentTime + 500) {
      back();
    }

    while (millis() < currentTime + 1000) {
      left();
    }
    while (LT_R);
  }
  else {
    while (millis() < currentTime + 500) {
      back();
    }

    while (millis() < currentTime + 1000) {
      right();
    }
    while (LT_L);
  }
}

/*Ultrasonic distance measurement Sub function */
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
  myservo.attach(3, 700, 2400); // attach servo on pin 3 to servo object
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

void avoid_obstacle() {
  //Evaluate options
  if (rightDistance > leftDistance) {
    right();
    delay(600);
  }
  else if (rightDistance < leftDistance) {
    left();
    delay(600);
  }
  else if ((rightDistance <= 20) || (leftDistance <= 20) || (middleDistance <= 20)) {
    back();
    delay(100);
  }
}

void loop() {
  /* Updates frequently */
  unsigned long currentTime = millis();

  if (LT_M && LT_R && LT_L) { /* if all three sensors triggered */
    while (millis() < currentTime + 500) {
      stop();
    }
    pick_side();
  }
  else if (LT_R) { /* right sensor triggered, reverse then turn left */
    while (millis() < currentTime + 500) {
      back();
    }

    while (millis() < currentTime + 1000) {
      left();
    }
    while (LT_R);
  }
  else if (LT_L) { /* left sensor triggered, reverse then turn right */
    while (millis() < currentTime + 500) {
      back();
    }

    while (millis() < currentTime + 1000) {
      right();
    }
    while (LT_L);
  }
  else { /* no sensors triggered, keep moving forward */
    while (millis() < currentTime + 10) {
      forward();
    }
  }//line

  /* This is event 1 ultrasonic sensor stuff */
  if (currentTime - previousTime_1 >= eventTime1_Ultrasonic) {
    myservo.write(90);
    middleDistance = Distance_test();

    if (middleDistance <= 40) {
      stop();
      myservo.write(120);
      delay(150);
      leftDistance = Distance_test();

      myservo.write(90);
      delay(150);
      middleDistance = Distance_test();

      myservo.write(60);
      delay(150);
      rightDistance = Distance_test();

      avoid_obstacle();
    }
    previousTime_1 = currentTime;
  }//event1

  /* This is event 2 stuff */
  if (currentTime - previousTime_2 >= eventTime_2_Ultrasonic) {
    previousTime_2 = currentTime;
  }//event2

  /* This is event 3 stuff */
  if (currentTime - previousTime_3 >= eventTime3_Ultrasonic) {
    previousTime_3 = currentTime;
  }//event3
  
}//loop
