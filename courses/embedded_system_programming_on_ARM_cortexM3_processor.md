# Embedded system programming on ARM cortex M3/M4 processor

## 1. Introduction

### 3. Motivate to learn ARM cortex M processor

- It's an embedded processor used in most of the Micro-controllers you see today and used in wide ranges of Embedded applications.
  - Battery powered devices like health monitoring and fitness tracking applications, medical meters, etc.
  - Automotive applications.
  - IOT applications.
  - Mobile and Home Appliances.
  - Home/building automation.
  - Toys and Consumer product.
  - PC and Mobile Accessories.
  - Test and measurement devices.

- Most of the manufactures love to use ARM cortex M processor in their design because of its minimal cost, minimal power and minimal silicon area.

- Its a 32 bit processor which will surely boost the computational performance of an application and it comes with almost same the price of 8 bit or 16 bit traditional processor.

- You can use this processor based MCUs in ultra low power to high performance based applications.

- Processor is customizable to include Floating Point Unit, DSP unit MPU, etc.

- Very powerful and easy to use interrupt controller which supports 240 external interrupts.

- RTOS friendly. That means it provides some exceptions, processor operational modes and access level configuration which helps to develop secured RTOS related applications.

- Its instruction set is rich and memory efficient. It uses Thumb instruction set which is collection of 16 bit as well as 32 bit instructions. Cortex M processor cannot execute the ARM instruction set instructions. It uses Thumb instruction set which gives the same 32 bit ARM instruction performance but in 16 bit format.

- ARM provides lots of documentations to learn more about this processor.

### 4. Processor core vs processor

- Core consists of ALU where data computation takes place and result will be generated.
  - It has the logic to decode and execute an instruction.
  - It has many registers to store and manipulate data.
  - It has pipe line engine to boost the instruction execution.
  - It consists of hardware multiplication and division engine.
  - Address generation unit.

- Processor includes: core + specific peripherals (NVIC, FPU, FPB, MPU, DWT, Bus metric etc.)
- Processor is also called as `CPU`.

- The Cortex-M4 processor talks to the external world through various bus interfaces (I-code, D-code and system bus interface AHB).

### 5. Processor vs Micro-controller

- Micro controller is nothing but processor plus peripherals of the manufactory (for example: ST). The peripherals (Flash, GPIO, timer, watchdog etc.) communicate with processor via processor's bus interfaces.

## 4. Embedded Hello world

### 16. Printf using SWV

- How are we going to see the message output without a display device connected to our board?
  - Using printf outputs on ARM Cortex M3/M4/M7 based MCUs
  - This discussion is only applicable to MCUs based on ARM Cortex M3/M4/M7 or higher processors.
  - printf works over SWO pin (serial wire output) of SWD interface.

ARM Cortex M4 Processor--SWO pin---> ST link v2 debug circuit --USB cab---> PC.

- ARM cortex M4 Processor have an specific peripheral that is **ITM unit** (Instrumentation Trace Macro-cell Unit).
  - The ITM is an optional application-driven trace source that supports printf style debugging to trace operating system and application events, and generates diagnostic system information.

- SWD:

  - Serial Wire Debug (SWD) is a two-wire protocol for accessing the ARM debug interface: 2 pin debug + 1 pin trace.
  - It is part of ARM Debug Interface Specification v5 and is an alternative to `JTAG`.
  - The physical layer of SWD consists of two lines.
    - `SWDIO`: a bidirectional  data line.
    - `SWCLK`: a clock driven by the host.

  - By using SWD interface should be able to program MCUs internal flash, u can access memory regions, add breakpoints, stop/run CPU.

  - The other good thing about SWD is you can use the serial wire viewer for your printf statements for debugging.

- `SWD` and `JTAG`:
  - `JTAG` was traditional mechanism for debug connections for ARM7/9 family, but with the Cortex-M family, ARM introduced the Serial Wire Debug (SWD) Interface. SWD is designed to reduce the pin count required for debug from the 4 used by `JTAG` (excluding GND) down to 2. In addition, SWD interface provides one more pin called SWO (Serial Wire Output) which is used for Single Wire Viewing (SWV), which is a low cost tracing technology.

- ITM unit (FIFO ||||||) ----> Debug connector (SWD) ----> SWO pin: this SWO pin is connected to ST link circuitry of the board and can be captured using our debug software (IDE).

- So we use them by edit the write system call:

```C
/* ITM register addresses */
#define ITM_STIMULUS_PORT0      *((volatile uint32_t*) 0xE0000000 )
#define ITM_TRACE_EN            *((volatile uint32_t*) 0xE0000E00 )

void ITM_SendChar(uint8_t ch)
{

    //Enable TRCENA
    DEMCR |= ( 1 << 24);

    //enable stimulus port 0
    ITM_TRACE_EN |= ( 1 << 0);

    // read FIFO status in bit [0]:
    while(!(ITM_STIMULUS_PORT0 & 1));

    //Write to ITM stimulus port0
    ITM_STIMULUS_PORT0 = ch;
}

__attribute__((weak)) int _write(int file, char *ptr, int len)
{
    int DataIdx;

    for (DataIdx = 0; DataIdx < len; DataIdx++)
    {
        ITM_SendChar(*ptr++);
        //__io_putchar(*ptr++);
    }
    return len;
}
```

- Instead call `__io_putchar()` function, we will call `ITM_SendChar()` to send character to the FIFO.

## 5. Access level and operation modes of the processor

### 19. Features of Cortex Mx Processor

- We will learn about features of the Cortex M0/M3/M4 processor:
  - Operational mode of processor.
  - The different access levels of the processor.
  - The register set of the processor.
  - The banked stack design of the processor.
  - Exceptions and exception handling.
  - Interrupt handling.
  - Bus interfaces and bus matrix.
  - Memory architecture of the processor, bit banding, memory map.
  - Endian-ness.
  - aligned and unaligned data transfer.
  - boot-loader and IAP.

### 20. Operational modes of the Cortex Mx Processor

- Processor gives two operational modes:
  - Thread mode.
  - Handler mode.

- All application code will execute under `Thread Mode` of the processor. This is also called as `User mode`.
- All the exceptions handlers or interrupt handlers will run under the `Handler mode` of the processor.
- Processor always starts with `Thread mode`.
- Whenever the core meets this the system exception or any external interrupts then the core will change its mode to handler mode in order to service the ISR associated with that system exception or the interrupt.

- We can use *Interrupt Program Status Register* to check mode that CPU is running on. The bit 0 is `0` means CPU is running on Thread mode, and `1` means CPU is run on Handler mode.

### Access level of the processor

- Processor offers two access level:
  - Privileged Access level (PAL).
  - Non-privileged Access level (N-PAL).

- If your code is running with PAL, then your code full access to all the processor specific resources restricted registers.

- If your code is running with N-PAL, then your code not have access to some of the restricted registers Processor.

- Default your code will run in PAL.

- When the processor is in `thread mode`, its possible to move processor into N-PAL. Once you move out of the PAL to N-PAL being in thread mode, then its not possible to come back to PAL unless u change the processor operational mode to `handler mode`.

- `Handler mode` code execution is always with PAL.

- USe the *CONTROL* register of the processor if you want to switch between the access level.
