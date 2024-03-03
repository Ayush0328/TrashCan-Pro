#include <Servo.h>
#include <Ultrasonic.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

#define TRIG_PIN 11
#define ECHO_PIN 12

Servo myServo;
Ultrasonic ultrasonic(TRIG_PIN, ECHO_PIN);

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};
bool correct = false;

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

LiquidCrystal_I2C lcd(0x27, 16, 2);  

unsigned long previousMillis = 0;  // Variable to store the previous time
const unsigned long interval = 1000;  // Interval in milliseconds (1 second)

char correctAnswer;
bool answered = false;
int currentQuestion = 0; // Variable to track current question

void setup() {
  Serial.begin(9600);
  myServo.attach(10);  // Attach servo to pin 10
  lcd.backlight();
  lcd.init(); 
}

void waitForAnswer(){
  Serial.print("wait for answer: ");
  answered = false;

  while(!answered){
    char customKey = customKeypad.getKey();
    if(customKey){
      if (customKey == correctAnswer){
        answered = true;
        correct = true;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Correct answer!");
        lcd.setCursor(0,1);
        lcd.print("Wave to the robot!");
        Serial.println("correct answer");
        break;
      }
      else if (customKey == '*'){
        lcd.scrollDisplayRight();
        Serial.println("hellorfhwuisfnsiv");
        Serial.println(customKey);
      }
      else if (customKey == '#'){
        lcd.scrollDisplayLeft();
        Serial.println(customKey);
        Serial.println("hellorfhwuisfnsiv");
      }
      else if ( (customKey == 'A' ||customKey == 'B' || customKey == 'C' || customKey == 'D') && customKey !=correctAnswer ){
        answered = true;
        correct = false;
        lcd.clear();
        lcd.setCursor(5,0);
        lcd.print("Wrong!");
        lcd.setCursor(0,1);
        lcd.print("Try again");
        delay(2500);
      }
    }
  }
}

void askQuestion(){
  Serial.print("ask question: ");
  lcd.clear();
  switch(currentQuestion) {
    case 0:
      lcd.setCursor(0,0);   //Set cursor to character 2 on line 0
      lcd.print("Do you recycle?");
      lcd.setCursor(0,1);   //Move cursor to character 2 on line 1
      lcd.print("a.Yes b.No c.Maybe");   
      correctAnswer='A';
      break;
    case 1:
      lcd.setCursor(0,0);
      lcd.print("What is recycling?");
      lcd.setCursor(0,1);
      lcd.print("a.Goofing b.Reusing c.Celebrating");
      correctAnswer='B';
      break;
    case 2:
      lcd.setCursor(0,0);
      lcd.print("Do you recycle paper?");
      lcd.setCursor(0,1);
      lcd.print("a.Of course b.Maybe c.Not at all");
      correctAnswer='A';
      break;
    case 3:
      lcd.setCursor(0,0);
      lcd.print("Do you recycle cans?");
      lcd.setCursor(0,1);
      lcd.print("a.No b.Sometimes c.Of course");
      correctAnswer='C';
      break;
  }
  currentQuestion++; // Move to next question
}

void loop() {
  if(currentQuestion < 4) {
    askQuestion();
    waitForAnswer();
  }

  if(correct) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Wave to the robot!");
    unsigned long previousMillis = millis();  // Get the current time
    
    while(millis() - previousMillis < 5000) {
      // Check for ultrasonic sensor input here
      unsigned int distance = ultrasonic.read();
      if (distance < 20) {  // Adjust distance threshold as needed
        // Object detected within 20cm, move servo to a specific angle
        myServo.write(0);  // Example angle, adjust as needed
        delay(4000); //keep the door open for 4 seconds
        // No object detected, return servo to default position
        myServo.write(90);  // Example angle, adjust as needed
      }
      // Wait for 5 seconds before resetting to the next question
    }
    correct = false;
  }

  if(currentQuestion == 4) {
    currentQuestion = 0; // Reset to first question
  }
}


