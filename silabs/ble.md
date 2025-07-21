# BLE

## BLE message format

- GATT-DB:

```C
#define SL_GATTDB_GENERIC_ACCESS             1
#define SL_GATTDB_DEVICE_NAME                3
#define SL_GATTDB_APPEARANCE                 5
#define SL_GATTDB_GENERIC_ATTRIBUTE          6
#define SL_GATTDB_SERVICE_CHANGED            8
#define SL_GATTDB_DEVICE_INFORMATION         10
#define SL_GATTDB_MANUFACTURER_NAME_STRING   12
#define SL_GATTDB_MODEL_NUMBER_STRING        14
#define SL_GATTDB_SERIAL_NUMBER_STRING       16
#define SL_GATTDB_HARDWARE_REVISION_STRING   18
#define SL_GATTDB_SOFTWARE_REVISION_STRING   20
#define SL_GATTDB_SYSTEM_ID                  22
#define SL_GATTDB_CHAT_SERVICE               23
#define SL_GATTDB_CHAT_COMMAND               25
#define SL_GATTDB_CHAT_RESPONSE              27
#define SL_GATTDB_CHAT_NOTIFIER              29
#define SL_GATTDB_SHADE_CONTROL              31
#define SL_GATTDB_SHADE_COMMAND              33
#define SL_GATTDB_SHADE_CURRENT_PROPERTIES   35
#define SL_GATTDB_SHADE_FIXED_POSITION       38
#define SL_GATTDB_SHADE_ACCELERATION         40
#define SL_GATTDB_SHADE_MODE                 42
#define SL_GATTDB_SHADE_SETTING              45
#define SL_GATTDB_ZWAVE_SHADE_CONTROL        46
#define SL_GATTDB_ZWAVE_COMMAND              48
#define SL_GATTDB_ZWAVE_STATUS               50
#define SL_GATTDB_ZWAVE_NODE_ID              53
#define SL_GATTDB_ZWAVE_NETWORK_ID           56
#define SL_GATTDB_BATTERY_SERVICE            57
#define SL_GATTDB_BATTERY_LEVEL              59
#define SL_GATTDB_CURRENT_TIME_SERVICE       62
#define SL_GATTDB_CURRENT_TIME               64
#define SL_GATTDB_LOCAL_TIME_INFORMATION     67
#define SL_GATTDB_REFERENCE_TIME_INFORMATION 69
```
