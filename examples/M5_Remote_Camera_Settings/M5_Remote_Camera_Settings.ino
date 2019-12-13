#include <M5StickC.h>
#include <BlueMagic32.h>

void setup()
{

    M5.begin();
    Serial.begin(115200);

    BMDConnection.begin("BlueMagic32");
    BMDControl = BMDConnection.connect();

    M5.Lcd.setRotation(3);
    M5.Lcd.fillScreen(TFT_BLACK);
}

void printOnScreen(int x, int y, String text)
{
    M5.Lcd.setCursor(x, y, 2);
    M5.Lcd.setTextColor(TFT_WHITE);
    M5.Lcd.setTextSize(1);
    M5.Lcd.println(text);
}

void loop()
{
    M5.update();

    if (M5.BtnB.wasPressed())
    {
        ESP.restart();
    }

    if (BMDConnection.available())
    {

        if (BMDControl->changed())
        {

            M5.Lcd.fillScreen(TFT_BLACK);

            printOnScreen(0, 0, "F" + String(BMDControl->getAperture(), 1));
            printOnScreen(0, 16, String(BMDControl->getShutter()) + "°");
            printOnScreen(0, 32, String(BMDControl->getIso()));
            printOnScreen(0, 48, String(BMDControl->getWhiteBalance()) + "K");
            printOnScreen(0, 64, String(BMDControl->getTint()));
        }
    }
}