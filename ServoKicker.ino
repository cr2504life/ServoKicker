/* 
ServoKicker
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

Increased throttle is clockwise --> towards a 0 degree command value
Reduced throttle is counter-clockwise --> towards a 70 degree command value
*/

#include <Servo.h>          //include the servo library
Servo myservo;              // create servo object to control a servo

int cmdval = 70;            // variable to read the value from the analog pin
int cmdmin = 0;             //max throttle limit
int cmdmax = 70;            //low throttle limit
int origval = 0;            // to store orginal throttle value

int FBState = 0;            // variable for reading the Full pushbutton status
int IBState = 0;            // variable for reading the Idle pushbutton status

const int Fled = 4;         //Full speed button led
const int Iled = 5;         //Idle speed button led
const int Incled = 6;       //Incr speed button led
const int Decrled = 7;      //Incr speed button led

const int FButton = 9;      //Full speed button 
const int IButton = 10;     //Idle speed button
const int IncButton = 11;   //Idle speed button
const int DecrButton = 12;  //Idle speed button

const int FullPin = 65;     //Full speed setting. (93% of 70 degrees)
const int IdlePin = 1;      //Idle speed setting. (1% of 70 degrees)
const int incrVal = 2;      //Inc/decr value in degress

//*********************************************************************************************
void setup()
{
  myservo.attach(3);            // attaches the servo on pin 3 to the servo object
  myservo.write(cmdval);   // sets the initial servo position to zero (or idle)
  
  pinMode(Fled, OUTPUT);        // set the pins for the button leds to outputs
  pinMode(Iled, OUTPUT);
  pinMode(Incled, OUTPUT);
  pinMode(Decrled, OUTPUT);
    
  pinMode(FButton, INPUT);      // set the pins for the buttons to inputs
  pinMode(IButton, INPUT);
  pinMode(IncButton, INPUT);
  pinMode(DecrButton, INPUT);
  
} //end setup

//**************************************************************************************************************

void loop()
{
  IBState = digitalRead(IButton);       // read the state of the Idle pushbutton value
  if (IBState == HIGH) {
    delay(250);
    Idle();
  } 

  FBState = digitalRead(FButton);       // read the state of the Full pushbutton value
  if (FBState == HIGH) {            
    delay(250);
    Full();
  } 

  if (digitalRead(IncButton)) {            
    Incr();
  } 

  if (digitalRead(DecrButton)) {            
    Decr();
  } 
  
} // end loop  
//****************************************************************************************************

void Idle() {
  IBState = 0;                          // set button state to low after pushed
  digitalWrite(Iled, HIGH);             // turn on button led
  origval = cmdval;                     // store original pin value
  while (IBState == LOW)  {             // run the Idle speed function until the Idle button is pushed again
    cmdval = 70 - IdlePin;              // decrease speed. servo mapped from 0 to 70 degrees. This reverses servo direction.
    myservo.write(cmdval);
    IBState = digitalRead(IButton);     // read the state of the Idle pushbutton value
  }
  cmdval = origval;
  myservo.write(cmdval);                // write the original speed to the servo
  digitalWrite(Iled, LOW);              // turn off button led
  delay(250);                           // to debounce button 
}

void Full() {
  digitalWrite(Fled, HIGH);             // turn on button led
  origval = cmdval;                     // store original pin value
  while (FBState == HIGH) {
    cmdval = 70 - FullPin;              // servo mapped from 0 to 70 degrees. This reverses servo direction.
    myservo.write(cmdval);
    FBState = digitalRead(FButton);     // read the state of the Full pushbutton value
  }
  cmdval = origval;
  myservo.write(cmdval);                // write the original speed to the servo
  digitalWrite(Fled, LOW);              // turn off button led
  delay(250);
} 

void Incr() {
  digitalWrite(Incled, HIGH);            // turn on button led
  if (cmdval >= (cmdmin+incrVal)){
    cmdval -= incrVal;                           // increase speed. 
  }
  myservo.write(cmdval);
  delay(100);
  while (digitalRead(IncButton))
  digitalWrite(Incled, LOW);             // turn off button led
}

void Decr() {
  digitalWrite(Decrled, HIGH);            // turn on button led
  if (cmdval <= (cmdmax-incrVal)){
    cmdval += incrVal;                           // decrease speed. 
  }
  myservo.write(cmdval);
  delay(100);
  while (digitalRead(DecrButton))
  digitalWrite(Decrled, LOW);             // turn off button led
}
