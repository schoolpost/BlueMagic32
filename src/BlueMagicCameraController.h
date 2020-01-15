#ifndef BlueMagicCameraController_h

#define BlueMagicCameraController_h

#include <BLEDevice.h>
#include "BlueMagicState.h"

enum CODEC_TYPE
{
  RAW = 0,
  DNXHD = 1,
  PRORES = 2,
  BRAW = 3
};

enum CODEC_QUALITY
{
  RAW_U = 0,
  RAW_31 = 1,
  RAW_41 = 2,
  PRORES_HQ = 0,
  PRORES_422 = 1,
  PRORES_LT = 2,
  PRORES_PROXY = 3,
  PRORES_444 = 4,
  PRORES_444XQ = 5,
  BRAW_Q0 = 0,
  BRAW_Q5 = 1,
  BRAW_31 = 2,
  BRAW_51 = 3,
  BRAW_81 = 4,
  BRAW_121 = 5
};

class BlueMagicCameraController
{
public:
  ~BlueMagicCameraController();
  BlueMagicCameraController();

  bool changed();

  bool settingsChanged();
  uint8_t *settingsData();

  bool timecodeChanged();
  uint8_t *timecodeData();

  bool statusChanged();
  uint8_t *statusData();

  void setCamera(uint8_t index);
  void custom(uint8_t *data, size_t len);

  uint8_t getCameraStatus();

  int8_t getTransportMode();

  void record(bool recording);
  void toggleRecording();
  bool isRecording();

  void play(bool play);
  bool isPlaying();

  void preview(bool preview);
  bool isPreviewing();

  void codec(CODEC_TYPE c, CODEC_QUALITY q);
  int8_t getCodecType();
  int8_t getCodecQuality();

  void focus(float value);
  float getFocus();
  void instantAutoFocus();

  void ois(bool enabled);
  bool getOis();

  void zoom(float value);
  float getZoom();

  void aperture(float value);
  void autoAperture();
  float getAperture();

  void autoExposureMode(int value);

  void iso(int32_t iso);
  int32_t getIso();

  void shutterSpeed(int32_t value);
  void shutterAngle(int32_t value);
  int32_t getShutter();

  void whiteBalance(int16_t whiteBalance, int16_t tint);
  void autoWhiteBalance();
  int16_t getWhiteBalance();
  int16_t getTint();

  void frameRate(int16_t frameRate);
  int16_t getFrameRate();

  void sensorFrameRate(int16_t sensorFrameRate);
  int16_t getSensorFrameRate();

  void resolution(int16_t width, int16_t height);
  int16_t getFrameWidth();
  int16_t getFrameHeight();

  String timecode();
  uint32_t timecodeRaw();

private:
  friend class BlueMagicCameraConnection;
  BlueMagicCameraController(BLERemoteCharacteristic *outgoingCameraControl);
  BLERemoteCharacteristic *_cameraControl;
  BlueMagicState *_state = BlueMagicState::getInstance();
  uint8_t _cameraIndex = 255;
  uint32_t mapFloat(float value);
};

#endif
