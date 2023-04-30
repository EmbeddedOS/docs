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

- What are Real time Applications (RTAs)?

  - RTAs are not fast executing applications.
  - RTAs are time deterministic applications, that means, their response time to events is almost constant.
  - There could be small deviation in RTAs response time, in terms of ms or seconds which fall into the category of `soft real time applications`.
  - Hard real time functions **must** complete within a given time limit failure to do so will result in absolute failure of the system.

### 8. What is realtime OS?

- We are known about the realtime application: time deterministic - response time to events is always almost constant. -> U can always trust RTAs in terms of its timings in responding to events.
- But how do u run real-time applications?
  - To run `RTA`, u need real time OS.

- So what is it?
  - It's a OS, specially designed to run applications with very precise timing and a high degree of reliability.

- To be considered as `real-time`, an OS must have a known maximum time for each of the critical operations that it performs. Some of these operations include:
  - Handling of interrupts and interval system exceptions.
  - Handling of Critical Sections.
  - Scheduling Mechanism.

- GPOS (General purpose OS) examples: Linux, Windows, IOS, android, etc.
- RTOS examples: VxWorks, QNX, FreeRTOS, Integrity, etc.

### 9. RTOS vs GPOS task scheduling

- GPOS - task scheduling
  - GPOS is programmed to handle scheduling in such a way that it manages to achieve high throughput.
  - Throughput means - the total number of processes that complete their execution per unit time.
  - Some times execution of high priority will get delayed in order to serve 5 or 6 low priority tasks. High throughput is achieved by serving 5 low priority tasks than by serving a single high priority one.
  - So if 5 or 6 low priority applications are waiting to run, then GPOS may delay 1 or 2 high priority task in order to increase the throughput.

  - In a GPOS, the scheduler typically uses a fairness policy to dispatch threads and processes onto the CPU.
  - Such a policy enables the high overall throughput required by desktop and server applications, but offers no guarantees that high-priority, time critical threads or processes will execute in preference to lower-priority threads.

- RTOS - Task Scheduling
  - On the other hand in RTOS, threads execute in the order of their priority. If a high-priority thread becomes ready to run, it will take over the CPU from any lower-priority thread that may be executing.
  - Here a high priority thread gets executed over the low priority ones. All `low priority thread execution` will get paused. A high priority thread execution will get override only if a request comes from an even high priority threads.

- RTOS vs GPOS: Task Scheduling
  - Does that mean, RTOS are **very poor** in throughput?
  - RTOS may yield less throughput than the General-Purpose OS, because it always favors the high priority task to execute first, but that does not mean it has very poor throughput!
  - A quality RTOS will still deliver decent overall throughput but can sacrifice throughput for being deterministic or to achieve time predictability.
  - GPOS: Meet higher throughput.
  - RTOS: Meet time predictability.
  - For the RTOS, `achieving predictability or time deterministic nature is more important than throughput`, but for the GPOS achieving higher throughput for user convenience is more important.

### 10. GPOS vs RTOS task switching latency

- GPOS vs RTOS task switching latency
  - In computer, Latency means `The time that elapses between a stimulus and the response to it.`

  - Task switching latency means, that time gap between `A triggering of an event and the time at which the task which takes care of that event is allowed to run on the CPU`.

  - GPOS: Task switching latency may vary significantly.
    - Task switching latency may depends on number of tasks.
  - RTOS: Task switching latency remains almost constant.
    - In the case of RTOS this latency is always time bounded and doesn't vary significantly.

- GPOS vs RTOS interrupt latency
  Task 1 - (IRQ Occurs) -------> ISR >------- (Task Resumes) -> Task 2
         |                     |     |                       |
  -------|Interrupt latency    |-----|Scheduling latency     |------->
                                                                (Time)

- Interrupt Latency: RTOS should have this latency as minimal as possible and must be deterministic.

- Both the interrupt latency and scheduling latency of the RTOS is as small as possible and time bounded. But in the case of GPOS, due to increase in system load these parameters may vary significantly.
