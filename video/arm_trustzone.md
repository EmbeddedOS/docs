# ARM TrustZone

- <https://www.youtube.com/watch?v=-kvrP1KjHII>

## ARM Cortex-A vs Cortex-M

- Cortex-A:
  - Application processor cores.
  - Performance intensive.
  - Much higher power consumption.
  - Large instruction set.
  - Meant to run an OS.
- Cortex-M:
  - Micro-controller cores.
  - Embedded applications.
  - Low power consumption.
  - Subset of instructions from Cortex-A.
  - Meant to run firmware.

## TrustZone: Cortex-A vs Cortex-M

```text
|   Feature/Arch  |  TrustZone for ARMv8-A      | TrustZone for ARMv8-M        |
|-----------------|-----------------------------|------------------------------|
|    Additional   | SEL0 - Trusted Apps.        | Secure thread - Trusted code/|
| security state  | SEL1 - Trusted OS.          | data.                        |
|                 | EL3 - Trusted Boot and FW   | Secure handler - Trusted     |
|                 | (ARMv8-A).                  | device drivers, RTOS, LIBs,  |
|                 |                             | managers...                  |
|-----------------|-----------------------------|------------------------------|
|     Secure      | Yes.                        | Yes (fast).                  |
|   Interrupts    |                             |                              |
|-----------------|-----------------------------|------------------------------|
| State transition| Software transition.        | Hardware transition (Fast).  |
|    (Boundary    |                             |                              |
|    crossing)    |                             |                              |
|-----------------|-----------------------------|------------------------------|
|Memory Management| Virtual memory MMU with     | Secure Attribution Unit (SAU)|
|                 | secure attributes.          | and MPU memory partitions.   |
|-----------------|-----------------------------|------------------------------|
|     System      | Yes.                        | Yes.                         |
|   interconnect  |                             |                              |
|     security    |                             |                              |
|-----------------|-----------------------------|------------------------------|
|Secure code, data| Yes.                        | Yes.                         |
|   and memory.   |                             |                              |
|-----------------|-----------------------------|------------------------------|
| Trusted boot    | Yes                         | Yes.                         |
|-----------------|-----------------------------|------------------------------|
| Software        | ARM trusted fw (and third   | Keil CMSIS, ARM embed OS,    |
|                 | party TEEs).                | mbed uVisor and third-party  |
|                 |                             | software.                    |
|-----------------|-----------------------------|------------------------------|
```

## Important Takeaways

- Memory Management Unit vs Memory Protection Unit.
  - MMU handles virtual address translation.
  - MPU ONLY handles memory protection.
    - Very simple comparatively.
- State transition differences.
  - Cortex-A uses software.
  - Cortex-M uses hardware.
    - The hardware itself handles the stack between state transitions.

## What is the ARM TrustZone (Cortex-M)

- Hardware based software isolation!.
- Allows a secure application to control a non-secure applications's access to sensitive information.
- Isolates untrusted code from trusted code.

```text
                                      __________________________________
 _____________________               |     _______________________      |
|  Non secure state   |              |    |     Secure State      |     |
|  ________________   |              |    |  ___________________  |     |
| |    User app    |<======Start============|___System__Start___| |     |
| |                |  |              |    |  ___________________  |     |
| |                |====Function call======>| Secure Firmware   | |     |
| |________________|  |              |    | |___________________| |     |
|  ________________   |              |    |  ___________________  |     |
| |  I/O Driver    |<====Function call======| Secure Firmware   | |     |
| |________________|  |              |    | |___________________| |     |
|_____________________|              |    |_______________________|     |
                                     |                                  |
                                     |_______TrustZone for Cortex-M_____|
```

## How it works: Cortex-M

- NSC: Non-Secure Callable Memory.
- SG: Secure Gateway.

```text

                   _______________     _______________         _______________ 
                  |  Non secure   |   |  Secure NSC   |       |    Secure     |
                  |  ___________  |   |  ___________  |       |  ___________  |
                  | |           | |   | |           | |       | |           | |
Non Secure code   | |    (1)==Branch (BL)==>|SG|(2)=====Branch (BL)==>      | |
calling secure    | |     <==============Return(BXNS)===============(3)     | |
code              | |           | |   | |           | |       | |           | |
                  | | Non Secure| |   | |  Secure   | |       | |   Secure  | |
                  | |    Code   | |   | |    API    | |       | |  Library  | |
                  | |           | |   | |           | |       | |           | |
  Secure code     | |     <=============Branch(BLXNS)===============(1)     | |
     calling      | |    (2)================Branch(BX)===============>      | |
 non-secure code  | |___________| |   | |___________| |       | |___________| |
                  |_______________|   |_______________|       |_______________|
```

## Direct Memory Access Prevention

- Only allow memory access at SG address, access to other secure memory region will be rejected.
- Incorrect/malicious memory accesses from non-secure code result in a **SecureFault**.

```text
  _________________            _______________         _______________ 
 |   Non secure    |          |  Secure NSC   |       |    Secure     |
 |                 |          |               |       |               |
 | ...             |  /=Rejected======================> DCD 0xe97     |
 | MOV r0, #10     | /        |   ...         |       | MOVS r1, #0   |
 | MOV r1, #abc    | |=OK=======> SG          |       | MOVS r3, #1   |
 | ADD r2, r0, r1  | |        |   MOVS r1, #0 |       | ...           |
 | ...             | |=Rejected=> MOVS r3, #1 |       | BXNS lr       |
 |                 | |        |   ...         |       | ...           |
 | BLX secure_addr==>/        |               |       |               |
 | CMP r1, #result |          |               |       |               |
 | ...             |          |               |       |               |
 |_________________|          |_______________|       |_______________|
```

## How does code actually get executed???

- S: Secure.
- NSC: Non-secure callable.
- NS: Non Secure

```text
 ___________________ 0x00000000
|     BOOT_S        |
|___________________| BS * 0x100 - BNSC * 0x20 
|     BOOT_NSC      |
|___________________| BS * 0x100
|     BOOT_NS       |
|___________________| BOOTPROT * 0x100
|      APP_S        |
|                   |
|___________________| (BOOTPROT+AS) * 0x100 - ANSC * 0x20
|      APP_NSC      |
|___________________| (BOOTPROT+AS) * 0x100
|      APP_NS       |
|                   |
|                   |
|___________________| 0x1F400000
    Flash (up to 64KB)
```

- Or:

```text
 ___________________ 0x00000000
|      APP_S        |
|___________________| (AS * 0x100) - (ANSC * 0x20) = 0x7C00
|      APP_NSC      |
|___________________| (AS * 0x100) = 0x8000
|      APP_NS       |
|                   |
|                   |
|___________________| 0x10000
```

- The NonSecure only can jump to Non-secure callable memory region gateway address.

## Next steps

- Now that we know how applications get executed and how to configure the hardware.
- Take more in-depth look at the IBM TPM implementation.
- Decide options on porting it to ARM (from Linux).
- Design model around being the Secure Application.
  - Bootloader doesn't have to implemented.
- Begin implementing the actual code.
- Create non-secure application in parallel to test functionality.
