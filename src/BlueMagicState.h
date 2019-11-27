#ifndef BlueMagicState_h

#define BlueMagicState_h
#include "Arduino.h"

class BlueMagicState
{
private:
    static BlueMagicState *instance;
    BlueMagicState();
    String _timecode;
    uint32_t _timecodeRaw;

    bool _recording = false;
    uint8_t _status = 0;

    float _aperture = 0;
    float _focus = 0;

    int32_t _iso = 0;
    int32_t _shutter = 0;
    int16_t _whiteBalance = 0;
    int16_t _frameRate = 0;
    int16_t _sensorFrameRate = 0;
    int16_t _frameWidth = 0;
    int16_t _frameHeight = 0;

public:
    static BlueMagicState *getInstance();

    void setCameraStatus(uint8_t status);
    uint8_t getCameraStatus();

    void setTimecode(uint8_t H, uint8_t M, uint8_t S, uint8_t f);
    String getTimecode();
    uint32_t getTimecodeRaw();

    void setRecording(bool recording);
    bool getRecording();

    void setAperture(float aperture);
    float getAperture();

    void setFocus(float aperture);
    float getFocus();

    void setIso(uint32_t iso);
    int32_t getIso();

    void setShutter(uint16_t shutter);
    int32_t getShutter();

    void setWhiteBalance(uint16_t whiteBalance);
    int16_t getWhiteBalance();

    void setFrameRate(int16_t frameRate);
    int16_t getFrameRate();

    void setSensorFrameRate(int16_t sensorFrameRate);
    int16_t getSensorFrameRate();
};

#endif