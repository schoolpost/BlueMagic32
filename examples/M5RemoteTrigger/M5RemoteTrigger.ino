#include <M5StickC.h>
#include <BlueMagic32.h>

void setup() {

  M5.begin();
  Serial.begin(115200);

  BMDConnection.begin("BlueMagic32");
  BMDControl = BMDConnection.connect();

  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(TFT_BLACK);

}

void loop() {
  M5.update();

  if (M5.BtnB.wasPressed()){
    
    BMDControl->aperture(0);
    vTaskDelay(3000);
    BMDControl->aperture(0.5);
    vTaskDelay(3000);
    BMDControl->aperture(1);
  }

  if (BMDConnection.available()) {
    
    M5.Lcd.drawCentreString(BMDControl->timecode(), 80, 40, 2);
  
    if (M5.BtnA.wasPressed()) {
      BMDControl->toggleRecording();
    }

    if (BMDControl->isRecording()) {
      M5.Lcd.setTextColor(RED);
    } else {
      M5.Lcd.setTextColor(WHITE);
    }

    M5.Lcd.fillScreen(TFT_BLACK);

  }

}