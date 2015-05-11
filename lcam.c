#include <stdio.h>
#include <wiringPi.h>

const int greenPin = 23;
const int redPin = 18;
const int butPin = 22;

int main(void)
{
    wiringPiSetupGpio();
    pinMode(greenPin, OUTPUT);
    pinMode(redPin, OUTPUT);
    pinMode(butPin, INPUT);
    pullUpDnControl(butPin, PUD_UP);
    pwmWrite(redPin, 255);
    pwmWrite(greenPin, 255);

    while(1)
    {
        if (digitalRead(butPin))
        {
            digitalWrite(greenPin, LOW);
            digitalWrite(redPin, HIGH);
            delay(75);
            digitalWrite(redPin, LOW);
            delay(75);
        }
        else 
        {
            digitalWrite(redPin, LOW);
            digitalWrite(greenPin, HIGH);
            delay(75);
            digitalWrite(greenPin, LOW);
            delay(75);
        }
    }

    return 0;
}
