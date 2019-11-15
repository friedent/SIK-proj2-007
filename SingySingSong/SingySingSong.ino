#include <SD.h>
#include <Servo.h>

String Notes[] = {"C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#3", "A3", "A#3", "B3", "C4", "C#4", "D4", "D#4", "E4", "F4", "F#4", "G4", "G#4", "A4", "A#4", "B4", "C5", "C#5", "D5", "D#5", "E5", "F5", "F#5", "G5", "G#5", "A5", "A#5", "B5"};
float Frequencies[] = {130.81, 138.59, 146.83, 155.56, 164.81, 174.61, 185, 196, 207.65, 220, 233.08, 246.94, 261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392, 415.3, 440, 466.16, 493.88, 523.25, 554.37, 587.33, 622.25, 659.25, 698.46, 739.99, 783.99, 830.61, 880, 932.33, 987.77};
const int SPEAKER = 5;
File myFile;
Servo myServo;
void setup() {
  // put your setup code here, to run once:
  myServo.attach(1);
  myServo.write(90);
  delay(500);
  Serial.begin(9600);
  if(!SD.begin(4)){ //CS Port
    Serial.println("SD Card Initialization failed");
    while(1);
  }
  Serial.println("here");
  
  myFile = SD.open("JB2.txt");
  if (myFile) {
    Serial.println("JB2.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      String line = myFile.readStringUntil(10);
      Serial.println(line);
      myServo.write(90);
      play(line.substring(0, line.indexOf(' ')), line.substring(line.indexOf(' ') + 1).toFloat());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening JB2.txt");
  }
  pinMode(SPEAKER, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  
}

void play(String note, float beats) {
  int numNotes = 36;
  int index = 0;
  int beatlength = 450;
  if("0".equals(note)){
    noTone(SPEAKER);
  }else{
    for(int i = 0; i < numNotes; i++){
      if(Notes[i]==note){
        index = i;
      }
    }
    //Serial.println(note + "+" + "+" + Frequencies[index] + beats);
    tone(SPEAKER, Frequencies[index], beats * beatlength);
  }
  delay(beats * beatlength);
  delay(50);
}
