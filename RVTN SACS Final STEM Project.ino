
/*  
* When the predator is close, flap wings vary hard and turn the RGBs red and set LCD to red with skull custom character play a buzzer 
* And when a button is pressed turn the buzzer off 
*  
* When the predator it at a medium distance flap wings a little higher. turn the RGBs yellow, and a buzzer will buzz and set LCD to "Warning!" and buzz a buzzer 
* And when a button is pressed, turn the buzzer off 
*  
* When the predator is far, turn RGBs green and turn LCD screen to safe with smiley face, and when a button is pressed, the buzzer will buzz 
*/ 

  

#include <LiquidCrystal.h>              //tell Arduino we are using a lcd and to include the built in functions for it 
  

LiquidCrystal lcd(35,34,33,32,31,30);   // set up the lcd pins 


//set up skull custom character 
byte skull[] = { 
B00000, 
B01110, 
B10101, 
B11011, 
B01110, 
B01110,  
B00000, 
B00000
}; 

  

//set up smile custom character 
byte smile[] = { 
B00000, 
B00000, 
B00000, 
B01010, 
B00000, 
B10001, 
B01110, 
B00000 
}; 

  
int button = 13;              // set button in pin 13 
const int buzzer = 12;        // set the buzzer in pin 12 
  

//name variables and set up pin 
int trigger=52; 
int echo=53; 


//name variables to hold distance 
int duration=0; 
float distance=0.0; 
  


//set up RGB1 pins/variables 
int r=11; 
int g=10; 
int b=9; 

  
//set up RGB2 
int r2=4; 
int g2=3; 
int b2=2; 
  

//tell Arduino that we will be using a servo and to retrieve the template from the library. 
#include <Servo.h> 
  

//name servos 
Servo leftWing;     // set up left wing variable 
Servo rightWing;    // set up right wing variable 
  

  

void setup() { 
  

  lcd.createChar(2, smile);        // set up smile variable 
  lcd.createChar(1, skull);        // set up skull variable 
  

  lcd.begin(16,1);                 //tell the lcd that the display is 16 characters wide and 1 characters high 
  lcd.clear();                     //clear the display 
  

pinMode(buzzer, OUTPUT);           // set up buzzer output 
pinMode(5, INPUT_PULLUP);          // set up button 
   

   

  // set up utra sonic sensor
pinMode(trigger,OUTPUT); 
pinMode(echo,INPUT); 



Serial.begin(9600); // set up ther serial monitor
  

  

//set RGB1 up as an output 
pinMode(r,OUTPUT); 
pinMode(g,OUTPUT); 
pinMode(b,OUTPUT); 

  

  

//set up RGB2 as output 
pinMode(r2,OUTPUT); 
pinMode(g2,OUTPUT); 
pinMode(b2,OUTPUT); 

  


//set up servo pins 
rightWing.attach(5);          // set up right wing in pin 5 
leftWing.attach(6);        // set up the left wing in pin 6 

pinMode(buzzer, OUTPUT);     //set the buzzer as an output 
   

} 

  

  

void loop() {                  

// call up ultra-sonic sensor function 
us();
  

// make wings move fast turn rgbs red make a lcd say danger! (skull) and play a buzzer 
if(distance<21){ 
  

  nobuz();                    //call up when the is button pressed turn buzzer off function 
  lcd.clear();                // clear the lcd   
  lcd.setCursor(0, 0);        //set the cursor to the 0,0 position 
  lcd.print("Danger!");       //print Danger! starting at the position 
  lcd.write(byte(1));         //call up skull character 
  rgb1(255,0,0);              //make RGB1 turn red 
  rgb2(255,0,0);              //make RGB2 turn red 
  servos(180,0,250);          //turn mini servos and flap wings fast 
  servos(0,180,500);          //turn servos opposite way 
} 
  

//make wing spin slower and less make rgb go yellow and when a button is pressed turn off a buzzer 
else if ((distance>20)&&(distance<31)){ 
    nobuz();                     //all up the when button pressed turn buzzer off function 
    lcd.clear();                 //clear the lcd 
    lcd.setCursor(0, 0);         //set the cursor to the 0,0 position 
    lcd.print("Warning!");       //print Warning! starting at the position 
    rgb1(255,255,0);             //set RGB1 to yellow 
    rgb2(255,255,0);             //set RGB2 to yellow 
    servos(110,0,250);           //make servos move slow to flap wings 
    servos(0,110,500);           //counter move servos to bring wing back down 
  } 
  

//make RGBs go green when a button is pressed turn a buzzer on and make a lcd print safe (smile) 
  else{ 
    buzbuton();                       //hen a button is pushed turn a buzzer on 
    lcd.clear();                      //clear the lcd 
    lcd.setCursor(0, 0);              //set the cursor to the 0,0 position (top left corner) 
    lcd.print("Safe ");               //print safe 
    lcd.write(byte(2));               //print the smile custom character 
    rgb1(0,255,0);                    //make RGB1 green 
    rgb2(0,255,0);                    //make RGB2 green 
  

  }
}  
  
/* 
* Set up function(s) for void loop 
*/ 
  

  

  

//a function to get ultrasonic sensor readings 
void us(){                               
  
//turn off sensor 
  digitalWrite(trigger,LOW);         // turn off ping    
  delayMicroseconds(2);              // wait 2 microseconds 


//send trigger ping 
  digitalWrite(trigger,HIGH);        // send ping 
  delayMicroseconds(10);             // wait 10 microseconds 
  digitalWrite(trigger,LOW);         // turn off ping 


//get data and calculate distance 
  duration=pulseIn(echo,HIGH);       // receive ping 
  distance=((float)(duration))/58.2;  // calculate ping 
  

//display distance on serial monitor when the Arduino is plugged in 
  Serial.print("distance= ");        //tell the serial monitor to say distance= 
  Serial.print(distance);            //say what the distance actually is
  Serial.println(" cm");             //print cm at the end and make a new line 

} 

  

//set up RGB template 
void rgb1(int rsig,int gsig,int bsig){ 

  analogWrite(r,rsig);    //setup red RGB as Redsignal 
  analogWrite(g,gsig);    //setup green RGB as Greensignal 
  analogWrite(b,bsig);    //setup blue RGB as Bluesignal 

} 

  

//set up RGB2 funtion template 
void rgb2(int r2sig,int g2sig,int b2sig){ 
  
  analogWrite(r2,r2sig);    //setup red RGB as Redsignal 
  analogWrite(g2,g2sig);    //setup green RGB as Greensignal 
  analogWrite(b2,b2sig);    //setup blue RGB as Bluesignal
  
} 

  

//set up servo function template 
void servos(int leftsignal,int rightsignal,int delaytime){ 
  
leftWing.write(leftsignal);   //setup Servo as left wing 
rightWing.write(rightsignal); //setup Servo as rightwing 
delay(delaytime);             //setup delay 

}
  

  

//set up buzzer function template (this was not used in the final version of our code) 
void buz(int buztone,int delaytime){ 
  tone(buzzer,buztone); //setup buzzer 
  delay(delaytime);     //setup buzzer delay 
} 

  
//set up function whare when the button is pressed the buzzer plays 
void buzbuton(){ 
  
  digitalRead(13);                         //read the button 
  if(digitalRead(button) == LOW){          // if the button is pushed 
  tone(buzzer, 1000);                      // turn the buzzer on 
  } 
else{ 
  noTone(buzzer);                          // turn the buzzer off 
  } 
} 

  

//set up function where the buzzer plays until the button is pushed 
void nobuz(){ 
  
digitalRead(13);                          // read the button 
  if(digitalRead(button) == LOW){         // if the button is pushed 
  noTone(buzzer);                         // turn the buzzer off 
  } 
else{ 
  tone(buzzer,1000);                      // turn the buzzer on 
  } 
} 

  

  

/* 
* Variable definitions: 
*  
* buzzer == buzzer 
*  
* r == red RGB pin 
* g == green RGB pin 
* b == blue RGB pin 
*  
* 2 == red RGB2 pin 
* g2 == green RGB2 pin 
* b2 == blue RGB2 pin 
*  
* echo == echo pin for ultrasonic sensor 
* trigger == trig pin for ultrasonic sensor 
* duration == integer for the deration of the sound going to and from the sensor 
* distance == a decimal for the actual distance of the object. 
*  
* button == the pin for a push button 
*  
* leftWing == the servo for the left wing on the robot 
* rightWing == the servo for the right wing on the robot 
*  
* skull == custom character skull 
* smile == custom character smile face 
*  
* LCD === lcd screen/liquid crystal 
*  
* buzzer == speaker that sounds at certain points  
*  
*  
* Sources: 
*  
* sparkfun: 
* https://learn.sparkfun.com/tutorials/sparkfun-inventors-kit-experiment-guide---v40/introduction 
*  
* makerguide: 
* https://www.makerguides.com/character-lcd-arduino-tutorial/ 
*  
* whenever we did not know how to code something we learned it from one of these sites 
*  
*/ 

 
