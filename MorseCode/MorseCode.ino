/*
 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified 7 Nov 2016
 by Arturo Guadalupi


*/

// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const byte rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
const byte readMode = 8;
const byte modeButton = 7;
const byte button = 9;

char readAlphabet;
byte q;
byte p;

byte modeState = 0;
byte old_modeState = 0;
byte currentReadMode = HIGH;
int redbuttonactive = LOW;

byte state = 0;
int secondsOn = 0;
int secondsOff = 0;
byte currentLetter = 0;
byte i =0;
char morseInput[17]; 

char data;
char letterBefore[6];
char newWord[10];
int k;
int j;

String morseAlphabet[36] = {"._ ", "_... ", "_._. ", "_.. ", ". ",
".._. ", "__. ", ".... ", ".. ", ".___ ", "_._ ", "._.. ", "__ ", 
"_. ", "___ ", ".__. ", "__._ ", "._. ", "... ", "_ ", ".._ ", "..._ ",
".__ ", "_.._ ", "_.__ ", "__.. ",".____ ","..___ ","...__ ", "...._ ","..... ","_.... ",
"__... ","___.. ","____. ","_____ "};

char alphabet[37] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

//Beggining of stringLength Function******************
int stringLength(String wordToCheck)
{
  int i = 0;
  while(wordToCheck[i]!= '\0') //Iterates I until null terminator is found
  {
    i++;
  }
  return i;
}
//End of string length function***********************

//Beggining of translateLetter Function***************
char translateLetter(char letterBefore[10])
{
  int wordLength;
  int wordLength2 = stringLength(letterBefore);
  
  for(int i = 0; i<36; i++)                         //This loop will check every possible letter and number
  {
   wordLength = stringLength(morseAlphabet[i]);
   
   if(wordLength == wordLength2)                   //If the lengths of the string match then check to see if they are identical otherwise move to the next string
   {
    for (int j = 0; j<wordLength; j++)
    {
      if(letterBefore[j]!= morseAlphabet[i][j])   //If there is a mismatch then break this loop and look at the next string
      {
        break;
      }
      
      if(morseAlphabet[i][j] == ' ' && letterBefore[j] == ' ')   //If the strings both reach the end marked by a whitespace then the word matches
      {
        //Serial.println(alphabet[i]);
        return alphabet[i];                                      //If there is a match return the letter at index i (alphabet[i])
      }
    }
   }
 
  }
  return '$';                                                   // $ Chosen to be the character to show that no match was found
}
//End of translateLetter Function***********


LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  pinMode(readMode, OUTPUT);
  pinMode(modeButton, INPUT);
  pinMode(button, INPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  digitalWrite(readMode, currentReadMode);
  while(currentReadMode == HIGH){
    lcd.setCursor(p,q);
    
    modeState = digitalRead(modeButton);
  
    if(modeState != old_modeState) {
      if(modeState == HIGH){
        currentReadMode = 1 - currentReadMode;
        digitalWrite(readMode, currentReadMode);
        //Serial.println("button pressed");
        redbuttonactive = HIGH;

        lcd.clear();
        p = 0;
        q = 0;
      }
      else{
      //Serial.println("button released");
      }
    }
    old_modeState = modeState;
    delay(50);
    //what needs to be done on when readmode is high
    
    if(Serial.available()){
      lcd.write(Serial.read());
      p++;
    }
  }
  
  while(currentReadMode == LOW){
    modeState = digitalRead(modeButton);
    
    
    if(modeState != old_modeState) {
      if(modeState == HIGH){
        currentReadMode = 1 - currentReadMode;
        digitalWrite(readMode, currentReadMode);
        //Serial.println("button pressed");
        redbuttonactive = LOW;
        
        lcd.clear();
        for(int g = 0; g < 17; g++){
          morseInput[g] = ' ';
        }
        p = 0;
        q = 0;
      }
      else{
        //Serial.println("button released");
      }
      
      
    }
    old_modeState = modeState;
    delay(50);
      
    state = digitalRead(button);

    secondsOn = 0;
    secondsOff = 0;
    
    if(state == HIGH){
      redbuttonactive = HIGH;
      while(digitalRead(button) == HIGH && secondsOn < 80){
        secondsOn++;
        delay(25);
      }
    }
    else if(state == LOW && redbuttonactive == LOW){
      
    }
    else if(state == LOW && redbuttonactive == HIGH){
      while(digitalRead(button) == LOW && secondsOff < 70){
        secondsOff++;
        delay(25);
      }
      if(secondsOff >= 70){
        redbuttonactive = LOW;
        morseInput[currentLetter] = ' ';
        morseInput[currentLetter +1] = '\0';
        Serial.print("Morse Input sent: ");
        Serial.println(morseInput);
        data = translateLetter(morseInput);
        Serial.write(data);
        lcd.clear();
        p =0;
        currentLetter=0;
        p = 0;
        
      }
    }

    if(redbuttonactive == HIGH){
      if(secondsOn < 30 && secondsOn > 0){
        morseInput[currentLetter] = '.';
        currentLetter++;
        lcd.write('.');
        p++;
        //Serial.println(". has been added");
      }
      else if(secondsOn >= 30){
        morseInput[currentLetter] = '_';
        currentLetter++;
        lcd.write('_');
        p++;
        //Serial.println("_ has been added");
      }
      if(secondsOff >= 30 && secondsOff < 70){
        morseInput[currentLetter] = ' ';
        morseInput[currentLetter +1] = '\0';
        
        Serial.print("Morse Input sent: ");
        Serial.println(morseInput);
        data = translateLetter(morseInput);
        Serial.write(data);
        lcd.clear();
        p =0;
        currentLetter=0;
        p = 0;
      }
    }
    
    
  }
}

