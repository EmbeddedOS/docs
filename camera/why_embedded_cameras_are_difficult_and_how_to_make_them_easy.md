# Why embedded cameras are difficult and how to make them easy

- Problem:
  - Cameras are complex devices, that need heavy hardware image processing operations.
  - Control of the processing is based on advanced algorithms that must run a programmable processor.
    - This has traditionally been implemented in a dedicated MCU in the camera.
    - But in embedded devices algorithms have been moved to the main CPU to save cost! Blurring the boundary between camera devices and Linux often left the user with no other option than a vendor-specific closed-source solution.

- Solution:
  - To address this problem the Linux media community has very recently started collaboration with the industry to develop a camera stack that will be open source friendly while still processing vendor core IP. `libcamera` was born out of that collaboration and will offer modern camera support to Linux based systems including traditional Linux distributors, ChromeOS and Android.

- Why?

- In the original, it quite simple:

    ```text
              |Ov5640|
                ||           Camera sensor
    ----------i-||------------------------
                \/
              |CSI-2|
                ||
                \/
              |scaler|
                ||    SoC camera interrace
    ------------||------------------------
                \/
            |  capture  |
            |/dev/video0|              API
    --------------------------------------
    ```

  - You have camera sensor, for example `OV5450`, that is connected to the SoC via CSI-2 (camera serial interface) interface.

- 2010, 2012, It more complex, more complex sensors, more complex kernel driver, and more complex user API.

```text
|Camera Application|
        ||
        \/
      |V4L2|
        ||
        \/
  |User space API|
        ||
        \/
  |Kernel driver|
        ||
        \/
    |Sensors|
```

```text
|Camera Application|
        ||
        \/
| libv4l + plugin       |
  ||        ||       ||
 v4l2       MC      subdev
  \/        \/       \/
 |API|   |sensor|   |driver|

```

## 1. libcamera

- New project to resolve complex of the camera system.

- Camera Stack:

```text
 _______________         _______________         _______________         _______________
|    Native     |       |   Framework   |       |    Native     |       |   Android     |
|     V4L2      |       |  Application  |       |   libcamera   |       |    Camera     |
|__Application__|       |__(gstreamer)__|       |__Application__|       |___Framework___|
        /\                      /\                      /\                      /\          Application
--------||----------------------||----------------------||----------------------||---------------------
 _______\/______         _______\/______                ||               _______\/______
|     V4L2      |       |    Camera     |               ||              |    Android    |
|    Compat     |       |   Framework   |               ||              |    Camera     |
|_______________|       |__(gstreamer)__|               ||              |______HAL______|
        /\                      /\                      ||                      ||            libcamera
        ||                      ||                      ||                      ||           adaptation
--------||----------------------||----------------------||----------------------||---------------------
        ||                    __||______________________||______________________||__
        ||                   |                      Language bindings               |
        ||                   |_______________________(Optional)_____________________|
        ||                      ||                      ||                      ||
        ||                      ||                      ||                      ||
     ___\/______________________\/______________________\/______________________\/___
    |                                   libcamera                                    |
    |________________________________________________________________________________|
                /\                          /\                      /\                       libcamera
                ||                          ||                      ||                       framework
----------------||--------------------------||----------------------||--------------------------------
                ||                          ||                      ||                      User-space
================||==========================||======================||================================
                ||                          ||                      ||
             ___\/____                   ___\/___                ___\/___                 Kernel-space
            |  Media  |<===============>| Video  |<============>| V4L2   |
            |__Device_|                 |_Device_|              |_subdev_|
```

## 2. Camera Devices & Enumeration

## 3. Camera capabilities & profiles

## 4. Streams

- You may want to use the camera for multi purpose at the same time: display, capture, save camera stream, etc.

## 5. Per-frame controls

## 6. 3A & Image enhancement Algorithms

## 7. Adaptation layer

- That adapt any frameworks: Gstream, Android, V4L2 can use the lib camera.

## 8. libcamera architecture

```text
-----------------------< libcamera Public API>----------------------
    /\
  __||____       ___________________________________________________
 | Camera |     |Camera device                                      |
 | Device |     |  ______________________________________________   |
 |_Manager|     | | Device-Agnostic  ____________________________|  |
    ||          | |                 |    _________________          |
    ||          | |                 |<->|Image Processing |         |
    ||          | |                 |   |____Algorithms___|         |
    ||          | |                 | ==========================    |
    ||          | |                 |    _________________          |
    ||          | |                 |<->|     Pipeline    |         |
    ||          | |                 |   |____ Handler_____|         |
    ||          | |_________________|             Device-specific   |
    ||          |___________________________________________________|
    ||                        /\                   /\
    ||                        ||                   ||
  __\/________________________\/___________________\/_______________
 | Helpers and Support Classes                                      |
 |   ___________      ___________    __________      __________     |
 |  | MC & V4L2 |    |   Buffers |  |sandboxing|    | Plugins  |    |
 |  |__support__|    |_Allocator_|  |___IPC____|    |_manager__|    |
 |   __________      _________                                      |
 |  | Pipeline |    |         |                                     |
 |  |__runner__|    |_________|                                     |
 |__________________________________________________________________|
```

- With `Image Processing Algorithms` and `Pipeline Handler` are Device-specific (vendor specific components).
- `Image Processing Algorithms` is *Sandboxing*.

## 9. Camera Devices Manager

- Host plugging in cameras, and un-plugging, etc. The applications should be notified when the camera is disappear on the system etc.

## 10. Pipeline Handler

- **CSI** work directly with camera and share data to memory and **ISP** can check and handle it. The upper layer handle data from **ISP**.

## 11. V4L2 Compatibility

```text
|       Native V4L2 Application          |
|________________________________________|
    || open()       || ioctl()      ||mmap()
 ___\/______________\/______________\/__________
| LD PRELOAD = libcamera-v4l2.so                |
| open() {...}  ioctl(){...}      mmap(){...}   |
|_______________________________________________|
                    ||
              libcamera API
                    ||
 ___________________\/__________________________
|_______________libcamera_______________________|
```
