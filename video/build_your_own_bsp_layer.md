# Build your own Yocto BSP Layer

[Youtube](https://www.youtube.com/watch?v=PCbGLSEy4JA&t=3092s)

Before create a board support package (BSP) Layer, you need:

- A Yocto project.
- A development board.
  - We are not using BBB, because it's so popular, what we choose, raspberry Pi, Jetson nano, etc.

## What is the BSP?

- The Board Support Package os the tools that will boot a board.
- It consists of tools like:
  - A bootloader, e.g.
    - u-boot.
    - Grub.
  - Kernel.
  - Kernel configuration.
    - `.config` and possibly the Device Tree files.
  - Hardware initialization.
    - Serial.
    - PCI.
    - BUS.

## Standard BSPs in a Yocto Project

- The meta layer in Poky contains BSP recipes:
  - `poky/meta/recipes-bsp`
- Providing Boot, kernel and configuration options of QEMU images.
- For generic x86, ARM and beagleBone Black include:
  - `poky/meta-yocto-bsp`
- Extends Kernel support for these boards.

## What is in a BSP layer?

- Machine Configuration.
- Bootloader modifications to boot up your board.
  - u-boot.
  - GRUB.
- Kernel Modifications: device tree -> recipe appends. ex: wifi, ble.
- General Feature support.

## For example: Add support for a raspberry Pi

- Download the Raspberry Pi BSP Layer: [Link](git://git.yoctoproject.org/meta-raspberrypi), [Link](https://layers.openembedded.org/layerindex/branch/master/layer/meta-raspberrypi/)
- Read the documentation.
- Add the layer to your `bblayers.conf`.
- Change your `MACHINE` in `local.conf` to a supported machine.
- Build your Image.

## How to find supported MACHINEs

1. Read the documentation.
2. Look at the `conf/machine` folder of the layer, this is where the machine definition files are stored. For example: `layers/third-party/meta-raspberrypi/conf/machine/`.

## Machine configuration

- Defines the architecture:
  - e.g. `DEFAULTTUNE ?= "cortexa7thf-neon-vfpv4"`
- Sets Features:
  - e.g. `MACHINE_FEATURES += "pci"`
- Defines the DTB:
  - e.g. `RPI_KERNEL_DEVICETREE = "broadcom/bcm2712-rpi-5-b.dtb"`
- Define configurations:
  - e.g. `UBOOT_MACHINE = "odroid-n2_defconfig"`
- etc.

## Bootloader configurations

- The recipes-bsp directory contains additional configuration.
- Often includes u-boot configuration fragments (.cfg files). Separate your configurations to multiple files, overlay the default, so you don't have to make too many instance of configs.
- e.g. For Odroid N2+ boards: `meta-meson/recipes-bsp/u-boot/u-boot/fit.cfg`. Only contains configs that change the default configurations.

```text
CONFIG_FIT=y
CONFIG_FIT_SIGNATURE=y
CONFIG_VERBOSE=y
```

- We can overlay the default configuration as well.

## Kernel configuration

- Configuration is done in `meta-layer/recipes-kernel`.
- Contains `.bb` and/or `.bbappend` files for:
  - Default config.
  - Device Tree.
  - Kernel version.
  - Define `COMPATIBLE_MACHINE`.
  - Extra features required.

## Build Image

- Having got your BSP Layer in place, what now?
- Set your MACHINE variable in `local.conf` and build an image.
  - Remember the `conf/machine` directory of the layer.
    - Get your machine name from there.
- You will end up with an image in:
  - `build/tmp/deploy/images/<MACHINE>/<IMAGE>-<MACHINE>`

## Build the same image for different hardware boards

- Many manufacturers provide layers to support their hardware.
- Look at:
  - [open source machine list](https://layers.openembedded.org/layerindex/branch/master/machines)
  - Maybe your board already there.
  - Or you can search `meta-your-board` on Github.

## Creating your own BSP

- In case your board is not available on the internet.
- For a bespoke board you will need to create your onw BSP Layer.
- This needs a Layer:
  - The layer configuration.
  - The machine configuration.
  - The kernel configuration.

## Creating the layer

- Create directory `meta-my-bsp` (chose a better name!).
- Create a file `meta-my-bsp/conf/layer.conf` with the contents:

```text
# We have a conf and classes directory, add to BBPATH
BBPATH .= ":${LAYERDIR}"

# We have recipes-* directories, add to BBFILES
BBFILES += "${LAYERDIR}/recipes-*/*/*.bb ${LAYERDIR}/recipes-*/*/*.bbappend"

BBFILE_COLLECTIONS += "meta-my_bsp"
BBFILE_PATTERN_meta-my_bsp = "^${LAYERDIR}/"
BBFILE_PRIORITY_meta-my_bsp = "20"

LAYERDEPENDS_meta-my_bsp = "core"
LAYERSERIES_COMPAT_meta-my_bsp = "scarhgrap"
```

## Add the Machine

- In most instances you should start with the closest existing machine configuration and copy it.
- Create the file `meta-my-bsp/conf/<MACHINE>.conf`.

- For example, you take from beaglebone-yocto:

```text
#@TYPE: Machine
#@NAME: Beaglebone-yocto machine
#@DESCRIPTION: Reference machine configuration for BBB.

PREFERRED_PROVIDER_virtual/xserver ?= "xserver-xorg"
MACHINE_EXTRA_RECOMMEND = "kernel-modules"

EXTRA_IMAGE_DEPENDS += "virtual/bootloader"

DEFAULTTUNE ?= "cortexa8hf-neon"
include conf/machine/include/arm/armv7a/tune-cortexa8.inc

SERIAL_CONSOLES ?= "115200;ttyS0 115200;ttyO0 115200;ttyAMA0"
KERNEL_IMAGE_TYPE = "zImage"
DTB_FILES = "am335x-bone.dtb am335x-boneblack.dtb am335x-bonegreen.dtb"
KERNEL_DEVICETREE = '${@' '.join('ti/omap/%s' % d for do in '${DTB_FILES}'.split())}'
```

- You clone to your own:

```text
#@TYPE: Machine
#@NAME: Demo machine
#@DESCRIPTION: Configuration file for the Demo Board.

MACHINE_EXTRA_RECOMMEND = "kernel-modules kernel-devicetree"

EXTRA_IMAGE_DEPENDS += "virtual/bootloader"

DEFAULTTUNE ?= "cortexa8hf-neon"
include conf/machine/include/arm/armv7a/tune-cortexa8.inc

SERIAL_CONSOLES ?= "115200;ttyS0 115200;ttyO0 115200;ttyAMA0"
SERIAL_CONSOLES_CHECK = "${SERIAL_CONSOLES}"

KERNEL_IMAGE_TYPE = "zImage"
DTB_FILES = "demo-board.dtb"
KERNEL_DEVICETREE = '${@' '.join('ti/omap/%s' % d for do in '${DTB_FILES}'.split())}'

KERNEL_EXTRA_ARGS += "LOADADDR=${UBOOT_ENTRYPOINT}"
```

## Add the kernel changes

- You will need to append the Kernel recipe to add:

- A `COMPATIBLE_MACHINE` entry:
  - COMPATIBLE_MACHINE:demo-board = "demo-board"
- The `defconfig` and Device Tree files.

## Bootloader, etc

- If you need to make changes to the bootloader, etc.
- Again - start with existing recipes fot your board and modify them with a bbappend file.
