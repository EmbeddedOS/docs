# How ARM systems are booted: An introduction to ARM Boot flow

- <https://www.youtube.com/watch?v=GXFw8SV-51g&list=WL&index=24>

## Implementations

- Implementations are vendor specific:
  - NXP, NVIDIA, Qualcomm, Samsung, ARM.
- ARMv7 is not looked at:
  - Vendor specific kernel drivers, specific suspend/idle drivers.
- ARMv8 de-factro standardized on ARM Trusted Firmware (TF-A).

## Table of contents

- Introduction.
- Exception Levels.
- Requirements for booting.
- TF-A services.
- Kernel Start sequence.

## Exception Levels

```text
     _______________ _______________
EL 0|               |               |
    |               | Secure World  |
    |               |               |
EL 1|               |_______________|
    |  Normal World |
    |               |
EL 2|               |
    |               |
    |_______________|_______________
EL 3|         Secure Monitor        |
    |_______________________________|
```

- 4 different exception levels.
- EL3 is highest exception level: TF-A runtime services.
- EL2 is where hypervisor run.
- EL1 is where kernel run.
- EL0 is where applications run.

## Exception Levels & Binary Naming Overview

```text
           _____     _____       ______________
        | | ROM |   | SPL |     | EL3 services |
EL3     | |_____|   |_____|     |______________|
--------|---------------------------------------
        |                       ____   ________
        |                      | BL | |  HYP   |
EL2     |                      |____| |________|
--------|---------------------------------------
        |                               _______
        |                              |  OS   |
EL1     |                              |_______|
--------|---------------------------------------
        |                                 _____
        |                                | APP |
EL0     |                                |_____|
        |
   Reset Vector
```

## TF-A naming scheme

```text
           _________     _________       ___________________
        | | ROM=BL1 |   | SPL=BL2 |     | EL3 services=BL31 |
EL3     | |_________|   |_________|     |___________________|
--------|----------------------------------------------------
        |                            _________   ___________
        |                           | BL=BL33 | | HYP=Linux |
EL2     |                           |_________| |___________|
--------|----------------------------------------------------
        |                                      _____________
        |                                     |  OS=Linux   |
EL1     |                                     |_____________|
--------|----------------------------------------------------
        |                                           ________
        |                                          | APP=US |
EL0     |                                          |________|
        |
   Reset Vector
```

- ROM is named as Boot loader 1.
- SPL is named as Boot loader 2.
- EL3 services is named as Boot loader 3-1.
- BL (BootLoader) is Boot loader 3-3.
- APP is named as User Space.

## First stage (BL1): ROM code

- Fused into SoC.
- Uses SRAM for memory is required.
- Implements vendor specific storage access/next stage loading.
- May implement USB/Serial Upload of Next stages, networking.
- Smaller = better (less size = less cost).
- Use sane default settings: Clocks, power.

## Second stage (BL2): TF-A/U-boot SPL/Barebox PBL

- Loaded by first stage, less restrictions.
- Needs to setup RAM training code.
- Will load the Next stage again from storage medium.
- Can be:
  - U-Boot SPL -> TF-A -> U-Boot.
  - Barebox PBL -> TF-A -> Barebox.
  - TF-A BL2 -> BL33(U-Boot, Barebox).
- It's up to you and your implementation.

## ARM Trusted Firmware (TF-A)

- Framework to implement standard firmware services.
  - PSCI, SCMI, etc.
  - EL3 Secure Monitor and SIP Router.
- Can be used as BL2 and/or BL3* dispatcher.
- MIT licensed:
  - Which means the source code is not always available...

## TF-A: SiP

- Silicon Provider Service.
- Implement SoC specific Services.
  - HAB (Secure Boot) API for i.MX8M*
  - DDR Frequency scaling.
- Often require higher exception level access.

## ARM SMC Calling Convention

- How do exception levels communicate?
  - SMC - Secure Monitor Call.
  - ERET - Exception Return.
- Communication via Registers.
- Defines argument register placement.

```text
                Processor States
     ___________        |   ___________
    |   Normal  |       |  |   Secure  |
    |   World   |       |  |   World   |
    |___________|       |  |___________|
      ||      /\        |    ||      /\
      SMC     ||        |    SMC     ||
      ||     ERET       |    ||     ERET
    __\/______||________|____\/______||__
   |            EL3 Firmware             |
   |_____________________________________|
```

## TF-A Services: PSCI

- Power State Coordination Interface.
- Unified discoverable API for:
  - CPU on/off.
  - System Sleep.
  - CPU Idle.
- Standard on ARMv8 Systems, usage in ARMv7 possible.

## TF-A Services: SCMI

- System Control and Management Interface.
- Unified discoverable API for clocks, power, etc...
- Useful since Normal and Secure World may require clocks.
- Simplified control interface for the linux kernel.

## Excursion: Device Trees

- Many slightly different SoC versions.
- Shared components across SoC generations.
  - i.MX6 -> i.MX8 UART.
  - i.MX8 HW-Encoder -> RK Encoder.
- Solution: Device Trees. -> discover what hardware available on the system.

```text
/dts-v1;

#include "imx8mq.dtsi"

/{
        model = "NXP i.MX8MQ EVK";
        compatible = "fsl,imx8mq-evk", "fsl,imx8mq";
[...]
&fec1 {
    pinctrl-names = "defaults";
    pinctrl-0 = <&pinctrl_fec1>;
    phy-mode = "rgmii-id";
    phy-handle = <&ethphy0>;
    status = "okay";
    mdio {
        #address-cells = <1>;
        #size-cells = <0>;

        ethphy0: ethernet-phy@0 {
```

## BL33: Barebox Proper

- We are in EL2 now.
- Barebox provides additional services.
  - Networking/NFS Boot.
  - Bootspec Parsing.
  - USB Gadget Support for:
    - Serial.
    - Mass storage.
    - fastboot.

## BL33: Kernel Start

- Decompress the kernel.
  - Aarch64 doesn't implementation decompression in the kernel.
- Copy DTB into memory.
- Mask interrupts. (keep interrupts off).
- Initialize standard ARM timer.

## BL33: Kernel Start 2

- Load Kernel at offset specified in header.
- Disable MMU, Data Caches.
- Initialize CPU registers for either EL2 or EL1.
  - e.g. x0 for device tree blob.
- Jump to it.
