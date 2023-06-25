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

### 22. Access level of the processor

- Processor offers two access level:
  - Privileged Access level (PAL).
  - Non-privileged Access level (N-PAL).

- If your code is running with PAL, then your code full access to all the processor specific resources restricted registers.

- If your code is running with N-PAL, then your code not have access to some of the restricted registers Processor.

- Default your code will run in PAL.

- When the processor is in `thread mode`, its possible to move processor into N-PAL. Once you move out of the PAL to N-PAL being in thread mode, then its not possible to come back to PAL unless u change the processor operational mode to `handler mode`.

- `Handler mode` code execution is always with PAL.

- USe the *CONTROL* register of the processor if you want to switch between the access level.

### 23. Cortex M core registers

- The processor core registers are:
  - General-purpose registers (13 registers): R0-R7 (Low register), R8-R12 (High register)
  - Stack pointer: SP (R13) - PSP (Process Stack pointer) - MSP (Main stack pointer)--> banked version of SP
  - Link register: LR (R14)
  - Program counter: PC (R15)
  - Program status register: PSR                                          |
  - Exception mask registers (3 registers): PRIMASK, FAULTMASK, BASEPRI   |===> Special registers
  - CONTROL register: CONTROL                                             |

- These registers actually reside in  Cortex-M4 core (core of processor).
- All the core registers are 32 bits wide.

- *Link register*: stores the return information for subroutines, function calls, and exceptions. On Reset, the processor sets the LR value to `0xFFFFFFFF`.

- For example:

```C
// caller:
void main (void)
{
  func(); // -> PC jumps to func() address.

  instruction_1; // LR = return address (address of next instruction, so PC can jump back to the caller).

}

// callee:
void func (void)
{ // func() address is here.

  // Do some things

} // -> FUnction return PC = LR, PC jump back to resume main() run instruction `instruction_1`.
  // LR contains address of `instruction_1` so when PC finish func(), it can back to caller.

```

- *Program counter*: contains the current program address (address of next instruction to be executed). On reset the processor loads the PC with the value of the reset vector, which is at address `0x00000004`. Bit[0] of value is loaded into the `EPSR` T-bit at reset must be `1`.

- *Program Status Register*: combines:
  - *Application Program Status Register* (APSR): The APSR contains the current state of the condition flags from previous executions. For example, the bit assignments are: bit[31] is `Negative flag`, bit[30] is `Zero flag`.
  - *Interrupt Program Status Register* (IPSR): contains  the exception type of the current `Interrupt Service Routine (ISR)`. bit[0] is `thread mode` state, bit[2] is `NMI`, bit[5] is `BusFault` and more.
  - *Execution Program Status Register* (EPSR): contains the Thumb state bit, and the execution state bits for either the:
    - *If-then (IT)* instruction.
    - *Interruptible-Continuable Instruction (ICI)* field for an interrupted load multiple or store multiple instruction.

  - These registers are mutually exclusive bit-fields in the 32-bit PSR.
  - Access these registers individually or as a combination of any two or all threes, using the register name as an argument to the MSR or MRS instructions. For example:
    - Read all of the registers using PSR with the MRS instruction.

### 24. Memory map and non-memory map register

- Non-memory mapped registers (Processor core registers):
  - The registers do not have unique addresses to access them. Hence there are not part of the processor memory map.
  - You cannot access these registers in a `C` program using address dereferencing.
  - To access these registers, you have to use assembly instruction.

- Memory mapped registers (Registers of the processor specific peripherals (NVIC, MPU, SCB, DEBUG, etc) + Registers of the Micro-controller specific peripherals (RTC, I2C, TIMER, CAN, USB, etc)):
  - Every register has its address in the processor memory map.
  - U can access these registers in `C` program using address dereferencing.

## 6. ARM GCC inline assembly coding

- ARM GCC inline assembly code usage
  - Inline assembly code is used to write pure assembly code inside a `C` program.
  - GCC inline assembly code syntax as shown below:
    - Assembly instruction: `MOV R0,R1`
    - inline assembly statement: `__asm volatile("MOV R0,R1");`

  - For multiple ASM instructions, you can use format with two characters `\n\t` are end of each line. For example:

    ```C
    __asm volatile (
      "LDR R0, [R1]\n\t"
      "LDR R1, [R2]\n\t"
      "ADD R1, R0\n\t"
      "STR R1, [R3]\n\t"
    );
    ```

- `C` variable and inline assembly
  - Move the content of `C` variable `data` to ARM register R0.
  - Move the content of the CONTROL register to the `C` variable `control_reg`.

- General form of an inline assembly statement:

```C
__asm volatile (code : output operand list : input operand list : clobber list);
```

- With:
  - `volatile` is attribute to the asm statement to instruct the compiler not to optimize the assembler code.
  - `code` assembly mnemonic defined as a single string.
  - `output operand list` A list of output operands, separated by commas.
  - `input operand list` A list of input operands, separated by commas.
  - `clobber list` is mainly used to tell the compiler about modifications done by the assembler code.

- For example:
  `__asm volatile ("MOV R0,R1");` is same as `__asm volatile("MOV R0,R1":::);`.

- For example load two values from memory, add them and store the result back to the memory using inline assembly statements.

```C
    // Load 1 to R1 and 2 to R2.
    __asm volatile ("LDR R1, =#0x00000001");
    __asm volatile ("LDR R2, =#0x00000002");

    // Add R1 with R2 and store result to the R0 register.
    __asm volatile ("ADD R0,R1,R2");

    // Store the result back to address that is hold by R1 (0x20000000).
    __asm volatile ("LDR R1, =#0x20000000");
    __asm volatile ("STR R0, [R1]");

```

### 29. Input/output operands and constraint string: Input example

- Input/output operands and constraint string
  - Each input and output operand is described by a constraint string followed by a C expression in parentheses.
  - Input/output operand format: `"<Constraint string>" (<'C' expression>)`
    - Constraint string = constraint character + constraint modifier

- Example 1: Move the content of C variable `val` to ARM register `R0`:
  - Instruction -> MOV
  - Source      -> a C variable `val` (INPUT)
  - Destination -> R0 (ARM core register)

  - We have the C instruction: `__asm volatile ("MOV R0, %0": : "r"(val));`
    - NOTE: we don't have output operand in this instruction, so we need to empty entry.
    - We have one input operand: `"r"(val)` with:
      - `"r"` is a constraint string with only constraint character `r`, does not contains constraint modifier.
      - `val` is a 'C' expression (variable).

    - In the code section, we have: `"MOV R0, %0"`
      - `%0`: operand indexing using `%` sign followed by a digit:
        - `%0` refers to the first operand.
        - `%1` refers to the second operand.
        - and so on.

- For ARM processors, GCC 4 provides the following constraints:

|Constraint |Usage in ARM state                   |Usage in Thumb state     |
|-----------|-------------------------------------|-------------------------|
|f          |floating point registers f0..f7      |Not available            |
|r          |general register r0..r15             |Not available            |
|I          |Immediate value in data processing   |Constant in the range    |
|           |instructions. eg: ORR R0,R0,#operand |0..255. eg: SWI operand  |
|...        |...                                  |...                      |

- constraint modifier:

|Modifier |Specifies                                                  |
|---------|-----------------------------------------------------------|
|=        |Write-only operand, usually used for all output operands   |
|+        |Read-write operand, must be listed as an output operand    |
|&        |A register that should be used for output only             |

- For example:

```C
int val = 50;
__asm volatile ("MOV R0, %0": : "r"(val));
```

```disassembly
39            int val = 50;
08000200:   movs    r3, #50 ; 0x32
08000202:   str     r3, [r7, #4]
40            __asm volatile ("MOV R0, %0": : "r"(val));
08000204:   ldr     r3, [r7, #4]
08000206:   mov     r0, r3
```

- For the inline assembly statement, compile took these actions:
  - 1. First variable(val) value is read from the memory in to `r3`.
  - 2. Then `r3` is used as source register in the final data copy instruction (mov).

- If we use the `I` constraint character we can push value immediately. for example, that equal to the previous example:

```C
__asm volatile ("MOV R0, %0": : "I"(0x32));
```

```disassembly
41            __asm volatile ("MOV R0, %0": : "I"(0x32));
08000208:   mov.w   r0, #50 ; 0x32
```

- So this example means, the `r` constraint character forced the compiler to use an register in `%0`'s place of the instruction.

### 30. Input/output operands and constraint string: Output + Input example

- Example: Move the content of the CONTROL register to C variable `control_reg`
  - CONTROL register is a special register of the ARM core.
  - To read CONTROL register u have to use `MRS` instruction.
    - `MRS`: Move from special register to register.
  - Instruction: MRS
  - Source: CONTROL register
  - Destination: A C variable `control_reg` (OUTPUT operand)

```C
uint32_t control_reg;
__asm volatile ("MRS %0, CONTROL":"=r"(control_reg)::);
```

```disassembly
44            __asm volatile ("MRS %0, CONTROL":"=r"(control_reg)::);
0800020c:   mrs     r3, CONTROL
08000210:   str     r3, [r7, #0]
```

- In this example, we use the `=` constraint modifier tell to the compiler that operation is **write only**.

- Next example, we will the assembly inline instruction to copy two C variable:
  - Copy the content of `C` variable `var1` to `var2`:
  - Instruction: MOV
  - Source: a C variable `var1` (INPUT operand)
  - Destination: a C variable `var2` (OUTPUT operand)

```C
int var1 = 10, var2;
__asm volatile ("MOV %0, %1": "=r"(var2): "r"(var1)); // var2 = var1
```

- Next example, copy the contents of a pointer into another example:

```C
int p1, *p2;
p2 = (int *) 0x20000008;
__asm volatile ("LDR %0, [%1]": "=r"(p1): "r"(p2)); // p1 = *p2
```

- Load the value of address is pointed by p2 to the p1 variable.

## 7. Reset sequence of the processor

### 31. Reset sequence of the processor

- Reset sequence of the Cortex M processor
  - 1. When u reset the processor, the PC is loaded with the value `0x00000000`.
  - 2. Then processor reads the value @ memory location `0x00000000` in to MSP.
    - MSP = value@0x00000000
    - MSP is a Main Stack Pointer register.
    - This means, processor first initializes the Stack Pointer point to stack memory.
  - 3. After that processor reads the value @ memory location `0x00000004` in to PC.
    - That value is actually address of the reset handler.
  - 4. PC jumps to the reset handler.
  - 5. A reset handler is just a C or assembly function written by you to carry out any initializations required.
  - 6. From reset handler you call your `main()` function of the application.

- For example, in file `startup_xxx.s`, we have reset handler function:

```assembly
Reset_Handler:
  ldr   r0, =_estack
  mov   sp, r0          /* set stack pointer */
/* Call the clock system intitialization function.*/
  bl  SystemInit

/* Copy the data segment initializers from flash to SRAM */
  ldr r0, =_sdata
  ldr r1, =_edata
  ldr r2, =_sidata
  movs r3, #0
  b LoopCopyDataInit

CopyDataInit:
  ldr r4, [r2, r3]
  str r4, [r0, r3]
  adds r3, r3, #4

LoopCopyDataInit:
  adds r4, r0, r3
  cmp r4, r1
  bcc CopyDataInit

/* Zero fill the bss segment. */
  ldr r2, =_sbss
  ldr r4, =_ebss
  movs r3, #0
  b LoopFillZerobss

FillZerobss:
  str  r3, [r2]
  adds r2, r2, #4

LoopFillZerobss:
  cmp r2, r4
  bcc FillZerobss

/* Call static constructors */
  bl __libc_init_array
/* Call the application's entry point.*/
  bl main

LoopForever:
    b LoopForever
```

- So, what does reset handler do?
  - 0. Initialize sp point to begin stack memory.

    ```asm
    Reset_Handler:
      ldr   r0, =_estack
      mov   sp, r0          /* set stack pointer */
      //...

    g_pfnVectors:
      .word _estack
      .word Reset_Handler
      .word NMI_Handler
      //...
    ```

  - 1. Initialize data section.
  - 2. Initialize bss section.
  - 3. Initialize `C` std library `__libc_init_array()`.
  - 4. `main()`.

## 8. Access level and T bit

### 33. Demonstration of access level of the processor

- To change the access level in thread mode, we need to change the CONTROL register:
  - bit 0 - nPRIV: Defines the Thread mode privilege level:
    - 0 - privileged.
    - 1 - unprivileged.

- Change the Control registers:
  - read:

    ```C
    uint32_t control_reg;
    __asm volatile ("MRS %0, CONTROL":"=r"(control_reg)::);
    ```

  - modify:

    ```C
    control_reg |= (1 << 0);
    ```

  - write:

    ```C
    __asm volatile ("MSR CONTROL, %0"::"r"(control_reg):);
    ```

- All in one function:

```C
void change_access_level_unprivileged()
{
    __asm volatile ("MRS %0, CONTROL":"=r"(control_reg)::);
    control_reg |= (1 << 0);
    __asm volatile ("MSR CONTROL, %0"::"r"(control_reg):);
}
```

- So, how do we switch back to privileged:
  - First we set un-privileged for thread mode (by set first bit of the CONTROL reg). SO WE CAN SWITCH BACK IN THREAD MODE.
  - So when interrupt/ exception occurs, we always run in `privileged` access level (CONTROL always is 0).
  - Then return to thread mode that is always is un-privileged (because we set it before).

### 34. Importance of T bit of the EPSR

- T bit of the `EPSR` Execution program status register:
  - 1. Various processors support ARM-Thumb inter-working, that means the ability to switch between ARM and thumb state.
  - 2. The processor must be in ARM state to execute instructions which are from ARM ISA and processor must be in Thumb state to execute instructions of Thumb ISA.
  - 3. If `T` bit of the `EPSR` is set(1), processor thinks that the next instruction which it is about to execute is from Thumb ISA.
  - 4. If `T` bit of the `EPSR` is reset(0), processor thinks that the next instruction which is about to execute is from ARM ISA.
  - 5. The Cortex Mx Processor does not support the `ARM` state. Hence, the value of `T` bit must always be 1. Failing  to maintain this is is illegal and this will result in the `Usage fault` exception.
  - 6. The lsb (bit 0) of the program counter (PC) is linked to this `T` bit. When you load a value or an address in to PC the bit[0] of the value is loaded into the T-bit.
  - 7. **Hence, any address you place in the PC must have its 0th bit as 1**. This is usually taken care by the compiler and programmers need not to worry most of the time.
    - For example, every function address is automatically generated by compiler with 0th bit as a 1, etc.
    - So, if u try to load a invalid address (0th bit as 0) to the CPU, it will emit some system exception: Usage Fault, etc.
  - 8. This is reason why you see all vector addresses and incremented by 1 in the vector table.

## 9. Memory map and bus interfaces of ARM Cortex Mx Processor

### 35. Memory map

- Memory map explains mapping of different peripheral registers and memories in the processor addressable memory location range.
- The processor, addressable memory location range, depends upon the size of the address bus.
- The mapping of different regions in the addressable memory location range is called `memory map`.

- The processor has a *fixed default memory map* that provides up to `4GB` of addressable memory.
  ARM Cortex M4 CPU <---32 bit address channel<--AHB--->|---> Data memory
                        32 bit data channel             |---> Code memory
                                                        |---> GPIO
                                                        |---> Timers
                                                        |---> etc

  - For example, we need to load data from ADC, so processor will be load data from ADC via bus with 32bit channel, next it store the data into data memory via bus.

- The bus interface provides 32 bit address and 32 bit data channels, so processor can access up to 2^32 = 4GB addressable memory.

- Memory map of the Cortex Mx processor 0x00000000 -> 0xFFFFFFFF:
                                0xFFFFFFFF
  |Vendor-specific memory 511MB|0xE0100000
  |----------------------------|0xE00FFFFF
  |Private peripheral bus   1MB|0xE0000000
  |----------------------------|0xDFFFFFFF
  |External device          1GB|0xA0000000
  |----------------------------|0x9FFFFFFF
  |External RAM             1GB|0x60000000
  |----------------------------|0x5FFFFFFF
  |Peripheral             0.5GB|0x40000000
  |----------------------------|0x3FFFFFFF
  |SRAM                   0.5GB|0x20000000
  |----------------------------|0x1FFFFFFF
  |CODE                   0.5GB|0x00000000

- CODE region - 512MB (0x00000000 -> 0x1FFFFFFF):
  - This is region where the MCU vendors should connect CODE memory.
  - Different type of CODE memories are: Embedded flash, ROM, OTP, EEPROM, etc.
  - Processor by default fetches vector table information from this region right after reset.

- SRAM region:
  - The SRAM (Static-RAM) region is in the next 512 MB of the memory space after the CODE region.
  - It is primarily for connecting SRAM, mostly on-chip SRAM.
  - The first 1MB of the SRAM region is bit addressable.
    - Bit band region: 0x20000000 -> 0x20100000
    - Bit band address alias: 0x22000000 -> 0x23FFFFFF
  - You can also execute program code from this region.

- Peripherals region:
  - The region is also 512 MB.
  - Used almost for on-chip peripherals.
  - Like the SRAM, the first 1MB of the peripheral region is bit addressable if the bit optional bit band feature is included.
  - This is an **eXecute Never** (`XN`) region.
  - Trying to execute code from this region will trigger fault exception.

- External RAM region:
  - This region is intended for either on-chip or off-chip memory.
  - You can execute code in this region.
  - E.g, connecting external SDRAM.

- External Device Region
  - This region is intended for external devices and/or shared memory.
  - This is an **eXecute Never** (`XN`) region.

- Private peripheral bus
  - This region includes the NVIC, system timer, and system control block.
  - This is an **eXecute Never** (`XN`) region.

### 36. Bus protocols and bus interfaces

- In Cortex Mx Processors the bus interfaces are based on advanced micro-controller bus architecture (`AMBA`) specification.
- `AMBA` is a specification designed by ARM which governs standard for on-chip communication inside the system on chip.
- `AMBA` specification supports several bus protocols.
  - `AHB` Lite (`AMBA` High performance Bus).
  - `APB` (`AMBA` Peripheral Bus).

- AHB and APB:
  - AHB Lite bus is mainly used for the main bus interfaces.
  - APB bus is used for PPB access and some on-chip peripheral access using an AHB-APB bridge.
  - AHB Lite bus majorly used for *high-speed communication* with peripherals that demand high operation speed.
  - APB bus is used for *low-speed communication* compared to AHB. Most of the peripherals which don't require high operation speed are connected to this bus.

- Processor provides 4 bus interface: PPB, System, D-Core, I-Core. By using other interfaces, you can access to different memory region.

ARM cortex Mx Processor |
                        |
                    PPB |<-----AHB 32 bit---->|PPB|
                        |
                  System|<-AHB (access read/write)--->|SRAM, Peripheral |
                        |        \                    |Ext RAM, Device  |
                        |         \
                        |          \--------->|MCU vendor specific region|
                        |
                  D-Core|<--AHB (Data access)------------->|CODE region    |
                  I-Core|<-AHB(instruction fetch)/         |(Program Image)|

- PPB: Private peripheral bus
  - This region includes the NVIC, system timer, and system control block.
  - This is an **eXecute Never** (`XN`) region.

- Actually, the APB bus (low performance) is connected to Processor via a conversation bridge APB to AHB: Peripherals <---> APB/AHB bridge <---> Processor.

### 37. Bit banding

- What is bit-banding?
  - It is the capability to address a single bit of a memory address.
  - This feature is optional. i.e, MCU manufacturer supports it or many not support this feature.
  - For example, let say u have memory of 1kB, how is the memory organize?
      Memory of 1KB
    |               |
    |1024 locations |
    |               |0x20000001
    |               |0x20000000
    <-----8 bits---->

    - If the processor want read one bytes, it access memory via the byte's address.
    - What if you only want read the bit 7 of the byte, for example? what if u only want to read just one bit?
      - You need to using operands: or, and, to evaluate the bit.
    - Bit banding mean, you can address the only one bit. Use The unique address for them.

  - For example, if you want to read the bit you can using the its address:

    ```asm
    LDRB R0, [R1] #R1 hold the address of the bit.
    ```

- Bit band and bit band alias addresses:
  - 1MB Bit band region need to 32 * 1MB = 32MB bit band address.
  - We can use the bit band alias addresses to access to Bits in bit band region.
  - For example:
    Bit-band region                     Alias Equivalent
    0x20000000 bit[0] <---------------->0x22000000 bit[0]
    0x20000000 bit[1] <---------------->0x22000004 bit[0]
    0x20000000 bit[2] <---------------->0x22000008 bit[0]
    0x20000000 bit[3] <---------------->0x2200000C bit[0]
    ...
    0x20000000 bit[31] <--------------->0x2200007C bit[0]

  - The regions for SRAM and peripherals include optional bit-band regions.
  - Bit-banding provides atomic operations to bit data.

- For example, Calculation of bit band alias address:
  - Calculate the band alias address for given bit band memory address and bit position.
  - 7th bit position of the memory location 0x2000_0200 using its alias address.
  - General formula: `alias_address = alias_base + 32 * (bit_band_memory_addr - bit_band_base) + bit * 4`
    - For example: `alias_address = 0x22000000 + 32 * (0x20000200 - 0x20000000) + 7 * 4`

```C
  // point to byte at address 0x20000200
  uint8_t *ptr = (uint32_t *)0x20000200;

  // Set all bits of the byte
  *ptr = 0xff;

  // Clear 7th bit position.
  *ptr &= ~(1 << 7);

  // Point to the alias address to read the bit.
  uint8_t *alias_addr = ALIAS_BASE + (32 * (0x20000200 - 0x20000000)) + 7 * 4;
```

## 10. Stack memory and placement

### 39. Introduction to stack memory

- Stack memory is part of the main memory (Internal RAM or external RAM) reserved for the temporary storage of data (transient data).
- Mainly used during function, interrupt/exception handling.
- Stack memory is accessed in *Last In First Out* fashion (LIFO).
- The stack can be accessed using PUSH and POP instructions or using any memory manipulation (LD, STR).
- The stack is traced using a stack pointer (SP) register. PUSH and POP instructions affect (decrement or increment) stack pointer register (SP, R13).

- Stack memory uses:
  - The temporary storage of processor register values.
  - The temporary storage of local variables of the function.
  - During system exception or interrupt, stack memory will be used to save the context (some general-purpose registers, processor status register, return address) of the currently executing code.

- SRAM will be used for: Global data, Heap, stack, etc.
  - Even you can store instructions in RAM reserved for `global data` and execute it.

### 40. Different stack model operation

- Different stack model operation:
  - 1. Full ascending stack:
    - Memory usage increase from lower to higher: 0x00000000-> 0xffffffff
    - Stack increase following the increasing of the memory address:
      - PUSH: SP increase to next address, for example: sp:0x00002000 -> sp:0x00002004
  - 2. Full descending stack:
    - Memory usage increase from higher to lower: 0xffffffff -> 0x00000000
    - Stack increase following the increasing of the memory address:
      - PUSH: SP increase to next address, for example: sp:0x00002004 -> sp:0x00002000
  - 3. Empty ascending stack:
    - Memory usage increase from lower to higher: 0x00000000-> 0xffffffff
    - The stack pointer is placed on an empty location that is the next address of current stack.
    - Stack increase following the increasing of the memory address:
    - For example, PUSH data to 0x00002000,  sp will be pointed to next empty location on address: 0x00002004
  - 3. Empty descending stack:

### 41. Stack placement

- Stack placement in an application:
  - type 1:
    RAM
    |Unused space |
    |-------------|
    |Stack        |  ||
    |             |  \/
    |-------------|
    |Heap         |  /\
    |             |  ||
    |-------------|
    |Data         |
  - type 2:
    RAM
    |Stack        |  ||
    |             |  \/
    |-------------|
    |Unused space |
    |-------------|
    |Heap         |  /\
    |             |  ||
    |-------------|
    |Data         |

- This defined by linker script. We will type-2 model.

### 42. Banked stack pointer registers of the ARM Cortex Mx

- Banked stack pointers:
  - 1. Cortex M Processor physically has 3 stack pointers: SP (R13), MSP and PSP.
    - MSP: Main stack pointer.
    - PSP: Process stack pointer.
    - SP is called as current stack pointer.
  - 2. After processor reset, by default, MSP will be selected as current stack pointer. That means, SP copies the contents of MSP.
  - 3. Thread mode can change the current stack pointer to PSP by configuring the CONTROL register's `SPSEL` bit.
  - 4. Handler mode code execution will always use MSP as the current stack pointer. Thats also means that, changing the value of `SPSEL` bit being in the handler mode doesn't make any sense. The write will be ignored.
  - 5. MSP will be initialized automatically by the processor after reset by reading the content of the address `0x00000000`.

    ```asm
    Reset_Handler:
      ldr   r0, =_estack
      mov   sp, r0          /* set stack pointer */
      /*...*/

    g_pfnVectors:
      .word _estack
      .word Reset_Handler
      .word NMI_Handler
      /*...*/
    ```

    - In linker file:

    ```C
    /* Highest address of the user mode stack */
    _estack = ORIGIN(RAM) + LENGTH(RAM); /* end of "RAM" Ram type memory */
    ```

  - 6. If you want to use the PSP then make sure that you initialize the PSP to valid stack address in your code.

- For example:
  - SPSEL = 0: current SP (R13)|0x20000034| ----> MSP |0x20000034|
                                                  PSP |0x200000D4|

  - SPSEL = 1: current SP (R13)|0x200000D4|       MSP |0x20000034|
                                          \-----> PSP |0x200000D4|

- `SPSEL` bit: Defines the currently active stack pointer: In handler mode this bit reads as *zero* and ignores writes. The Cortex-M4 updates this bit automatically on exception return:
  - 0 - MSP is the current stack pointer.
  - 1 - PSP is the current stack pointer.

### 43. Stack exercise: change to use the PSP stack

- We need to make valid memory for the PSP to point to.

```C
#define SRAM_START 0x20000000U
#define SRAM_SIZE (128 * 1024)
#define SRAM_END (SRAM_START + SRAM_SIZE)
#define STACK_START SRAM_END

#define STACK_MSP_START STACK_START
#define STACK_MSP_END (STACK_MSP_START - 512)
#define STACK_PSP_START STACK_MSP_END
```

- Load the address to the PSP to make it point to it and enable PSP:

```C
/*
 * This attribute allows the compiler to construct the requisite function declaration,
 * while allowing the body of the function to be assembly code.
 */
__attribute__((naked)) void change_sp_to_psp(void)
{
  // Set PSP stack point to the valid memory region.
  uint32_t psp_start_address = STACK_PSP_START;
  __asm volatile("MOV R0, %0"::"r"(psp_start_address):);
  __asm volatile("MSR PSP, R0");
  // Set 1th bit in control register to use the PSP register.

  __asm volatile("MOV R0, #0x02");
  __asm volatile("MSR CONTROL, R0");

  // Return instructions.
  __asm volatile("BX LR");
}
```

- Summary:
  - 1. Physically there are 2 stack pointer registers in Cortex-M Processors.
  - 2. Main stack pointer (MSP): This is the default stack pointer used after reset, and is used for all exception/interrupt handlers and for codes which run in thread mode.
  - 3. Process Stack pointer (PSP): This is an alternate stack pointer that can only be used in thread mode. It is usually used for application task in embedded systems and embedded OS.
  - 4. After power-up, the processor automatically initializes the MSP by reading the first location of the vector table.
  - 5. Changing SP:
    - To access MSP and PSP in assembly code, u can use the MSR and MRS instructions.
    - In a `C` program u can write a **naked** function ('C' like assembly function which doesn't have epilogue and prologue sequences) to change the current selected stack pointer.

## 11. Exception model of ARM Cortex Mx Processor

### 47. Exception model

- What is exception?
  - Exception can be anything which disturbs the normal operation of the program by changing the operation mode of the processor.

- There are two types of exceptions:
  - System exceptions.
  - Interrupt.

- What are system exception?
  - System exceptions are generated by the processor itself internally.
  - Interrupts come from the external world to the processor.

- Whenever the processor core meets with exception it changes the operational mode to `Handler mode`.

- There are in total 15 system exceptions supported by the Cortex M processors. And 240 interrupts.
- So, in total Cortex M processors support **255 exceptions**.

### 48. Different system exception

- Different System Exceptions:
  - There is room for 15 system exceptions.
  - Exception number 1 is Reset Exception.
  - Only 9 Implemented System Exceptions. 6 are reserved for future implementations.
  - Exception number 16 is Interrupt 1 (IRQ 1).

- `Reset`: is system exception that is invoked on power up or a warm reset. The exception model treats reset as a special form of exception.
  - When reset is asserted, the operation of the processor stops, potentially at any point in an instruction.
  - When reset is de-asserted, execution restarts from the address provided by the reset entry in the vector table.
  - Execution restarts as *privileged execution in Thread Mode*.

- `NMI`: A *Non Maskable Interrupt* (NMI) can be signalled by a peripheral or triggered by software. This is the **highest priority exception** other than reset. It is permanently enabled and has a fixed priority of `-2` NMIs cannot be:
  - Masked or prevented from activation by any other exception.
  - Preempted by any exception other than Reset.

- `HardFault`: A HardFault is an exception that occurs because of an *error during exception processing*, or because an exception cannot be managed by any other exception mechanism.

- `MemManage`: A MemManage fault is an exception that occurs because of a memory protection related fault. The fixed memory protection constraints determines this fault, for both instruction and data memory transactions.
  - This fault is always used to abort instruction accesses to *Execute Never* (XN) memory region.

- `BusFault`: A Bus fault is an exception that occurs because of a memory related fault for an instruction or data memory transaction. This might be from an error detected on a bus in the memory system.

- `UsageFault`: is an exception that occurs because if a fault related to instruction execution. This includes:
  - An undefined instruction.
  - An illegal unaligned access.
  - Invalid state on instruction execution.
  - An error on exception.

  - The following can cause a UsageFault when the core is configured to report them:
    - An unaligned address on word and half-word memory access.
    - Division by zero.

- `SVCall`: A *supervisor call* (SVC) is an exception that is triggered by the `SVC` instruction. In an OS environment. applications can use `SVC` instructions to access OS kernel functions and device drivers.

- `PendSV`: PendSV is an interrupt-driven request for system-level service. In an OS environment, use PendSV for context switching when no other exception is active.

- `SysTick`: A Sys-tick exception is an exception the system timer generates when it reaches zero. Software can also generate a Sys-tick exception. In an OS environment, the processor can use this exception as system tick.

- Interrupt (IRQ) A interrupt, or IRQ, is an exception signalled by a peripheral, or generated by a software request. All interrupts are asynchronous to instruction execution. In the system, peripherals use interrupts to communicate with the processor.
