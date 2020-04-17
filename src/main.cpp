/* 
blink3 sketch
Demonstrates calling a function
by Mark Kellogg 4/9/2020
 */

#include <Arduino.h>

const int ledPin = 5;
const int inputPin = 7;
//const int Sw1 = 7;

int blink3(int period); //declare function Blink3

void setup()
{
  pinMode(ledPin, OUTPUT);
  //pinMode(Sw1, INPUT);
  pinMode(inputPin, INPUT);

  digitalWrite(inputPin, HIGH); // use internal pull-up resistor

  Serial.begin(9600);

} // end setup

void loop()
{
  Serial.println("Press and hold the switch to stop blinking.");
  int count = blink3(250); //blink the LED 250ms on and 250ms off
  Serial.print("The number of times the LED blinked was: ");
  Serial.println(count);
}

/*-------------Blink3 Function-------------------
Blink an LED using the passed in delay period
Return the number of times the LED flashed
*/

int blink3(int period)
{
  int result = 0;
  int switchVal = HIGH; // with pull-up this will be high when switch is open

  while (switchVal == HIGH) //repeat this loop until switch is pressed
                            // it will go low when pressed
  {
    digitalWrite(ledPin, HIGH);
    delay(period);
    digitalWrite(ledPin, LOW);
    delay(period);
    result = result + 1;               //increment the count
    switchVal = digitalRead(inputPin); // read input value
  }
  // here, when switchVal is no longer HIGH because the switch is pressed, return result
  return result; //this is the value returned
} // End Blink3
