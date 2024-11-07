# QEMU developer information

- If you wish to implement a new hardware mode you will want to read through the QEMU Object Model (QOM) [QOM](https://qemu-project.gitlab.io/qemu/devel/qom.html#qom).
- Those wishing to enhance or add new CPU emulation capabilities will want to read [TCG Emulation](https://qemu-project.gitlab.io/qemu/devel/index-tcg.html#tcg) documentation, especially the overview of the [Translator Internals](https://qemu-project.gitlab.io/qemu/devel/tcg.html#tcg-internals).

## 1. QEMU Community Processes

## 2. QEMU build system

### 2.1. The QEMU build system architecture

- As with projects using GNU autotools, the QEMU build system has two stages;
  - First the developer runs the `configure` script to determine the local build environment characteristics;
  - Then they run `make` to build the project.

#### 2.1.1. Stage 1: configure

- Five tasks of `configure` script:
  1. Detect the host arch.
  2. List targets for which to build emulators.
  3. Find the compilers (native or cross) used to build exe, fw and tests. Results are written as: Makefile fragments (`config-host.mak`) or Meson machine file (`config-meson.cross`).
  4. Create a virtual env in which Python code runs during the build.
  5. Invoke Meson in virtual env, to perform the actual config step for emulator build.

#### 2.2.2. Stage 2: Meson

- Meson build system describes the build and install process for:
  1. executable:
       - tools: `qemu-img`, `qemu-nbd`, `qemu-ga` (guest agent), etc.
       - System emulators: `qemu-system-$ARCH`
       - Userspace emulators: `qemu-$ARCH`
       - Unit tests.
  2. doc.
  3. ROMs.
  4. Other data files, such as icons or desktop files.

- The source code is highly modularized, split across many files to facilitate building of all of these components with as little duplicated compilation as possible.
- Using `sourceset` functionality, `meson.build` files group the source files in rules. Sourcesets belong to 1 of 4 groups:
  1. **Subsystem sourceset**: `block_ss` for block device subsystem, `char_ss` for character device subsystem. etc. They are turned into static libs.
  2. **Target-independent emulator sourceset**: code lives in the `common_ss` (used everywhere), `system_ss`(used in system emulator) and `user_ss` (used in userspace emulator) sourcesets. Helpers, platform portability wrapper functions, etc. Are linked to binaries if needed.
  3. **Target-dependent emulator sourceset**:
        - CPU emulation, included in the `specific_ss` sourceset, for example `arm_ss`.
        - source code for each emulator are in the `hw/` and `target/` subdirectories.
        - Each subdirectory in `hw/` adds one sourceset to the `hw_arch` directory, for example: `hw/arm/meson.build` define arm sourceset:

            ```meson
            arm_ss = ss.source_set()
            arm_ss.add(files('boot.c'), fdt)
            ...
            hw_arch += {'arm': arm_ss}
            ```

  4. **Module sourceset**:
  5. **Utility sourcesets**: All binaries link with a static library `libqemuutil.a` built in some general sourcesets: `util_ss`, `stub_ss`.

#### 2.2.3. Stage 3: Make
