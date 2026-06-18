#include "joystick_ble.h"



static const char HID_SERVICE[] = "1812";
static const char HID_REPORT_MAP[] = "2A4B";
static const char HID_REPORT_DATA[] = "2A4D";
static const uint32_t scanTime = 0; /** 0 = scan forever */
static void notifyCB(NimBLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify);
static void scanEndedCB(NimBLEScanResults results);
static bool doConnect = false;
static NimBLEAdvertisedDevice* advDevice;
static joystick_t Joystick_Report;
static bool Joystick_New_Data = false;


/** Define a class to handle the callbacks when advertisments are received */
class AdvertisedDeviceCallbacks: public NimBLEAdvertisedDeviceCallbacks {
  void onResult(NimBLEAdvertisedDevice* advertisedDevice) {
    if (advertisedDevice->haveServiceUUID() &&
        advertisedDevice->isAdvertisingService(NimBLEUUID(HID_SERVICE))) {
      Serial.print("Advertised HID Device found: ");
      Serial.println(advertisedDevice->toString().c_str());

      /** stop scan before connecting */
      NimBLEDevice::getScan()->stop();
      /** Save the device reference in a global for the client to use*/
      advDevice = advertisedDevice;
      /** Read to connect now */
      doConnect = true;
    }
  }
};

/** Create a single global instance of the callback class to be used by all
 * clients
 */

/**  None of these are required as they will be handled by the library with defaults. **
 **                       Remove as you see fit for your needs                        */
class ClientCallbacks : public NimBLEClientCallbacks {
  void onConnect(NimBLEClient* pClient) {
    Serial.println("Connected");
    /** After connection we should change the parameters if we don't need fast response times.
     *  These settings are 150ms interval, 0 latency, 450ms timout.
     *  Timeout should be a multiple of the interval, minimum is 100ms.
     *  I find a multiple of 3-5 * the interval works best for quick response/reconnect.
     *  Min interval: 120 * 1.25ms = 150, Max interval: 120 * 1.25ms = 150, 0 latency, 60 * 10ms = 600ms timeout
     */
    pClient->updateConnParams(120, 120, 0, 60);
  }

  void onDisconnect(NimBLEClient* pClient) {
    Serial.print(pClient->getPeerAddress().toString().c_str());
    Serial.println(" Disconnected - Starting scan");
    NimBLEDevice::getScan()->start(scanTime, scanEndedCB);
  }

  bool onConnParamsUpdateRequest(NimBLEClient* pClient, const ble_gap_upd_params* params) {

    return true;
  }

  uint32_t onPassKeyRequest() {
    Serial.println("Client Passkey Request");
    return 123456;
  }

  bool onConfirmPIN(uint32_t pass_key) {
    Serial.print("The passkey YES/NO number: ");
    Serial.println(pass_key);
    return true;
  }

  void onAuthenticationComplete(ble_gap_conn_desc* desc){
    if (!desc->sec_state.encrypted) {
      Serial.println("Encrypt connection failed - disconnecting");
      NimBLEDevice::getClientByID(desc->conn_handle)->disconnect();
      return;
    }
  }
};

ClientCallbacks clientCB;

bool connectToServer() {
  NimBLEClient* pClient = nullptr;

  if (NimBLEDevice::getClientListSize()) {
    pClient = NimBLEDevice::getClientByPeerAddress(advDevice->getAddress());
    if (pClient) {
      if (!pClient->connect(advDevice, false)) {
        Serial.println("Reconnect failed");
        return false;
      }
      Serial.println("Reconnected client");
    }
    else {
      pClient = NimBLEDevice::getDisconnectedClient();
    }
  }

  if (!pClient) {
    if (NimBLEDevice::getClientListSize() >= NIMBLE_MAX_CONNECTIONS) {
      Serial.println("Max clients reached - no more connections available");
      return false;
    }

    pClient = NimBLEDevice::createClient();

    Serial.println("New client created");

    pClient->setClientCallbacks(&clientCB, false);
    pClient->setConnectionParams(12, 12, 0, 51);
    pClient->setConnectTimeout(5);


    if (!pClient->connect(advDevice)) {
      /** Created a client but failed to connect, don't need to keep it as it
       * has no data
       */
      NimBLEDevice::deleteClient(pClient);
      Serial.println("Failed to connect, deleted client");
      return false;
    }
  }

  if (!pClient->isConnected()) {
    if (!pClient->connect(advDevice)) {
      Serial.println("Failed to connect");
      return false;
    }
  }

  Serial.print("Connected to: ");
  Serial.println(pClient->getPeerAddress().toString().c_str());
  Serial.print("RSSI: ");
  Serial.println(pClient->getRssi());

  NimBLERemoteService* pSvc = nullptr;
  NimBLERemoteCharacteristic* pChr = nullptr;
  NimBLERemoteDescriptor* pDsc = nullptr;

  pSvc = pClient->getService(HID_SERVICE);
  if (pSvc) {     
    std::vector<NimBLERemoteCharacteristic*>*charvector;
    charvector = pSvc->getCharacteristics(true);
    for (auto &it : *charvector) {
      if (it->getUUID() == NimBLEUUID(HID_REPORT_DATA)) {
        Serial.println(it->toString().c_str());
        if (it->canNotify()) {
          if (!it->subscribe(true, notifyCB)) {
            Serial.println("subscribe notification failed");
            pClient->disconnect();
            return false;
          }
        }
      }
    }
  }
  Serial.println("Done with this device!");
  return true;
}

void BLE_Client_Joystick::begin() {
  /** Initialize NimBLE, no device name spcified as we are not advertising */
  NimBLEDevice::init("");

  NimBLEDevice::setSecurityAuth(true, false, true);
  NimBLEDevice::setPower(ESP_PWR_LVL_P9); /** +9db */

  NimBLEScan* pScan = NimBLEDevice::getScan();

  pScan->setAdvertisedDeviceCallbacks(new AdvertisedDeviceCallbacks());

  pScan->setInterval(45);
  pScan->setWindow(15);

  pScan->setActiveScan(true);
  pScan->start(scanTime, scanEndedCB);
}

void BLE_Client_Joystick::loop() {
  /** Loop here until we find a device we want to connect to */
  if (doConnect) {
    doConnect = false;

    /** Found a device we want to connect to, do it now */
    if (!connectToServer()) {
      NimBLEDevice::getScan()->start(scanTime, scanEndedCB);
    }
  }
  if (Joystick_New_Data) {
    static uint8_t last_x;
    static uint8_t last_y;

    movement_callback_t f = this->get_movement_callback();
    if (f) {
      if ((last_x != Joystick_Report.x) || (last_y != Joystick_Report.y)) {
        (*f)(Joystick_Report.x, Joystick_Report.y);
        last_x = Joystick_Report.x;
        last_y = Joystick_Report.y;
      }
    }
    static uint8_t last_buttons = 0;
    static uint8_t last_select_start = 0;
    //shift 4 bits to right because just high bits change xxxx1111;
    uint8_t changed = last_buttons ^ Joystick_Report.buttons >> 4;
    uint8_t buttons = Joystick_Report.buttons >> 4;
    uint8_t changedUser = last_select_start ^ Joystick_Report.user;
    // select and start buttons
    uint8_t userButtons = Joystick_Report.user;
    for (size_t i = 0; i < 4; i++) {
      button_callback_t f = this->get_button_callback(i);
      if (f && (changed & 1)) {
        (*f)(buttons & 1);
      }
      changed >>= 1;
      buttons >>= 1;
    }

  
    button_callback_t select = this->get_button_callback(6);
    button_callback_t start = this->get_button_callback(7);
    if (select && ((changedUser & 0x04) >> 2)) {
      (*select)(((userButtons & 0x04) >> 2));
    }
    if (start && ((changedUser & 0x08) >> 3)) {
      (*start)((userButtons & 0x08) >> 3);
    }

    last_buttons = Joystick_Report.buttons >> 4;
    last_select_start = Joystick_Report.user;
    Joystick_New_Data = false;
  }
}

/** Notification / Indication receiving handler callback */
// WARNING: This device has 4 Characteristics = 0x2a4d but with different
// handle values.
static void notifyCB(NimBLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {

#if 0
  for (int i = 0; i < length; i++) {
    Serial.printf("%i ", pData[i]);
  }
  Serial.println("");
#endif

  if (pRemoteCharacteristic->getHandle() == 65) {
    memcpy(&Joystick_Report, pData, sizeof(Joystick_Report));
    Joystick_New_Data = true;
  }
}

/** Callback to process the results of the last scan or restart it */
static void scanEndedCB(NimBLEScanResults results) {
  Serial.println("Scan Ended");
}