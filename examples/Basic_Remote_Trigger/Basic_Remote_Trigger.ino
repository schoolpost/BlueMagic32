#include <BlueMagic32.h>

#define BUTTON_PIN 0

int buttonState;           // the current reading from the input pin
int lastButtonState = LOW; // the previous reading from the input pin

unsigned long lastDebounceTime = 0; // the last time the output pin was toggled
unsigned long debounceDelay = 50;   // the debounce time; increase if the output flickers

void setup()
{

    Serial.begin(115200);

    BMDConnection.begin("BlueMagic32");
    BMDControl = BMDConnection.connect();

    pinMode(BUTTON_PIN, INPUT);
}

void loop()
{

    int reading = digitalRead(BUTTON_PIN);

    // If the switch changed, due to noise or pressing:
    if (reading != lastButtonState)
    {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay)
    {
        if (reading != buttonState)
        {
            buttonState = reading;

            // only toggle the LED if the new button state is HIGH
            if (buttonState == HIGH)
            {
                if (BMDConnection.available())
                {
                    BMDControl->toggleRecording();
                }
            }
        }
    }

    lastButtonState = reading;
}