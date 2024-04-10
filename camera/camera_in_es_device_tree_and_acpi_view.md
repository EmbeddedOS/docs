# Cameras in Embedded Systems: Device Tree and ACPI View

- [Link](https://www.youtube.com/watch?v=tB6x95N2yHQ&list=PLPQM2EfGI7JEGMyeOhlhxXUuYk9a7_HBK&index=1)

## 1. A typical embedded system with a camera

- 1. Image Signal Processor (ISP).
- 2. Raw Camera.
- 3. Lens voice coil.

```text

|   Camera module   |               |      SOC          |
|    |lens|         |<--vana,vdig---|-->|regulator|     |
| |lens voice coil| |<-----reset----|-->|GPIO|          |
|   |sensor|        |<-----i2c------|-->|I2C|           |
|                   |<-CSI-2, clock-|-->|ISP|           |
```

## 2. Raw sensors

- Raw sensors have little processing logic in the sensor itself.
  - Analogue and digital gain but not much more.

## 3. Image Signal Processor (ISP)

- Processing the image for viewing.

## 4. Video4Linux and Media Controller

- **Video4Linux** (V4L2) is the Linux API for capturing images.
  - Video capture cards.
  - USB webcams.
  - Cameras in embedded devices.
- Media controller is a control interface for complex media devices.
  - Image pipeline discovery and configuration.
  - Device discovery.

## 5. Example of media graph
