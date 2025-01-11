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

- AArch32:

```text
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

- ARMv8-A introduces a new set of AArch64 execution states. The same software integration is needed. AArch64:

```text
 __Non-Secure___  _____Secure_______
|EL0            ||EL0               |
|               ||                  |
| |App1| |App2| || |App1| |App2|    |
|_______________||__________________|
|EL1            ||EL1  |Trusted OS| |
|     |OS|      ||     |Secure FW | |
|_______________||__________________|
|EL2            |
| |Hypervisor|  |
|_______________| __________________
                 |EL3  |Secure FW | |
                 |     |Secure    | |
                 |     |Monitor   | |
                 |     |ROM FW    | |
 ________________|__________________|
|             ARM SoC               |
|___________________________________|
```

## Why Now, why ARMv8-A?

### Challenge #1: Rewriting the Firmware

- To use AArch64, EL3 must be AArch64.
- AArch64 demands a different approach in the Secure Monitor.
  - EL1 (OS) professor state must saved and restored by the Secure Monitor Software.
- Separation of the Trusted OS at Secure-EL1 from the Secure Monitor at EL3 requires a redesign of the interaction bw the Trusted OS and Monitor.

- Everyone writing secure privileged code has some substantial work to do - it's not just a port of ARM assembler code to A64 instructions.
  - How much of this code is common?

### Challenge #2: A need to standardize

- A single kernel image has to work on all platforms - including the ones that have not been created yet.
  - Particularly for Enterprise systems.
  - This demands that interaction with the hardware platform is standardized around specified peripheral and firmware interfaces.
- ARM has been creating some of these standards to make this possible:
  - SMC calling convention - to enable standard and vendor specific firmware services to coexist.
  - PSCI - a firmware interface for CPU power control.
- Working to define support for ARM systems in existing standards such UEFI and ACPI.
- How many implementations of the standards do we need?
  - Is there a reference implementation?

### SMC Calling convention

- Defines a standard calling convention Secure Monitor Calls in ARMv7 and ARMv8-A:
  - Register use for parameters and return values, use of immediate.
- Defines a partitioning of function ID space to allow multiple vendors to coexist in secure firmware:
  - OEMs, SiPs and Trusted OS vendors.
- Providing number of services e.g.
  - Standard firmware services (e.g. power management).
  - Trusted OS.
  - Errata management.

### Power State Coordination interface

- Defines a standard interface for making power management requests across exception levels/OSes.
- Supports virtualization and a communications with between normal and secure world.
- Allows secure firmware to arbitrate power management requests from secure and non-secure software.
- Default method for power control in Linux AArch64 kernel.

```text
 ______________________
|EL1                   |
|                      |
|     |Rich OS kernel| |      | Add/Remove cores.
|_________________||___|      | Secondary boot.
                  ||--------->| Idle.
 _________________||___       | Shutdown.
|EL2              \/   |      | Reset.
|         |Hypervisor| |
|_________________||___|
                  ||
 _________________||___      ______________________
|EL3              \/   |    |S-EL1                 |
| |Secure Platform FW|<------->|Trusted OS|        |
|______________________|    |______________________|
```

### Challenge #3: Dealing with bugs

- Working around hardware errata involves firmware.
  - May require setting secure processor state during boot.
  - May require runtime access to secure processor registers during OS execution - is the firmware call standard across SoCs?
- Errata do not always show up before a product is released.
  - Can the firmware be updated.
- Secure firmware isn't exempt from defects either.
  - Some firmware functionality is common across SoCs, multiple implementations provides multiple opportunities for defects.

### Taking the Opportunity

- Reduce duplicated effort by standardizing on a single implementation framework for EL3 software for ARMv8-A.
  - Provide reference implementations and test suites for standard interfaces and firmware behaviors.
  - Provide reference secure initialization code, including errata handling, for ARM CPUs and system peripherals.
- A suitably designed, portable implementation will allow easier integration of the various pieces of secure software.
- A demonstration of a multi-stage authenticated boot flow will encourage the use of updatable firmware in products.
- The diversity of integration needs is best met by an open collaboration.

## ARM Trusted Firmware overview

[ARM Trusted FW Architecture](../resources/arm_trusted_fw_arch.jpg)
