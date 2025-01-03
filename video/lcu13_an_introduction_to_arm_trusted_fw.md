# LCU13 An Introduction to ARM Trusted Firmware

- <https://www.youtube.com/watch?v=q32BEMMxmfw&t=657s>

## ARM Trusted FW

- Reference implementation of secure world software for ARMv8-A, including Exception Level 3 (EL3) software.
- Various ARM interface standards:
  - Power State Coordination interface (PSCI).
  - Trusted Board Boot Requirements (TBBR).
  - Secure Monitor code.
- Designed for porting to other implementations.
- Continue collaborative development as an Open Source Project licensed under BSD. <https://github.com/ARM-software/arm-trusted-firmware>

## Firmware on ARM SoCs

### A quick primer on ARM architecture

- ARMv6, how Linux would like to think it is running on ARM:

```text
ARMv6
 __Non-Secure___
|User           |
|               |
| |App1| |App2| |
|_______________|
|svc            |
|     |OS|      |
|_______________|
|   ARM SoC     |
|_______________|
```

- ARMv7, now that we have KVM,Xen on ARMv7 it looks like this:

```text
ARMv7
 __Non-Secure___
|User           |
|               |
| |App1| |App2| |
|_______________|
|svc            |
|     |OS|      |
|_______________|
|hyp            |
| |Hypervisor|  |
|_______________|
|   ARM SoC     |
|_______________|
```

- But that is forgetting the software in secure execution states. Effectively opaque to OS/Hypervisor: it looks like *firmware*.

```text
ARMv7
 __Non-Secure___  _____Secure_______
|User           ||User              |
|               ||                  |
| |App1| |App2| || |App1| |App2|    |
|_______________||__________________|
|svc            |
|     |OS|      |
|_______________|
|hyp            |
| |Hypervisor|  |
|_______________| __________________
                 |svc  |Trusted OS| |
                 |mon  |Secure FW | |
                 |     |Secure    | |
                 |     |Monitor   | |
                 |                  |
 ________________|__________________|
|             ARM SoC               |
|___________________________________|
```

- The trusted OS, secure FW, etc, now actually is just software that running in the secure state of the code. But who writes the software?
  - OS code from multiple vendors Linux, Windows, Android, QNX, etc. needs to be integrated.
  - Along with hypervisor code from multiple virtualization vendors (Hyper-V, Xen, KVM, VMware, etc.) which needs to be integrated.
  - Even with secure software from multiple vendors to create each product (OEMs, Silicon providers, Trusted OS vendors).

### Firmware is fragmented

- Today in ARM products the secure firmware code is tightly integrated.
- Resulting in distinct software integration effort for each SoC/TOS/OS combination.
- OEM provides additional secure requirements...

### Introduction ARMv8-A

- 8:11
