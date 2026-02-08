#pragma once

#include <NimBLEDevice.h>
#include <NimBLEHIDDevice.h>

#define HID_MOUSE 0x03C2

class BleHID {
public:
    BleHID();
    ~BleHID();

    void begin();
    void end();
    bool isConnected();
    void sendScrollEvent(int delta);

    // Public method so the callback can update state
    void setConnectionStatus(bool connected);

private:
    NimBLEServer *server;
    NimBLEHIDDevice *hid;
    NimBLECharacteristic *inputMouse;
    bool deviceConnected = false;
};