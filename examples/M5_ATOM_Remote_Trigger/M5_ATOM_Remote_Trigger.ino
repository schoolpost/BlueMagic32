#include "M5Atom.h"
#include <BlueMagic32.h>

void setup()
{

    M5.begin(true, false, true);
    Serial.begin(115200);

    M5.dis.drawpix(0, 0xffffff);

    BMDConnection.begin("BlueMagic32");
    BMDControl = BMDConnection.connect();
}

void loop()
{
    M5.update();

    if (BMDConnection.available())
    {

        if (M5.Btn.wasPressed())
        {
            BMDControl->toggleRecording();
        }

        if (BMDControl->isRecording())
        {
            M5.dis.drawpix(0, 0x00ff00);
        }
        else
        {
            M5.dis.drawpix(0, 0xffffff);
        }
    }
}