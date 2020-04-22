/*B&O McKenzie Division Staging Yard Project

Designed for use on Jeroen Gerr
Uses a main control panel Pro-Mini to set up active staging yard tracks with a 
second Pro-Mini to drive the rr-CirKits MotorMan boards, control track power, etc.
The project requires remote control panels, so the second Pro_Mini is used to eliminate
a large cable bundle to the remote panel.

Panel

Track power timer

Occupancy detectors

by Mark Kellogg 4/17/2020
 */

#include <Arduino.h>

#define ROTARYSTEPS 1
#define ROTARYMIN 7
#define ROTARYMAX 12


//---------DEBUG LED---------------
const int ledPin = 5;
const int switchPin = 7; // push switch between pin 4 and GND
const int blinkDelay = 500;



//------------------GLOBAL VARIABLES----------------------


bool railPower = false;

int tracknumChoice =  ROTARYMAX;
int tracknumActive =  ROTARYMAX;
int tracknumDisplay =  ROTARYMAX;
int tracknumLast =  ROTARYMAX;

unsigned long timerConstant;  
bool timerOn;
bool timerStart;
unsigned long timerCount;
bool timerReset;
bool timerStatus;

bool sensorRevLoop = false;
bool sensorThroat = false;
bool sensorCycled = false;
bool sensorClear = true;

int knobPosition = ROTARYMAX;
bool knobClick = false;
bool knobMoving = false;

long lastTime = 0; // the time the LED blinked on or off

//---------------SETUP STATE Machine and Functions----------------------
enum {HOUSEKEEP, STAND_BY, TRACK_SELECT, TRACK_SETUP, TRACK_ACTIVE, OCCUPIED,} mode;

void runHOUSEKEEP();
void runSTAND_BY();
void runTRACK_SELECT();
void runTRACK_SETUP();
void runTRACK_ACTIVE();
void runOCCUPIED();
boolean switchPressed();  //might be knobClick in final


void setup() 
{
  pinMode(ledPin, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP);
  mode = HOUSEKEEP;

  Serial.begin(115200);
  Serial.println("---ANOTHER INOVATIVE PRODUCT FROM THE B&O MckENZIE DIVISION---");
  //delay(4000);        //--------------debug--------------
}

void loop() 
{
  //delay(700);        //-----------debug----------------
  if (mode == HOUSEKEEP)
  {
    runHOUSEKEEP();
  }

  else if (mode == STAND_BY)
  {
    runSTAND_BY();
  }

  else if (mode == TRACK_SELECT)
  {
    runTRACK_SELECT();

  }
  
  else if (mode == TRACK_SETUP)
  {
    runTRACK_SETUP();
  }

  else if (mode == TRACK_ACTIVE)
  {
    runTRACK_ACTIVE();
  }
  else if (mode == OCCUPIED)
  {
    runOCCUPIED();
    
  }
    
}
//--------------------HOUSEKEEP Function-----------------
void runHOUSEKEEP()
{
  railPower = false;         //send railPower off to TCA
  tracknumChoice =  ROTARYMAX;
  tracknumActive =  ROTARYMAX;
  tracknumDisplay =  ROTARYMAX;
  tracknumLast =  ROTARYMAX;
  Serial.println("HOUSEKEEP");
  mode = STAND_BY;
  
}  

//-----------------------STAND_BY Function-----------------
void runSTAND_BY()
{
  Serial.println("STAND_BY");
  mode = TRACK_SELECT;
} 

//-----------------------TRACK_SELECT Function----------------------
void runTRACK_SELECT()
{
  Serial.println("TRACT_SELECT");
  if (switchPressed())
  {
    mode = OCCUPIED;
  }
  else
  {
    mode = TRACK_SETUP;
  } 
}


//-----------------------TRACK_SETUP Function-----------------------
void runTRACK_SETUP()
{
  Serial.println("TRACK_SETUP");
  mode = TRACK_ACTIVE;
}



//-----------------------TRACK_ACTIVE Function----------------------
void runTRACK_ACTIVE()
{
  Serial.println("TRACK_ACTIVE");
  mode = HOUSEKEEP;
}


//-------------------------OCCUPIED Function---------------------------
void runOCCUPIED()
{
  Serial.println("OCCUPIED");
  if (switchPressed())
  {
    mode = OCCUPIED;
  }
  else
  {
  mode = HOUSEKEEP;
    
  }
  
}
//------------------------------RotaryKnob pressed---------------------
boolean switchPressed()
{
  if (digitalRead(switchPin) == LOW)
  {
    delay(100); // debounce
    return true;
  }
  return false;
}