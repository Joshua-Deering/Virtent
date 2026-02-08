#include "Ble_HID.hpp"

/* * Standard Mouse + Scroll Wheel Report Descriptor
 * (Raw hex to avoid macro dependency issues)
 */
static const uint8_t hidReportDescriptor[] = {
    0x05, 0x01, // USAGE_PAGE (Generic Desktop)
    0x09, 0x02, // USAGE (Mouse)
    0xa1, 0x01, // COLLECTION (Application)
    0x09, 0x01, //   USAGE (Pointer)
    0xa1, 0x00, //   COLLECTION (Physical)
    0x85, 0x01, // REPORT_ID (1) - Matches hid->getInputReport(1)
    0x05, 0x09, //     USAGE_PAGE (Button)
    0x19, 0x01, //     USAGE_MINIMUM (Button 1)
    0x29, 0x03, //     USAGE_MAXIMUM (Button 3)
    0x15, 0x00, //     LOGICAL_MINIMUM (0)
    0x25, 0x01, //     LOGICAL_MAXIMUM (1)
    0x95, 0x03, //     REPORT_COUNT (3)
    0x75, 0x01, //     REPORT_SIZE (1)
    0x81, 0x02, //     INPUT (Data,Var,Abs)
    0x95, 0x01, //     REPORT_COUNT (1)
    0x75, 0x05, //     REPORT_SIZE (5)
    0x81, 0x03, //     INPUT (Cnst,Var,Abs)
    0x05, 0x01, //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30, //     USAGE (X)
    0x09, 0x31, //     USAGE (Y)
    0x09, 0x38, //     USAGE (Wheel)
    0x15, 0x81, //     LOGICAL_MINIMUM (-127)
    0x25, 0x7f, //     LOGICAL_MAXIMUM (127)
    0x75, 0x08, //     REPORT_SIZE (8)
    0x95, 0x03, //     REPORT_COUNT (3)
    0x81, 0x06, //     INPUT (Data,Var,Rel)
    0xc0,       //   END_COLLECTION
    0xc0        // END_COLLECTION
};

/* * Callback Class
 * UPDATED for NimBLE 2.0 signatures
 */
class ServerCallbacks : public NimBLEServerCallbacks {
    BleHID *_hidInstance;

public:
    // This constructor fixes the "no matching function" error
    ServerCallbacks(BleHID *hid) : _hidInstance(hid) {}

    // NimBLE 2.0 requires these specific parameters
    void onConnect(NimBLEServer *pServer, NimBLEConnInfo &connInfo) override {
        _hidInstance->setConnectionStatus(true);
        Serial.println("Bluetooth Connecting!");
        // // Optional: Update connection parameters
        // pServer->updateConnParams(connInfo.getConnHandle(), 6, 6, 0, 100);
    }

    void onDisconnect(NimBLEServer *pServer, NimBLEConnInfo &connInfo, int reason) override {
        _hidInstance->setConnectionStatus(false);
        Serial.print("Bluetooth Disconnecting! (Reason ");
        Serial.print(reason);
        Serial.println(")");

        if (reason != 531) { // if user manually disconnects, we stop advertising
            NimBLEDevice::startAdvertising();
        }
    }
};

/* BleHID Implementation */

BleHID::BleHID() : deviceConnected(false) {}

BleHID::~BleHID() {}

void BleHID::setConnectionStatus(bool connected) {
    deviceConnected = connected;
}

void BleHID::begin() {
    NimBLEDevice::init("Scroll Wheel Prototype");

    server = NimBLEDevice::createServer();
    server->setCallbacks(new ServerCallbacks(this));

    // hid setup
    hid = new NimBLEHIDDevice(server);
    inputMouse = hid->getInputReport(1);
    hid->setManufacturer("Joshua Deering");
    hid->setPnp(0x02, 0x1234, 0x5678, 0x0100);
    hid->setHidInfo(0x00, 0x01);
    hid->setReportMap((uint8_t *)hidReportDescriptor, sizeof(hidReportDescriptor));
    hid->startServices();

    // security/bonding setup
    NimBLEDevice::setSecurityAuth(true, false, false);
    NimBLEDevice::setSecurityIOCap(BLE_HS_IO_NO_INPUT_OUTPUT);
    NimBLEDevice::setSecurityInitKey(BLE_SM_PAIR_KEY_DIST_ENC | BLE_SM_PAIR_KEY_DIST_ID);
    NimBLEDevice::setSecurityRespKey(BLE_SM_PAIR_KEY_DIST_ENC | BLE_SM_PAIR_KEY_DIST_ID);

    // advertising
    NimBLEAdvertising *advertising = NimBLEDevice::getAdvertising();

    NimBLEAdvertisementData scanResponse;
    scanResponse.setName("Scroll Wheel Prototype");
    advertising->setScanResponseData(scanResponse);

    advertising->setAppearance(HID_MOUSE);
    advertising->addServiceUUID(hid->getHidService()->getUUID());
    advertising->start();

    hid->setBatteryLevel(100);
    Serial.println("Bluetooth Connected");
}

void BleHID::end() {
    NimBLEDevice::deinit(true);
    server = nullptr;
    inputMouse = nullptr; 
    hid = nullptr;
    Serial.println("Bluetooth Killed");
}

bool BleHID::isConnected() {
    return deviceConnected;
}

void BleHID::sendScrollEvent(int delta) {
    if (deviceConnected) {
        // [Buttons, X, Y, Wheel]
        uint8_t report[4] = {0, 0, 0, (uint8_t)delta};
        inputMouse->setValue(report, sizeof(report));
        inputMouse->notify();
    }
    // if we are not connected and not advertising, start advertising so that wheel doesnt just go dead
    // !! will enable once scroll logic is in place !!
    else if (!NimBLEDevice::getAdvertising()->isAdvertising()) {
        NimBLEDevice::startAdvertising();
    }
}