# Embedded Linux using Yocto

## I. Introduction

### 1. Elements of Embedded Linux

- 4 elements of embedded Linux:
  - 1. **Toolchain**: The compiler and other tools need to create code for your target device. Everything else depends on the toolchain.
  - 2. **Boot-loader**: The program that initializes the board and loads the Linux Kernel.
  - 3. **Kernel**: This is the heart of the system, managing system resources and interfacing with hardware.
  - 4. **Root file system**: Contains the libraries and programs that are run once the kernel has completed its initialization.

- One more element can be collection of programs specific to your embedded application which make the device do what ever it is supposed to do, be it weight groceries, display movies, control a robot, or fly a drone.

### 2. What is Yocto?

- Non-technical definition: Yocto is the smallest SI metric system prefix.
  - Like `m` standard for milli (10^-3).
  - So is Yocto (y=10^-24)

- What is Yocto project?
  - Yocto project provides open source, high quality infrastructure and tools to help developers create their own custom Linux distribution for any hardware architecture.
- History:
  - Founded in 2010, in an effort to reduce work duplication, provide resources and information catering to both new and experienced users.
  - Collaboration of:
    - 1. Many hardware manufactures.
    - 2. Open source OSes vendors and electronics companies.
  - Yocto is also a project working group of the Linux Foundation.

### 3. Input and Output of Yocto Project

- To understand the outcome provided by the Yocto Project Project, we can use the analogy of the computing machine.
  - Input: Set of data that describes what we want, that is our specification (Kernel configuration, Hardware Name, Packages/Binaries to be installed).
  - Output: Linux Based Embedded Product (Linux Kernel, Root FS, Boot-loader, Device Tree, Toolchain).

### 4. Setting up build machine

- Prerequisites:
  - 1. 50 GB of free disk space.
  - 2. Runs a supported Linux Distribution (i.e recent releases of Fedora, openSUSE, CentOS, Debian, or Ubuntu).
  - 3. Git, tar, Python.
- Packages and package installation vary depending on your development system. Install the required packages for Yocto to Work from: [ubuntu-packages](https://docs.yoctoproject.org/ref-manual/system-requirements.html#ubuntu-and-debian)

```bash
sudo apt install gawk wget git diffstat unzip texinfo gcc build-essential chrpath socat cpio python3 python3-pip python3-pexpect xz-utils debianutils iputils-ping python3-git python3-jinja2 python3-subunit zstd liblz4-tool file locales libacl1
sudo locale-gen en_US.UTF-8
```

### 5. What is Poky?

- Poky is a reference distribution of Yocto Project. The work `reference` is used to mean `example` in this context.
- Yocto project uses Poky to build images (kernel, system, and application software) for targeted hardware.

- At the technical level it is a combined repository of the components:
  - Bitbake.
  - OpenEmbedded Core.
  - meta-yocto-bsp.
  - Documentation.

- Note: Poky does not contain binary files, it is a working example of how to build your own custom Linux distribution from the source.

- What is difference between poky and Yocto?
  - The exact difference between Yocto and Poky is Yocto refers to the organization (like one would refer to `Canonical` the company behind Ubuntu) and Poky refers to the actual bits downloaded (analogous to `Ubuntu`).

### 6. What is Metadata?

- Non Yocto: A set of data that describes and gives information about other data.

- Yocto World:
  - Metadata refers to the build instructions.
  - Commands and data used to indicate what versions of software are used.
  - Where they are obtained from.
  - Changes or additions to the software itself (patches) which are used to fix bugs or customize the software for use in a particular situation.

- Metadata is collection of:
  - 1. Configuration files (.conf).
  - 2. Recipes (.bb and .bbappend).
  - 3. Classes (.bbclass).
  - 4. Includes (.inc).

### 7. What is oe-core?

- Open-Embedded Project.
- From [OpenEmbedded](https://www.openembedded.org/wiki/Main_Page).

- OpenEmbedded offers a best-in-class cross-compile environment. It allows developers to create a complete Linux Distribution for embedded systems.

- What is the difference between OpenEmbedded and the Yocto Project?
  - 1. The Yocto Project and OpenEmbedded share a core collection of metadata called openembedded-core.
  - 2. However, the two organizations remain separate, each with its own focus.
  - 3. **OpenEmbedded** provides a comprehensive set of metadata for a wide variety of architectures, features, and applications.
  - 4. Not a reference distribution.
  - 5. Designed to be the foundation for others.
  - 6. The Yocto Project focuses on providing powerful, easy-to-use, interoperable, well-tested tools, metadata, and Board Support Packages (BSPs) for a core set of architectures and specific boards.

- The Yocto Project and OpenEmbedded have agreed to work together and share a common core set of metadata (recipes, classes and associated files): oe-core.

### 8. What is bitbake?

- Bitbake is a core component of the Yocto Project.
- It basically performs the same functionality as of `make`.
- It's a task scheduler that parses python and shell script mixed code.
- The code parsed generates and run tasks, which are basically a set of steps ordered according to code's dependencies.
- It reads recipes and follows them by fetching packages, building them and incorporating the result into boot-able images.
- It keeps track of all tasks being processed in order to ensure completion, maximizing the use of processing resources to reduce build time and being predictable.

### 9. `meta-yocto-bsp`

- A Board Support Package (BSP) is a collection of information that defines how to support a particular hardware device, set of devices, or hardware platform.

- The BSP includes information about the hardware features present on the device and kernel configuration information along with any additional hardware drivers required.
- The BSP also lists any additional software components required in addition to a generic Linux Software stack for both essential and optional platform features.
- The `meta-yocto-bsp` layer in Poky maintains several BSPs such as the Beaglebone, EdgeRouter, and generic versions of both 32-bit and 64-bit IA machines.

- Machines supported:
  - Texas Instruments BeagleBone (beaglebone).
  - Free-scale MPC8315E-RDB (mpc8315e-rdb).
  - Intel x86-based PCs and devices (generic x86 and generic x86-64).
  - Ubiquiti Networks EdgeRouter Lite (edgerouter).

- NOTE: To develop on different hardware, you will need to complement Poky with hardware-specific Yocto layers.

### 10. Other Poky Repositories

- `meta-poky`, which is Poky-specific metadata.
- Documentation, which contains the Yocto Project source files used to make the set of user manuals.

- Conclusion, Poky includes:
  - Some OE components (oe-core).
  - bitbake.
  - demo-BSP's.
  - Helper scripts to setup environment.
  - Emulator QEMU to test the image.

```text
 _______________________________________________________________________
|                         Yocto Project (YP)                            |
|  _____________________________________________                        |
| |                   PoKy                      | YP-Compatible BSPs    |
| |  ___________________________  meta-poky     | (meta-intel, ti,etc)  |
| | |                           | meta-yocto-bsp| YP-Compatible Layers  |
| | |OpenEmbeddedCore(metadata) | Document      | YP Auto-builder (QA)  |
| | |Bitbake Build Engine       |               | Development tools     |
| | |                           |               | Other layers, etc.    |
| |_|                           |_______________| Pseudo, Document.     |
|   |                           |                                       |
|___|                           |_______________________________________|
    |                           |
    |                           |
    |                           |
    |  meta-openembedded        |
    |                           |
    |_______OpenEmbedded________|
```

### 11. Hello World of Poky

- Workflow of Yocto Project:
  - Step 1: Download the Poky Source Code:
    - `git clone git://git.yoctoproject.org/poky`
  - Step 2: Checkout the latest branch/release (zeus):
    - `git checkout zeus`
  - Step 3: Prepare the build environment:
    - Poky provides you a script `oe-init-build-env`, which should be used to setup the build environment.
