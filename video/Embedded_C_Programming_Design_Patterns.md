# Embedded C Programming Design Patterns | Clean Code | Coding Standards

- [link](https://www.youtube.com/watch?v=_lv5hin6eiM&t=559s)

## 1. High Level View

- There is a kind of data that is called high level data - it gives us structure of the universe which makes learning easier.
- All knowledge is then leaves of this structure - so first step we must do is get hold of this high level data.

- In programming (an mn universe in general) everything works like a fractal.
- The principles at higher levels are the same as principles at the lower levels - only at different levels of complexity.
- I'll show you how everything is very much the same.

## 2. What are we trying to do as programmers?

- 1. We are trying to create a binary image (program) that our processor can run without choking and produce the results that we want - as a result of carefully ordered mathematical operations.
- 2. We have been trying to do this since 1972 and we are still doing it to this data. Everything is a binary image. Some binary images run other binary images or accept text commands (also binary images).

## 3. The Binary Image

- Executable And Linkable File Format (ELF) is used by most Linux based systems today to store compiled program image.
- Even a bootable kernel is an ELF file that the bootloader loads into memory as the first program to be run.
- Bootloader itself is compiled into ELF container but when whe flash it we take the insides out and flash these insides instead (leaving ELF container behind).

- 1. A program is separated into a handful set of arrays called:
  - 1. `.text`: program code.
  - 2. `.data`: initialized data.
  - 3. `.bss`: uninitialized data.
- 2. The processor hardware directly traverses `.text` array and occasionally peeks into `.data` and `.bss` arrays to supply program with data.
- 3. All programs are compiled into a set of arrays like this.
- 4. We use linker scripts to create binary program image.

## 4. ELF File Format

- Central to all programs. This is the container which contains software program.
- Divided into 4 main sections:
  - `.text`
  - `.data`
  - `.bss`
  - `isr_vectors` (interrupt table)
- Many other sections are used for less important things - like debug info.
- Note: Dump section headers: `arm-none-eabi-readelf -IS <elf_file>`

## 5. Firmware

- A firmware is a collection of programs or applications that are either linked together into a single image or combined together as a combinations of code and data/
- Very similar to how a program container is made except contains many different programs and often filesystem as well.

- 1. Bootloader - loads the main program from one of the supported locations (flash, uart, usb, network).
- 2. Main program - RTOS or kernel - either is the only program that runs or loads other programs from data storage.
- 3. Application - a program loaded or started after the main program has started.

## 6. How does chip boot your program

- In desktop software entry point is specified in ELF file.
- On micro-controller entry point is fixed: typically first bytes in flash are stack pointer and initial instruction pointer.
- CPU loads SP and IP into internal registers and starts to run the program. Whatever is  at address specified is executed.

## 7. So What is the Main program

- The purpose of the main program is to enforce time. For more complex systems we can bundle our main program (kernel) with other applications.
- We use **build systems** to achieve this.

- 1. On MCU we use RTOS or program directly against hardware.
- 2. On Micro-processors we use LInux and build firmware using firmware builder:
  - 1. Buildroot.
  - 2. OpenWrt.
  - 3. Yocto.
- 3. Main program takes care of lowest level hardware events: interrupts.

## 8. We Use C Because It maps Perfectly into thi structure

- No matter which language you use, you can not escape the fact that you have to create the initial startup binary and C just so happens to be the language that maps most tightly into how hardware works.
- Which is why C is still nr 1 most used programming language after almost 50 years.
- There is an almost one to one relationship between C code and binary image (ELF file). Even if optimizations are used - it's still very close.

- 1. C language describes code that is compiled directly into the `.text`  array of machine instructions.
- 2. C arrays and variables that have initial values are compiled into the data section.
- 3. C arrays and variables that are not initialized are grouped in `.bss` array so that they can be initialized by startup code.
- 4. Once everything is running we can switch to a higher level language or scripting engine.

## 9. So do we just code then?

- C is a way to express program structure in text form. It says nothing about the structure we are going to be expressing.
- We need **design patters** to write good software in any language (specially in C) because typically do not enforce patterns.
- Design patterns are concerned with organizing higher level concepts into arrays and trees (code, objects, device driver, etc.).

## 10. High Level Design Patterns For Driver Development

- 1. Struct pattern.
- 2. Multi-level struct pattern (containment).
- 3. Opaque objects (implementation hidden).
- 4. Data abstraction pattern using `container_of`.
- 5. Abstract interface using `container_of` and virtual methods.

## 11. Struct Pattern

- 1. All methods operate on collections of data.
- 2. Fine grained control over data scope thus minimizing unintentional data manipulation.
- 3. Ensures functions are re-entrant (No globally manipulated state).
- 4. Simplifies multithreaded programming because you can easily locate relevant data.
- 5. Simplicity of testing because code can be easily compiled in isolation and fed with mock data.
- 6. Functions never operate on global state.

- All data must be stored in struct passed as arguments.
- Struct variables are never declared global.
- All functions accept pointer to struct where they can find the data that they need.
- Data flow is always through the code and not outside of it.

- For example `mcp4461 driver`:

```C
struct mcp4461 {
    i2c_device_t i2c;
    gpio_device_t gpio;
    uint8_t addr;
    uint32_t reset_pin, wp_pin;
    struct analog_device;
};

// ...
static int _mcp4461_write_reg(struct mcp4461 *self, uint8_t reg, uint8_t val)
{
    int ret = 0;
    uint8_t data[] = {
        reg | MCP4XXX_OP_WRITE | (uint8_t)(val >> 8),
        (uint8_t)(val & 0xff)
    };

    if ((ret = i2c_transfer(
                    self->i2c,
                    self->addr,
                    data, 2, NULL, 0, MCP4461_TIMEOUT)) < 0)
    {
        return ret;
    }

    return 0;
}
```

- mcp4461 is an i2c potentiometer.
- We represent an INSTANCE using struct.
- We have methods that operate on that struct.
- Everything needed is inside the struct.

## 12. Hierarchical Containment

- Enables reuse of behavior (functionality) for all instance of the same `type`.
- Allows one type to **have-an** instance of another type.
- Only possible if you use struct pattern throughout your application from the start.

- Related variables are grouped in structs and included into other structs.
- Every function (behavior) takes a pointer to struct that contains only data relevant to that behavior.
- Multiple instances of a behavior can be easily created when you use this pattern.

## 13. Opaque Objects

- Enables hiding the implementation.
- Structure of the object is hidden from all other code in the project (technically still accessible but programmers can't access it inadvertently).

- Opaque objects are typically allocated on heap (for example at startup).
- Object definition is in THE SAME source file as implementation (ie. NOT public).
- Public interface only contains methods that operate on a pointer to the opaque object.

- For example: Console Driver

```C
// src/driver/tty/console.c
struct console {
    struct console_device dev;
    serial_port_t serial;
    struct list_head commands;
    size_t ncommands;
    // ...
}
```

- Application doesn't need to know about particular implementation of the console application.
- Console data structure is defined locally in the `.c` file.

```C
struct console *self = kzmalloc(sizeof(struct console));
```

- The object is then registered as an interface for use with other parts of the application.
- Application deals with pointer only.

```C
console_device_init(&self->dev, fdt, fdt_node, &_console_ops);
console_device_register(&self->dev);
console_add_command(&self->dev.ops, self, "ps", "Show list of processors", "", _cmd_ps);
console_add_command(&self->dev.ops, self, "md", "Dump raw memory location", "", _cmd_md);
```
