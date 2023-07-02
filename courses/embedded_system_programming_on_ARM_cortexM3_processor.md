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

### 45. Function call and AAPCS standard

- Caller vs Callee

```C
void func_x(void)
{ // This is caller that calls func_y()
  int ret;
  ret = func_y(1, 2);
}

void func_y(int a, int b)
{ // This is callee function.
  return a + b;
}
```

- The AAPCS standard describes Procedure Call Standard used by the application binary interface (ABI) for ARM architecture.

- SCOPE:
  - The AAPCS defines how subroutines can be **separately written compiled**, and **separately assembled** to work together. It describes a called routine that defines:
    - Obligations on the caller to create a program state in which the called routine may start to execute.
    - Obligations on the called routine to preserve the program state of the caller across the call.
    - The rights of the called routine to alter the program state of its caller.

- When a `C` compiler compiles code for the ARM arch, it should follow the AAPCS specification to generate code.

- According to this standard, a `C` function can modify the registers R0, R1, R2, R3, R14 (LR) and PSR and it's not the responsibility of the function to save these registers before any modification. -> Caller should be save them before call a function (should be push to stack before call and pop when the callee exit).

- If a function wants to make use of R4 to R11 registers, then it's the responsibility of the function to save its previous contents before modifying those registers and retrieve it back exiting the function. -> Callee should restore them before exit it's function body. (Should be push to stack when start routine and pop before exit routine).

- R0, R1, R2, R3, R12, R14 (LR) registers are called **caller saved registers**, it's the responsibility of the caller to save these registers on stack before calling the function if those values will still be needed after the function call and retrieve it back once the called function returns. Register values that are not required after the function call don't have to be saved.

- R4->R11 are called `callee saved registers`. This function or subroutine being called needs to make sure that, content of these registers will be unaltered before exiting the function.

- According to this standard, caller functions uses R0, R1, R2, R3 registers to send input arguments to the callee function.

- The callee function uses registers R0 and R1 to send the result back to the caller function.

- For example:

```C
void func_x(void)
{ // This is caller that calls func_y()
  int ret;
  ret = func_y(1, 2, 3, 4);  // set R0 = 1, R1 = 2, R2 = 4, R3 = 4
                             // Get Ret = R0 = (a + b + c + d)
}

void func_y(int a, int b, int c, int d)
{ // get R0 = 1, R1 = 2, R2 = 4, R3 = 4 to local variable: a, b, c, d
  return a + b + c + d; // Set R0 = (a + b + c + d)
}
```

- If the arguments is greater 4, stack will be used.

- For example, the main call `func_add(int a, int b, int c)` pass three arguments to R2, R1, R0, and get result via the R0:

```assembly
  ret = func_add(1, 5, 6);
 80002e6:  2206        movs  r2, #6
 80002e8:  2105        movs  r1, #5
 80002ea:  2001        movs  r0, #1
 80002ec:  f7ff ffd0   bl  8000290 <func_add>
 80002f0:  6078        str  r0, [r7, #4]
```

- In the `func_add(int a, int b, int c)`, callee push R7 to save it before use it and restore them before exiting:

```assembly
int func_add(int a, int b, int c)
{
 8000290:  b480        push  {r7}
 8000292:  b085        sub  sp, #20
 8000294:  af00        add  r7, sp, #0
 8000296:  60f8        str  r0, [r7, #12]
 8000298:  60b9        str  r1, [r7, #8]
 800029a:  607a        str  r2, [r7, #4]
  return a +b + c;
 800029c:  68fa        ldr  r2, [r7, #12]
 800029e:  68bb        ldr  r3, [r7, #8]
 80002a0:  441a        add  r2, r3
 80002a2:  687b        ldr  r3, [r7, #4]
 80002a4:  4413        add  r3, r2
}
 80002a6:  4618        mov  r0, r3
 80002a8:  3714        adds  r7, #20
 80002aa:  46bd        mov  sp, r7
 80002ac:  f85d 7b04   ldr.w  r7, [sp], #4
 80002b0:  4770        bx  lr
  ...

```

### 47. Stack activities during interrupt and exception

- To allow a `C` function to be used as an exception/interrupt handler, the exception mechanism needs to save R0 to R3, R12, LR, and XPSR at exception entrance automatically and restore them at exception exit under the control of the processor hardware.

- In this way, when returned to the interrupted program, all the registers would have the same value as when the interrupt entry sequence started.

- when we switch to Interrupt/Exception handler:
  - The processor saves ``caller saved registers`` since there is no caller for interrupt or exception handler. So, u can write an interrupt/exception handler as normal `C` function without worrying about AAPCS rules.
  - Stack frame: |R0|R1|R2|R3|R12|LR|PC|XPSR|

- Processor does **STACKING** automatically (push registers to stack frame) when entering handler mode.
- And, **UN-STACKING** automatically (pop stack frame to registers) when exiting handler mode.

- Stack initialization:
  - Before reaching main.
  - After reaching the main function, u may again reinitialize the stack pointer.

- Stack initialization tips:
  - 1. Evaluate your targeted application. Decide the amount of stack that would be needed for the worst-case scenario of your application runtime.
  - 2. Know your processor's consumption model (FD, FA, ED, EA)
  - 3. Decide stack placement in the RAM (middle, end, external memory)
  - 4. In many applications, there may be second stage stack init. For example, if you want to allocate stack in external SDRAM then first start with internal RAM, in the main or startup code initialize the SDRAM then change the stack pointer to point to SDRAM.
  - 5. If you are using ARM cortex Mx Processor, make sure that the first location of the vector table contains the initial stack address (MSP). The startup code of the project usually does this.
  - 6. You may also use the linker script to decide the stack, heal and other RAM area boundaries. Start up code usually fetches boundary information from linker scripts.
  - 7. In an RTOS scenario, the kernel code may use MSP to trace its own stack and configure PSP for user task's stack.

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

### 50. System exception control registers

- Every System exception control registers lie on The address map of the Private peripheral bus (PPB) (address map: 0xE000E[xxx])

- System Control Block (SCB):
  - The System Control Block (SCB) provides system implementation information and system control. This includes configuration, control, and reporting of the system exceptions.
  - Explore registers from ARM Cortex M4 generic user guide.
  - By using it:
    - U can enable fault handlers.
    - Get pending status of the fault exceptions.
    - Trap processor for divide by zero and unaligned data access attempts.
    - Control sleep and sleep wake-up settings.
    - Configure the priority of the system exceptions.
    - Sys-tick timer control and status.

- Default system exception status:
  - Hard fault: Always enable by default, can be masked.
  - NMI: Always enabled, cannot be masked.
  - Usage fault: Disable by default.
  - Mem manage fault: Disable by default.
  - Bus fault: Disable by default.
  - Sys-tick exception: Disable by default and triggers whenever sys-tick timer is enabled and expired.
  - SVC exception: Triggers only when the SVC instruction is executed.
  - PendSV exception: Disabled by default.
  - Debug monitor exception: Disabled by default.

### 51. NVIC

- NVIC (Nested Vector interrupt Controller):
  - 1. NVIC is one of the peripheral of the Cortex Mx Processor Core.
  - 2. It is used to configure the 240 interrupts.
  - 3. Using NVIC registers u can enable/disable/pend various interrupts and read the status of the active and pending interrupts.
  - 4. You can configure the priority and priority grouping of various interrupts.
  - 5. It is called as `Nested` because it supports preempting a lower priority interrupt handler when higher priority interrupt arrives.

- Enable/disable/pend various interrupts using NVIC register:
  - 1. Cortex M processor supports 240 interrupts.
  - 2. These interrupts are managed and configured using NVIC.
  - 3. What are those 240 interrupts?
    - This is highly vendor specific and it is triggered by various on chip peripherals of the MCU like SPI, GPIOs, CAN, Timer, DMA, etc.
    - For example, interrupt Handler addresses of STM32F407: 0x0000 0040 - 0x0000 01A8
      - STM32F407 micro controller only delivers 90 interrupts to the processor, while processor supports 240.

- Model:
  MCU                   CPU
  |WD|----0-->|---->|    |          |
        --1-->|---->|    |          |
        --2-->|---->|    |          |
              |---->|NVIC|-->Core   |
  |I2C|---54->|---->|    |          |
              |---->|    |          |
        --90->|---->|    |          |

- MCU using IRQ numbers to trigger to NVIC and NVIC will emit to processor core. It's peripherals are connected to those lines to do this.
  - For example, I2C peripheral trigger interrupt via IRQ number 54.

### 52. NVIC registers

- NVIC have 5 group registers summary:
  - Interrupt Set-Enable Registers
  - Interrupt Clear-Enable Registers
  - Interrupt Set-pending Registers
  - Interrupt Clear-pending Registers
  - Interrupt Active Bit Registers

- Interrupt Set-Enable Registers:
  - `NVIV_ISER0` - `NVIV_ISER7`: 8 registers (32 *  8 = 256) enable corresponding interrupts and can show which interrupts are enabled or not.
    - write:
      - 0 = effect.
      - 1 = enable interrupt.
    - read:
      - 0 = interrupt disabled.
      - 1 = interrupt enabled.
    - You CANNOT use these registers to disable interrupt, you can only enable or read state of them.

- Interrupt Clear-Enable Registers:
  - `NVIV_ISER0` - `NVIV_ISER7`: 8 registers (32 *  8 = 256) disable corresponding interrupts and can show which interrupts are disabled or not.
    - write:
      - 0 = no effect.
      - 1 = disable interrupt.
    - Read:
      - 0 = interrupt disabled.
      - 1 = interrupt enabled.

- Interrupt Set-pending Registers:
  - `NVIC_ISPR0` - `NVIC_ISPR7`: 8 registers (32 *  8 = 256) force interrupts into the pending state, and show which interrupts are pending.
    - write:
      - 0 = no effect.
      - 1 = changes interrupt state to pending.
    - Read:
      - 0 = interrupt is not pending.
      - 1 = interrupt is pending.

- Interrupt Clear-pending Registers:
  - `NVIC_ICPR0` - `NVIC_ICPR7`: 8 registers (32 *  8 = 256) remove the pending state from interrupts, and show which interrupts are pending.
    - write:
      - 0 = no effect.
      - 1 = remove pending state from interrupt.
    - Read:
      - 0 = interrupt is not pending.
      - 1 = interrupt is pending.

- Interrupt Active Bit Registers:
  - `NVIC_IABR0` - `NVIC_IABR7`: registers indicate which interrupts are active. That means corresponding interrupt handler is executing by processor.
    - Active flags:
      - 0 = interrupt not active.
      - 1 = interrupt active.

### 53. Peripheral interrupt exercise

- Exercise-Enabling and Pending of USART3 interrupt:

  MCU           |             Processor
UASRT3----------|-IRQ39-----> |39 NVIC| <-----> |CPU|
                |
                |
                |

- Steps to program an MCU peripheral interrupt
  - 1. Identify the IRQ number of the peripheral by referring to the MCU vector table. IRQ numbers are vendor-specific.
  - 2. Program the Processor register to enable that IRQ (only when you enable the IRQ, the processor will accept the interrupt over that line). Set the priority (optional).
  - 3. Configure the peripheral (USART3) using its peripheral configuration register. For example, in the case of USART3, whenever a packet is received, it will automatically issue an interrupt on the IRQ line 39.
  - 4. When the interrupt is issued on the IRQ line, it will first get pended in the pending register of the processor.
  - 5. NVIC will allow the IRQ handler associated with the IRQ number to run only if the priority of the new interrupts higher than the currently executing interrupt handler. Otherwise newly arrived interrupt will stay in pending state.
  - 6. Please note that if peripheral issues an interrupt when the IRQ number is disabled (not activated from the processor side), then still interrupt will get pended in the pending register of the NVIC. As soon as IRQ is enabled, it will trigger the execution of the ISR of the priority is higher than the currently active ISR.

- Data packet arrives from external world in to USART peripheral buffer
  -> UART peripheral issues an interrupt.
  -> interrupt pends in the pending register of the NVIC (Add to queue).
  -> if IRQ enable, NVIC emit to CPU.
  -> CPU fetch the ISR address from vector table and jumps to ISR.
  -> in ISR { we will copy data from rx buffer to SRAM and handle it}.

- Enable the interrupt 39:

```C
int main()
{
  // We we pend the pending bit manually because we don't have real device, that is same with issueing the interrupt.
  uint32_t *pISPR1 = (uint32_t *)0xE000E204;
  *pISPR1 |= (1 << (USART3_IRQNO % 32));

  // Enable the USART3 IRQ number in NVIC.
  uint32_t *pISER1 = (uint32_t *)0xE000E104;
  *pISER1 |= (1 << (USART3_IRQNO % 32));
}

void USART3_IRQHandler(void)
{
  printf("I'm USART3_IRQHandler\n");
}
```

## 12. Interrupt priority and configuration

### 55. Interrupt priority explaination

- What is priority?
  - It means urgency
- What is priority value?
  - The priority value is a measure of urgency (decides how urgent from others).

- Relation between priority and it's value.
  - For example, timer priority value is 4, and ADC's value is 5. So timer is more URGENT than ADC interrupt.
  - We say timer priority os HIGHER than ADC priority.
  - If both interrupt hits the NVIC at the same time, NVIC allows TIMER interrupt first, so TIMER ISR will be executed first by the processor.

- Diferent priority levels:
  - Priority values are also called as priority levels.

- How many different priority levels are there in ARM cortex Mx processor?
  - It depends on the **Interrupt Priority Register** implementation by the MCU vendors.
    - STM32F4x MCU has 16 different priority levels.

- By using these registers u can configure priority levels for interrupts (IRQs) only.
  - we have 60 Interrupt Priority Registers (60 * 32 / 8 = 240): NVIC_IPR0-NVIC_IPR59
    - 8 bits for an interrupt.

- Real implementation by the MCU vendors: they just use four bit [4-7] to make priority value because vendor just make only.
  - So currently, we 16 priority levels.

### 56. Preempt and sub priority

- What if two interupts of the same priority hit the processor at the same time?
  - The sub priority value of the interrupts will be checked to resolve the conflict. An interrupt with lower sub priority value will be allowed first.

- Priority Grouping
  - Pre-Empt Priority: when the processor is running interrupt handler, and another interrupt appears, *then the pre-empt priority values will be compared*, and interrupt with higher pre-empt priority (less in value) value will be allowed to run.
  - Sub priority: This value is used only when two interrupts with the same preemt priority values occur at the same time. In this case, the interrupt with higher sub-priority(less in value) will be handled first.

- Priority grouping value decides how the Interrupt Priority register should be read:

|Priority group |pre-empt priority field|sub-priority field   |
|0(default)     |Bit[7:1]               |Bit[0]               |
|1              |Bit[7:2]               |Bit[1:0]             |
|2              |Bit[7:3]               |Bit[2:0]             |
|3              |Bit[7:4]               |Bit[3:0]             |
|4              |Bit[7:5]               |Bit[4:0]             |
|5              |Bit[7:6]               |Bit[5:0]             |
|6              |Bit[7:7]               |Bit[6:0]             |
|7              |None                   |Bit[7:0]             |

- We can change the Priority grouping by changing `PRIGROUP`: bit[8-10] of the **Application Interrupt And Reset Control Register**.
- however, because the bit[0-3] in 8 bits of interrupt priority, so there are only five valid cases, the other cases are same with default mode:

|Priority group |pre-empt priority field|sub-priority field   |
|0(default)     |Bit[7:1]               |Bit[0]               |
|4              |Bit[7:5]               |Bit[4:0]             |
|5              |Bit[7:6]               |Bit[5:0]             |
|6              |Bit[7:7]               |Bit[6:0]             |
|7              |None                   |Bit[7:0]             |

- What if two interrupts of the same pre-empt priority and sub priority hit the processor at the same time?
  - *Interrupt with the lowest IRQ number will be allowdd first*.

### 57. Interrupt Priority configuration: Exercise

- Generate the below peripheral interrupts using NVIC interrupt pending register and observe the execution of ISRs priorities are same and different.
  - TIM2 global interrupt.
  - I2C1 event interrupt.

```C
#define IRQNO_TIMER2   28
#define IRQNO_I2C1    31

uint32_t *pNVIC_IPRBase = (uint32_t *) 0xE000E400;  // Interrupt Priority Base Register.
uint32_t *pNVIC_ISPRBase = (uint32_t *) 0xE000E200; // Interrupt Set-Pending Base Register.
uint32_t *pNVIC_ISERBase = (uint32_t *) 0xE000E100; // Interrupt Set-Enable Base Register.

void configure_priority_for_irqs(uint8_t irq_no, uint8_t priority_value)
{
  /* 1. Set priority. */
  uint8_t ipr_offset = (irq_no * 8) / 32;
  uint32_t *ipr = pNVIC_IPRBase + ipr_offset;
  uint8_t ipr_bit_pos = (irq_no * 8) % 32;
  *ipr &= (0xFF << ipr_bit_pos); // Clear old priority byte.
  *ipr |= (priority_value << ipr_bit_pos); // Set new priority byte.

  /* 2. Set pending state. */
  uint8_t ispr_offset = irq_no / 32;
  uint32_t *ispr = pNVIC_ISPRBase + ispr_offset;
  uint8_t ispr_bit_pos = irq_no % 32;
  *ispr |=  (1 << ispr_bit_pos);

  /* 3. Enable Interrupt. */
  uint8_t iser_offset = irq_no / 32;
  uint32_t *iser = pNVIC_ISERBase + iser_offset;
  uint8_t iser_bit_pos = irq_no % 32;
  *iser |=  (1 << iser_bit_pos);
}

int main(void)
{
  configure_priority_for_irqs(IRQNO_I2C1, 0b01100000); // enable I2C1 event interrupt with priority 6.
}

void TIM2_IRQHandler(void)
{
  printf("TIM2_IRQHandler.\n");
  while(1)
  {
  }
}

void I2C1_EV_IRQHandler(void)
{
  printf("I2C1_EV_IRQHandler.\n");
  configure_priority_for_irqs(IRQNO_TIMER2, 0b01000000); // enable timer 2 interrupt with priority 4.
  while(1)
  {
  }
}

```

### 58. Pending interrupt behavior

- Case 1: Single Pended Interrupt
  - When an interrupt request occurs, pending bit is set:
    - processor swith to handler mode: stacking & vector fetch
    - When interrupt handler start running:
      - Pending bit is clear.
      - Interrupt Active Status Bit is set.
    - when exception return:
      - Interrupt Active Status Bit is clear.
      - Processor unstacking and switch to thread mode.

- Case 2: Double pended interrupt:
  - The pending bit of current interrupt will be pended again if there is new interrupt with higher priority occurs.
  - If the new interrupt with low priority occurs, it will be hold on pending state.

## 13. Exception entry and exit sequences

### 59. Exception entry and exit sequences

- Exception Entry Sequence:
  - 1. Pending bit set.
  - 2. Stacking and Vector fetch.
  - 3. Entry into the handler and Active bit set.
  - 4. Clears the pending status (processor does it automatically).
  - 5. Now processor mode changed to handler mode.
  - 6. Now handler code is executing.
  - 7. The MSP will be used for any stack operations inside the handler.

- Exception Exit Sequence:
  - In Cortex-M3/M4 processors the exception return mechanism is triggered using a special return address called `EXC_RETURN`.
  - `EXC_RETURN` is generated during exception entry and is stored in the LR.
  - When `EXC_RETURN` is written to PC it triggers the exception return.

- `EXC_RETURN` when it is generated?
  - During an exception handler entry, the value of the return address(PC) is not stored in the LR as it is done during calling of a normal C function. Instead the exception mechanism stores **the special value called EXC_RETURN in LR**.

- Decoding `EXC_RETURN` value:

|Bits   |Descriptions                 |Values                                                 |
|-------|-----------------------------|-------------------------------------------------------|
|31:28  |EXC_RETURN indicicator       |0xF                                                    |
|27:5   |Reserved(all 1)              |0xEFFFFF                                               |
|4      |Stack frame type             |always 1 when floating point unit is not available.    |
|3      |Return mode                  |1 = return to thread mode, 0 = return to handler mode  |
|2      |Return stack                 |1 = return with PSP, 0 = return with MSP               |
|1      |Reserved                     |0                                                      |
|0      |Reserved                     |1                                                      |

- For example `EXC_RETURN` = `0xFFFFFFF1` return handler mode, exception return gets state from the main stack. Execution uses MSP after return.

### 60. Analyzing stack contents during exception entry and exit

- Saved registers:
  |XPSR |
  |PC   |
  |LR   |
  |R12  |
  |R3   |
  |R2   |
  |R1   |
  |R0   |

- When the `EXC_RETURN` is put on the `PC` register, exception exit will be triggered.
  - `EXC_RETURN` = `0xFFFFFFF9` means exception return to thread mode and get state from main stack, execution uses MSP after return.
  - After that PC get return address from stack frame and jump to it.

## 14. Fault handling and analysis

### 61. Introduction to processor faults

- What is a fault?
  - The fault is an exception generated by the processor (system exception) to indicate an error.

- Why fault happens?
  - faults happen because of programmers handling processor by violating the design rules or may be due to interfaces with which the processor deals.
  - Whenever a fault happens, internal processor registers will be updated to record the type of fault, the address of instruction at which the fault happened, and if an associated exception is enabled, the exception handler will be called by the processor.
  - In the exception handler programmers may implement the code to report, resolve, or recover from the fault.
  - For example, if your code tries to divide a number by zero, then divide by 0 fault will be raised from the hardware, which will invoke usage fault exception handler (if enabled). In the exception handler, u may make certain decisions to get rid of the problem, like closing the task, etc.
  - Most of the time, fault happens by programmer's code not adhering to processor programming guidelines.

- Different types of fault exceptions in Cortex Mx Processor:
  - Hard fault exception* {Enabled by default, non-configurable priority}
  - Usage fault exception       |
  - Mem manage fault exception  |--> Disbled by default, configurable priority.
  - Bus fault exception         |

- *Hard-fault exception can be disabled by code using `FAULTMASK` register.

- Causes of fault:
  - Divide by zero (if enabled).
  - Undefined instruction.
  - Attempt to execute code from memory region which is marked as eXecute Never (XN) to prevent code injection.
  - MPU guarded memory region access violation by the code.
  - Unaligned data access (if enabled).
  - Returning to thread mode keeping active interrupt alive.
  - Bus error (example no response from memory device (e.g, SDRAM)).
  - Executing SVC instruction inside SVC handler or calling a function in SVC handler which eventually execute hidden SVC instruction.
  - Debug monitor settings and related exceptions.

### 62. Hard fault exception

- A Hardfault is an exception that occurs because of an error during exception processing, or because an exception cannot be managed by any other exception mechanism. it has 3rd highest fixed priority (-1) after reset and NMI meaning it has higher priority than any exception with configurable priority.

- Causes:
  - 1. Escalation of configurable fault exceptions.
  - 2. Bus error returned during a vector fetch.
  - 3. Execution of break point instruction when both halt mode and debug monitor is disabled.
  - 4. Executing SVC instruction inside SVC handler.

- For example:
  - An Configurable exceptions (usage, mem manage, bus fault) issues, Is Configurable exception enabled?
    - No - Escalated to hardfault (FORCED) -> Hardfault handler.
    - Yes - Handled by configurable exceptions handlers.
  - Bus fault during vector fecth from vector table ---Always escaleted to hard fault---> Hardfault handler.

- HardFault Status Register:
  - bit[1]: VECTTBL - Indicates a BusFault on a vector table read during exception processing:
    - 0 - no BusFault on vector table read.
    - 1 - Busfault on vector table read,
    - This error is always handled by the hard fault handler.

### 63. Other configurable faults

- Mem manage fault exception:
  - This is a configurable fault exception. Disabled by default.
  - You can enable this exception by configuring the processor register **System Handler Control and State Register (SHCSR)**
  - When mem manage fault happens mem manage fault exception handler will be executed by the processor.
  - Priority of this fault exception is configurable.

- Causes:
  - 1. As its name indicate this fault exception triggers when memory access violation is detected (access permission, etc).
  - 2. Unprivileged thread mode code (such as user code in RTOS) tries to access memory region which is marked as *privileged access only* by the MPU.
  - 3. Writing to memory regions which are marked as read-only by the MPU4.
  - 4. This fault can also be triggered when trying to execute program code from *peripheral* memory regions. Peripheral memory regions are marked as XN (eXecute Never) regions by the processor design to avoid code injection attacks through peripherals.

- Bus fault exception:
  - This is a configurable fault exception. Disabled by default.
  - You can enable this exception by configuring the processor register **System Handler Control and State Register (SHCSR)**
  - When Bus fault happens Bus fault exception handler will be executed by the processor.
  - Priority of this fault exception is configurable.

- Causes:
  - 1. Due to error response returned by the processor bus interfaces during access to memory device.
    - During instruction fetch.
    - During data read or write to memory devices.
  - 2. If bus error happens during vector fetch, it will be escalated to a hard fault even if bus fault exception is enabled.
  - 3. Memory device sends error response when the processor bus interface tries to access invalid or restricted memory locations whicl could generate a bus fault.
  - 4. When the device is not ready to accept memory transfer.
  - 5. You may encounter such issues when you play with external memories such as SDRAM connected via DRAM controllers.
  - 6. Unprivileged access to the private peripheral bus.

- Usage fault exception:
  - This is a configurable fault exception. Disabled by default.
  - You can enable this exception by configuring the processor register **System Handler Control and State Register (SHCSR)**
  - When Usage fault happens Usage fault exception handler will be executed by the processor.
  - Priority of this fault exception is configurable.

- Causes:
  - 1. Execution of undefined instruction (M4 only supports thumb ISA).
  - 2. Executing floating point instruction keep floating point unit is disabled.
  - 3. Trying to switch to ARM state to execute ARM ISA instructions. The T bit of the processor decides ARM state or THUMB state. Making T bit is 0 would result in a fault.
  - 4. Trying to return to thread mode when an exception/interrupt is still active.
  - 5. Unaligned memory access with multiple load or multiple store instructions.
  - 6. Attempt to divide by zero (if enabled, by default divide by zero results in zero).
  - 7. For all unaligned data access from memory (only if enabled ,otherwise cortex M supports unaligned data access).

### 64. Configurable fault exception exercise

- Write a program to enable all configurable fault exceptions, implement the fault exception handlers and cause the fault by follwing method:
  - 1. Execute an undefined instruction.
  - 2. Divide by zero.
  - 3. Try executing instruction from peripheral region.
  - 4. Executing SVC inside the SVC handler.
  - 5. Executing SVC instruction inside interrupt handler whose priority is same or lesser than SVC handler.

- Enable configurable exceptions:

```C
  /* 1. Enable configurable system exceptions */
  uint32_t *pSHCSR = (uint32_t *)0xE000ED24; // Get System Handler Control and State Register.

  *pSHCSR |= (1 << 16); // Mem manage fault
  *pSHCSR |= (1 << 17); // Bus fault
  *pSHCSR |= (1 << 18); // Usage fault
```

- 1. Execute an undefined instruction:

  ```C
    /* Test execute undefined instructions.*/
    uint32_t *pSRAM = (uint32_t *) 0x20010000;
    *pSRAM = 0xFFFFFFFF;
    void (*some_func_ptr)(void);
    some_func_ptr = *pSRAM;

    // Jump to invalid opcode.
    some_func_ptr();
  ```

  - The MemManage_Handler is issued.

- Detect fault causes:
  - Fault status and address information:
    - When a fault happens, inside the fault handler, you can check a couple of fault status and address information registers to get more details about the fault and the instruction address at which the fault happened. This will be helpful during debugging.

  - Registers:
    - HardFault Status Register
    - MemManage Fault Status Register
    - BusFault Status Register
    - UsageFault Register

### 65. Analyzing stack frame

- When a exception triggers, *Stacking of thread mode context* will be occurs. And then the handler mode is executed.

- Stack looks like:
  Top of stack before exception SP (MSP) ------>|Previous stack content |   Memory (+)
                                          SP-4  |         XPSR          |     /\
                                          SP-8  |          PC           |     ||
                                          SP-12 |          LR           |     ||
                                          SP-16 |          R12          |     ||
                                          SP-20 |          R3           |     ||
                                          SP-24 |          R2           |     ||
                                          SP-28 |          R1           |     ||
  Top of stack after exception triggers ->SP-32 |          R0           |     ||
            SP (MSP)

- We can get value of MSP in Exception handler and dump it, for example:

```C
void MemManage_Handler(void)
{
  __asm ("MRS R0, MSP");
  register uint32_t msp_value __asm("r0");
  uint32_t *pMSP = (uint32_t *)msp_value;
  // Do some thing with MSP

  printf("MemManage_Handler\n");
  while(1);
}
```

- Or capture it in `naked` function and pass to C function, as below:

```C
__attribute__((naked)) void MemManage_Handler(void)
{
  __asm("MRS R0, MSP");
  __asm("B MemManage_Handler_in_c"); // RO is passed to MemManage_Handler_in_c as first argument.
}

void MemManage_Handler_in_c(uint32_t * base_stack_frame)
{
  // Do some thing with MSP
  printf("MSP value: %p\n", base_stack_frame);
  printf("R0 value: %lx\n", base_stack_frame[0]);
  printf("R1 value: %lx\n", base_stack_frame[1]);
  printf("R2 value: %lx\n", base_stack_frame[2]);
  printf("R3 value: %lx\n", base_stack_frame[3]);
  printf("R12 value: %lx\n", base_stack_frame[4]);
  printf("LR value: %lx\n", base_stack_frame[5]);
  printf("PC value: %lx\n", base_stack_frame[6]);
  printf("XPSR value: %lx\n", base_stack_frame[7]);
  while(1);
}
```

- Value print on screen:

```text
MSP value: 0x2001ffc8
R0 value: 20000000
R1 value: 20000064
R2 value: ffffffff
R3 value: ffffffff
R12 value: 0
LR value: 80002d5
PC value: fffffffe
XPSR value: 61000000
```

- Test divide by zeeo:

```C
  /* 1. Enable divide by zero trap. */
  uint32_t *pCCR = (uint32_t *)0xE000ED14; // Configuration and Control Register.
  *pCCR |= (1 << 4);

  int x = 0;
  int y = 5/x;
```

- Summary error reporting when fault happens:
  - Implement the handler which takes some remedial actions.
  - Implement a user call back to report errors.
  - Reset the micro-controller/processor.
  - For an OS environment, the task that triggered the fault can be terminated and restarted.
  - Report the fault status register and fault address register values.
  - Report additional information of stack frame through debug interface such as printf.
