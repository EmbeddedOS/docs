# Getting to know the Linux Kernel: A beginner's Guide

- <https://www.youtube.com/watch?v=QatE61Ynwrw&list=WL&index=2&t=2149s>

## What is the Linux Kernel

- Core component of the Linux OS.
- Manages hardware and software interactions.
- Enable multitasking and resource management.
- Created by Linus Torvalds in 1991.
- Open-source with thousands of contributors.

## Linux Kernel subsystems and structure

- Process management.
- Memory management.
- Virtual File System.
- Networking stack.
- Device drivers.
- Arch specific logic.

## Modifying the Linux Kernel

- Modify Kernel Configs:
  - Enable/disable features.
  - Optimize kernel performance and size.
  - Add support for new hardware.
  - Enable/disable Debugging options.
  - Apply security features and settings.
- `make menuconfig` text-based interface for configuring kernel options.
- `make xconfig` graphic interface for configuring kernel options.
- `make oldconfig` update existing `.config` file newer kernel version.
- `make defconfig` generate recommended default `.config` for your system.
- Configure manually - Edit the `.config` file directly (not recommended).

## Modify Kernel code

- Implement new features.
- Develop custom drivers.
- Custom security or access control.
- Fix bugs or security vulnerabilities.
- Optimize for specific hardware.
- Debugging.

## Build and install your kernel

- <https://kernelnewbies.org/KernelBuild>
- Take your current running kernel config:

```bash
cp /boot/config-`uname -r`* .config
```

## Testing and Debugging the Linux Kernel

- Manual testing:
  - printk.
  - dmesg.
- Automated testing:
  - Linux Test Project (LTP): <https://github.com/linux-test-project/ltp>
  - Linux Kernel selftests (kselftests): part of the Linux kernel.
- Static analysis tools.
- Dynamic analysis tools.
