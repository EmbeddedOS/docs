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