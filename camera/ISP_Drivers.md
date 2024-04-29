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
