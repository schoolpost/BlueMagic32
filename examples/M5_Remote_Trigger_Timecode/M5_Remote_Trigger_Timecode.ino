#include <M5StickC.h>
#include <BlueMagic32.h>

#define LED_PIN 10

void setup()
{

  M5.begin();
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  BMDConnection.begin("BlueMagic32");
  BMDControl = BMDConnection.connect();

  M5.Lcd.setTextColor(TFT_WHITE);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(TFT_BLACK);
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

    M5.Lcd.drawCentreString(BMDControl->timecode(), 80, 34, 2);

    if (M5.BtnA.wasPressed())
    {
      BMDControl->toggleRecording();
    }

    if (BMDControl->isRecording())
    {
      M5.Lcd.setTextColor(RED);
      digitalWrite(LED_PIN, LOW);
    }
    else
    {
      M5.Lcd.setTextColor(TFT_WHITE);
      digitalWrite(LED_PIN, HIGH);
    }

    M5.Lcd.fillScreen(TFT_BLACK);
  }
}