#include <SD.h>
#include <Servo.h>

//Pin Controls


const int AIN1 = 13;
const int AIN2 = 12;
const int PWMA = 11;
const int PWMB = 5;
const int BIN2 = 6;
const int BIN1 = 8;
const int SWITCHPIN = 7;
//const int LEFTSWITCH = 6;
const int SERVO = 9;
const int ECHO = 3;
const int TRIG = 2;

//Distance sensor
const int TOOFAR = 10;
const int TOOFARSIDE = 6;



String Notes[] = {"C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#3", "A3", "A#3", "B3", "C4", "C#4", "D4", "D#4", "E4", "F4", "F#4", "G4", "G#4", "A4", "A#4", "B4", "C5", "C#5", "D5", "D#5", "E5", "F5", "F#5", "G5", "G#5", "A5", "A#5", "B5"};
float Frequencies[] = {130.81, 138.59, 146.83, 155.56, 164.81, 174.61, 185, 196, 207.65, 220, 233.08, 246.94, 261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392, 415.3, 440, 466.16, 493.88, 523.25, 554.37, 587.33, 622.25, 659.25, 698.46, 739.99, 783.99, 830.61, 880, 932.33, 987.77};
const int SPEAKER = 10;
const int beatLength = 450;
int i = 0;
int increment = 8; //Reciprocal of beat beats per time
int next = 0;
int nextMove = 0;
File myFile;
File moveFile;
Servo myServo;
void setup() {
  // put your setup code here, to run once:
  if(!SD.begin(4)){ //CS Port
    Serial.println("SD Card Initialization failed");
    while(1);
  }
  myFile = SD.open("JB3.txt");
  moveFile = SD.open("D.txt");
  pinMode(SWITCHPIN, INPUT_PULLUP);
  pinMode(A0, INPUT_PULLUP);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  myServo.attach(SERVO);
  myServo.write(90);
  delay(500);
  Serial.begin(9600);
  
  pinMode(SPEAKER, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  if (myFile) {
    // read from the file until there's nothing else in it:
    if(i == next){
      if (myFile.available()) {
        String line = myFile.readStringUntil(10); //Read file until newline character
        Serial.println(line);
        //myServo.write(90);
        String note = line.substring(0, line.indexOf(' '));
        float beats = line.substring(line.indexOf(' ') + 1).toFloat();
        next = i + increment * beats;
        delay(50);
        play(note, beats);
      }else{
        //myFile.close();
      }
    }
      // close the file:
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening JB3.txt");
  }
  if(moveFile){
    if(i == nextMove){
      if(moveFile.available()){
        String line = moveFile.readStringUntil(10); //Read file until newline character
        Serial.println(line);
        //myServo.write(90);
        String note = line.substring(0, line.indexOf(' '));
        float beats = line.substring(line.indexOf(' ') + 1).toFloat();
        next = i + increment * beats;
        delay(50);
        steps(note, beats);
      }
    }else{
      //moveFile.close();
    }
  }else{
    Serial.println("error Opening D.txt");
  }
  i += 1;

  delay(beatLength/increment);
}

void play(String note, float beats) {
  int numNotes = 36;
  int index = 0;
  if(note.equals("0")){
    noTone(SPEAKER);
  }else{
    for(int i = 0; i < numNotes; i++){
      if(Notes[i]==note){
        index = i;
      }
    }
    //Serial.println(note + "+" + "+" + Frequencies[index] + beats);
    tone(SPEAKER, Frequencies[index], beats * beatLength);
  }
  //delay(beats * beatLength);
  //delay(50);
}

void steps(String action, float beats){
  if(action.equals("f")){
    forward(100);
  }else if(action.equals("r")){
    reverse(100);
  }else if(action.equals("tl")){
    turnLeft(100);
  }else if(action.equals("tr")){
    turnRight(100);
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
