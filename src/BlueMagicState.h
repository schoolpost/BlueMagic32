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

    // bool _recording = false;
    int8_t _transportMode = 0;
    uint8_t _status = 0;

    float _aperture = 0;
    float _focus = 0;
    int16_t _zoom = 0;

    int32_t _iso = 0;
    int32_t _shutter = 0;

    int8_t _codec = 0;
    int8_t _quality = 0;
    int16_t _frameRate = 0;
    int16_t _sensorFrameRate = 0;
    int16_t _frameWidth = 0;
    int16_t _frameHeight = 0;

    int16_t _tint = 0;
    int16_t _whiteBalance = 0;

    bool _changed = false;

    uint8_t *_settingsData = nullptr;
    uint8_t *_timecodeData = nullptr;
    uint8_t *_statusData = nullptr;

    bool _settingsChanged = false;
    bool _timecodeChanged = false;
    bool _statusChanged = false;

public:
    static BlueMagicState *getInstance();

    void setChanged(bool changed);
    bool changed();

    void settingsNotify(bool changed, uint8_t *data);
    bool settingsChanged();
    uint8_t *settingsData();

    void timecodeNotify(bool changed, uint8_t *data);
    bool timecodeChanged();
    uint8_t *timecodeData();

    void statusNotify(bool changed, uint8_t *data);
    bool statusChanged();
    uint8_t *statusData();

    void setCameraStatus(uint8_t status);
    uint8_t getCameraStatus();

    void setTimecode(uint8_t H, uint8_t M, uint8_t S, uint8_t f);
    String getTimecode();
    uint32_t getTimecodeRaw();

    void setTransportMode(int8_t mode);
    int8_t getTransportMode();

    void setAperture(float aperture);
    float getAperture();

    void setFocus(float aperture);
    float getFocus();

    void setZoom(int16_t zoom);
    int16_t getZoom();

    void setIso(uint32_t iso);
    int32_t getIso();

    void setShutter(int32_t shutter);
    int32_t getShutter();

    void setCodec(int8_t codec);
    int8_t getCodec();

    void setQuality(int8_t quality);
    int8_t getQuality();

    void setFrameRate(int16_t frameRate);
    int16_t getFrameRate();

    void setSensorFrameRate(int16_t sensorFrameRate);
    int16_t getSensorFrameRate();

    void setFrameWidth(int16_t width);
    int16_t getFrameWidth();

    void setFrameHeight(int16_t height);
    int16_t getFrameHeight();

    void setWhiteBalance(int16_t whiteBalance);
    int16_t getWhiteBalance();

    void setTint(int16_t tint);
    int16_t getTint();
};

#endif