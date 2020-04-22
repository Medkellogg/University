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
#include <RotaryEncoder.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define ROTARYSTEPS 1
#define ROTARYMIN 7
#define ROTARYMAX 12

//--- Setup a RoraryEncoder for pins A2 and A3:
RotaryEncoder encoder(A2, A3);

//--- Last known rotary position.
int lastPos = -1;

//---Setup Rotary Encoder switch on pin D2 with other side to ground - active low
int rotarySwitch = 2;

//---------DEBUG LED---------------
const int ledPin = 5;
const int switchPin = 7; // push switch between pin 4 and GND
const int blinkDelay = 500;
long lastTime = 0; // the time the LED blinked on or off


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

int knobClick = 1;
int knobClickLast = 1;
int knobPosition = ROTARYMAX;
bool knobMoving;



//---------------SETUP STATE Machine and Functions----------------------
enum {HOUSEKEEP, STAND_BY, TRACK_SETUP, TRACK_ACTIVE, OCCUPIED,} mode;

void runHOUSEKEEP();
void runSTAND_BY();
void runTRACK_SETUP();
void runTRACK_ACTIVE();
void runOCCUPIED();
boolean switchPressed();  //might be knobClick in final
void readEncoder();


void setup() 
{
  pinMode(ledPin, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP);
  encoder.setPosition(ROTARYMIN / ROTARYSTEPS); // start with the value of ROTARYMIN .

  pinMode(rotarySwitch, INPUT_PULLUP);
  mode = HOUSEKEEP;

  Serial.begin(115200);
  Serial.println("---ANOTHER INOVATIVE PRODUCT FROM THE B&O MckENZIE DIVISION---");
  //delay(4000);        //--------------debug--------------
}

void loop() 
{
    delay(1000);        //-----------debug----------------
  if (mode == HOUSEKEEP)
  {
    runHOUSEKEEP();
  }

  else if (mode == STAND_BY)
  {
       runSTAND_BY();
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
  //tracknumChoice =  ROTARYMAX;
  //tracknumActive =  ROTARYMAX;
  //tracknumDisplay =  ROTARYMAX;
  //tracknumLast =  ROTARYMAX;  //####---UPDATE this with true number from Maybe just ignore
  //####Send TCA tracknumActive to TCA - Call comm function here------------#####
  Serial.println("HOUSEKEEP");
  mode = STAND_BY;
  
}  

//-----------------------STAND_BY Function-----------------
void runSTAND_BY()
{
  Serial.println("STAND_BY");
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
//------------------------------Track Sensor pressed---------------------
boolean switchPressed()
{
  if (digitalRead(switchPin) == LOW)
  {
    delay(100); // debounce
    return true;
  }
  return false;
}

//------------------------ReadEncoder Function----------------------

void readEncoder()
{
  encoder.tick();
  ;

  // get the current physical position and calc the logical position

  int newPos = encoder.getPosition() * ROTARYSTEPS;
  
   /*----------Use of ROTARYMAX inside the if statement: counter will roll from min to max and keep counting in a loop.  To make counter stop at lower limit use ROTARYMIN */
    
  if (newPos < ROTARYMIN)
  {
    encoder.setPosition(ROTARYMAX / ROTARYSTEPS);
    newPos = ROTARYMAX;
  }

  /*------------Use of ROTARYMIN inside the if statement: counter will roll from max to min and keep counting in a loop.  o make counter stop at upper limit use ROTARYMAX. */
  
  else if (newPos > ROTARYMAX)
  {
    encoder.setPosition(ROTARYMIN / ROTARYSTEPS);
    newPos = ROTARYMIN;

  }  // End if newPos
  

  if (lastPos != newPos)
  {
    if (newPos == ROTARYMAX)
    {
      Serial.print("REV LOOP");
      Serial.println();
    }
    else
    {
      Serial.print(newPos);
      Serial.println();
    }
    lastPos = newPos;
    tracknumDisplay = lastPos;

      Serial.print("tracknumDisplay: ");
      Serial.print(tracknumDisplay);
      Serial.println();

      Serial.print("tracknumActive: ");
      Serial.print(tracknumActive);
      Serial.println();
      
  } // End if lastPos

    //------Read the rotarySwitch for new track selection and update the active track 
    knobClick = digitalRead(rotarySwitch);   
    delay(10);
    if(knobClick != knobClickLast)          
    {
      tracknumActive = tracknumDisplay;
    } // End new active track selection    

    
  
}