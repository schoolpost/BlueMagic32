#include <BlueMagic32.h>

BluetoothCameraConnection BMDConnection;
BluetoothCameraController *BMDControl;

void setup()
{
  Serial.begin(115200);

  BMDConnection.init("BLE");
  BMDConnection.scan(false, 5);
  if (BMDConnection.connected() != CAMERA_CONNECTED || BMDConnection.connected() != CAMERA_CONNECTING)
  {
    BMDControl = BMDConnection.connect();
  }

}

void loop()
{
}