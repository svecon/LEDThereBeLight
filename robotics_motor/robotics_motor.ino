/*
Use a transistor to spin a motor at different speeds, with pulse-width modulation (PWM) to vary the speed of a motor.
The transistor is turned on and off with the digitalWrite() function, which can also be used to vary the speed of the motor.
The analogWrite() function pulses a pin, varying the width of the pulse from 0% to 100%. We call this technique "PWM", for "Pulse-Width Modulation".

Note: for PWM values below 50 or so

*/

const int sensorPin1 = 1;
const int sensorPin2 = 2;
const int sensorPin3 = 3;
const int sensorPin4 = 4;

const int motorPin1 = 7;
const int motorPin2 = 8;
const int motorPin3 = 9;
const int motorPin4 = 10;

const int LIGHT_THRESHOLD = 127;

int lightLevel1;
int lightLevel2;
int lightLevel3;
int lightLevel4;
int high1 = 0, low1 = 1023;
int high2 = 0, low2 = 1023;
int high3 = 0, low3 = 1023;
int high4 = 0, low4 = 1023;

int maxSpeed1 = 50;
int maxSpeed2 = 100;
int maxSpeed3 = 150;
int maxSpeed4 = 200;

int speed1 = maxSpeed1;
int speed2 = maxSpeed2;
int speed3 = maxSpeed3;
int speed4 = maxSpeed4;

void setup()
{
    // Set up the motor pin to be an output:
    pinMode(motorPin1, OUTPUT);
    pinMode(motorPin2, OUTPUT);
    pinMode(motorPin3, OUTPUT);
    pinMode(motorPin4, OUTPUT);

    // Set up the serial port:
    Serial.begin(9600);
}

void loop()
{
	speed1 = 0;
	speed2 = 0;
	speed3 = 0;
	speed4 = 0;

    lightLevel1 = autoTune(sensorPin1, &low1, &high1);
    lightLevel2 = autoTune(sensorPin2, &low2, &high2);
    lightLevel3 = autoTune(sensorPin3, &low3, &high3);
    lightLevel4 = autoTune(sensorPin4, &low4, &high4);

    if (lightLevel1 > LIGHT_THRESHOLD) {
    	speed1 = maxSpeed1;
    }
    if (lightLevel2 > LIGHT_THRESHOLD) {
    	speed2 = maxSpeed2;
    }
    if (lightLevel3 > LIGHT_THRESHOLD) {
    	speed3 = maxSpeed3;
    }
    if (lightLevel4 < LIGHT_THRESHOLD) { // run when dark
    	speed4 = maxSpeed4;
    }

    analogWrite(motorPin1, speed1);
    analogWrite(motorPin2, speed2);
    analogWrite(motorPin3, speed3);
    analogWrite(motorPin4, speed4);
}

int autoTune(int sensorPin, int* low, int* high)
{
    // As we mentioned above, the light-sensing circuit we built
    // won't have a range all the way from 0 to 1023. It will likely
    // be more like 300 (dark) to 800 (light).

    // In the manualTune() function above, you need to repeatedly
    // change the values and try the program again until it works.
    // But why should you have to do that work? You've got a
    // computer in your hands that can figure things out for itself!

    // In this function, the Arduino will keep track of the highest
    // and lowest values that we're reading from analogRead().

    // If you look at the top of the sketch, you'll see that we've
    // initialized "low" to be 1023. We'll save anything we read
    // that's lower than that:

    int lightLevel = analogRead(sensorPin);

    if (lightLevel < *low)
    {
        *low = lightLevel;
    }

    // We also initialized "high" to be 0. We'll save anything
    // we read that's higher than that:

    if (lightLevel > *high)
    {
        *high = lightLevel;
    }

    // Once we have the highest and lowest values, we can stick them
    // directly into the map() function. No manual tweaking needed!

    // One trick we'll do is to add a small offset to low and high,
    // to ensure that the LED is fully-off and fully-on at the limits
    // (otherwise it might flicker a little bit).

    lightLevel = map(lightLevel, *low+30, *high-30, 0, 255);
    lightLevel = constrain(lightLevel, 0, 255);

    return lightLevel;
}
