# Image Signal Processing (ISP) Drivers & How to merge one upstream

- Main goal of this presentation:
  - Overview of Camera --> ISP --> Memory pipeline.
  - Overview of Media Framework.
  - Design choices when implementing a driver.
  - Lessons learned when up-streaming `rkisp1` driver.
  - User-space tools (`libcamera`).

## 1. Camera --> ISP --> Memory Pipeline

- Camera is composed by sensor, this sensor is composed by smaller light sensors. The reading of all those sensors, is sent to your SoC, processed in some way and the image is made available to your application.

### 1.1. What is an ISP?

- Image Signal Processor.
- Common use case:
  - ISP receives the reading all those small color sensors.
  - Transforms in an image usable for user-space.
- Perform several other image transformations:
  - Format conversation (debayering, RGB, YUV).
  - Crop/Resize.
  - White balance.
  - Compose.
  - Image stabilization.
  - Effects/filters.
  - Flip/Rotate.
  - Etc.

  - Hardware accelerated image processing.
  - Offloads the CPU.

### 1.2. Statistics

- ISP can generate statistics:
  - Histograms.
  - Area contract.
  - etc.
- Used by user-space to implement algorithms such as:
  - Histogram equalization.
  - 3A (Auto-focus, Auto-exposure, Auto-white balance).

### 1.3. What an ISP is not?

- ISP is not a codec.
- ISPs work with raw/uncompressed images.
- Codecs:
  - Encoders: raw image -> compressed image format.
  - (such as H.264, JPEG, VP9).
  - Decoders: compressed image -> raw image.

### 1.4. ISPs architecture: Inline vs Offline

- **Offline**:
  - Have 2 phases:
    - 1. Sensors -> memory: Retrieve image from sensor and store in memory.
    - 2. Memory -> ISP -> Memory: Process byte data and store back to memory.
  - Usually implemented in **two separate drivers**.
    - Communication between them is coordinated by user-space.
    - Example Intel IPU3:
      - IPU3 CIO2 (camera interface) driver: gets the image from the sensor.
      - IPU3 ImgU driver: process this image and sends to user-space.
- **Inline**:
  - Data reaches memory only the end:
    - Sensor --> ISP --> Memory.
      - Sensor is connected directly to the ISP without touch the memory.
  - Example: `rkisp1` driver.

- **Hybrid**:
  - Can get the image directly from the sensor or from memory.
  - Can behave as inline, or perform the second phase of offline.
  - Ex: MT8183 P1.

```text
            _________
           |   DMA   |-->|\
           |__Input__|   |  \     __________     __________
                         |MUX|-->|  Bayer   |-->|Processed |
 ______     _________    |  /    |Processing|   |__Output__|
|Sensor|-->| Camera  |-->|/
           |Interface|
```

### 1.5. BUS - MIPI D-PHY

- Very common bus used in the market for camera and displays.
- Specified by MIPI Alliance.
- Physical layer with high data-rate.
- 4K images with a good frame rate.

- Up to 4 data lanes.
- I2C bus for configuration.
- On top of this bus there can be two protocols:
  - MIPI DSI-2: Display Serial Interface, to output images.
  - MIPI CSI-2: Camera Serial Interface, to capture images.
- MIPI DPHY/CSI-2 -> frequent term in ISP land.

```text
|         Camera        |  D-PHY    |          Host         |
|-----------------------|           |-----------------------|
|        ______________ |           | ______________        |
|       |  CSI Master  ||           ||  CSI Slave   |       |
|       |  Data lane 0 ||---------->||  Data lane 0 |       |
|       |   ...        ||---------->||   ...        |       |
|       |  Data lane n ||---------->||  Data lane n |       |
|       |  Clock lane  ||           ||  Clock lane  |       |
|       |______________||           ||______________|       |
|        ______________ |           | ______________        |
|       |  I2C/CCI     ||           ||  I2C/CCI     |       |
|       |   Data       ||<--------->||   Data       |       |
|       |   Clock      ||<----------||   Clock      |       |
|       |______________||           ||______________|       |
|_______________________|           |_______________________|
```

### 1.6. Study case - RKISP1

- Rockchip RK3399 ISP:
  - `rkisp1` is the driver of the ISP block present in Rockchip RK3399 SoCs.
  - RK3399 SoC can be found in devices such as:
    - Scarlet Chromebooks.
    - RockPi boards.
    - Pinebook Pro laptops.

  - Originally written by Rockchip.
  - Merged in kernel 5.6.
  - `drivers/staging/`.
  - 9K+ lines of code.

#### 1.6.1. Hardware architecture

```text
                                                    |=rkisp1-resizer.c=|  |====rkisp1-capture.c====|
                     |======rkisp1-isp.c=====|      |========Main Picture Path=====================|
 ________            |______    __ __ __ __  |      |__________    ____                  __________|
|  MIPI  |-->|\      |      |  |  |  |  |  | |   /->|___Crop___|->|_RSZ|--------------->|          |
|________|   |  \    |      |  |  |  |  |  | |  /                                       | Memory   |
             |MUX|-->| ISP  |->|IE|IE|IE|IE| |->                                        |Interface |
 ________    |  /    |      |  |0 |1 |2 |3 | |  \    __________    ____    __________   |          |
|Parallel|-->|/      |      |  |  |  |  |  | |   \->|___Crop___|->|_RSZ|->|RGB_Rotate|->|__________|
|________|           |      |  |  |  |  |  | |      |                                              |
                     |______|__|__|__|__|__|_|      |========Self Picture Path=====================|
```

- ISP comprises with:
  - Image signal processing.
  - Many Image Enhancement Blocks.
  - Crop.
  - Resizer.
  - RBG display ready image.
  - Image Rotation.
- Self-Path: Preview.
- Main-Path: Picture.

## 2. Kernel Media Framework

### 2.1. Media topology

- Linux Kernel Exposes a topology to user-space.
- User-space can query `/dev/mediaX`
  - Retrieve how inner blocks are interconnected.
  - Order of image processing.

- For instance, the user-space can see hardware blocks (sensor, ISP, etc.) and how it connect to each other, the order of image processing.
  - We have a sensor, that is directly connected to the ISP, and the ISP is connected to some DMA engine where the image reaches memory.

- Topology example:

```text
 _________________________
|Sensor `/dev/v4l-subdev3`|
|_________________________| (green)
|___________0_____________|
            ||
 ___________\/____________
|___________0_____________|
|          ISP            |
|    `/dev/v4l-subdev0`   | (green)
|_________________________|
|___________1_____________|
            ||
 ___________\/____________
|DMA Engine /dev/video0   | (yellow)
|_________________________|
```

- We have two types of nodes:
  - 1. `subdevices` (green): inner parts of the hardware: **User space can perform some configurations there**.
  - 2. `video devices` (yellow): DMA engine, where user-space queues and dequeues buffers, containing images or metadata to/from the hardware. User can perform some configurations also.
    - In this example, it is from the hardware because we are retrieving image from the sensor. But in another examples, we are going to see, the user-space can inject the image into the driver or some kind of metadata.
- All blocks connected by **links** between **pads**.
  - In this example we have pad 0, pad 1, etc.

- NOTE: Sensor-Driver is usually a separated driver, that because we can reuse the same sensor with different kind of hardware.
  - For example, on pi board and rock board we have same sensor, so we can use same sensor driver, but use different ISP driver.

### 2.2. Offline

#### 2.2.1. IPU3 CIO2 -- Offline -- 1st phase

```text

 ____(green)____     __(green)__     __(green)__     __(green)__ 
|_______________|   |_____0_____|   |_____0_____|   |_____0_____|
|imx355 10-001a |   |ipu3-csi2 1|   |ipu3-csi2 2|   |ipu3-csi2 3|
|_______0_______|   |_____1_____|   |_____1_____|   |_____1_____|
       ||                 ||              ||             ||
   ____\/__(green)        ||              ||             ||
  |_____0_____|      _____\/____     _____\/____    _____\/____
  |ipu3-csi2 0|     |ipu3-cio2 1|   |ipu3-cio2 2|  |ipu3-cio2 3|
  |_____1_____|     |/dev/video1|   |/dev/video2|  |/dev/video3|
        ||             (yellow)        (yellow)      (yellow)
        ||
   _____\/____
  |ipu3-cio2 0|
  |/dev/video0|
     (yellow)
```

- The camera interface that performs the first phase of the offline ISP, we can see four blocks of the `ipu3-csi2` sub-devices. It means that we have four buses ad we **can retrieve image from four sensors at the same time**.
- We only have a single sensor `imx355 10-001a`.

- We have (Yellow) blocks where the user space is going to interact `/dev/videoX` to retrieve from the kernel.

#### 2.3.1. IPU3 ImgU -- Offline -- 2nd phase

```text
 _________________       ______________________
|ipu3-imgu 0 input|     |ipu3-imgu 0 parameters|
| /dev/video4     |     |   /dev/video5        |
|_________________|     |______________________|
        \\                        //
         \\                      //
          \\==||            ||==//
             _\/____________\/__
            |___0_____|____1____|
            |   ipu3-imgu 0     |
            | /dev/v4l-subdev7  |
            |___________________|
            |___2__|__3__|__4___|
                ||    ||      \\=================\\
 _______________\/_   \/____________________   ___\/_______________
|ipu3-imgu 0 output| |ipu3-imgu 0 viewfinder| |ipu3-imgu 0 3a stat |
|   /dev/video6    | |   /dev/video7        | |   /dev/video8      |
|__________________| |______________________| |____________________|

 _________________       ______________________
|ipu3-imgu 1 input|     |ipu3-imgu 1 parameters|
| /dev/video9     |     |   /dev/video10       |
|_________________|     |______________________|
        \\                        //
         \\                      //
          \\==||            ||==//
             _\/____________\/__
            |___0_____|____1____|
            |   ipu3-imgu 1     |
            | /dev/v4l-subdev8  |
            |___________________|
            |___2__|__3__|__4___|
                ||    ||      \\=================\\
 _______________\/_   \/____________________   ___\/_______________
|ipu3-imgu 1 output| |ipu3-imgu 1 viewfinder| |ipu3-imgu 1 3a stat |
|   /dev/video11   | |   /dev/video12       | |   /dev/video13     |
|__________________| |______________________| |____________________|
```

- The camera interface will place the image in memory in some format, that is very specific to this Intel Driver. It's up to the user-space to get this image and feed to these other drivers of IPU imaging unit that performs **a second phase of the offline ISP**. Then user space is going to retrieve the image from the previous driver, and put into some another drivers.

- For example:
  - 1. We get images from `ipu3-cio2 0` that is mounted into `/dev/video0`.
  - 2. And send to `ipu3-imgu 0 input` that is mounted to `/dev/video4`.
  - 3. The driver perform some kind of processing and it will make the results output to some `ipu3-imgu 0 output` interface `/dev/video6`.
    - This driver also can generate some statistics through the node `ipu3-imgu 1 3a stat`. The user can read some statistics and send some parameters on that node `ipu3-imgu 0 parameters` to config drivers.

- In that case we have two instances of the ISP, so we can process two images at the same time.

### 2.3. RKISP1 -- inline

- Here is the topology of  `RKISP1` driver:

```text
 ___(green)__________         _____(green)________
|____________________|       |____________________|
|    ov5695 7-0036   |       |    ov2685 7-003c   |  ___(yellow)____
| `/dev/v4l-subdev3` |       | `/dev/v4l-subdev4` | | rkisp1_params |
|____________________|       |____________________| |_`/dev/video3`_|
|________0___________|       |________0___________|     ||
            ||                ||                        ||
             \\======\\      //      //================//
                      ||    ||       ||
                     _\/____\/_______\/_
                    |___0_____|____1____|
                    |   rkisp1_isp      |
                    | /dev/v4l-subdev0  |
                    |___________________|
                    |___2______|___3____|=========================\\
                     ||    ||                                      ||
     //=============//     \\=======\\                             ||
     ||                              ||                            ||
 ____\/_(green)___________       ____\/_(green)___________   ______\/_(yellow)
|____________0____________|     |____________0____________| | rkisp1_stats |
| rkisp1_resizer_mainpath |     | rkisp1_resizer_selfpath | | `/dev/video2`|
|   `/dev/v4l-subdev1`    |     |   `/dev/v4l-subdev2`    | |______________|
|_________________________|     |_________________________|
|____________1____________|     |____________1____________|
            ||                               ||
     _______\/_(yellow)               _______\/_(yellow)
    |rkisp1_mainpath|                |rkisp1_selfpath|
    | `/dev/video0` |                | `/dev/video1` |
    |_______________|                |_______________|
```

- It's the tablet, and it have front and back cameras. Those camera sensors are represented by `ovXXXX` blocks. Just one of them can be connected to the ISP at a time.
- ISP perform some kinds of processing and it output the image to one of those two paths: `mainpath` and `selfpath`.
- Also the driver can generate statistics and receiving parameters from user space.

## 3. Driver config architecture: Auto vs Manual config propagation

### 3.1. Auto config propagation

- Auto configuration scheme, user-space will do all the configurations and operations, and all the DMA engine node.

```text

 ____(green)____     __(green)__     __(green)__     __(green)__ 
|_______________|   |_____0_____|   |_____0_____|   |_____0_____|
|imx355 10-001a |   |ipu3-csi2 1|   |ipu3-csi2 2|   |ipu3-csi2 3|
|_______0_______|   |_____1_____|   |_____1_____|   |_____1_____|
       ||                 ||              ||             ||
   ____\/__(green)        ||              ||             ||
  |_____0_____|      _____\/____     _____\/____    _____\/____
  |ipu3-csi2 0|     |ipu3-cio2 1|   |ipu3-cio2 2|  |ipu3-cio2 3|
  |_____1_____|     |/dev/video1|   |/dev/video2|  |/dev/video3|
        ||             (yellow)        (yellow)      (yellow)
        ||
   _____\/____
  |ipu3-cio2 0|
  |/dev/video0|
     (yellow)
```

- For example, when the user-space set a resolution, it will set the resolution though the node `/dev/videoX`. And it's up to the driver to propagate this configuration on all the blocks of the topology. Including the sensor that can be a separated driver.

- Some nodes, doesn't have any node expose it on the FileSystem.

### 3.2. Manual config propagation Example

```text
 ___(green)__________         _____(green)________
|____________________|       |____________________|
|    ov5695 7-0036   |       |    ov2685 7-003c   |  ___(yellow)____
| `/dev/v4l-subdev3` |       | `/dev/v4l-subdev4` | | rkisp1_params |
|____________________|       |____________________| |_`/dev/video3`_|
|________0___________|       |________0___________|     ||
            ||                ||                        ||
             \\======\\      //      //================//
                      ||    ||       ||
                     _\/____\/_______\/_
                    |___0_____|____1____|
                    |   rkisp1_isp      |
                    | /dev/v4l-subdev0  |
                    |___________________|
                    |___2______|___3____|=========================\\
                     ||    ||                                      ||
     //=============//     \\=======\\                             ||
     ||                              ||                            ||
 ____\/_(green)___________       ____\/_(green)___________   ______\/_(yellow)
|____________0____________|     |____________0____________| | rkisp1_stats |
| rkisp1_resizer_mainpath |     | rkisp1_resizer_selfpath | | `/dev/video2`|
|   `/dev/v4l-subdev1`    |     |   `/dev/v4l-subdev2`    | |______________|
|_________________________|     |_________________________|
|____________1____________|     |____________1____________|
            ||                               ||
     _______\/_(yellow)               _______\/_(yellow)
    |rkisp1_mainpath|                |rkisp1_selfpath|
    | `/dev/video0` |                | `/dev/video1` |
    |_______________|                |_______________|
```

- In other hand, in the  Manual config propagation scheme, user spaces that responsible for configuring all the paths through all the image pipeline.

- For instance, the user wants to confirm a resolution,
  - 1. it need to config the resolution in the `ovXXXX` sensor, needs to generate to the path 0.
  - 2. And config the resolution to the ISP block `rkisp1_isp` in path 0 to inform the ISP which resolution should expect from the sensor.
  - 3. Then user-space needs to configure the resolution that the ISP needs to generate and it needs to match the resolution that the resizer `rkisp1_resizer_mainpath` expects.
  - 4. This should config the resolution that the `rkisp1_resizer_mainpath` will generate.
  - 5. Finally, setting the resolution expected inside the memory buffer for final images.

### 3.3. Manual config propagation

- Manual config propagation:
  - Increases complexity for user-space.
  - It formats don't match --> fail on STREAMON.
  - Finer grain configuration in inner blocks of the hardware -> We need more point to config -> but we can control more -> extendable.
  - **More blocks exposed, more complex**.
  - Extendable.

- The manual is hard to control but, if you want to add more blocks in the future:
  - If you choose the auto-config, you'll realize that this new block could benefit the manual configuration, then you won't be able to change it. Maybe you can load more parameters to the driver but then it won't be compatible with other application from user space anymore.

### 3.4. Why `rkisp1` is manual?

- Crop:
  - Specify a sub-rectangle in the image.
  - How do crop the image? You just select the sub-rectangle in the main image or the sub image that you are interested in.
- The media-subsystem allows you to expose an API, that allows user-space to select sub-rectangle.

```text
 ___(green)__________         _____(green)________
|____________________|       |____________________|
|    ov5695 7-0036   |       |    ov2685 7-003c   |  ___(yellow)____
| `/dev/v4l-subdev3` |       | `/dev/v4l-subdev4` | | rkisp1_params |
|____________________|       |____________________| |_`/dev/video3`_|
|________0___________|       |________0___________|     ||
            ||                ||                        ||
             \\======\\      //      //================//
                      ||    ||       ||
  Set Sub-           _\/____\/_______\/_
  Tangle here?=====>|___0_____|____1____|
      ||             |   rkisp1_isp      |
    Or here          | /dev/v4l-subdev0  |
      ||             |___________________|
      ||             |___2______|___3____|=========================\\
      \/              ||    ||                                      ||
     //=============//     \\=======\\                             ||
     ||                              ||                            ||
 ____\/_(green)___________       ____\/_(green)___________   ______\/_(yellow)
|____________0____________|     |____________0____________| | rkisp1_stats |
| rkisp1_resizer_mainpath |     | rkisp1_resizer_selfpath | | `/dev/video2`|
|   `/dev/v4l-subdev1`    |     |   `/dev/v4l-subdev2`    | |______________|
|_________________________|     |_________________________|
|____________1____________|     |____________1____________|
            ||                               ||
     _______\/_(yellow)               _______\/_(yellow)
    |rkisp1_mainpath|                |rkisp1_selfpath|
    | `/dev/video0` |                | `/dev/video1` |
    |_______________|                |_______________|
        /\
        ||
Instead of here?
```

- We could allow user-space to select a sub-rectangle via final node `/dev/video0`. But we are going to see some problems because:
  - 1. The `rkisp1` driver allow cropping the image from the sensor.
  - 2. The `rkisp1` driver allow cropping the image before resizing.
  - 3. Exposing crop once in the video node would be confusing.

- So in this example, we allows user space to select sub-rectangle on pad 0 of the ISP, so it can select which part of the image from the sensor it should work with. And also in pad 0 from resizer `rkisp1_resizer_mainpath`. So we can select which part of image is zoom or shrink.

### 3.5. Image stabilizer

- Usually we have the main image, and the Image stabilizer just selects the sub-rectangle in the main image.
  - The idea is that only the outer rectangle would shake and not the inner one. So if you shake your phone, the image shouldn't appear that shakes that much.

## 4. PHY subsystem

- `rkisp1` -- original topology:

```text
 ___(green)__________         _____(green)________
|____________________|       |____________________|
|    ov5695 7-0036   |       |    ov2685 7-003c   |  ___(yellow)____
| `/dev/v4l-subdev3` |       | `/dev/v4l-subdev4` | | rkisp1_params |
|____________________|       |____________________| |_`/dev/video3`_|
|________0___________|       |________0___________|     ||
            ||                ||                        ||
             \\======\\      //       //===============//
                      ||     ||      ||
             _________\/_____\/___   ||
            |_________0___________|  ||
Removed====>|rockchip-sy-mipi-dphy|  ||
            | `/dev/v4l-subdev1`  |  ||
            |_________1___________|  ||
                      ||             ||
                      ||             ||
                     _\/_____________\/_
                    |___0_____|____1____|
                    |   rkisp1_isp      |
                    | /dev/v4l-subdev0  |
                    |___________________|
                    |___2______|___3____|=========================\\
                     ||    ||                                      ||
     //=============//     \\=======\\                             ||
     ||                              ||                            ||
 ____\/_(green)___________       ____\/_(green)___________   ______\/_(yellow)
|____________0____________|     |____________0____________| | rkisp1_stats |
| rkisp1_resizer_mainpath |     | rkisp1_resizer_selfpath | | `/dev/video2`|
|   `/dev/v4l-subdev1`    |     |   `/dev/v4l-subdev2`    | |______________|
|_________________________|     |_________________________|
|____________1____________|     |____________1____________|
            ||                               ||
     _______\/_(yellow)               _______\/_(yellow)
    |rkisp1_mainpath|                |rkisp1_selfpath|
    | `/dev/video0` |                | `/dev/video1` |
    |_______________|                |_______________|
```

- In the original, we have the `rockchip-sy-mipi-dphy` block, and they removed it.
  - Why they do this?

- Phy Abstraction Layer:
  - Manual config propagation --> more `subdevices` -> more complex to user-space.
  - Re-think exposed blocks.
    - Phy block -> no image configuration exposed -> It doesn't expose any config.
  - Topology -> Image processing steps.
  - Same processing steps can be used on top of different buses.

- The idea is that that one doesn't provide much information.

## 5. Lessons Learned

### 5.1. PHY -- Lesson Learned

- Migrate bus code to PHY Abstraction Layer (`drivers/phy/`).
- Generic topology for any bus -- less complex for user-space.
- ISP driver is much more cleaner.
- Phy driver can be used for DSI (other protocols, etc.).

=> Separated driver to two different drivers: one inside media subsystem and the other one inside the PHY abstraction layer subsystem.

### 5.2. Community perspective

- Updating to staging:
  - 1. V4L2 community is open to accept drivers in staging. (With the condition that you work on it to move it out ASAP).
  - 2. Detailed TODO list.
  - 3. Make it available to other people to use.
  - 4. Improve workflow easier to get contributions from others, testing, bug reports.
  - 5. Decrease maintenance cost -> no need to keep rebasing.

### 5.3. More lessons learned

- Don't be afraid to re-organize the code (files, naming, code order, re-writing functions).
- Split the code between different files per implementation node, at least between video nodes and sub-device nodes.
- Separate the code that configures the hardware, from the code that deals with the V4L2 API.
- Removing code you are not using that you can't test. For example:
  - `rk3288` support.
  - Phy driver ports (SoC has 2 MIPI-DPHY/CSI2 ports, I had was only using one).
  - Simplify the code - but keep extendable.
  - Lots of macros in headers.

## 6. User-space support

### 6.1. `libcamera`

- Complex topologies:
  - Not all features are auto discoverable.
  - Example (`rkisp1`):
    - sub-rectangle for cropping.
    - vs sub-rectangle for image stabilizer.
    - Meta-data buffers structure:
      - `rkisp1_stats`
      - `rkisp1_params`
  - Requires user-space specific implementation for specific drivers.
  - Specific applications to specific hardware.
  - Not very re-usable code.
  - Hard to test.

- That where the `libcamera` comes in.

- `libcamera`:
  - Open source camera stack for many platforms with a core user space library.
  - User space drivers.
  - Allow you to plug: Image Processing Algorithms.

### 6.2. `libcamera` architecture

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

## 7. Tips

- Add/push/update support for your hardware in `libcamera`.
- Easier to test.
- More users.
- More developers involved.
- Contribute with the project.
