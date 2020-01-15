#include "BlueMagicState.h"

BlueMagicState *BlueMagicState::instance = 0;

BlueMagicState *BlueMagicState::getInstance()
{
    if (instance == 0)
    {
        instance = new BlueMagicState();
    }

    return instance;
}

BlueMagicState::BlueMagicState()
{
}

void BlueMagicState::setChanged(bool changed)
{
    _changed = changed;
}

bool BlueMagicState::changed()
{
    bool changed = _changed;
    _changed = false;
    return changed;
}

void BlueMagicState::settingsNotify(bool changed, uint8_t *data)
{
    _settingsChanged = changed;
    _settingsData = data;
}

bool BlueMagicState::settingsChanged()
{
    bool changed = _settingsChanged;
    _settingsChanged = false;
    return changed;
}

uint8_t *BlueMagicState::settingsData()
{
    return _settingsData;
}

void BlueMagicState::timecodeNotify(bool changed, uint8_t *data)
{
    _timecodeChanged = changed;
    _timecodeData = data;
}

bool BlueMagicState::timecodeChanged()
{
    bool changed = _timecodeChanged;
    _timecodeChanged = false;
    return changed;
}

uint8_t *BlueMagicState::timecodeData()
{
    return _timecodeData;
}

void BlueMagicState::statusNotify(bool changed, uint8_t *data)
{
    _statusChanged = changed;
    _statusData = data;
}

bool BlueMagicState::statusChanged()
{
    bool changed = _statusChanged;
    _statusChanged = false;
    return changed;
}

uint8_t *BlueMagicState::statusData()
{
    return _statusData;
}

void BlueMagicState::setCameraStatus(uint8_t status)
{
    _status = status;
}

uint8_t BlueMagicState::getCameraStatus()
{
    return _status;
}

void BlueMagicState::setTimecode(uint8_t H, uint8_t M, uint8_t S, uint8_t f)
{
    char timecode[12] = "00:00:00:00";
    char timecodeR[10] = "00000000";
    sprintf(timecode, "%02d:%02d:%02d:%02d", H, M, S, f);
    sprintf(timecodeR, "%02d%02d%02d%02d", H, M, S, f);
    _timecode = String(timecode);
    _timecodeRaw = String(timecodeR).toInt();
}

String BlueMagicState::getTimecode()
{
    return _timecode;
}

uint32_t BlueMagicState::getTimecodeRaw()
{
    return _timecodeRaw;
}

void BlueMagicState::setTransportMode(int8_t mode)
{
    _transportMode = mode;
}

int8_t BlueMagicState::getTransportMode()
{
    return _transportMode;
}

void BlueMagicState::setAperture(float aperture)
{
    _aperture = aperture;
}

float BlueMagicState::getAperture()
{
    return _aperture;
}

void BlueMagicState::setFocus(float focus)
{
    _focus = focus;
}

float BlueMagicState::getFocus()
{
    return _focus;
}

void BlueMagicState::setZoom(int16_t zoom)
{
    _zoom = zoom;
}

int16_t BlueMagicState::getZoom()
{
    return _zoom;
}

void BlueMagicState::setIso(uint32_t iso)
{
    _iso = iso;
}

int32_t BlueMagicState::getIso()
{
    return _iso;
}

void BlueMagicState::setShutter(int32_t shutter)
{
    _shutter = shutter;
}

int32_t BlueMagicState::getShutter()
{
    return _shutter;
}

void BlueMagicState::setCodec(int8_t codec)
{
    _codec = codec;
}

int8_t BlueMagicState::getCodec()
{
    return _codec;
}

void BlueMagicState::setQuality(int8_t quality)
{
    _quality = quality;
}

int8_t BlueMagicState::getQuality()
{
    return _quality;
}

void BlueMagicState::setFrameRate(int16_t frameRate)
{
    _frameRate = frameRate;
}

int16_t BlueMagicState::getFrameRate()
{
    return _frameRate;
}

void BlueMagicState::setSensorFrameRate(int16_t sensorFrameRate)
{
    _sensorFrameRate = sensorFrameRate;
}

int16_t BlueMagicState::getSensorFrameRate()
{
    return _sensorFrameRate;
}

void BlueMagicState::setFrameWidth(int16_t width)
{
    _frameWidth = width;
}

int16_t BlueMagicState::getFrameWidth()
{
    return _frameWidth;
}

void BlueMagicState::setFrameHeight(int16_t height)
{
    _frameHeight = height;
}

int16_t BlueMagicState::getFrameHeight()
{
    return _frameHeight;
}

void BlueMagicState::setWhiteBalance(int16_t whiteBalance)
{
    _whiteBalance = whiteBalance;
}

int16_t BlueMagicState::getWhiteBalance()
{
    return _whiteBalance;
}

void BlueMagicState::setTint(int16_t tint)
{
    _tint = tint;
}

int16_t BlueMagicState::getTint()
{
    return _tint;
}
