# CMSIS-RTOS

## Generic RTOS Interface

- The CMSIS-RTOS API is a generic RTOS interface for ARM Cortex-M processor based devices.

- CMSIS-RTOS provides a standardized API for software components that require RTOS functionality and gives therefore serious benefits to the users and the software industry.
  - CMSIS-RTOS provides basic features that required in many applications or technologies such as UML or Java (JVM).
  - The unified feature set of the CMSIS-RTOS API simplifies sharing of software components and reduces learning efforts.
  - Middleware components that use the CMSIS-RTOS API are RTOS agnostic. CMSIS-RTOS compliant middleware is easier to adapt.
  - Standard project templates (such as motor control) of the CMSIS-RTOS API may be shipped with freely available CMSIS-RTOS implementations.

- Note:
  - The CMSIS-RTOS API defines a minimum feature set. Implementations with extended features may be provided by RTOS vendors.

---------Application Code-----------
----main, thread, interrupt, etc.---
                ||
                \/
------------CMSIS-RTOS API----------
                ||
                \/
---------Real Time Kernel-----------

- CMSIS-RTOS API structure
  - A typical CMSIS-RTOS API implementation interfaces to an existing real-time kernel. Thr CMSIS-RTOS API provides the following attributes and functionalities:
    - Function names, identifiers, and parameters are descriptive and easy to understand. The functions are powerful and flexible which reduces the number of functions exposed to the user.
    - Thread management allows to define, create, and control threads.
    - Interrupt Service Routines (ISR) can call some CMSIS-RTOS functions. When a CMSIS-RTOS function cannot be called from ISR context, it rejects the invocation.
    - Three different thread event types support communication between multiple threads and/or ISR:
      - **Signal**: is a flag that may be used to indicate specific conditions to a thread. Signal can be modified in an ISR or set from other threads.
      - **Message**: is a 32-bit value that can be sent to a thread or an ISR. Messages are buffered in a queue. The message type and queue size is defined in a descriptor.
      - **Mail**: is a fixed-size memory block that can be sent to a thread or an ISR. Mails are buffered in a queue and memory allocation is provided. The mail type and queue size is defined in descriptor.

    - Mutexes and Semaphores are incorporated.
    - CPU time can be schedule with the following functionalities:
      - A `timeout` parameter is incorporate in many CMSIS-RTOS functions to avoid system lookup. When a timeout is specified, the system waits until a resource is available or an event occurs. While waiting, other threads are scheduled.
      - The `osDelay` function puts a thread into the state **WAITING** for a specified period of time.
      - The generic `osWait` function waits for events that are assigned to a thread.
      - The `osThreadYield` provides co-operative thread switching and passes execution to another thread of the same priority.

- The CMSIS-RTOS API is designed to optionally incorporate multi-processor and **Mail** and **Message** queues can therefore reside in shared memory resources.

- The CMSIS-RTOS API encourages the software industry to evolve existing RTOS implementations. Kernel objects are defined and accessed using macros. This allow differentiation. RTOS implementations can be different and optimized in various aspect towards the Cortex-M processors. Optional features may be for example:
  - Generic **wait** function; i.e. with support of time intervals.
  - Support of the Cortex-M memory protection unit (MPU).
  - Zero-copy mail queue.
  - Support of multi-processor systems.
  - Support of a DMA controller.
  - Deterministic context switching.
  - Round-robin context switching.
  - Deadlock avoidance, for example with priority inversion.
  - Zero interrupt latency by using the Cortex-M3/M4 instructions LDREX and STREX.
