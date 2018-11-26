#include <HID-Project.h>
#include <HID-Settings.h>

int fader1Pin = A2;
int fader2Pin = A1;
int fader3Pin = A0;

int fader1Value = 0;
int fader2Value = 0;
int fader3Value = 0;

int led1 = 2;      // select the pin for the LED
int led2 = 4;      // select the pin for the LED
int led3 = 6;      // select the pin for the LED

int button1 = 3;
int button2 = 5;
int button3 = 7;

bool led1state = LOW;
bool led2state = LOW;
bool led3state = LOW;

unsigned long b1LastPush = 0;
unsigned long b2LastPush = 0;
unsigned long b3LastPush = 0;

int pressDelay = 250;

int vol1 = 0;
int vol2 = 0;
int vol3 = 0;

int vol1percent = 0;
int vol2percent = 0;
int vol3percent = 0;

void setup() {
    // declare the ledPin as an OUTPUT:
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);

    pinMode(button1, INPUT_PULLUP);
    pinMode(button2, INPUT_PULLUP);
    pinMode(button3, INPUT_PULLUP);

    Keyboard.begin();
}

void loop() {
    doVolume();

    unsigned long currentMillis = millis();

    if (!digitalRead(button1) && b1LastPush < currentMillis) {
        b1LastPush = currentMillis + pressDelay;
        // turn LED on:
        led1state = !led1state;
        digitalWrite(led1, led1state);
        Keyboard.write(KEY_F20);
    } 

    if (!digitalRead(button2) && b2LastPush < currentMillis) {
        b2LastPush = currentMillis + pressDelay;
        // turn LED on:
        led2state = !led2state;
        digitalWrite(led2, led2state);
        Keyboard.write(MEDIA_PLAY_PAUSE);
    } 

    if (!digitalRead(button3) && b3LastPush < currentMillis) {
        b3LastPush = currentMillis + pressDelay;
        // turn LED on:
        led3state = !led3state;
        digitalWrite(led3, led3state);
        Keyboard.write(KEY_F13);
    } 
}

void doVolume() {

    //Master Vol
    int vol = analogRead(fader1Pin);
    int newvolpercent = ((float)vol / 1024.0) * 150;
    // Serial.println(newvolpercent);
    if (vol1percent != newvolpercent)
    {
        vol1percent = newvolpercent;
        if(vol1 < vol1percent)
        {
            while (vol1 < vol1percent)
            {
                Keyboard.write(KEY_F18);
                vol1 += 3;
                delay(1);
            }
        } else if (vol1 > vol1percent)
        {
            while (vol1 > vol1percent)
            {
                Keyboard.write(KEY_F19);
                vol1 -= 3;
                delay(1);
            }
        }
    }

    //Music Vol
    vol = analogRead(fader2Pin);
    newvolpercent = ((float)vol / 1024.0) * 100;
    Serial.println(newvolpercent);
    if (vol2percent != newvolpercent)
    {
        vol2percent = newvolpercent;
        if(vol2 < vol2percent)
        {
            while (vol2 < vol2percent)
            {
                Keyboard.write(KEY_F16);
                vol2 += 3;
                delay(1);
            }
        } else if (vol2 > vol2percent)
        {
            while (vol2 > vol2percent)
            {
                Keyboard.write(KEY_F17);
                vol2 -= 3;
                delay(1);
            }
        }
    }

    //Mic Vol
    vol = analogRead(fader3Pin);
    newvolpercent = ((float)vol / 1024.0) * 100;
    Serial.println(newvolpercent);
    if (vol3percent != newvolpercent)
    {
        vol3percent = newvolpercent;
        if(vol3 < vol3percent)
        {
            while (vol3 < vol3percent)
            {
                Keyboard.write(KEY_F14);
                vol3 += 3;
                delay(1);
            }
        } else if (vol3 > vol3percent)
        {
            while (vol3 > vol3percent)
            {
                Keyboard.write(KEY_F15);
                vol3 -= 3;
                delay(1);
            }
        }
    }
}

