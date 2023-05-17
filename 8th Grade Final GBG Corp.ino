 
 
/* 
* Angry Beaver; LCD Screen displays game options, Button 1 (red) starts game and player selection. Button 2 (blue) confirms button 1 selection. 
* Button 3 (white) acts as the game-play button. RGB LED's display beaver mood, along with a micro servo. A number generator generates a number based on the number of players that are selected. 
* Once Button 3 (white) is pressed the pre-decided number of times, the "beaver" will become aggravated, whatever player that is selected loses.  
* Spinning wheel dictates how many times Button 3 (white) is pressed per turn.  
*  
* RNG: https://reference.arduino.cc/reference/en/language/functions/random-numbers/random/ 
* millis() reference: https://www.best-microcontroller-projects.com/arduino-millis.html 
*/ 
 
 
 
// System Variables: 
unsigned long lastTime; // a variable to hold the last recorded millis() (unsigned long just means really big int) 
 
// Game Variables: 
 
  // player variables: 
int numplrs = 1;  // a variable to hold the number of players 
int curntplr = 1; // a variable to hold who's turn it is 
 
  // Game Button variables: 
int numPokes;     // a variable to hold how many pokes it takes to anger the beaver 
int curntPokes;   // a variable to hold how many times you have already poked the beaver 
int neededPokes;  // a variable for how many pokes the current player has
int yourPokes;    // a variable for how many pokes the current player needs to do
 
// Hardware Variables: 
 
  // Setup button pins 
int button1 = 22;  // set up button 1 
int button2 = 23;  // set up button 2 
int button3 = 25;  // set up button 3 
 
  // Setup RGB pins 
int red = 10;  // RGBs red pin 
int green = 9; // RGBs green pin 
int blue = 8;  // RGBs blue pin 
 
  // Setup Servo pins 
#include <Servo.h> // call/include, the servo library   
Servo tail; // let Arduino know that tail is a servo 
 
  // setup LCD 
#include <LiquidCrystal.h> // tell Arduino to use the LCD library 
LiquidCrystal lcd(24, 27, 26, 29, 28, 31); // setup LCD variable and LCD pins 
 
 
 
// setup binary array for the custom LCD character 
byte Skull[] = { 
  0b00000, 
  0b01110, 
  0b10101, 
  0b11011, 
  0b01110, 
  0b01110, 
  0b00000, 
  0b00000 
}; 
 
   
void setup() { 
   
  Serial.begin(9600); // setup serial monitor 
 
  // setup buttons as High/Low input 
  pinMode(button1, INPUT_PULLUP);  // set up button 1 as input 
  pinMode(button2, INPUT_PULLUP);  // set up button 2 as input 
  pinMode(button3, INPUT_PULLUP);  // set up button 3 as input 
 
  // setup RGB pins as output 
  pinMode(red, OUTPUT); 
  pinMode(green, OUTPUT); 
  pinMode(blue, OUTPUT); 
 
  tail.attach(5);  // set up the tail servo in pin 5  
   
  lcd.begin(16,2); // tell Arduino how large your LCD is 
  lcd.clear();     // clear the LCD 
 
  lcd.createChar(0, Skull); // define skull as char 0 
 
  randomSeed(analogRead(0)); // set up the R.N.G. (random number generator) 
 
  servo(90);    // tell the servo to stop (we only want this done once, so it is in void setup) 
 
} 
 
 
void loop() { 
 
  beginGame(); // call the function to start the game 
 
} 
 
 
// function for game navigation 
void beginGame() {  
 
  rgb(0,225,0); // turn the RGBs green 
 
  lcd.setCursor(0,0); // set the LCD cursor in the top left 
  lcd.print("Hello, Beaver!"); // print Hello, Beaver! on the LCD 
  
  plrselect(); // call the function to determine how many players there are 
  
  game();      // play the game 
} 
 

// setup function to determine how many players there are 
void plrselect () { 
  
  bool isReading = true;   // true false variable for if the user is reading instructions 
  bool isSelecting = true; // true false variable for if the user is curently selecting the number of players 

  int part = 2;       // variable to hold what "slide" the LCD is showing 
  int dlytime = 3000; // variable to tell the LCD how long to wait before changing "slides"
 
  while (isReading == true){ // while the player is reading the instructions (has not clicked button 1) 
    if ((millis() - lastTime) > dlytime) { lastTime = millis(); // fancy delay, but still necessary (more info at bottom)
      if (part == 2){ // if the LCD was just showing the second “slide” 
         
        // print Press B1 to on the first line of the LCD
        lcd.clear(); 
        lcd.setCursor(0,0);         lcd.print("Press B1 to");  
 
        // print edit number of on the second lint of the LCD 
        lcd.setCursor(0,1); 
        lcd.print("edit number of "); 
         
        part = 1;       // change the part 
        dlytime = 2500; // change the delay time 
       
      } else { // if the LCD was just showing the first "slide" 
 
        // print players. on the first line of the LCD 
        lcd.clear(); 
        lcd.setCursor(0,0); 
        lcd.print("players."); 
 
        part = 2;       // change the part 
        dlytime = 2000; // change the delay time 
      } 
    } 
    if (digitalRead(button1) == LOW){ // if button 1 is pressed 
      isReading = false; // tell the Arduino that the user is done reading instructions and to break out of the loop 
    }   
  } 
 
    // print Plyers: on the first line of the LCD 
    lcd.clear(); 
    lcd.setCursor(0,0); 
    lcd.print("Players:"); 
 
  while (isSelecting == true){ // while the user has not confirmed the number of players 
 
    // print the number of currently selected players on the second line 
    lcd.setCursor(0,1); 
    lcd.print(numplrs); 
 
    if (digitalRead(button1) == LOW) { // when button 1 is pressed 
 
      if (numplrs < 4) {numplrs ++;} else {numplrs = 2;} // update the number of players 
      while(digitalRead(button1) == LOW); // wait until the button is released  
     
    } 

    if (digitalRead(button2) == LOW) { // when button 2 is pressed 
      isSelecting = false; // tell the Arduino that the user is done selecting the number of players and break out of the loop 
   } 

 
  } 
} 
 
 
// function for the meat of the game 
void game() { 
 
  rgb(0,255,0); // turn the RGBs green in case they are not already 
   
  lcd.clear(); // clear the LCD 
 
  curntPokes = 0; // set the number of current pokes to 0 
  bool plrgoing = true; // tell the Arduino that the game is being played 
   
  // firgure out how many pokes it takes to anger the beaver 
  if(numplrs == 2) {              // if there are 2 players 
    numPokes = random(5,25);      // randomize the number of pokes that it takes to anger the beaver (between 5 and 25) 
  } else if (numplrs == 3) {      // if there are 3 players 
    numPokes = random(7,35);      // randomize the number of pokes (7-35) 
  } else {                        // if there are 4 players 
    numPokes = random(9,45);      // randomize the number of pokes (9-45) 
  } 

  // get how many pokes the first player needs
  yourPokes = random(1,10); // setupvariable for how many pokes the player needs to do
  neededPokes = 0; // set the amount of pokes the current player done to zero
    // pint the number of pokes the player needs to do
  lcd.setCursor(0,1);
  lcd.print(yourPokes);

  curntplr = 1; // set the current player to one if it has not been already

  while (plrgoing == true) { // while the game is being played 
   
    String curntplrS = String(curntplr); // create a variable to hold a string version of which player's turn it is  

    // print who's turn it is to the LCD 
    lcd.setCursor(0,0); 
    lcd.print("Player " + curntplrS + "'s turn!" ); 
 
    if (digitalRead(button3)==LOW){ // if button 3 is pressed (if poked with the stick) 
       
      curntPokes++; // add 1 to the current number of pokes 
      neededPokes++; // add 1 to the curent player pokes
       
       if (!(neededPokes < yourPokes)) { // if needed pokes is the sam or more than your pokes
          if (curntplr < numplrs) {curntplr ++;} else {curntplr = 1;} // add change the current player

          yourPokes = random(1,10); // setupvariable for how many pokes the player needs to do
          neededPokes = 0; 
          // pint the number of pokes the player needs to do
          lcd.setCursor(0,1);
          lcd.print(yourPokes); 
       }
      // print the current number of pokes to the LCD 
      lcd.setCursor(7,1); 
      lcd.print(curntPokes); 
      
       
      while(digitalRead(button3)==LOW); // wait until the button is released 
       
    // get the number of times the player needs to pres the button
    }
 
    if (curntPokes == numPokes) { 
       
      // print BEAVER IS ANGRY! to the LCD 
      lcd.clear();  
      lcd.setCursor(0,0); 
      lcd.print("BEAVER IS ANGRY!");  
 
      plrgoing = false; // tell the Arduino that the game is no longer being played, and that it can break out of the loop 
     
      for (int i = 0; i < 5; i ++) {  // do 5 times 
        rgb(255,0,0); // RGB red 
        servo(30); // servo turn at average speed 
        delay(280); // wait 300 milliseconds 
 
        rgb(0,0,0); // turn the RGB off 
        servo(150); // spin the servo in the opposite direction 
        delay(300); // wait 300 milliseconds 
      }   
    
      rgb(250,0,0); // turn the RGB solid red 
      servo(90); // stop the servo 

      // print the game over message 
      lcd.clear(); 
      lcd.setCursor(2,0); 
      lcd.write(byte(0));  // print the skull custom character 
      lcd.print("GAME OVER"); 
      lcd.write(byte(0));  // print the skull custom character 
      
      // show which player lost 
      lcd.setCursor(0,1); 
      lcd.print("Player " + curntplrS + " looses!"); 

      delay(5000); // wait five seconds 
 
      // print instructions on how to return to the home screen 
      lcd.clear(); 
      lcd.setCursor(0,0); 
      lcd.print("B1 to retun home"); 

      // print instructions on how to replay 
      lcd.setCursor(2,1); 
      lcd.print("B2 to replay"); 

      bool isSelecting = true; // setup variable for if the user is deciding what to do 

      while(isSelecting == true) { // while the user is deciding what to do 
        if (digitalRead(button1) == LOW) {    // if button 1 is pressed 
          isSelecting = false;                // set is Selecting to false 
          while(digitalRead(button1) == LOW); // wait until the button is released 
          beginGame();                        // call the function to start the game over from the top. 
         
        } else if (digitalRead(button2) == LOW) { // if button 2 is pressed 
          isSelecting = false;                    // set is selecting to false 
          while(digitalRead(button2) == LOW);     // wait until the button is released 
          game();                                 // call the function to replay the main part of the game 
        } 
      } 
    } 
  } 
} 
 

// function for RGB LEDs 
void rgb(int r, int g, int b) { 
  analogWrite(red, r);   // red pin 
  analogWrite(green, g); // green pin 
  analogWrite(blue, b);  // blue pin 
} 
 

// setup function to change the speed of the servo more conveniently 
void servo (int tspeed) { 
   tail.write(tspeed); // change the servo speed 
} 

 
/* 
* On line 122, in the preselect function, we use the millis() function because we still want something to happen at a timed interval, but do not want to stop the whole program for it. 
* The Best wat to do this is with millis(). millis() is the time in milliseconds since the program has started. 
* Arduino already does this in the background every time a program is started so we do not have to initiate it. 
*/ 
