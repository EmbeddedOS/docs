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

### 11. RTOS - GPOS: priority inversion

- GPOS: Priority inversion effects are in-significant.
- RTOS: Priority inversion effects must be solved.

- What are the features that a RTOS has but a GPOS doesn't?
  - Priority based preemptive scheduling mechanism.
  - No or very short critical sections which disables the preemption.
  - Priority inversion avoidance.
  - bounded interrupt latency.
  - bounded scheduling latency, etc.

### 12. What is MUlti-tasking?

- How do u run multiple tasks on a processor which has got only one core?
  - Running multiple tasks on a processor is accomplished by `scheduler`.

## 3. IDE installation and development board

### 20. Adding FreeRTOS kernel source to project

---------Application Code-----------
----main, thread, interrupt, etc.---
                ||
                \/
------------CMSIS-RTOS API----------
                ||
                \/
---------Real Time Kernel-----------

- The application code layer only depends on CMSIS-RTOS layer, so any change in architecture, kernel, processor, etc. We must not change the application code.

- The STM32 cubeIDE will add new code of CMSIS-RTOS API layer to our project. If we don't use the CMSIS layer, we will have to work directly with the kernel layer, and our code will be depended on the kernel layer.

- For example:

----------Application---------
              ||
      osThreadCreate()
              ||
              \/
----------CMSIS-CORE layer----
              ||
        vTaskCreate()
              ||
              \/
------------FreeRTOS----------

- osThreadCreate(): API provided by CMSIS-RTOS layer to create an RTOS task (independent of an underlying RTOS).
- vTaskCreate(): Actual API provided by FreeRTOS to create a Task (specific).

- CMSIS-RTOS API is a generic RTOS interface for ARm Cortex-M processor-based devices.

- In other hand, the CMSIS-RTOS can work directly with processor architecture.

----------Application---------
              ||
              \/
----------CMSIS-CORE layer----
              ||
              \/
------Cortex-M processor -----

### 22. FreeRTOSConfig.h and custom settings

- FreeRTOS is customised using a configuration file called FreeRTOSConfig. Every FreeRTOS application must have a `FreeRTOSConfig.h` header file in its preprocessor include path. `FreeRTOSConfig.h` tailors the RTOS Kernel to the application being built. It is therefore specific to the application, not the RTOS, and should be located in application directory, not in one of the RTOS kernel source code directory.

- Template file:

```C
#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/* Here is a good place to include header files that are required across
your application. */
#include "something.h"

#define configUSE_PREEMPTION                    1
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 0
#define configUSE_TICKLESS_IDLE                 0
#define configCPU_CLOCK_HZ                      60000000
#define configSYSTICK_CLOCK_HZ                  1000000
#define configTICK_RATE_HZ                      250
#define configMAX_PRIORITIES                    5
#define configMINIMAL_STACK_SIZE                128
#define configMAX_TASK_NAME_LEN                 16
#define configUSE_16_BIT_TICKS                  0
#define configIDLE_SHOULD_YIELD                 1
#define configUSE_TASK_NOTIFICATIONS            1
#define configTASK_NOTIFICATION_ARRAY_ENTRIES   3
#define configUSE_MUTEXES                       0
#define configUSE_RECURSIVE_MUTEXES             0
#define configUSE_COUNTING_SEMAPHORES           0
#define configUSE_ALTERNATIVE_API               0 /* Deprecated! */
#define configQUEUE_REGISTRY_SIZE               10
#define configUSE_QUEUE_SETS                    0
#define configUSE_TIME_SLICING                  0
#define configUSE_NEWLIB_REENTRANT              0
#define configENABLE_BACKWARD_COMPATIBILITY     0
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS 5
#define configUSE_MINI_LIST_ITEM                1
#define configSTACK_DEPTH_TYPE                  uint16_t
#define configMESSAGE_BUFFER_LENGTH_TYPE        size_t
#define configHEAP_CLEAR_MEMORY_ON_FREE         1

/* Memory allocation related definitions. */
#define configSUPPORT_STATIC_ALLOCATION             1
#define configSUPPORT_DYNAMIC_ALLOCATION            1
#define configTOTAL_HEAP_SIZE                       10240
#define configAPPLICATION_ALLOCATED_HEAP            1
#define configSTACK_ALLOCATION_FROM_SEPARATE_HEAP   1

/* Hook function related definitions. */
#define configUSE_IDLE_HOOK                     0
#define configUSE_TICK_HOOK                     0
#define configCHECK_FOR_STACK_OVERFLOW          0
#define configUSE_MALLOC_FAILED_HOOK            0
#define configUSE_DAEMON_TASK_STARTUP_HOOK      0
#define configUSE_SB_COMPLETED_CALLBACK         0

/* Run time and task stats gathering related definitions. */
#define configGENERATE_RUN_TIME_STATS           0
#define configUSE_TRACE_FACILITY                0
#define configUSE_STATS_FORMATTING_FUNCTIONS    0

/* Co-routine related definitions. */
#define configUSE_CO_ROUTINES                   0
#define configMAX_CO_ROUTINE_PRIORITIES         1

/* Software timer related definitions. */
#define configUSE_TIMERS                        1
#define configTIMER_TASK_PRIORITY               3
#define configTIMER_QUEUE_LENGTH                10
#define configTIMER_TASK_STACK_DEPTH            configMINIMAL_STACK_SIZE

/* Interrupt nesting behaviour configuration. */
#define configKERNEL_INTERRUPT_PRIORITY         [dependent of processor]
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    [dependent on processor and application]
#define configMAX_API_CALL_INTERRUPT_PRIORITY   [dependent on processor and application]

/* Define to trap errors during development. */
#define configASSERT( ( x ) ) if( ( x ) == 0 ) vAssertCalled( __FILE__, __LINE__ )

/* FreeRTOS MPU specific definitions. */
#define configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS 0
#define configTOTAL_MPU_REGIONS                                8 /* Default value. */
#define configTEX_S_C_B_FLASH                                  0x07UL /* Default value. */
#define configTEX_S_C_B_SRAM                                   0x07UL /* Default value. */
#define configENFORCE_SYSTEM_CALLS_FROM_KERNEL_ONLY            1
#define configALLOW_UNPRIVILEGED_CRITICAL_SECTIONS             1
#define configENABLE_ERRATA_837070_WORKAROUND   1

/* ARMv8-M secure side port related definitions. */
#define secureconfigMAX_SECURE_CONTEXTS         5

/* Optional functions - most linkers will remove unused functions anyway. */
#define INCLUDE_vTaskPrioritySet                1
#define INCLUDE_uxTaskPriorityGet               1
#define INCLUDE_vTaskDelete                     1
#define INCLUDE_vTaskSuspend                    1
#define INCLUDE_xResumeFromISR                  1
#define INCLUDE_vTaskDelayUntil                 1
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_xTaskGetSchedulerState          1
#define INCLUDE_xTaskGetCurrentTaskHandle       1
#define INCLUDE_uxTaskGetStackHighWaterMark     0
#define INCLUDE_uxTaskGetStackHighWaterMark2    0
#define INCLUDE_xTaskGetIdleTaskHandle          0
#define INCLUDE_eTaskGetState                   0
#define INCLUDE_xEventGroupSetBitFromISR        1
#define INCLUDE_xTimerPendFunctionCall          0
#define INCLUDE_xTaskAbortDelay                 0
#define INCLUDE_xTaskGetHandle                  0
#define INCLUDE_xTaskResumeFromISR              1

/* A header file that defines trace macro can be included here. */

#endif /* FREERTOS_CONFIG_H */
```

- `config` Parameters:
  - `configUSE_PREEMPTION`: Set to 1 use the preemptive RTOS scheduler, or 0 to use the cooperative RTOS scheduler.
  - `configUSE_PORT_OPTIMISED_TASK_SELECTION`: Some FreeRTOS ports have two methods of selecting the next to execute - a generic method, and a method that is specific to that port.

- For the `stm32f407` MCU, we can use the template file in directory (FreeRTOS project): `reeRTOS/Demo/CORTEX_M4F_STM32F407ZG-SK/FreeRTOSConfig.h`, and copy it to our project.

- We also need to edit the `FreeRTOSConfig.h` file, because of macro `configCPU_CLOCK_HZ`.

```C
#if defined(__ICCARM__) || defined(__GNUC__) || defined(__CC_ARM)
  #include <stdint.h>
  extern uint32_t SystemCoreClock;
#endif
```

- Because the `SVC_Handler`, `PendSV_Handler`, `SysTick_Handler` interrupt handlers is generated automatically, so when we add the FreeRTOS code to our project, they will be redefined -> compiler error (FreeRTOS uses them to schedule tasks). So will need to remove them by using STM32CubeIDE -> System core -> NVIC -> Code generation -> disable:
  - Time base: System tick timer.
  - Pendable request for system service.
  - System service call via SWI instruction.

- After that, just press `Ctrl + S`.

- Disable three macros to avoid compiler errors:

```C
#define configUSE_TICK_HOOK             0
#define configUSE_MALLOC_FAILED_HOOK    0
#define configCHECK_FOR_STACK_OVERFLOW  0
```

### 23. Time base selection for STM32-FreeRTOS project

- Time base source selection
  - FreeRTOS uses ARM Cortex Mx processor's internal sys-tick timer as its time base (RTOS ticking).
  - STM32 Cube HAL layer also by default uses sys-tick timer as its time base source.
  - If you are using both freeRTOS and STM32 Cube HAL layer in your project, there will be a conflict to use time-base source.
  - To resolve this, it is strongly recommended to use STM32 cube HAL layer time base source other than sys-tick timer (use any timer peripheral of the micro controller).

## 6. FreeRTOS Task Creation

### 24 What is task?

- Application and tasks:
  - For example, temperature monitoring application includes three main tasks:
    - Task 1: sensor data gathering.
    - Task 2: Updating display.
    - Task 3: User input processing.

- Creating and implementing a Task:
  - Task creation API:

    ```C
    BaseType_t xTaskCreate(TaskFunction_t pvTaskCode,
                            const char * const pcName,
                            configSTACK_DEPTH_TYPE usStackDepth,
                            void *pvParameter,
                            UBaseType_t uxPriority,
                            TaskHandle_t *pxCreatedTask
                          );
    ```

  - Task Implementation:

    ```C
    void vATaskFunction(void *pvParameters)
    {

      /* Variables can be declared just as per a normal function. Each instance
       * of a task created using this function will have its own copy of the
       * var_example variable. This would not be true if the variable was declared
       * static - in which case only one copy of the variable would exist and
       * this copy would be shared by each created instance of the task.
       */
      int var_example = 0; // Local variable which will be created in the stack space of this task.

      /* A task will normally be implemented as in infinite loop. */
      for (;;)
      {
        /* Task application code here. */
      }

      /* Should the task implementation ever break out of the above loop then
       * the task must be deleted before reaching the end of this function. The
       * NULL parameter passed to the `vTaskDelete()` function indicates that
       * the task to be deleted is the calling (this) task.
       */
      vTaskDelete(NULL);
    }
    ```

### 25. FreeRTOS task creation API

```C
/* xTaskCreate: this API creates a new FreeRTOS Task using dynamic memory
 * allocation and adds the newly created task (TCB) to ready queue of the kernel.
 *
 * @pvTaskCode: Address of the associated task handler.
 * @pcName: A descriptive name to identify this task.
 * @usStackDepth: Amount of stack memory allocated to this task (memory in words not in bytes).
 * @pvParameter: pointer of the data which needs to be passed to the task handler once it gets scheduled.
 * @uxPriority: Task priority value.
 * @pxCreatedTask: Used to save the task handle (an address of the task created).
 *
 * Returns: If the task eas created successfully then pdPASS is returned. Otherwise errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY is returned.
 */
BaseType_t xTaskCreate(TaskFunction_t pvTaskCode,
                        const char * const pcName,
                        configSTACK_DEPTH_TYPE usStackDepth,
                        void *pvParameter,
                        UBaseType_t uxPriority,
                        TaskHandle_t *pxCreatedTask
                      );
```

- For more information, refer to: [link](https://www.freertos.org/a00106.html)
- Word:
  - It's a max size of the data which can be accessed (load/store) by the processor in a single clock cycle using a single instruction.
  - Processor design also offers native support (register width, bus width) to load/store word-sized data.
  - Word size could be 8 bit/16 bit/32 bit or more, depending upon the processor design. For ARM Cortex Mx based micro controllers, word size = 32 bits.

### 26. Task priorities in FreeRTOS

- Task priorities
  - A priority value comes into the picture when there are 2 or more tasks in the system.
  - Priority value helps the scheduler to decide which task should run first on the processor.
  - Lower the priority value, lesser the task priority (urgency).
  - In FreeRTOS each task can be assigned a priority value from 0 to `(configMAX_PRIORITIES-1)` where `configMAX_PRIORITIES` may be defined in `FreeRTOSConfig.h`.
  - U must decide `configMAX_PRIORITIES` as per your application need. Using too many task priority values could lead to RAM's over-consumption. If many tasks are allowed to execute with varying task priorities, it may decrease the system's overall performance.
  - Limit `configMAX_PRIORITIES` to 5, unless u have a valid reason to increase this.

### 28. Scheduling policies

- Scheduling:
  - Now, u have created 2 tasks, which are in the ready task list of the freeRTOS (READY state).
  - Tasks will be dispatched to run on the CPU by the scheduler.
  - Scheduler is nothing but a piece of code that is part of the freeRTOS kernel, which runs in the privileged mode of the processor.
  - U should invoke the scheduler first by calling the below API provide bt FreeRTOS.
    - `vTaskStartScheduler()`

- Scheduling policy:
  - Scheduler schedules tasks to run on the CPU according to the scheduling policy configured:
    - Pre-emptive scheduling
    - Co-operative scheduling

- Scheduler:
  - `#define configUSE_PREEMPTION 1`: Pre-emptive scheduling (default)
  - `#define configUSE_PREEMPTION 0`: Co-operative scheduling

- Pre-emptive scheduling
  - What is preemption?
    - Replace a running task with another task.
    - During preemption, the running task is made to give up the processor even if it hasn't finished the work. The scheduler does this to run some other tasks of the application.
    - The task which gave up the processor simply returns to ready state.

- Two type options of Pre-emptive scheduling:
  - Round robin (cyclic) pre-emptive scheduling.
  - Priority based pre-emptive scheduling.

- Round-robin pre-emptive scheduling:
  - Scheduling tasks without priority (aso known as cyclic executive).
  - Time slices are assigned to each task in equal portions and in circular order.
  - Ready queue/list of the RTOS:

    READY         READY         READY           READY
    Task-1        Task-4        Task-2          Task-3

    |S|CPU run task-1|S|CPU run task-4|S|CPU run task-2|S|CPU run task-3|
    t1<--time-slice->t2               t3               t4
    |                |                |                 |
    |------1ms-------|----------------|-----------------|----RTOS ticks--->

    - S: Scheduler run context switch
      - Context switching save the context of current task and retrieving CPU to context of the next task.
    - `#define configTICK_RATE_HZ( ( TickType_t ) 1000 )`

- Priority based pre-emptive scheduling
  - Tasks are scheduled to run on the CPU based on their priority. A task with higher priority will be made to run on the CPU forever unless the task gets deleted/blocked/suspended or leaves voluntarily to give chance for others.

- Co-operative scheduling
  - A Task cooperates with other tasks by explicitly giving up the processor (Processor yielding).
  - There is no `pre-emption` of the tasks by the scheduler. that is, the running task will never be interrupted by the scheduler.
  - The RTOS tick interrupt doesn't cause any pre-emption, but the tick interrupts are still needed to keep track of the kernel's real-time tick value.
  - Tasks give up the CPU when they are done or periodically or blocked/suspended waiting for a resource.

### 31. Behind scene task creation

- WHat happens when you create a TASK?
  - Total RAM = SRAM1 + SRAM2 = 112 + 16 = 128KiB

  |<-------HEAP (configTOTAL_HEAP_SIZE)------------>|                                             |
  |TCB1 |STACK1 |TCB2 |STACK2 |SCB|ITEM LIST|       | This RAM space is used for                  |
  |     |       |     |       |   |         |       | Global data, arrays, static variables, etc. |
  |     |       |     |       |   |         |       | Kernel stack                                |
  Low------------------------------------------------------------------------------------------->High

- We can config heap size in FreeRTOS with the config `configTOTAL_HEAP_SIZE`.

- When we use `xTaskCreate()` to create a task, the memory for TCB (Task control block `TCB1`) will be allocated from the HEAP section of the RAM. And stack memory (STACK1) for this task is also allocated from the HEAP section of the RAM (This size is specified by `usStackDepth` parameter).

- If you create a semaphore by `xSemaphoreCreateBinary()`, this also creates a control block called semaphore control block (`SCB`) in HEAP section of the RAM.

- When you use `xQueueCreate()`, this is an API to create a queue. The queue control block will be created in the heap and also it's `ITEM LIST`, Queue item list will be maintained in HEAP section of the RAM.

- All These APIs are called as APIs which create the `kernel objects` dynamically. And memory for them is allocated dynamically if you use such APIs.

- Summary of Task Creation:
  - When we use `xTaskCreate()`, three important things will happen:
    - **TCB**: TCB (Task Control Block) will be created in RAM (Heap section) and initialized. TCB is basically a C structure, and it has got various member elements and those member elements will be initialized. One of important member of TCB is `pxTopOfStack`, this actually holds a **Top of Stack** information of task private stack.
    - **Stack**: Dedicated Stack memory will be created for a task and initialized. This stack memory will be tracked using PSP register of the ARM Cortex Mx Processor. This stack memory will be tracked using PSP register of the ARM Cortex Mx Processor. It has two stack pointers: One is `PSP` which stands for **Process Stack Pointer** and another one is `MSP` which stands for **Main Stack Pointer**.

    - **Task Ready list Maintained by freeRTOS kernel**: Task will be put under `READY` list for scheduler to pick.
