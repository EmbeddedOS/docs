# Mastering RTOS: hands on FreeRTOS and STM32Fx with debugging

## 2. RTOS introduction

### 7. What is real time application (RTAs)

- Myth:
  - The real-Time Computing is equivalent to fast computing.
- Truth:
  - The Real-Time Computing is equivalent to predictable computing.

- What is Real time?
  - *Real-Time deals with guarantees, not with raw speed.*
  - Having more processors, more RAM, faster bus interfaces doesn't make a system real time.
  - A realtime system deals with guarantees, not with promises.
  - A real time system is one in which the correctness of the computations not only depends upon logical correctness of the computation, but also upon the time at which the result is produced. If the timing constraints are not met, system failure is said to have occurred.
  - In RT, The response time is guaranteed.

### 8. What are Real time Applications (RTAs)?

- RTAs are not fast executing applications.
- RTAs are time deterministic applications, that means, their response time to events is almost constant.
- There could be small deviation in RTAs response time, in terms of ms or seconds which fall into the category of `soft real time applications`.
- Hard real time functions **must** complete within a given time limit failure to do so will result in absolute failure of the system.

### 9. What is realtime OS?

- We are known about the realtime application: time deterministic - response time to events is always almost constant. -> U can always trust RTAs in terms of its timings in responding to events.
- But how do u run real-time applications?
  - To run `RTA`, u need real time OS.

- So what is it?
  - It's a OS, specially designed to run applications with very precise timing and a high degree of reliability.

- To be considered as `real-time`, an OS must have a known maximum time for each of the critical operations that it performs. Some of these operations include:
  - Handling of interrupts and interval system exceptions.
  - Handling of Critical Sections.
  - Scheduling Mechanism.

- GPOS examples: Linux, Windows, IOS, android, etc.
- RTOS examples: VxWorks, QNX, FreeRTOS, Integrity, etc.
