
//Pin Controls


const int AIN1 = 13;
const int AIN2 = 12;
const int PWMA = 11;
const int PWMB = 10;
const int BIN2 = 9;
const int BIN1 = 8;
const int TRIG = 2;
const int ECHO = 3;
const int SWITCHPIN = 7;
const int LEFTSWITCH = 6;

//Distance sensor
const int TOOFAR = 2;

void setup() {
  Serial.begin(9600);
  pinMode(SWITCHPIN, INPUT_PULLUP);
  pinMode(LEFTSWITCH, INPUT_PULLUP);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

}

void loop() {
  if(digitalRead(SWITCHPIN)==LOW){
    float distance = getDistance();
    if(distance>TOOFAR){
      forward(100);
    }else if(distance<=TOOFAR){
      reverse(100);
      delay(500);
      halt();
      delay(100);
      if(digitalRead(LEFTSWITCH)==HIGH){
        turn90Right(100);
      }else{
        turn90Left(100);
      }
      
    }
  }else{
    halt();
  }
}

//------------------FUNCTIONS-------------------------------

//RETURNS THE DISTANCE MEASURED BY THE HC-SR04 DISTANCE SENSOR
float getDistance()
{
  float echoTime;                   //variable to store the time it takes for a ping to bounce off an object
  float calculatedDistance;         //variable to store the distance calculated from the echo time
  
  //send out an ultrasonic pulse that's 10ms long
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10); 
  digitalWrite(TRIG, LOW);

  echoTime = pulseIn(ECHO, HIGH);      //use the pulsein command to see how long it takes for the
                                          //pulse to bounce back to the sensor

  calculatedDistance = echoTime / 148.0;  //calculate the distance of the object that reflected the pulse (half the bounce time multiplied by the speed of sound)
  
  return calculatedDistance;              //send back the distance that was calculated
}

void spinMotor(int motorSpeed, int motor){ //0 for motor A, 1 for motor B
  if(motor == 0){
      if (motorSpeed > 0)                                 //if the motor should drive forward (positive speed)
      {
        digitalWrite(AIN1, HIGH);                         //set pin 1 to high
        digitalWrite(AIN2, LOW);                          //set pin 2 to low
      }
      else if (motorSpeed < 0)                            //if the motor should drive backward (negative speed)
      {
        digitalWrite(AIN1, LOW);                          //set pin 1 to low
        digitalWrite(AIN2, HIGH);                         //set pin 2 to high
      }
      else                                                //if the motor should stop
      {
        digitalWrite(AIN1, LOW);                          //set pin 1 to low
        digitalWrite(AIN2, LOW);                          //set pin 2 to low
      }
      analogWrite(PWMA, abs(motorSpeed));                 //now that the motor direction is set, drive it at the entered speed
  }else {
      motorSpeed = motorSpeed * 1.03;
      if (motorSpeed > 0)                                 //if the motor should drive forward (positive speed)
      {
        digitalWrite(BIN1, HIGH);                         //set pin 1 to high
        digitalWrite(BIN2, LOW);                          //set pin 2 to low
      }
      else if (motorSpeed < 0)                            //if the motor should drive backward (negative speed)
      {
        digitalWrite(BIN1, LOW);                          //set pin 1 to low
        digitalWrite(BIN2, HIGH);                         //set pin 2 to high
      }
      else                                                //if the motor should stop
      {
        digitalWrite(BIN1, LOW);                          //set pin 1 to low
        digitalWrite(BIN2, LOW);                          //set pin 2 to low
      }
      analogWrite(PWMB, abs(motorSpeed));                 //now that the motor direction is set, drive it at the entered speed
  }
}

void forward(int motorSpeed){
  spinMotor(motorSpeed, 0);
  spinMotor(motorSpeed, 1);
}

void turnLeft(int motorSpeed){
  spinMotor(motorSpeed/1.03, 1);
  spinMotor(0, 0);
}

void turnRight(int motorSpeed){
  spinMotor(motorSpeed, 0);
  spinMotor(0, 1);
}

void halt(){
  forward(0);
}

void pirouetteTurn(int motorSpeed){
  spinMotor(motorSpeed, 0);
  spinMotor(-1*motorSpeed, 1);
}

void reverse(int motorSpeed){
  forward(-1*motorSpeed);
}

void turn90Left(int motorSpeed){
  turnLeft(motorSpeed);
  delay(70000/motorSpeed);
  halt();
}

void turn90Right(int motorSpeed){
  turnRight(motorSpeed);
  delay(70000/motorSpeed);
  halt();
}
