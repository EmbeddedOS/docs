# Write your own 64-bit OS

## 1. Introduction

### 3. OS concepts

- Address space:
  - Address space that a process can use to address memory.
  - 64-bit address.
  - byte addressing.
  - 0 -> 2^64-1 (byte).
- Process:
  - Layout:

    Kernel (Code, data)
        Stack           | Max size
        Heap            |
        Data            |
    Instructions        | 0

  - Each process has its `address space` which process can read and write.
  - Process also contains other resource such as registers, a list of open file
  - Process likes the container that holding all information in order to run the program.
  - Typically the address is from 0 to maxsize which can be defined by the OS. The OS kernel resides in memory above the maxsize.
  - When we have multiple processes running on the computer, what happens is that each process has its own address space. The program instructions, data, stack, are different among processes. The memory reference within one process does not affect the address space of other processes. However, the kernel which resides in the higher part of address space are the same across all the process.

- Operation Modes:
    Kernel (Code, data) | Kernel mode
                            ||
                        system call
                            \/
        Stack           | User mode
        Heap            |
        Data            |
    Instructions        |

- The system call acts as an interface between US and the OS. When a program calls a system call, the control is transferred to the OS. The OS will perform the operation the program needs and return to the user program once it is done.

### 4. OS structure

- Two popular structures:
  - Monolithic System.
  - Micro-kernel.

## 2. Installation and Setup

### 7. Install on linux

- Setup Linux host x86-64 bit

```bash
apt install gcc nasm

# To create the image file
apt install bochs
```

## 3. Boot up

### 9. The first program

- BIOS: Basic Input Output System
  - Is usually stored on a small chip on the motherboard.
  - It initializes hardware and provides runtime services for OS.
- BIOS functions:
  - Services run in real mode.
- BIOS services:
  - Print characters.
  - Disk services.
  - Memory map.
  - Video mode.
  - These features will become unavailable when we switch to protected mode and long mode.

- When we boot our computer, BIOS initializes and tests the hardware, then find the boot device, read the first sector.

- Boot:
  - Power On Self Test (POST), initialize devices.
  - Find boot device and read the first sector it into memory location 0x7c00 and control is transferred to it.
    - The information on the first sector is also known as **Master Boot Record**.
    - When BIOS reads it into the memory, our code will be executed.
  - Jump to 0x7c00.
      Memory
    |         | Max size
    |         |
    | MBR code| 0x7c00
    |         |
    |         | 0

- When booting our OS, we will go through 3 different modes: real mode -> protected mode -> Long mode. When our boot code executes, we are in real mode.
- Then we enter the protected mode for preparing for the long mode. Finally, we will switch to long mode.

- Long mode consists of two sub modes, `64-bit mode` and `compatibility mode`. In compatibility mode, we can run 16-bit or 32 bit programs under 64-bit OS without recompiling those programs.

- UEFI:
  - Modern computers also come with UEFI (Unified Extensible Firmware Interface) which is expected to replace BIOS.

- Real mode: what we do in real mode includes loading kernel in memory, and get information about hardware. Then we will switch to long mode and the OS will be running in 64-bit mode.
  - Load Kernel.
  - Retrieve information about hardware.

- Memory addressing and how memory address is formed in real mode:
  - There are 4 special registers called segment registers which are 16 bits registers. Actually, there are more than four segment registers we can use, but, we only use these registers to write simple OS.
  - CS: code segment.
  - DS: data segment.
  - ES: extra segment.
  - SS: stack segment.

- Address format:
  - Segment register: offset (logical address)
  - (Segment register * 16) + offset = physical address
    - We shift the value in segment register left by 4 bits (equal multiple the value by 16) and then add the offset to the result which produces 20-bit physical address
  - For example:

      ```assembly
      mov ax, [ds:50h]  ; With ds = 0x1000
      ; 0x1000* 116 + 0x50 = 0x10050
      ; This instruction will copy the data in address at 0x10050 to register ax.

      pop bx            ; assume: ss = 0x100, sp = 0x500

      ; When we pop the value on the stack into register bx, the address of value we pop is: 0x100 * 16 + 0x500
      ; So the address of the memory we actually reference is 0x1500.
      ```

- General purpose registers:
  - 8 bit:    al    ah    bl    bh
  - 16 bit:   ax    bx    cx    dx
  - 32 bit:   eax   ebx   ecx   edx
  - 64 bit:   rax   rbx   rcx   rdx are not available

- Simple boot program:

```assembly
[BITS 16]
[ORG 0x7c00]

start:
        xor ax,ax
        mov ds,ax
        mov es,ax
        mov ss,ax
        mov sp,0x7c00

PrintMessage:
        mov ah,0x13
        mov al,1
        mov bx,0xa
        xor dx,dx
        mov bp,Message
        mov cx,MessageLen
        int 0x10

End:
        hlt
        jmp End

Message:        db "Hello world"
MessageLen:     equ $-Message

times (0x1be-($-$$)) db 0
        db 80h
        db 0,2,0
        db 0f0h
        db 0ffh,0ffh,0ffh
        dd 1
        dd (20*16*63-1)

        times (16*3) db 0

        db 0x55
        db 0xaa
```

- The first part consists of two directive: `[BITS 16]`, `[ORG 0x7c00]` to tell the assembler that our boot code is running on the 16-bit mode.
  - The `ORG` directive indicates that the code is supposed to be running at memory address `0x7c00`.
  - Remember BIOS loads the boot code from the first sector into memory address `0x7c00` and transfer the control to it. If we don't specific `0x7c00` in this case, the memory reference within our code will be incorrect when the program is running.

- In the next part, this block code after `start` label, we will initialize the segment registers to zero. So, we will not concern about the value in these registers when we reference memory address later in the code. the offset is actually the memory address we want to access because segment registers are 0 now.
  - We also need to assign `0x7c00` to `sp` register, here is the memory map after we initialize segment registers and stack pointer:
        Memory
      |         |
      |Boot code|
      |---------| 0x7c00
      | Stack   |
      |         | 0
    - The boot code start at `0x7c00` and the stack is setup below `0x7c00`. Because stack *grows downwards*, when we push a value into stack, the stack pointer will point to 2 bytes below `0x7c00` because the default operand size for push and pop instructions is 2 bytes in 16-bit mode.
        Memory
      |         |
      |Boot code|
      |---------| 0x7c00
      |         | <--sp (0x7bfe)
      | Stack   |
      |         | 0
      - At we push more data on the stack, `sp` register will *decrement*.

- The next part, print messages on the screen so that we can see our system is running. Printing characters is done by calling BIOS service. The BIOS services can be accessed with BIOS interrupts.
  - Before we call a BIOS function, we need to pass parameters:
    - `ah`: register holds the function code, we are using `13` which means print string.
    - `al`: register specifies the write mode, we set it to `1`, meaning that the cursor will be placed at the end of the string.
    - `bx`: `bh` register is the higher part of `bx` register represents page number. `bl` register, the lower part of `bx` holds the information of character attributes. We are writing `0xa` to the `bx` register, means we want to display characters with bright screen.
    - `dx`: clear the `dx` register to display bright screen.
    - `bp`: holds the address of the string we want to print.
      - If we want to copy the data only, square the message: `mov bp,Message`, by the way, we'll copy the data in address of message into the register.
    - `cx`: In the `cx` register we can specify number of characters to print.

- The `$` in `equ $-Message` means the current assembly position which is the end of the message in this case. Minus the address of the message, the result is the size of string or the number of characters.

- SO all the parameters initialized. Then, we see the message printed on screen once we execute `int` instruction.
- `hlt` instruction places the processor in a **halt state**.

- In final part, we can see directive `times` repeats command the specific times. `times (0x1be-($-$$)) db 0` here is the expression specifies HOW MANY TIMES `db` is repeated.
  - The double dollar `$$` means the beginning of the current section.
  - The dollar `$` minus double dollar `$$` represents the size from the *start of the code* to *the end of the message*.
  - Then we subtract it from `0x1be`. The memory map look likes:
            Memory
      |                 | $$ (the start of code)
      |                 |
      |-----------------| $ (The end of the message)
      |This region is   |
      |filled with 0    | 0x1be

- So what is in the offset `0x1be`? In offset `0x1be`, we have what it's called **partition entries**.
  - There are four entries with each entry being 16 bytes in size.
  - We need to define first entry:

    ```assembly
    db 80h
    db 0,2,0
    db 0f0h
    db 0ffh,0ffh,0ffh
    dd 1
    dd (20*16*63-1)
    ```

  - And other entries are simply set to `0`: `times (16*3) db 0`.

  - Format of partition entry:

    ```assembly
    db 80h              ; First byte is `boot indicator`.
    db 0,2,0            ; Starting CHS.
    db 0f0h             ; Type.
    db 0ffh,0ffh,0ffh   ; Ending CHS.
    dd 1                ; Starting sector.
    dd (20*16*63-1)     ; Size.
    ```

    - First byte is `boot indicator`, simply set `0x80` meaning that this is a boot-able partition.
    - The next three byes construct a starting `c`, `h`, `s` value.
      - `c` stands for cylinder.
      - `h` head.
      - `s` sector.
      - The first byte represents head value.
      - The second byte is divided into two parts. Bits 0-5 is used as sector value, 6-7 is used as cylinder value.
      - The last bytes holds the lower 8 bits of cylinder value.
      - The head and cylinder values start from 0, so cylinder 0 is the first cylinder.
      - Sector values start from 1 meaning that sector 1 is the first sector.
    - The fourth byte is partition byte, we set to `0f0h`.
    - The next three bytes construct the ending `c`, `h`, `s` value. Here we simply set them to `0xff`. `0xff` is the max value we can set in a byte.
    - The next double word represents **LBA** (Logical block address) address of starting sector. We will load our file using LBA instead of `c`, `h`, `s` value.
    - The last double words specifies how many sectors the partition has.

- NOTE: The value in the partition entry does not reflect the real partition, The reason we define this entry is that some BIOS will try to find the valid looking partition entries. If it is not found, the BIOS will treat usb flash drive as floppy disk. We want BIOS to boot the usb flash drive as hard disk. So we add the partition entry to construct a seemingly valid entries.

- The last two bytes of our boot file is signature which should be `0x55AA`.
- The size of boot file is 512 bytes, So sector size is assumed to be 512, when we write the boot file into the boot sector, it gets the same data as boot file.

- Then, we need to write some script files to do this. First, assembling the boot file.

```bash
nasm -f bin -o boot.bin boot.asm
```

- Next, we are going to write the binary file into the boot image to make it **BOOT-ABLE**.

```bash
dd if=boot.bin of=boot.img bs=512 count=1 conv=notrunc
```
