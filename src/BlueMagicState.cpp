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

void BlueMagicState::setRecording(bool recording)
{
    _recording = recording;
}

bool BlueMagicState::getRecording()
{
    return _recording;
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

void BlueMagicState::setIso(uint32_t iso)
{
    _iso = iso;
}

int32_t BlueMagicState::getIso()
{
    return _iso;
}

void BlueMagicState::setShutter(uint16_t shutter)
{
    _shutter = shutter;
}

int32_t BlueMagicState::getShutter()
{
    return _shutter;
}
