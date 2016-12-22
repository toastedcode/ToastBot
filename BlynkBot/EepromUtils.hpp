#include "ConfigServer.hpp"

class EepromUtils
{

public:

   static void begin();

   static DeviceConfig getDeviceConfig();

   static void setDeviceConfig(
    const DeviceConfig& deviceConfig);

   static WifiConfig getWifiConfig();

   static void setWifiConfig(
      const WifiConfig& wifiConfig);

private:    

  static void initializeEeprom();
};

