# Understand MIPI | Synopsys

- [link](https://www.youtube.com/watch?v=sxuwbJPXSqk&list=PLPQM2EfGI7JEGMyeOhlhxXUuYk9a7_HBK&index=2)
- High level view of MIPI.

## 1. MIPI serves Mobile IC markets

- MIPI Alliance: `Interface Specifications for Mobile Products`

- For mobile phone, tablet, etc.

## 2. Popular MIPI interfaces

```text
 ___________________________
|       Baseband IC         |
|                ___________|        ___________
|               | Modem     |<----->|_4G_RF_IC__|
|               | Interface |        ___________
|               |___________|<----->|2G/3G_RF_IC|
|___________________________|

 ___________________________
|  Application Processor    |
|                ___________|        _____________
|               | Display   |<----->|_Display_IC__|
|               | Interface |
|               |___________|
|                ___________|        _____________
|               | Camera    |<----->|_Image_sensor|
|               | Interface |
|               |___________|
|___________________________|
```

- Popular MIPI interfaces enable communicate between baseband IC, application processor with ICs.

## 3. MIPI Camera Serial Interface (CSI-2)

```text
|   Image Sensor        |  D-PHY    |Application Processor  |
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

- High performance serial interface.
- MIPI D-PHY for physical layer.
- MIPI CSI-2 supports packetized transmission with error detection and correction.

## 4. MIPI Display Serial Interface (DSI)

```text
| Application processor |  D-PHY    |   Display IC          |
|-----------------------|           |-----------------------|
|        ______________ |           | ______________        |
|       |  CSI Master  ||           ||  CSI Slave   |       |
|       |  Data lane 0 ||<--------->||  Data lane 0 |       |
|       |   ...        ||---------->||   ...        |       |
|       |  Data lane n ||---------->||  Data lane n |       |
|       |  Clock lane  ||           ||  Clock lane  |       |
|       |______________||           ||______________|       |
|_______________________|           |_______________________|
```

- 1GB/s per lane.
- High performance serial interface.
- Pixel and data commands serialized into single physical stream.
- Utilizing MIPI D-PHY for physical layer.
