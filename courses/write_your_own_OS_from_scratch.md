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

- Create a hard disk image:

```bash
bximage
1
hd
flat
512
10
boot.img
```

- Configure the image file with command:

```bash
bochs
3

6

# 1G pages support in long mode.
26
```
