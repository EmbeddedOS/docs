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

- The ISP is controlled by single driver, read write to sensors.

## 6. Probing

- Each driver is probed separately.

```text
                                                |Sensors |        |lens voice coil|
                                                    ||                  ||
                                                    ||------------------||
                      |ISP|                         ||
                        ||                    |I2C Controller|
PCI, AMBA, etc.---------||------------- ------------||
```

- **How to tell drivers they all are part of the same media device?**

## 7. Media device setup

|       Media device driver             |       Sensor driver                   |
|---------------------------------------|---------------------------------------|
|1. media_device_init()                 |                                       |
|2. v4l2_device_register()              |                                       |
|3. video_register_device()             |                                       |
|                                       |4. v4l2_device_register_subdev(sensor) |
|5. v4l2_device_register_subdev(isp)    |                                       |
|6. v4l2_register_subdev_nodes()        |                                       |
|7. media_device_register()             |                                       |

## 8. V4L2 async

- The V4L2 async framework facilitates sub-device registration.
- V4L2 sub-device device node creation and media device registration postponed after probe.
- To do its job, the V4L2 async framework makes use of firmware provided information.

## 9. Device Tree

- System hardware description in a human readable format.
- Originates from Sparc/Open Firmware.
- Primarily used on embedded systems:
  - ARM.
  - Also PowerPC, Sparc and x86.
- Tree structure:
  - Nodes.
  - Properties.
- Source code compiled into binary before use.

## 10. Device Tree standard and bindings

- Device Tree specification maintained by `devicetree.org`.
  - Syntax and some semantics.
- Bindings define the interface between the firmware and the software.
  - Bindings are OS specific.
  - Linux Device Tree binding documentation part of the Linux Kernel Source.
  - FreeBSD developers appear to be converging towards using Linux DT bindings.

## 11. Device Tree graphs

- **Phandle** properties can be used to refer to other nodes in the tree.
- **Port** is an interface in a device (as in an **IP block**).
- **Endpoint** describes one end of a connection to a port.

## 12. Device Tree Sensor node example

```text
&i2c2 {
    smia_1:camera@10 {
        compatible ="nokia,smia";
        reg =<0x10>;
        /* No reset GPIO. */
        vana-supply = <&vaux3>;
        clocks = <&isp 0>;
        clock-frequency = <9600000>;
        nokia,nvm-size = <(16*64)>;
        port {
            smia_1_1: endpoint {
                link-frequencies= /bits/ 64 <199200000 210000000 499200000>;
                clock-lanes = <0>;
                data-lanes = <1 2>;
                remote-endpoint = <&csi2a_ep>;
            }
        }
    }
};
```

- We have node `smia_1`, we have the port node describe CSI to transmitter in the sensor. We have one `endpoint` so that mean we have one connection.

## 13. Device Tree ISP node board specific part

```text
&isp {
    vdd-csiphy1-supply = <&vaux2>;
    vdd-csiphy2-supply = <&vaux2>;
    ports {
        port@2 {
            reg = <2>;
            csi2a_ep: endpoint {
                remote-endpoint = <&smia_1_1>;
                clock-lanes = <2>;
                data-lanes = <1 3>;
                crc = <1>;
                lane-polarities = <1 1 1>;
            }
        }
    }
};
```

## 14. OF graph API

- Parse port and endpoint nodes under device nodes.
- Enumerate over endpoints.
- Obtain remote endpoint:
  - Based on the **phandle** value.

## 15. ACPI

- Advanced Configuration and Power Interface.
- Operating system independent.
- Origins in x86 and PC.
  - Increasingly used in embedded systems.
- Device discovery and enumeration.
- Power management.
- ACPI methods.
  - Runnable code.
  - ACPI virtual machine.

- ACPI specifications developed by UEFI forum.
  - Roughly one specification per year.
- What do you do if you need to add a new kind of a device?
  - A new ACPI specification?

- There is a solution for that.

### 15.1. ACPI Device Specific Data

- _DSD object type part of ACPI 5.1 and later.
  - Key-value pairs (property extension) and
  - Tree structures (hierarchical data extension)
- Together proper and data extension could be used to implement very similar functionality to Device Tree.
- _DSD property registry
  - Light-weight approach for registering _DSD properties.

## 16. fwnode property API

- Access properties independently of underlying firmware implementation.
  - Device Tree.
  - ACPI.

- Makes use of ACPI _DSD property extension.

## 17. V4L2 fwnode API

- V4L2 ACPI support.
  - Embedded systems with I2C components.
  - Requires both fwnode graph API and V4L2 fwnode API.
- Same functionality as V4L2 OF API.
- V4L2 fwnode and V4L2 OF fully interoperable.
  - Sub-device driver using V4D2 fwnode works with a media device driver using V4L2 OF.
  - and vice versa.

## 18. FLash

- LED flash devices supported.
- But the kernel has no knowledge which sensor they're related to:
  - This is rather important if there are multiple cameras in the system, such as most mobile phones nowadays.
- Standardise phandle property for this?

## 19. Camera module

- Currently there's no `camera module` concept in the kernel (nor DT or ACPI).
- Camera module construction is important for the user space:
  - Which sensor and lens are related?
  - What kind of lens is there?
  - What's the voice coil spring constant?
  - Is there an infra red filter? what kind of filter is it?
  - What's the aperture size?

## 20. Camera power on and power off sequences

- Regulators, clocks and /or GPIOs may be shared between module components.
- Power on and power off sequences device component specific:
  - Which order and when each resource may be enabled?
  - E.g. regulator and clock are enabled, then after 10ms the reset GPIO can be lifted and device is ready for use.
- Requirements of both lens and sensor must be considered for module power-up sequence.
