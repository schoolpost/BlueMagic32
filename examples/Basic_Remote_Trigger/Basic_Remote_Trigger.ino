#include <BlueMagic32.h>

#define BUTTON_PIN 2

void setup()
{

    Serial.begin(115200);

    BMDConnection.begin("BlueMagic32");
    BMDControl = BMDConnection.connect();
}

void loop()
{

    if (BMDConnection.available())
    {
    }
}