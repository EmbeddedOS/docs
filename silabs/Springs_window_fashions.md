# Springs Window Fashions

## Shade control method

- The third generation shades can be controlled in 4 ways:
  - **Direct control through BLE**:
    - A `single shade` is connected to the smartphone via BLE, and the app will issue commands directly to the shade.
    - In case of a command is expected to affect a `group of shades`, the mobile application shall use Wi-Fi instead to directly issue commands.
  - **Local control through Wi-Fi**:
    - This is the scenario when the mobile app is in the same local Wi-Fi connection as the shades, and a command must be issued (by the app) to one or more shades. In this case, no connection to the cloud is required.
  - **Remote Control**: In this case, the cloud is sending/receiving commands from shades. This is when the smartphone does not have access to the same Wi-Fi connection in which the shades are present. The cloud might also send other commands (e.g. firmware update) without app’s/user’s intervention.
  - **Z-Wave**: This is when the shade is also included in a Z-Wave network. Commands are received from the Z-Wave NCP.

## Bluetooth and Wi-Fi coexistence

- Both BLE and WiFi operate in the same 2.4-GHz ISM band. To improve communication reliability, two strategies can be taken.
  - 1. Using non overlapping channels as shown in the figure below. The advertising channels, for instance, are using these frequencies to prevent contention.
  - 2. Implement Wi-Fi and BLE coexistence algorithm (i.e preventing one radio from transmitting when the other one has taken the ownership). *The SiW-917 embedded stack implements Wi-Fi coexistence solution*, based on IEEE 802.15.2 packet traffic arbitration (PTA).

## Motor Driver

- Two motor drivers shall be supported.
- The first one comes with an UART interface, which allow so send commands and get status reports.
  - The communication occurs using a 19200 bps, 8 bits per character, no parity, 1 stop bits UART interface.
  - The second motor driver is based on TMC2300 IC.

## Functional Specification

### Introduction

- After each reset,the device boot-loader will establish if a device update is required.
- If no update is required, the device shall verify and execute the application.

- The application will set up the system operate according to the functional specification described in the followings.

### Bluetooth operation

- Bluetooth is used to *allow direct connection between the smartphone mobile app and a single shade*. When a connection to a single shade is established, the following operations can be performed:
  - Network configuration (WiFi and Z-Wave).
  - Fast, direct control of a single shade.

- The device shall support a single simultaneous connection to a smartphone running the mobile app. The device will operate as BLE peripheral, and will advertise with an interval of `1s`. Such time is considered small enough **not to impact the user experience**, but large enough to allow for a **significant power saving**.

- After a period of inactivity, BLE connection shall be closed and the device should be brought to advertising mode.

- Wifi network credentials are transmitted in encrypted format via BLE.

### WiFi 6 operation

- Due to power budget constraints, WiFi connection is available only on WiFi 6 networks, i.e. where the router supports the IEEE 802.11.ax.

- The device will make use of TWT (target Wake Time) to improve battery life by:
  - Allowing to go into the deepest power mode for thw majority of the device lifetime.
  - Prevent collision with other devices, i.e. reducing the number of re-transmission events.

- During night-time, the TWT period shall be extended, to improve the battery life even further. This feature is by default enabled, but the end-user will have the ability of disable it/

- Wi-Fi operation is used for:
  - **Local operation**. This is used when the end user is in range with the local Wi-Fi network. In this case the mobile application can send commands via Wi-Fi to one or more shades. A shade cannot forward messages to more than 5 other shades without cloud assistance.
  - **Remote operation**. This occurs when:
    - The user is out of range of both BLE and local Wi-Fi network, and wants to issue any kind of commands to one or more shades.
    - The user has direct BLE connection to at least one shade, and wants to issue a command to a group of shades.

### Z-Wave network

- Z-Wave network is used for integration of third generation shades on the end-customer existing Z-wave network, which might include First and Second generation shades as well.
  - old generation shades will communicate with the cloud through the Z-Wave gateway.
  - 3rd generation shades will communicate with the cloud through the Wi-Fi 6 router.

### Time Synchronization

- To comply with the *maximum shade-to-shade delay*, an accurate time synchronization must be periodically performed, to prevent the so called “pop-corn” effect, i.e. when shades from the same group start moving at different, random times.

- An absolute time precision is not required. Instead, relative synchronization is mandatory, and the maximum shade-to-shade delay should be **no larger than 100 ms**.

- Wi-Fi Timing synchronization function can be used at this purpose. The device can periodically wake-up and listen for AP beacons, which contain the timestamp, in microsecond. As calculated before, the frequency of such synchronization is some orders of magnitude larger than the TWT period, e.g. once per hour.

### Cloud

- The IOT Cloud will be Google CLoud.
- The connection should be encrypted with TLS1.3.
