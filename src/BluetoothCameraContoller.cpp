#include "BluetoothCameraController.h"

double mapf(double val, double in_min, double in_max, double out_min, double out_max)
{
  return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

BluetoothCameraController::BluetoothCameraController(BLERemoteCharacteristic *outgoingCameraControl)
{
  _cameraControl = outgoingCameraControl;
}

BluetoothCameraController::~BluetoothCameraController(){};

uint32_t BluetoothCameraController::mapFloat(float value)
{
  return (uint32_t)mapf(value, 0, 1.0, 0, 2047.0);
}

void BluetoothCameraController::setCamera(uint8_t index)
{
  _cameraIndex = index;
}

void BluetoothCameraController::custom(uint8_t *data, size_t len)
{
  _cameraControl->writeValue(data, len, true);
}

uint8_t BluetoothCameraController::getCameraStatus()
{
  _state->getCameraStatus();
}

void BluetoothCameraController::record(bool record)
{
  uint8_t data[12] = {255, 5, 0, 0, 10, 1, 1, 0, 0, 0, 0, 0};
  if (record)
  {
    data[8] = 2;
  }
  _cameraControl->writeValue(data, 12, true);
}

void BluetoothCameraController::toggleRecording()
{
  bool rec = _state->getRecording();
  uint8_t data[12] = {255, 5, 0, 0, 10, 1, 1, 0, 0, 0, 0, 0};
  if (!rec)
  {
    data[8] = 2;
  }
  _cameraControl->writeValue(data, 12, true);
}

bool BluetoothCameraController::isRecording()
{
  return _state->getRecording();
}

void BluetoothCameraController::play(bool play)
{
  uint8_t data[12] = {255, 5, 0, 0, 10, 1, 1, 0, 0, 0, 0, 0};
  if (play)
  {
    data[8] = 1;
  }
  _cameraControl->writeValue(data, 12, true);
}

void BluetoothCameraController::preview()
{
  uint8_t data[12] = {255, 5, 0, 0, 10, 1, 1, 0, 0, 0, 0, 0};
  _cameraControl->writeValue(data, 12, true);
}

void BluetoothCameraController::ois(bool enabled)
{
  uint8_t data[10] = {255, 6, 0, 0, 0, 6, 0, 0, 0};
  if (enabled)
  {
    data[8] = 1;
  }
  _cameraControl->writeValue(data, 10, true);
}

void BluetoothCameraController::codec(CODEC_TYPE c, CODEC_QUALITY q)
{
  uint8_t data[12] = {255, 5, 0, 0, 10, 0, 1, 0, 0, 0, 0, 0};
  data[8] = c;
  data[9] = q;
  _cameraControl->writeValue(data, 12, true);
}

CODEC_TYPE BluetoothCameraController::getCodecType()
{
}

CODEC_QUALITY BluetoothCameraController::getCodecQuality()
{
}

void BluetoothCameraController::focus(float focus)
{
  if (value < 0 && value > 1)
    return;

  uint16_t val = mapFloat(focus);

  uint8_t xlow = val & 0xff;
  uint8_t xhigh = (val >> 8);

  uint8_t data[10] = {255, 6, 0, 0, 0, 0, 128, 0, xlow, xhigh};
  _cameraControl->writeValue(data, 10, true);
}

void BluetoothCameraController::instantAutoFocus()
{
  uint8_t data[12] = {255, 4, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0};
  _cameraControl->writeValue(data, 12, true);
}

float BluetoothCameraController::getFocus()
{
  return _state->getFocus();
}

void BluetoothCameraController::zoom(float value)
{
  if (value < 0 && value > 1)
    return;

  uint16_t val = mapFloat(focus);

  uint8_t xlow = val & 0xff;
  uint8_t xhigh = (val >> 8);

  uint8_t data[10] = {255, 6, 0, 0, 0, 8, 128, 0, xlow, xhigh};
  _cameraControl->writeValue(data, 10, true);
}

void BluetoothCameraController::aperture(float value)
{
  if (value < 0 && value > 1)
    return;

  uint16_t val = mapFloat(value);

  uint8_t xlow = val & 0xff;
  uint8_t xhigh = (val >> 8);

  uint8_t data[10] = {255, 6, 0, 0, 0, 3, 128, 0, xlow, xhigh};

  _cameraControl->writeValue(data, 10, true);
}

void BluetoothCameraController::autoAperture()
{
  uint8_t data[12] = {255, 4, 0, 0, 0, 5, 1, 0, 0, 0, 0, 0};
  _cameraControl->writeValue(data, 12, true);
}

float BluetoothCameraController::getAperture()
{
  return _state->getAperture();
}

void BluetoothCameraController::iso(int32_t iso)
{
  (if iso < 0 && iso > 0x7FFFFFFF) return;

  uint8_t xlow = iso & 0xff;
  uint8_t xhigh = (iso >> 8);

  uint8_t data[12] = {255, 8, 0, 0, 1, 14, 3, 0, xlow, xhigh, 0, 0};

  _cameraControl->writeValue(data, 12, true);
}

int32_t BluetoothCameraController::getIso()
{
  return _state->getIso();
}

void BluetoothCameraController::shutterAngle(int32_t shutter)
{
  if (shutter < 1 && shutter > 360)
    return;

  shutter = shutter * 100;

  uint8_t l = shutter & 0xff;
  uint8_t h = (shutter >> 8);

  uint8_t data[12] = {255, 8, 0, 0, 1, 11, 3, 0, l, h, 0, 0};
  _cameraControl->writeValue(data, 12, true);
}

void BluetoothCameraController::shutterSpeed(int32_t shutter)
{
  if (shutter < 24 && shutter > 2000)
    return;

  uint8_t l = shutter & 0xff;
  uint8_t h = (shutter >> 8);

  uint8_t data[12] = {255, 8, 0, 0, 1, 12, 3, 0, l, h, 0, 0};
  _cameraControl->writeValue(data, 12, true);
}

int32_t BluetoothCameraController::getShutter()
{
  return _state->getShutter();
}

void BluetoothCameraController::whiteBalance(int16_t whiteBalance, int16_t tint)
{
  if (whiteBalance < 2500 && whiteBalance > 10000)
    return;
  if (tint < -50 && tint > 50)
    return;

  uint8_t wbL = whiteBalance & 0xff;
  uint8_t wbH = (whiteBalance >> 8);

  uint8_t tL = tint & 0xff;
  uint8_t tH = (tint >> 8);

  uint8_t data[12] = {255, 8, 0, 0, 1, 2, 2, 0, wbL, wbH, tL, tH};
  _cameraControl->writeValue(data, 12, true);
}

void BluetoothCameraController::autoWhiteBalance()
{
  uint8_t data[12] = {255, 4, 0, 0, 1, 3, 1, 0, 0, 0, 0, 0};
  _cameraControl->writeValue(data, 12, true);
}

int16_t BluetoothCameraController::getWhiteBalance()
{
  return _state->getWhiteBalance();
}

void BluetoothCameraController::frameRate(int16_t frameRate)
{
  if (frameRate < 0)
    return;

  uint8_t frL = frameRate & 0xff;
  uint8_t frH = (frameRate >> 8);

  uint8_t data[18] = {255, 8, 0, 0, 1, 9, 2, 0, frL, frH, 0, 0, 0, 0, 0, 0, 0, 0};
  // uint8_t data[8] = {255, 8, 0, 0, 1, 9, 2, 0, frL, frH, 0, 0};
  _cameraControl->writeValue(data, 18, true);
}

int16_t BluetoothCameraController::getFrameRate()
{
  return _state->getFrameRate();
}

void BluetoothCameraController::sensorFrameRate(int16_t sensorFrameRate)
{
  if (sensorFrameRate < 0)
    return;

  uint8_t sfrL = sensorFrameRate & 0xff;
  uint8_t sfrH = (sensorFrameRate >> 8);

  uint8_t data[18] = {255, 8, 0, 0, 1, 9, 2, 0, 0, 0, sfrL, sfrH, 0, 0, 0, 0, 0, 0};
  // uint8_t data[8] = {255, 8, 0, 0, 1, 9, 2, 0, frL, frH, 0, 0};
  _cameraControl->writeValue(data, 18, true);
}

int16_t getSensorFrameRate()
{
  return _state->getSensorFrameRate();
}

String BluetoothCameraController::timecode()
{
  return _state->getTimecode();
}

uint32_t BluetoothCameraController::timecodeRaw()
{
  return _state->getTimecodeRaw();
}
