#include "BlueMagicCameraController.h"

double mapf(double val, double in_min, double in_max, double out_min, double out_max)
{
  return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

BlueMagicCameraController::BlueMagicCameraController(BLERemoteCharacteristic *outgoingCameraControl)
{
  _cameraControl = outgoingCameraControl;
}

BlueMagicCameraController::~BlueMagicCameraController(){};

bool BlueMagicCameraController::changed()
{
  return _state->changed();
}

bool BlueMagicCameraController::settingsChanged()
{
  return _state->settingsChanged();
}

uint8_t *BlueMagicCameraController::settingsData()
{
  return _state->settingsData();
}

bool BlueMagicCameraController::timecodeChanged()
{
  return _state->timecodeChanged();
}

uint8_t *BlueMagicCameraController::timecodeData()
{
  return _state->timecodeData();
}

bool BlueMagicCameraController::statusChanged()
{
  return _state->statusChanged();
}

uint8_t *BlueMagicCameraController::statusData()
{
  return _state->statusData();
}

uint32_t BlueMagicCameraController::mapFloat(float value)
{
  return (uint32_t)mapf(value, 0, 1.0, 0, 2047.0);
}

void BlueMagicCameraController::setCamera(uint8_t index)
{
  _cameraIndex = index;
}

void BlueMagicCameraController::custom(uint8_t *data, size_t len)
{
  _cameraControl->writeValue(data, len, true);
}

uint8_t BlueMagicCameraController::getCameraStatus()
{
  _state->getCameraStatus();
}

int8_t BlueMagicCameraController::getTransportMode()
{
  return _state->getTransportMode();
}

void BlueMagicCameraController::record(bool record)
{
  uint8_t data[12] = {255, 5, 0, 0, 10, 1, 1, 0, 0, 0, 0, 0};
  if (record)
  {
    data[8] = 2;
  }
  _cameraControl->writeValue(data, 12, true);
}

void BlueMagicCameraController::toggleRecording()
{
  bool rec = isRecording();
  uint8_t data[12] = {255, 5, 0, 0, 10, 1, 1, 0, 0, 0, 0, 0};
  if (!rec)
  {
    data[8] = 2;
  }
  _cameraControl->writeValue(data, 12, true);
}

bool BlueMagicCameraController::isRecording()
{
  int8_t mode = getTransportMode();
  if (mode == 2)
  {
    return true;
  }
  return false;
}

void BlueMagicCameraController::play(bool play)
{
  uint8_t data[12] = {255, 5, 0, 0, 10, 1, 1, 0, 0, 0, 0, 0};
  if (play)
  {
    data[8] = 1;
  }
  _cameraControl->writeValue(data, 12, true);
}

bool BlueMagicCameraController::isPlaying()
{
  int8_t mode = getTransportMode();
  if (mode == 1)
  {
    return true;
  }
  return false;
}

void BlueMagicCameraController::preview(bool preview)
{
  uint8_t data[12] = {255, 5, 0, 0, 10, 1, 1, 0, 0, 0, 0, 0};
  _cameraControl->writeValue(data, 12, true);
}

bool BlueMagicCameraController::isPreviewing()
{
  int8_t mode = getTransportMode();
  if (mode == 0)
  {
    return true;
  }
  return false;
}

void BlueMagicCameraController::ois(bool enabled)
{
  uint8_t data[10] = {255, 6, 0, 0, 0, 6, 0, 0, 0};
  if (enabled)
  {
    data[8] = 1;
  }
  _cameraControl->writeValue(data, 10, true);
}

bool BlueMagicCameraController::getOis()
{
  return true;
}

void BlueMagicCameraController::codec(CODEC_TYPE c, CODEC_QUALITY q)
{
  uint8_t data[12] = {255, 5, 0, 0, 10, 0, 1, 0, 0, 0, 0, 0};
  data[8] = c;
  data[9] = q;
  _cameraControl->writeValue(data, 12, true);
}

int8_t BlueMagicCameraController::getCodecType()
{
  return _state->getCodec();
}

int8_t BlueMagicCameraController::getCodecQuality()
{
  return _state->getQuality();
}

void BlueMagicCameraController::focus(float focus)
{
  if (focus < 0 && focus > 1)
    return;

  uint16_t val = mapFloat(focus);

  uint8_t xlow = val & 0xff;
  uint8_t xhigh = (val >> 8);

  uint8_t data[10] = {255, 6, 0, 0, 0, 0, 128, 0, xlow, xhigh};
  _cameraControl->writeValue(data, 10, true);
}

void BlueMagicCameraController::instantAutoFocus()
{
  uint8_t data[12] = {255, 4, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0};
  _cameraControl->writeValue(data, 12, true);
}

float BlueMagicCameraController::getFocus()
{
  return _state->getFocus();
}

void BlueMagicCameraController::zoom(float zoom)
{
  if (zoom < 0 && zoom > 1)
    return;

  uint16_t val = mapFloat(zoom);

  uint8_t xlow = val & 0xff;
  uint8_t xhigh = (val >> 8);

  uint8_t data[10] = {255, 6, 0, 0, 0, 8, 128, 0, xlow, xhigh};
  _cameraControl->writeValue(data, 10, true);
}

float BlueMagicCameraController::getZoom()
{
  return 0.0;
}

void BlueMagicCameraController::aperture(float value)
{
  if (value < 0 && value > 1)
    return;

  uint16_t val = mapFloat(value);

  uint8_t xlow = val & 0xff;
  uint8_t xhigh = (val >> 8);

  uint8_t data[10] = {255, 6, 0, 0, 0, 3, 128, 0, xlow, xhigh};

  _cameraControl->writeValue(data, 10, true);
}

void BlueMagicCameraController::autoAperture()
{
  uint8_t data[12] = {255, 4, 0, 0, 0, 5, 1, 0, 0, 0, 0, 0};
  _cameraControl->writeValue(data, 12, true);
}

float BlueMagicCameraController::getAperture()
{
  return _state->getAperture();
}

void BlueMagicCameraController::iso(int32_t iso)
{

  if (iso < 0 && iso > 0x7FFFFFFF)
    return;

  uint8_t xlow = iso & 0xff;
  uint8_t xhigh = (iso >> 8);

  uint8_t data[12] = {255, 8, 0, 0, 1, 14, 3, 0, xlow, xhigh, 0, 0};

  _cameraControl->writeValue(data, 12, true);
}

int32_t BlueMagicCameraController::getIso()
{
  return _state->getIso();
}

void BlueMagicCameraController::shutterAngle(int32_t shutter)
{
  if (shutter < 1 && shutter > 360)
    return;

  shutter = shutter * 100;

  uint8_t l = shutter & 0xff;
  uint8_t h = (shutter >> 8);

  uint8_t data[12] = {255, 8, 0, 0, 1, 11, 3, 0, l, h, 0, 0};
  _cameraControl->writeValue(data, 12, true);
}

void BlueMagicCameraController::shutterSpeed(int32_t shutter)
{
  if (shutter < 24 && shutter > 2000)
    return;

  uint8_t l = shutter & 0xff;
  uint8_t h = (shutter >> 8);

  uint8_t data[12] = {255, 8, 0, 0, 1, 12, 3, 0, l, h, 0, 0};
  _cameraControl->writeValue(data, 12, true);
}

int32_t BlueMagicCameraController::getShutter()
{
  return _state->getShutter() / 100;
}

void BlueMagicCameraController::whiteBalance(int16_t whiteBalance, int16_t tint)
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

void BlueMagicCameraController::autoWhiteBalance()
{
  uint8_t data[12] = {255, 4, 0, 0, 1, 3, 1, 0, 0, 0, 0, 0};
  _cameraControl->writeValue(data, 12, true);
}

int16_t BlueMagicCameraController::getWhiteBalance()
{
  return _state->getWhiteBalance();
}

int16_t BlueMagicCameraController::getTint()
{
  return _state->getTint();
}

void BlueMagicCameraController::frameRate(int16_t frameRate)
{
  // if (frameRate < 0)
  //   return;

  uint8_t frL = frameRate & 0xff;
  uint8_t frH = (frameRate >> 8);

  // uint8_t data[18] = {255, 8, 0, 0, 1, 9, 2, 0, frL, frH, 0, 0, 0, 0, 0, 0, 0, 0};
  uint8_t data[18] = {255, 14, 0, 0, 1, 9, 2, 0, frL, frH, frL, frH, 0, 0, 0, 0, 0, 0};
  _cameraControl->writeValue(data, 12, true);
}

int16_t BlueMagicCameraController::getFrameRate()
{
  return _state->getFrameRate();
}

void BlueMagicCameraController::sensorFrameRate(int16_t sensorFrameRate)
{
  if (sensorFrameRate < 0)
    return;

  uint8_t sfrL = sensorFrameRate & 0xff;
  uint8_t sfrH = (sensorFrameRate >> 8);

  uint8_t data[18] = {255, 8, 0, 0, 1, 9, 2, 0, 0, 0, sfrL, sfrH, 0, 0, 0, 0, 0, 0};
  _cameraControl->writeValue(data, 18, true);
}

int16_t BlueMagicCameraController::getSensorFrameRate()
{
  return _state->getSensorFrameRate();
}

int16_t BlueMagicCameraController::getFrameWidth()
{
  return _state->getFrameWidth();
}

int16_t BlueMagicCameraController::getFrameHeight()
{
  return _state->getFrameHeight();
}

String BlueMagicCameraController::timecode()
{
  return _state->getTimecode();
}

uint32_t BlueMagicCameraController::timecodeRaw()
{
  return _state->getTimecodeRaw();
}
