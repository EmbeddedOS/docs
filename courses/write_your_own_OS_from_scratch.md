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

- See memory layout on the disk image with `hexdump boot.img`

```bash
0000000 c031 d88e c08e d08e 00bc b47c b013 bb01
0000010 000a d231 1fbd b97c 000b 10cd ebf4 48fd
0000020 6c65 6f6c 7720 726f 646c 0000 0000 0000
0000030 0000 0000 0000 0000 0000 0000 0000 0000
*
00001b0 0000 0000 0000 0000 0000 0000 0000 0080
00001c0 0002 fff0 ffff 0001 0000 4ebf 0000 0000
00001d0 0000 0000 0000 0000 0000 0000 0000 0000
*
00001f0 0000 0000 0000 0000 0000 0000 0000 aa55
0000200 0000 0000 0000 0000 0000 0000 0000 0000
*
09d8000
```

### 13. Test the disk extension service

- We need to load our kernel from disk to the memory and jump to the kernel. We know that we use BIOS disk services to load our file from disk in boot process. When we read file from disk, we should provide `CHS` value in order to locate the sector we want to read.

- To make our boot file simple, we choose **logical block address** which disk extension service use to access the disk.

- Modern computers should support the extension service, but we will check it anyway.

- To check for this service is really simple. First off, we define a label to called `TestDiskExtension`. The parameters we should pass includes:

```assembly
TestDiskExtension:
        mov ah,0x41
        mov bx, 0x55AA
```

- The `dl` holds the drive id when BIOS transfers control to our boot code. Since we will call disk service more than once later in the boot process, and we need to pass the drive id to `dl` register before we call the disk service, so we define a variable called `DriveID` and save the value of `dl` to variable `DriveID`. Because we want to write the value into the memory location the `DriveID` represents, so we use the *square brackets* to access the location.

```assembly
TestDiskExtension:
        mov [DriveID], dl
        mov ah,0x41
        mov bx,0x55AA
        int 0x13
        jc NotSupport
        cmp bx,0xaa55
        jne NotSupport

; ...

DriveID:  db 0

NotSupport:
End:
  hlt
  jmp End
```

- Then we can call `int 0x13`, if the services is not supported, the carry flag is set. Se we use `jc` instruction which will jump to label not support if carry flag is set.

- If it passed the test, we compared `bx` with the value ``0xaa55`.

## 4. Loading the loader and switching to long mode

### 14. Loader

- Loader
  - Loader retrieves information about hardware.
  - Prepare for 64-bit mode and switch to it.
  - Loader loads kernel in main memory.
  - Jump to kernel.

- After disk extension check is passed, we load a new file called `loader` in the memory. The reason we need a loader file is that the MBR is fixed size which is `512` bytes.

- There are spaces reserved for other use such as partition entries, entries which leaves us less than 512 bytes for the boot code.

- The tasks that we should do in the boot process includes load kernel file, get memory map, switch to protected mode and then jump to long mode. Doing all these tasks requires the boot code larger than 512 bytes. -> SO we need a new file: loader file to do all these things. The loader file has no 512 bytes limits.

- Memory map when we load the loader file:
      Memory
    |         | Max size
    |         |
    | Loader  | 0x7e00
    | MBR code| 0x7c00
    |         |
    |         |
    |         | 0

  - The boot code is loaded by BIOS in the memory address `0x7c00`. The size of the boot code is 512 bytes which is `0x200` in hexadecimal. So here we simply load the loader file into the location right after the boot code. which is at the location `0x7e00`.

- We will make a label `LoadLoader`:

```assembly
LoadLoader:
        mov dl,[DriveID]
        mov ah,0x42
        int 0x13
```

- The disk service we use is interrupt `0x13` the function code is `0x42` is saved in `ah` register, which means we want to use disk extension service. And also, don't forget to save the `driveID` to `dl` register before we call the service.

- The parameter we pass to the service is actually a structure. We call this structure as `ReadPacket` and the size of the structure is 16 bytes. We need to `mov` the address of read packet to `si` register, now si holds the address of read packet.

```assembly
ReadPacket: times 16 db 0
```

```assembly
LoadLoader:
        mov si,ReadPacket
        mov dl,[DriveID]
        mov ah,0x42
        int 0x13
```

- `ReadPacket` structure holds:
  offset    field
  0         size
  2         number of sectors
  4         offset
  6         segment
  8         address lo
  12        address hi

- The first word holds the value of structure length. So we mov `0x10` to it.
- The second word is the number of sectors we want to read. Because our OS is small file, we simply read 5 sectors, which enough space for the loader.

- The next two words specify the memory location into which we want to read our file. We load the loader file into the memory address `0x7e00`, so we save `0x7e00` in the first word which is the offset. The second word holds the value of segment part of the address. We simply set it to `0`. So the logical address is `0x07e00` and the physical address it points to is `0x7e00`.

- The last two words are the 64-bit logical block address. The loader file will be written into the second sector of the disk. Therefore, we use **lba 1**. Remember logical block address is zero-based address. Meaning that first sector is sector 0, the second sector is sector 1 is so on. So we save 1 to the lower half of the 64-bit address.

- Now with all the parameters prepared, we call interrupt `0x13`:

```assembly
LoadLoader:
        mov si,ReadPacket
        mov word[si],0x10
        mov word[si+2],0x5
        mov word[si+4],0x7e00
        mov word[si+6],0x0
        mov dword[si+8],0x1
        mov dword[si+12],0x0

        mov dl,[DriveID]
        mov ah,0x42
        int 0x13
        jc ReadError

        mov dl,[DriveID]
        jmp 0x7e00
```

- If it fails to read sectors, the carry flag is set. So we use `jc` instruction and jump to label `ReadError`.
- And don't forget to pass the `driveID` to the loader, because we need to loader kernel using `driveID`. And finally jump to address `0x7e00`.

- Our loader file `loader.asm`:

```assembly
start:
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

Message:        db "Loader is running."
MessageLen:     equ $-Message
```

- The loader is supposed to be running at address `0x7e00`, then we define the label `start` of the loader.
- Also, we need to edit the build system:

```Makefile
all:
        nasm -f bin -o boot.bin boot.asm
        nasm -f bin -o loader.bin loader.asm
        dd if=boot.bin of=boot.img bs=512 count=1 conv=notrunc
        dd if=loader.bin of=boot.img bs=512 count=5 seek=1 conv=notrunc
clean:
        rm *.bin
```

- We specify `count=5` to write 5 sectors of data into boot image.
- The `seek=1` indicates that we will skip the first sector.

### 15. Long mode support

- The modern processors should support long mode. Since checking it is really simple, another check we need to perform is 1GB page. Our memory module allocates pages this feature. So our kernel requires the processor support 1GB page.

- First of all, we are going to do is we are going to save the `driveID` for later use. We are save it to `dl` late.

- To check long mode, there is a special instruction called `cpuid` which returns processor identification and feature information. We pass the input value `0x80000001` to `eax` and execute `cpuid`.
  - First, we need to check cpu support input value or not by passing `0x80000000` to eax and execute `cpuid`. If return value less than `0x80000001`, cpu does not support input value.
  - By passing different number to `eax`, we will get different information about processor returned by `cpuid`.
  - Pass the `0x80000001` to `eax` will return processor features.
    - The information about long mode support is saved in `edx`. We test bit 29 in `edx` register. If it is set, it means that long mode is supported. Otherwise long mode is not available and we jump to the label not support.

  - We also check `1GB` page support is at bit 26, so we test `edx` and the bit 26.

    ```assembly
    start:
            mov [DriveID],dl
            mov eax,0x80000000
            cpuid
            cmp eax,0x80000001
            jb NotSupport
            mov eax,0x80000001
            cpuid
            test edx,(1<<29)
            jz NotSupport
            test edx,(1<<26)
            jz NotSupport
    ```

- NOTE: when you run the OS image with qemu, please specify cpu model which support `1gb page`, for example:

```bash
qemu-system-x86_64 -cpu qemu64,pdpe1gb -hda boot.img
```

### 16. Load Kernel file

- After long mode is check is passed, what are going to do next, we are going to load our kernel file. As we have loaded the loader file, loading kernel is pretty much the same except that there are several parameters need to change.

- First off, we take a look at memory map and decide where we load the kernel file into memory.

- Memory map:
      Memory Map
    |           | Max size
    |   Free    |
    |-----------| 0x100000
    | Reserved  |
    |-----------| 0x80000
    |   Free    |
    |           |
    |-----------|
    |  Loader   |
    |-----------| 0x7e00
    |   Boot    |
    |-----------| 0x7c00
    |   Free    |
    |-----------|
    | BIOS data |
    |   vectors |
    |-----------| 0

  - At the bottom, we have the interrupt vectors and bios data.
  - then small area of free memory.
  - At address `0x7c00` our boot code is loaded by the BIOS.
  - And the boot code load the loader file at address `0x7e00`.
  - Then we get a relatively large area of free memory we can use. Our kernel can be loaded here.
  - The area above it is reserved for hardware and bios data.
  - Starting at `0x100000` which is `1MB` we have free memory again.

- NOTE: Memory map may vary among different computers and also gets some space reserved for hardware which is not available for us.
  - Therefore, we need to get a memory map by calling the BIOS service so that we know which part of memory is available to use.

- WITH THIS IN MIND: we choose to load the kernel at address `0x10000` and when we jump to `64-bit mode`, we copy the kernel into the memory above `1MB`.

- Memory map when we load the kernel:
      Memory Map
    |           | Max size
    |   Free    |
    |-----------| 0x100000
    | Reserved  |
    |-----------| 0x80000
    |   KERNEL  |
    |-----------| 0x10000
    |   Free    |
    |-----------|
    |  Loader   |
    |-----------| 0x7e00
    |   Boot    |
    |-----------| 0x7c00
    |   Free    |
    |-----------|
    | BIOS data |
    |   vectors |
    |-----------| 0

- To load the kernel into `0x10000`, we will load 100 sectors of data roughly 50KB which is enough for our kernel.
- The memory address we choose to load the kernel at is `0x10000`.
- NOTE: We cannot assign `0x10000` to a word variable, because `0x10000` is larger than what a word or two bytes can represent. We will get an overflow and result is not correct if we do it.
  - Instead, we assign `0x1000` to segment part of the address and leave offset 0. the physical address we get is `0x1000*16 + 0` equals `0x10000`.
- The boot file resides in the first sector. The loader file occupies the next file sectors. So we will write our kernel from seventh sector.

    ```assembly
    LoadKernel:
            mov si,ReadPacket
            mov word[si],0x10
            mov word[si+2],0x64
            mov word[si+4],0x0
            mov word[si+6],0x1000
            mov dword[si+8],0x6
            mov dword[si+12],0x0

            mov dl,[DriveID]
            mov ah,0x42
            int 0x13
            jc ReadError
    ```

### 17. Get Memory map

- By using the BIOS function, we can get the memory map. The service we use is called `system map service` by interrupt `0x15`, it returns a list of memory blocks to us. Each block us 20 bytes.

- Structure of blocks:
    Offset    field
    0         base address
    8         length
    16        type

  - The first `qword` is 64-bit physical start address of the memory region.
  - The second `qword` is length in bytes of the region.
  - The last dword indicates the type of memory with:
    - 1 being free memory which we can use.
    - 2 being not available to us.
  - We only collect memory region of type 1. After we get the free memory information, the memory module will use the information to allocate memory for our system.

```assembly
GetMemoryInfoStart:
        mov eax,0xe820
        mov edx,0x534d4150
        mov ecx,0x14
        mov edi,0x9000
        xor ebx,ebx
        int 0x15
        jc NotSupport
```

- We pass `0xe820` to `eax` and the ascii code for `smap` to `edx`.
- Then we save 20 which is the length of memory block to `ecx`.
- Next save the memory address in which we saved the memory block returned in register `edi`.
- And clear `ebx` before we call the function.
- We can call the service through interrupt `0x15`.
- If it returns the memory successfully, we continue to retrieve the memory info.

```assembly
GetMemoryInfoStart:
        mov eax,0xe820
        mov edx,0x534d4150
        mov ecx,0x14
        mov edi,0x9000
        xor ebx,ebx
        int 0x15
        jc NotSupport

GetMemoryInfo:
        add edi,0x14
        mov eax,0xe820
        mov edx,0x534d4150
        mov ecx,0x14
        int 0x15
        jc GetMemoryDone

        test ebx,ebx
        jnz GetMemoryInfo

GetMemoryDone:
```

- First off, we adjust `edi` to point to the next memory address to receive the next memory block. Each memory block is 20 bytes, we add 20 to `edi`.
- Then we pass the same parameters to `eax`, `ecx`, `edx`.
- NOTE: `ebx` must be preserved for the next call of the function. So we don't change `ebx` value.
- Call the service again.
- If carry flag is not set, we test `ebx`, if `ebx` is nonzero, we jump back to label `GetMemoryInfo`.

### 18. Test A20 line

- The A20 line was introduced from old machine. Back in the day when machines had 20-bit address, we can address memory of 2 to the power of 20 which is 1MB.

- Later machines come with address bus wider than 20 bits. For compatibility purposes, A20 line of the address bus is off. So when the address is higher than 1MB, the address will get truncated as if it starts from 0 again.

- Our system runs on 64-bit processor which has address wider than 20 bits.

- If we try to access memory with A20 line disabled, we will end up only accessing even megabytes, because A20 line is `0` and any address we are gonna access is actually the address with 20 bit cleared. Therefore we need to **TOGGLE** A20 line to access all the memory.

- There are different ways to activate A20 line and each method is not guaranteed to be working across all the different machines. But newer machines seem to have A20 line enabled by default.

- To make our project simple, We assume our machines enabled A20 line. What we are gonna do here is we are going to test to see if A20 line is already enabled.

- The logic for testing A20 line in our project is simple. If A20 line is not enabled, when we try to access address, for example, `0x107C00`. If we look at the value in binary format, u can see bit 20 is `1`. Suppose if A20 line is off, then bit 20 will be `0`.

  - Bit layout look like:
                20                       0
  0x107C00 ---> 1 0000 0111 1100 0000 0000 -> A20 is on.
                0 0000 0111 1100 0000 0000 -> A20 is off.

  - So, if A20 is off, the address we referenced in this case is actually `7c00`.

  - NOTE: the reason we choose this address is that `7c00` is the start address of the boot code. Remember our boot code is loaded at `7xc00`. Since the boot code has done its work, we can reuse that memory area to do the test.

```assembly
Test20A:
        mov ax,0xffff
        mov es,ax
        mov word[ds:0x7c00],0xa200 ; 0:0x7c00 = 0 * 16 + 0x7c00 = 0x7c00
        cmp word[es:0x7c10],0xa200 ; 0xfff:0x7c10 = 0xffff * 16 + 0x7c10 = 0x107c00
        jne SetA20LineDone
```

- In order to access memory `0x107C00`, we move `ax` with `0xffff` and copy it to `es` register.
- Next we write a random value, for example, `0xa200` to address `0x7c00`. We will use the `ds` register as a segment register which has been set to `0` by us. So the logical address now points to `0x7c00`. This instruction will copy `0xa200` at the memory `0x7c00`.
- Then we are gonna compare the content at address `0x107c00` with `0xa200`. So the segment register we use is `es` register. The physical address points to `0x107c00` which is the address we want.

- If the content at address `0x107c00` not equal to `0xa200`, we know that we have accessed the memory address `0x107c00` successfully and we jump to set A20 line done.
- IF the content at address `0x107c00` is equal to `a200`, it means there is high chance that the address `0x107c00` gets truncated to `0x7c00`, we actually access the same memory location. But maybe content at `0x107c00` happens to be the same value `A200`.
- So we do the second test. We write random value, for example, `0xb200` to `0x7c00` and then compare the content at `0x107c00` with `0xb200`. If they are still the same, it means that A20 line is not enabled and we jump to the end.

```assembly
TestA20lLine:
        mov ax,0xffff
        mov es,ax
        mov word[ds:0x7c00],0xa200
        cmp word[es:0x7c10],0xa200
        jne SetA20LineDone
        mov word[0x7c00],0xb200
        cmp word[es:0x7c10],0xb200
        je NotSupport

SetA20LineDone:
        xor ax,ax
        mov es,ax
        ; ...
```

- And don't forget change back es register to `0`.
