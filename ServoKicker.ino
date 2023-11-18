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
Servo myservo;              //create servo object to control a servo

int cmdval = 70;            //variable to read the value from the analog pin
int cmdmin = 0;             //full throttle limit
int cmdmax = 70;            //low throttle limit
int origval = 0;            //to store orginal throttle value
int prgmaval = 70;          //to store program A throttle value

int FBState = 0;            // variable for reading the Full pushbutton status
int IBState = 0;            // variable for reading the Idle pushbutton status

const int Fled = 4;         //Full speed button led
const int Iled = 5;         //Idle speed button led
const int Incled = 6;       //Incr speed button led
const int Decrled = 7;      //Incr speed button led
const int PrgmAled = 13;    //Incr speed button led

const int FButton = 9;      //Full speed button 
const int IButton = 2;      //Idle speed button
const int IncButton = 11;   //Incr speed button
const int DecrButton = 3;   //Decr speed button
const int PrgmAButton = 10; //Program A button

const int FullPin = 65;     //Full speed setting. (93% of 70 degrees)
const int IdlePin = 1;      //Idle speed setting. (1% of 70 degrees)
const int incrVal = 2;      //Inc/decr value in degress

//*********************************************************************************************
void setup()
{
  myservo.attach(14);             // attaches the servo on pin 3 to the servo object
  myservo.write(cmdval);          // sets the initial servo position to idle
  
  pinMode(Fled, OUTPUT);          // set the pins for the leds to outputs
  pinMode(Iled, OUTPUT);
  pinMode(Incled, OUTPUT);
  pinMode(Decrled, OUTPUT);
  pinMode(PrgmAled, OUTPUT);
    
  pinMode(FButton, INPUT);        // set the pins for the buttons to inputs
  pinMode(IButton, INPUT);
  pinMode(IncButton, INPUT);
  pinMode(DecrButton, INPUT);
  pinMode(PrgmAButton, INPUT);
  
} //end setup

//**************************************************************************************************************

void loop()
{
  IBState = digitalRead(IButton);       // read the state of the Idle pushbutton value
  if (IBState == HIGH) {
    delay(250);
    Idle();
  } 

  if (digitalRead(IncButton)) {            
    Incr();
  } 

  if (digitalRead(DecrButton)) {            
    Decr();
  } 
  
  if (digitalRead(PrgmAButton)) {            
    PrgmA();
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
    cmdval -= incrVal;                   // increase speed. 
    myservo.write(cmdval);
  }
  delay(100);
  while (digitalRead(IncButton))
  digitalWrite(Incled, LOW);             // turn off button led
}

void Decr() {
  digitalWrite(Decrled, HIGH);            // turn on button led
  if (cmdval <= (cmdmax-incrVal)){
    cmdval += incrVal;                    // decrease speed. 
    myservo.write(cmdval);
  }
  delay(100);
  while (digitalRead(DecrButton))
  digitalWrite(Decrled, LOW);             // turn off button led
}

void PrgmA() {
  int i = 0;
  digitalWrite(PrgmAled, HIGH);           // turn on button led
  while (digitalRead(PrgmAButton)) {
    delay(100);
    if(i > 20 ){                          // long press, hold the button down for 2 seconds to store current position as positionA
      prgmaval = cmdval;
      digitalWrite(PrgmAled, LOW);        // turn off button led
    }
    i++;
  }
  if(i < 4) {                             // short press --> commands positionA
    cmdval = prgmaval;
    myservo.write(cmdval);
  }
  digitalWrite(PrgmAled, LOW);            // turn off button led
}
