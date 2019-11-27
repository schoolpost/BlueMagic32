
#ifndef BLEDevice_h
#include <BLEDevice.h>
#endif

#ifndef Preferences_h
#include <Preferences.h>
#define PREF_INCLUDED false
#endif

#ifndef BluetoothCameraConnection_h

#define BluetoothCameraConnection_h

#include "Arduino.h"
#include "BlueMagicState.h"
#include "BluetoothCameraController.h"

enum CONNECTION_STATE
{
  CAMERA_CONNECTED = 1,
  CAMERA_DISCONNECTED = 2,
  CAMERA_CONNECTING = 3
};

class BluetoothCameraConnection
{

public:
  BluetoothCameraConnection();
  ~BluetoothCameraConnection();

  void begin();
  void begin(String name);
  void begin(String name, Preferences &pref);

  bool scan(bool active, int duration);

  BluetoothCameraController *connect();
  BluetoothCameraController *connect(uint8_t index);

  void disconnect();

  void clearPairing();

  bool available();

private:
  String _name;
  Preferences *_pref;

  bool _authenticated;
  int _connected;

  BLEDevice _device;
  BLEClient *_client;

  BLEAddress *_cameraAddress = nullptr;

  static BLERemoteCharacteristic *_outgoingCameraControl;
  static BLERemoteCharacteristic *_incomingCameraControl;
  static BLERemoteCharacteristic *_timecode;
  static BLERemoteCharacteristic *_cameraStatus;
  static BLERemoteCharacteristic *_deviceName;

  BLEScan *_bleScan;

  BluetoothCameraController *_cameraControl = nullptr;

  bool connectToServer(BLEAddress address);

  int connected();
  bool authenticated();

  void setController();

  void setState(CONNECTION_STATE state);
  void setAuthentication(bool authenticated);
  bool getAuthentication();
  void setCameraAddress(BLEAddress address);
  BLEAddress *getCameraAddress();
};

#endif
