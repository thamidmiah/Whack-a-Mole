#include <Servo.h>

Servo scoreservo;
int servo_pos = 90;

// Game Component Setup
int gameLED[] = {4,5,6, 10, 11, 12}; // BLUE LEDS FOR PLAYER 1
int whiteLED = 9; // WHITE SCORE LEDS

int buzzer = 8;

int p1Button = 2; // Player 1 Button
int p2Button = 3; // Player 2 Button

int flag1 = 0; // Debouncing for button 1
int flag2 = 0; // Debouncing for button 2

// PLAYER SCORE
volatile int p1score;
volatile int p2score;

// Game Setup Variables
int timeDelay=500; // time delay between lights on/off
int whiteLEDOn = 1; // whether point has been scored

int randNum; // for selection of random pin
int lastRand;
int probCount1, probCount2;

//setup interrupt, button input and LED outputs
void setup() {

  Serial.begin(9600);

  //Setting Servo 
  scoreservo.attach(7);

  //An interrupt is set up, not in loop.
  attachInterrupt(0, playerOneInput, FALLING); 
  attachInterrupt(1, playerTwoInput, FALLING);

  // Configuring Components
  for (int i=0; i<6; i++){
    pinMode(gameLED[i], OUTPUT);
  }

  pinMode(p1Button, INPUT);
  pinMode(p2Button, INPUT);
  pinMode(whiteLED, OUTPUT);

  pinMode(buzzer, OUTPUT);

  // Setting score to zero.
  p1score = 0; 
  p2score = 0;

  lastRand = -99;
  probCount1 = 0;
  probCount2 = 0;
}

// GAMELOOP
void loop() {

  /* Pseudocode for servo
    if p1score > p2score : turn servo 180 towards p1
    else if p2score > p1score : turn servo 180 towards p2
    else if p1score == p2score : no deflection
  */

  while(p1score<10 && p2score<10){ 

    if(p1score>p2score){
      scoreservo.write(0);
    }
    else if(p2score > p1score){
      scoreservo.write(180);
    }
    else{
      scoreservo.write(90);
    }

    // MY RANDOM GENERATOR
    randNum = random(6);

    if(randNum <= 2){
      if(lastRand == -99)
        probCount1 += 1;
      if(lastRand >=0 && lastRand<= 2)
        probCount1 +=1;
    }

    else{
      if(lastRand = -99)
        probCount2 += 1;
      if(lastRand >= 3 && lastRand <= 5)
        probCount2 += 1;
    }

    if(probCount1 > 2){
      int temp = random(3,6);
      digitalWrite(gameLED[temp], HIGH);
      probCount1 = 0;
      lastRand = -99;
      delay(timeDelay);
      digitalWrite(gameLED[temp], LOW);
      delay(timeDelay);
      continue;
    }
    else if(probCount2 > 2){
      int temp = random(0,3);
      digitalWrite(gameLED[temp], HIGH);
      probCount2 = 0;
      lastRand = -99;
      delay(timeDelay);
      digitalWrite(gameLED[temp], LOW);
      delay(timeDelay);
      continue;
    }

    digitalWrite(gameLED[randNum], HIGH);
    delay(timeDelay);
    digitalWrite(gameLED[randNum], LOW);
    delay(timeDelay);

    lastRand = randNum;
  }

  if(p1score >=10 || p2score >=10){
    // Flashes all LEDs when score is 10, i.e game is over
    if(p1score>=10 && p2score <10){
      digitalWrite(4,HIGH); //Red
      digitalWrite(5,HIGH); //Blue
      digitalWrite(6,HIGH); //Green
    }
    else{
      digitalWrite(10, HIGH);
      digitalWrite(11,HIGH);
      digitalWrite(12,HIGH);
    }

    digitalWrite(9, HIGH); //White
  }

}

// PLAYER 1 
void playerOneInput() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();

  if (interrupt_time - last_interrupt_time > 200) 
  {
    if(digitalRead(gameLED[0]) == HIGH || digitalRead(gameLED[1]) == HIGH || digitalRead(gameLED[2]) == HIGH){
      tone(buzzer, 85);
      delay(500);        
      noTone(buzzer);
      p1score +=1;
      Serial.print("PLAYER 1 SCORE => ");
      Serial.print(p1score);
      Serial.print("\tPLAYER 2 SCORE => ");
      Serial.println(p2score);
      digitalWrite(whiteLED, HIGH); 
      delay(1000);
      digitalWrite(whiteLED, LOW);
    }
  }
  last_interrupt_time = interrupt_time;
}

// PLAYER 2
void playerTwoInput(){
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();

  if (interrupt_time - last_interrupt_time > 200) 
  {
    if(digitalRead(gameLED[3]) == HIGH || digitalRead(gameLED[4]) == HIGH || digitalRead(gameLED[5]) == HIGH){
      p2score +=1;
      Serial.print("PLAYER 1 SCORE => ");
      Serial.print(p1score);
      Serial.print("\tPLAYER 2 SCORE => ");
      Serial.println(p2score);
      digitalWrite(whiteLED, HIGH); 
      delay(1000);
      digitalWrite(whiteLED, LOW);

      tone(buzzer, 85);
      delay(500);        
      noTone(buzzer);
    }
  }
  last_interrupt_time = interrupt_time;
}
