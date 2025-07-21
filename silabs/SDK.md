# SiWx917 - WiSeConnect

- SiWx917 offers a full network offload option for embedded systems with low-end host micro-controllers running an RTOS or bare-metal OS. The SiWx917 operating in two distinct modes: `SoC` (System-On-Chip) mode and `NCP` (Network Connectivity Processor) mode.

- In SoC mode (built-in host), the SiWx917 enables its on-chip ARM Cortex M4F to serve as the host MCU.
- In NCP mode (external host), the SiWx917 connects to an external host MCU using `UART` or `SPI` interfaces. A complete set of wireless, networking and security stacks run on the SiWx917 device, however the networking stack can be bypassed if required.

- Communication with the host MCU is achieved with a simple binary API referred to as `SAPI`. Embedded products provide greater than 20M-bps throughput with multiple operating modes including: WiFi Client, WiFi Access Point, Simultaneous WiFi Client & Access Point and Dual-mode Bluetooth.

## 1. SiWx917 SoC Programming Reference

### 1.1. SoC Overview

- SiWx917 SoC is the industry's first Wireless MCU family with a comprehensive **multi-protocol wireless sub-system**. It has an integrated ultra-low-power micro-controller, a built-in wireless subsystem, advanced security, high-performance mixed-signal peripherals, and integrated power management.

- SiWx917 SoC comprises of two processing cores:
  - Wireless Processor - NWP (TA)
  - Application Processor - MCU (Cortex - M4)
