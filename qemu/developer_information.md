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

- 