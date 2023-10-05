/* 
Trolling Motor Speed Controller
Joel Niebergal - Oct 2023

License GNU GPL version 3

Program allows you to control the speed of an outboard motor through a dial. In addition there are various
buttons that will run through a number of programs that vary the speed.

Full speed - Moves the throttle to 90% of 70 degrees for as long as you hold the button down to allow you
to control the boat if the wind picks up or if you need your lines and lures to raise up to go over an
obstacle. The led on the button is lit, letting you know the program is running. When the button is 
released, the throttle returns to the original setting on the dial and the led is turned off.

Idle speed - Moves the throttle to 5% throttle to allow you just enough forward speed to maintain control
and not sink your lines while fighting a fish. The led on the button is lit, letting you know the 
program is running. When the button is pushed again, the speed goes back to what was originally set on the 
dial and the led is turned off.

*/

#include <Servo.h>          //include the servo library
Servo myservo;              // create servo object to control a servo

int potpin = 0;             // analog pin 0 used to connect the potentiometer
int pinval = 0;             // variable to read the value from the analog pin
int origpinval = 0;         // to store orginal pin value
int FBState = 0;            // variable for reading the Full pushbutton status
int IBState = 0;            // variable for reading the Idle pushbutton status

const int Fled = 4;         //Full speed button led
const int Iled = 5;         //Idle speed button led
const int FButton = 9;      //Full speed button 
const int IButton = 10;     //Idle speed button
const int FullPin = 65;     //Full speed setting. (93% of 70 degrees) ** Modify to change amount of throttle **
const int IdlePin = 1;      //Idle speed setting. (1% of 70 degrees) ** Modify to change amount of throttle **

//*********************************************************************************************
void setup()
{
  myservo.attach(3);            // attaches the servo on pin 3 to the servo object
  myservo.write(70 - pinval);   // sets the initial servo position to zero or full idle (reversed servo)
  
  pinMode(Fled, OUTPUT);        // set the pins for the button leds to outputs
  pinMode(Iled, OUTPUT);
  pinMode(FButton, INPUT);      // set the pins for the buttons to inputs
  pinMode(IButton, INPUT);
  
  //For safety reasons, does not run the program until the dial is turned to idle. (<=2)
  pinval = analogRead(potpin);                // reads the value of the potentiometer (value between 0 and 1023)
  //scale it to use it with the servo (value between 0 and 70). Servos usually go from 0 to 180 degrees, this is 
  //set to 70 degrees so it doesn't go too far and bind the throttle linkage.
  pinval = map(pinval, 0, 1023, 70, 0);       
    while (pinval > 2) {
      pinval = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023)
      pinval = map(pinval, 0, 1023, 70, 0);   // scale it to use it with the servo (value between 0 and 70)
    } //end while
} //end setup

//**************************************************************************************************************

void loop()
{
  pinval = analogRead(potpin);              // reads the value of the potentiometer (value between 0 and 1023)
  pinval = map(pinval, 0, 1023, 70, 0);     // scale it to use it with the servo (value between 0 and 70)
  pinval = 70 - pinval;                     // servo direction reversed. This done so dial turns clockwise for more power
  myservo.write(pinval);                    // sets the servo position according to the scaled value
  delay(15);                                // waits for the servo to get there

  FBState = digitalRead(FButton);     // read the state of the Full pushbutton value
  if (FBState == HIGH) {            
    delay(250);
    Full();                           // go to Full function
    
  } 
  digitalWrite(Fled, LOW);            // turn off button led

  IBState = digitalRead(IButton);     // read the state of the Idle pushbutton value
  
  if (IBState == HIGH) {              // run the Idle speed function
    delay(250);                       // to debounce button
    Idle();                           // go to Idle function
  } 

 
}  // end loop
//****************************************************************************************************

void Full() {
  digitalWrite(Fled, HIGH);             // turn on button led
  //origpinval = pinval;                // store original pin value
  while (FBState == HIGH) {
    pinval = 70 - FullPin;              // servo mapped from 0 to 70 degrees. This reverses servo direction.
    myservo.write(pinval);
    FBState = digitalRead(FButton);     // read the state of the Full pushbutton value
  }
  //pinval = origpinval;
  //myservo.write(pinval);              // write the original speed to the servo
  digitalWrite(Fled, LOW);              // turn off button led
  delay(250);
} 

void Idle() {
  IBState = 0;                         // set button state to low after pushed
  digitalWrite(Iled, HIGH);            // turn on button led
  //origpinval = pinval;               // store original pin value
  while (IBState == LOW)  {            // run the Idle speed function until the Idle button is pushed again
    pinval = 70 - IdlePin;             // decrease speed. servo mapped from 0 to 70 degrees. This reverses servo direction.
    myservo.write(pinval);
    IBState = digitalRead(IButton);    // read the state of the Idle pushbutton value
  }
  //pinval = origpinval;
  //myservo.write(pinval);             // write the original speed to the servo
  digitalWrite(Iled, LOW);             // turn off button led
  delay(250);                          // to debounce button 
}
