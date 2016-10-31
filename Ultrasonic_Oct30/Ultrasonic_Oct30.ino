#include "Arduino.h"
#include <Servo.h>  // servo library


class Ultrasonic
{
  public:
    Ultrasonic(int pin);
        void DistanceMeasure(void);
    long microsecondsToCentimeters(void);
    long microsecondsToInches(void);
  private:
    int _pin;//pin number of Arduino that is connected with SIG pin of Ultrasonic Ranger.
        long duration;// the Pulse time received;
};
Ultrasonic::Ultrasonic(int pin)
{
  _pin = pin;
}
/*Begin the detection and get the pulse back signal*/
void Ultrasonic::DistanceMeasure(void)
{
    pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(_pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(_pin,LOW);
  pinMode(_pin,INPUT);
  duration = pulseIn(_pin,HIGH);
}
/*The measured distance from the range 0 to 400 Centimeters*/
long Ultrasonic::microsecondsToCentimeters(void)
{
  return duration/29/2;
}
/*The measured distance from the range 0 to 157 Inches*/
long Ultrasonic::microsecondsToInches(void)
{
  return duration/74/2;
}

// servo control object
Servo servo1;  //alive cat
Servo servo2; //dead cat
Servo servo3; //candy door


Ultrasonic ultrasonic(9); //Pin 9 for Ultrasonic sensor



void setup()
{
  Serial.begin(9600);

  servo1.attach(6); //assign servos to digital pins
  servo2.attach(7);
  servo3.attach(8);

}
void loop()
{


  int position;

  // Servos can move anywhere between 0 and 180 degrees.

  // Change position at full speed:
  // servo1.write(90);    // Tell servo to go to 90 degrees

 //  delay(1000);         // Pause to get it time to move

 //  servo1.write(180);   // Tell servo to go to 180 degrees

  // delay(1000);         // Pause to get it time to move


  // Change position at a slower speed:
  // Tell servo to go to 180 degrees, stepping by two degrees

 // for(position = 0; position < 180; position += 2)
  //{
   // servo1.write(position);  // Move to next position
 //   delay(20);               // Short pause to allow it to move
 // }



  long RangeInInches;
  long RangeInCentimeters;
  ultrasonic.DistanceMeasure();// get the current signal time;
    RangeInInches = ultrasonic.microsecondsToInches();//convert the time to inches;
  RangeInCentimeters = ultrasonic.microsecondsToCentimeters();//convert the time to centimeters

  //Serial.println("The distance to obstacles in front is: ");
  //Serial.print(RangeInInches);//0~157 inches
  //Serial.println(" inch");
  //Serial.print(RangeInCentimeters);//0~400cm
  //Serial.println(" cm");
  //delay(1000);

if(RangeInInches <= 40) {     //if a person is within 40cm from the box
    if(random(1) < 0.5) {     //if the cat is alive, alive cat pops up, candy door open, pause for 2 seconds then resume to original position
      servo1.write(180);
      servo3.write(90);
      delay(2000);
      servo1.write(0);
      servo3.write(0);
    } else if (random(1) >= 0.5) {    //if the cat is dead, dead cat pops up, candy door open, pause for 1 second then resume to original position
      servo2.write(180);
      servo3.write(90);
      delay(1000);
      servo2.write(0);
      servo3.write(0);
    }
    } else {                    //nothing happens if no one is around
      servo1.write(0);
      servo2.write(0);
      servo3.write(0);
    }

}
