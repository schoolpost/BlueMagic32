#include "BluetoothCameraConnection.h"

BLERemoteCharacteristic *BluetoothCameraConnection::_cameraStatus;
BLERemoteCharacteristic *BluetoothCameraConnection::_deviceName;
BLERemoteCharacteristic *BluetoothCameraConnection::_timecode;
BLERemoteCharacteristic *BluetoothCameraConnection::_outgoingCameraControl;
BLERemoteCharacteristic *BluetoothCameraConnection::_incomingCameraControl;

static BLEUUID OutgoingCameraControl("5DD3465F-1AEE-4299-8493-D2ECA2F8E1BB");
static BLEUUID IncomingCameraControl("B864E140-76A0-416A-BF30-5876504537D9");
static BLEUUID Timecode("6D8F2110-86F1-41BF-9AFB-451D87E976C8");
static BLEUUID CameraStatus("7FE8691D-95DC-4FC5-8ABD-CA74339B51B9");
static BLEUUID DeviceName("FFAC0C52-C9FB-41A0-B063-CC76282EB89C");

static BLEUUID ServiceId("00001800-0000-1000-8000-00805f9b34fb");
static BLEUUID BmdCameraService("291D567A-6D75-11E6-8B77-86F30CA893D3");

static void cameraStatusNotify(BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *pData, size_t length, bool isNotify)
{
  BlueMagicState *blu = BlueMagicState::getInstance();
  blu->setCameraStatus(pData[0]);
}

static void timeCodeNotify(BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *pData, size_t length, bool isNotify)
{
  // timecode
  uint8_t H, M, S, f;
  H = (pData[11] / 16 * 10) + (pData[11] % 16);
  M = (pData[10] / 16 * 10) + (pData[10] % 16);
  S = (pData[9] / 16 * 10) + (pData[9] % 16);
  f = (pData[8] / 16 * 10) + (pData[8] % 16);

  BlueMagicState *blu = BlueMagicState::getInstance();
  blu->setTimecode(H, M, S, f);
}

static void controlNotify(BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *pData, size_t length, bool isNotify)
{
  BlueMagicState *blu = BlueMagicState::getInstance();

  // recording
  if (length == 13 && pData[0] == 255 && pData[1] == 9 && pData[4] == 10 && pData[5] == 1)
  {
    if (pData[8] == 0)
    {
      blu->setRecording(false);
    }
    if (pData[8] == 2)
    {
      blu->setRecording(true);
    }
  }

  // aperture
  if (pData[0] == 255 && pData[4] == 0 && pData[5] == 2)
  {
    uint16_t low = pData[8];
    uint16_t high = pData[9] << 8;
    float aperture = sqrt(pow(2, (float(low + high) / 2048.0)));
    blu->setAperture(aperture);
  }

  // iso
  if (pData[0] == 255 && pData[4] == 1 && pData[5] == 14)
  {
    uint16_t low = pData[8];
    uint16_t high = pData[9] << 8;
    int32_t iso = low + high;
    blu->setIso(iso);
  }

  // shutter
  if (pData[0] == 255 && pData[4] == 1 && pData[5] == 11)
  {
    uint16_t low = pData[8];
    uint16_t high = pData[9] << 8;
    int32_t shutter = low + high;
    blu->setShutter(shutter);
  }
}

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{

  void onResult(BLEAdvertisedDevice advertisedDevice)
  {
    Serial.println("BLE Advertised Device found: ");

    if (advertisedDevice.haveServiceUUID() && advertisedDevice.getServiceUUID().equals(ServiceId))
    {
      advertisedDevice.getScan()->stop();
    }
    else
    {
      advertisedDevice.getScan()->erase(advertisedDevice.getAddress());
    }
  }
};

class MySecurity : public BLESecurityCallbacks
{
  uint32_t onPassKeyRequest()
  {
    Serial.println("---> PLEASE ENTER 6 DIGIT PIN (end with ENTER) : ");
    int pinCode = 0;
    char ch;
    do
    {
      while (!Serial.available())
      {
        vTaskDelay(1);
      }
      ch = Serial.read();
      if (ch >= '0' && ch <= '9')
      {
        pinCode = pinCode * 10 + (ch - '0');
        Serial.print(ch);
      }
    } while ((ch != '\n'));
    return pinCode;
  }

  void onPassKeyNotify(uint32_t pass_key)
  {
    pass_key + 1;
  }

  bool onConfirmPIN(uint32_t pin)
  {
    return true;
  }

  bool onSecurityRequest()
  {
    return true;
  }

  void onAuthenticationComplete(esp_ble_auth_cmpl_t auth_cmpl)
  {
    int index = 1;
  }
};

BluetoothCameraConnection::BluetoothCameraConnection()
{
}

BluetoothCameraConnection::~BluetoothCameraConnection()
{
  delete _cameraStatus, _deviceName, _timecode, _outgoingCameraControl, _incomingCameraControl, _device;
  delete _client;
  delete _cameraControl;
  _device.deinit(true);
}

void BluetoothCameraConnection::begin()
{
  begin("BlueMagic32");
}

void BluetoothCameraConnection::begin(String name)
{
  _name = name;
  setState(CAMERA_DISCONNECTED);

  if (!PREF_INCLUDED)
  {
    _pref = new Preferences();
  }

  _pref->begin(_name.c_str(), false);

  setAuthentication(_pref->getBool("authenticated", false));
  String addr = _pref->getString("cameraAddress", "");
  if (addr.length() > 0)
  {
    setCameraAddress(BLEAddress(addr.c_str()));
  }

  _pref->end();

  _device.init(_name.c_str());
  _device.setPower(ESP_PWR_LVL_P9);
  _device.setEncryptionLevel(ESP_BLE_SEC_ENCRYPT);
  _device.setSecurityCallbacks(new MySecurity());

  BLESecurity *pSecurity = new BLESecurity();
  pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_BOND);
  pSecurity->setCapability(ESP_IO_CAP_IN);
  pSecurity->setRespEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);
}

bool BluetoothCameraConnection::scan(bool active, int duration)
{
  if (getAuthentication() && getCameraAddress() != nullptr)
  {
    return false;
  }
  else
  {
    _bleScan = _device.getScan();
    _bleScan->clearResults();
    _bleScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    _bleScan->setActiveScan(active);
    _bleScan->start(duration);
  }
  return true;
}

int BluetoothCameraConnection::connected()
{
  return _connected;
}

bool BluetoothCameraConnection::available()
{
  return connected() && (_cameraControl != nullptr);
}

bool BluetoothCameraConnection::connectToServer(BLEAddress address)
{
  _client = _device.createClient();
  setState(CAMERA_CONNECTING);
  _client->connect(address);

  BLERemoteService *pRemoteService = _client->getService(BmdCameraService);
  if (pRemoteService == nullptr)
  {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(BmdCameraService.toString().c_str());
    return false;
  }

  _deviceName = pRemoteService->getCharacteristic(DeviceName);
  if (_deviceName != nullptr)
  {
    _deviceName->writeValue(_name.c_str(), _name.length());
  }

  _outgoingCameraControl = pRemoteService->getCharacteristic(OutgoingCameraControl);
  if (_outgoingCameraControl == nullptr)
  {
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(OutgoingCameraControl.toString().c_str());
    return false;
  }

  _incomingCameraControl = pRemoteService->getCharacteristic(IncomingCameraControl);
  if (_incomingCameraControl == nullptr)
  {
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(IncomingCameraControl.toString().c_str());
    return false;
  }
  _incomingCameraControl->registerForNotify(controlNotify, false);

  _timecode = pRemoteService->getCharacteristic(Timecode);
  if (_timecode == nullptr)
  {
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(Timecode.toString().c_str());
    return false;
  }
  _timecode->registerForNotify(timeCodeNotify);

  _cameraStatus = pRemoteService->getCharacteristic(CameraStatus);
  if (_cameraStatus == nullptr)
  {
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(CameraStatus.toString().c_str());
    return false;
  }
  _cameraStatus->registerForNotify(cameraStatusNotify);

  setState(CAMERA_CONNECTED);
  setController();
  return true;
}

void BluetoothCameraConnection::setController()
{
  _cameraControl = new BluetoothCameraController(_outgoingCameraControl);
}

void BluetoothCameraConnection::setState(CONNECTION_STATE state)
{
  _connected = state;
}

void BluetoothCameraConnection::setAuthentication(bool authenticated)
{
  _authenticated = authenticated;
}

bool BluetoothCameraConnection::getAuthentication()
{
  return _authenticated;
}

void BluetoothCameraConnection::setCameraAddress(BLEAddress address)
{
  _cameraAddress = &address;
}

BLEAddress *BluetoothCameraConnection::getCameraAddress()
{
  return _cameraAddress;
}

BluetoothCameraController *BluetoothCameraConnection::connect()
{
  return connect(0);
}

BluetoothCameraController *BluetoothCameraConnection::connect(uint8_t index)
{
  if (_cameraControl != nullptr)
  {
    return _cameraControl;
  }

  bool ok;
  bool scanned = scan(false, 5);

  BLEAddress address = BLEAddress("FF:FF:FF:FF:FF");

  if (scanned)
  {
    address = _bleScan->getResults().getDevice(index).getAddress();
    ok = connectToServer(address);
  }
  else
  {
    address = *getCameraAddress();
    ok = connectToServer(address);
  }

  if (ok)
  {
    setAuthentication(true);
    _pref->begin(_name.c_str(), false);
    _pref->putString("cameraAddress", address.toString().c_str());
    _pref->putBool("authenticated", getAuthentication());
    _pref->end();
    setCameraAddress(address);
    return _cameraControl;
  }

  return nullptr;
}

void BluetoothCameraConnection::disconnect()
{
  _client->disconnect();
  delete _cameraControl;
  _cameraControl = nullptr;
  setState(CAMERA_DISCONNECTED);
}

void BluetoothCameraConnection::clearPairing()
{
  if (connected() != CAMERA_DISCONNECTED)
  {
    disconnect();
  }
  esp_ble_remove_bond_device(*getCameraAddress()->getNative());
  setAuthentication(false);
  setCameraAddress(nullptr);
  _pref->begin(_name.c_str(), false);
  _pref->putString("cameraAddress", "");
  _pref->putBool("authenticated", getAuthentication());
  _pref->end();
}
