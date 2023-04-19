
/*
* ok, so this one I am kind of embaressed of. we had several weeks to do this project and this is all we came up with.
* we did win 2nd place thogh.
*/

int button1 = 13;              // set button 1 in pin 13 
int button2 = 12;              // set button 2 in pin 12


#include <Servo.h> // call servo lybrary  

// name servos 

Servo servo1;      
Servo servo2;


void setup() {
  
  Serial.begin(9600); // set up serial monotor

  pinMode(button1, INPUT_PULLUP);    // set up button 1 (define as input)
  pinMode(button2, INPUT_PULLUP);    // button 2
  
  servo1.attach(5);          // set up servo in pin 5 
  servo2.attach(6);
   

}

void loop() {

   String b1 = String(digitalRead(button1));
   String b2 = String(digitalRead(button2));
  Serial.println(b1 + b2);
  
  if (digitalRead(button1) == LOW) {

  
   servo1.write(180); 
   servo2.write(180);
  
  } else if (digitalRead(button2) == LOW) {

    
    servo1.write(0);
    servo2.write(0);
   
  
  } else {
    servo1.write(180);
    servo2.write(0);
  }
  
}
