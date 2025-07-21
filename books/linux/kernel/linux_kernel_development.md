# Linux Kernel development

## 1. Introduction to the Linux Kernel

## 2. Getting started with the kernel

- Kernel source tree:
  - `arch`: Architecture-specific source.
  - `block`: Block I/O layer.
  - `crypto`: Crypto API.
  - `Documentation`: Kernel source documentation.
  - `drivers`: Device drivers.
  - `firmware`: Device firmware needed to use certain drivers.
  - `fs`: The VFS and the individual file systems.
  - `include`: Kernel headers.
  - `init`: Kernel boot and initialization.
  - `ipc`: Inter-process communication code.
  - `kernel`: Core subsystems, such as the scheduler.
  - `lib`: Helper routines.
  - `mm`: Memory management subsystem and the virtual memory.
  - `net`: Networking subsystem.
  - `samples`: Sample, demonstrative code.
  - `scripts`: Scripts used to build kernel.
  - `security`: Linux security module.
  - `sound`: Sound subsystem.
  - `usr`: Early user-space code (called initramfs).
  - `tools`: Tools helpful for developing Linux.
  - `virt`: Virtualization infrastructure.

### 2.1. Building the kernel

#### 2.1.1. Configuring the kernel

- The simplest tool is a text-based command-line utility:

```bash
make config
```

- This utility goes through each option, one by one, and asks the user to interactively select `yes`, `no`, or `module` (for tristate). Because this tak a **long** time, unless you are paid by the hour, you should use an ncurses-based graphical utility:

```bash
make menuconfig
```

- Or this command creates a configuration based on the defaults for your architecture:

```bash
make defconfig
```

- After the kernel configuration is set -- however you do it -- you can build it with a single command:

```bash
make
```

#### 2.1.2. Installing the new kernel

- After the kernel is built, you need to install it. Installing modules, thankfully, is automated and architecture-independent. As root, simply run:

```bash
make modules_install
```

### 2.2. A Beast of a different nature

- Linux kernel has several unique attributes as compared to a normal user-space application. Although these differences do not necessarily make developing kernel code harder than developing user space code, they certainly make doing so different.

- These characteristics make the kernel a beast of a different nature. Some of the usual rules are bent; other rules are entirely new. Although some of the differences are obvious (we all know the kernel can do anything it wants), others are not obvious. The most important of these differences are:
  - The kernel has access to neither the C library nor the standard C headers.
  - The kernel is coded in GNU C.
    - Like any self-respecting Unix kernel, the Linux kernel is programmed in C.
  - The kernel lacks the memory protection afforded to user-space.
  - The kernel cannot easily execute floating-point operations.
  - The kernel has a small per-process fixed-size stack.
  - Because the kernel has asynchronous interrupts, is preemptive, and supports SMP, synchronization and concurrency are major concerns within kernel.
  - Portability is important.

## 3. Process Management

- Because running user applications is the reason we have OSs, the process management is a crucial part of any operating system kernel, including Linux.

### 3.1. The PROCESS

- A *process* is a program (object code stored on some media) in the midst of execution. Processes are, however, more than just the executing program code (often called the `text section` in Unix).
- They also include a set of resources such as:
  - open files.
  - pending signals.
  - internal kernel data.
  - processor state.
  - A memory address space with one or more memory mappings.
  - One or more `threads of execution`.
  - `data` section containing global variables.

- Processes, in effect, are the living result of running program code. The kernel needs to manage all these details efficiently and transparently.

- Threads of execution, often shortened to `threads`, are the objects of activity within the process. Each thread includes a unique program counter, process stack, and set of processor registers.

- Linux has a unique implementation of threads: **It does not differentiate between threads and processes**. To Linux, **a thread is just a special kind of process**.

- On modern OSs, processes provide two virtualizations: a virtualized processor and virtual memory.
  - The virtual processor gives the process the illusion that it alone monopolizes the system, despite possibly sharing the processor among hundreds of other processes.
  - Virtual memory lets the process allocate and manage memory as if it alone owned all the memory in the system.

- Interestingly, note that threads share the virtual memory abstraction, whereas each receives its onw virtualized processor.

- A process begins its life when, not surprisingly, it is created. In Linux, this occurs by means of the `folk()` system call, which creates a new process by duplicating an existing one. The process that calls `fork()` is the `parent`, whereas the new process is the `child`. The parent resumes execution and child starts execution at the same place: where the call to `folk()` return. The `fork()` system call returns from the kernel twice: once in the parent process and again in the newborn child.

- Often, immediately after a fork it is desirable ro execute a new, different program. The `exec()` family of functions call creates a new address space and loads a new program into it.
  - NOTE: `folk()` is actually implemented via the `clone()` system call.

- Finally, a program exits via the `exit()` system call. This function terminates the process and free it resources. When a process exits, it is placed into a special zombie state the represents terminated processes until the parent calls `wait()` or `waitpid()`.

- NOTE: Another name for a process is a *task*. The Linux kernel internally refers to processes as tasks.

#### 3.1.1. Process Descriptor and Task Structure

- The kernel stores the list of processes in a circular doubly linked list called the `task list`. Each element in the task list is a `process descriptor` of the type `struct task_struct`, which is defined in `<linux/sched.h>`. The process descriptor contains all the information about a specific process.

- The `task_struct` is a relatively large data structure, at around 1.7 kilobytes on a 32 bit machine. This size, however, is quite small considering that the structure contains all the information that the kernel has and needs about a process. The process descriptor contains the data that describes the executing program -- open files, the process's address space, pending signals, the process state, and much more.

#### 3.1.2. Allocating the Process Descriptor

- The `task_struct` structure is allocated via the `slab allocator` to provide object reuse and cache coloring. Prior to the 2.6 kernel series, `struct task_struct` was stored at the end of the kernel stack of each process. This allowed architectures with few registers, such as x86, to calculate the location of the process descriptor via the `stack pointer` without using an extra register to store the location.

- With the process descriptor now dynamically created via the `slab allocator`, a new structure, `struct thread_info`, was created that again lives at the bottom of the stack (for stacks that grow down) and at the top of the stack (for stacks that grow up).

- The `thread_info` structure is defined on x86 in `<asm/thread_info.h>` as:

```C
struct thread_info {
    struct task_struct      *task;
    struct exec_domain      *exec_domain;
    __u32                   flags;
    __u32                   status;
    __u32                   cpu;
    int                     preempt_count;
    mm_segment_t            addr_limit;
    struct restart_block    restart_block;
    void                    *sysenter_return;
    int                     uaccess_err;
}
```

- Each task's `thread_info` structure is allocated at the end of its stack. The `task` element of the structure is a pointer to the task's actual `task_struct`.

#### 3.1.3. Storing the process descriptor

- The system identifies processes by a unique `process identification` value or ID. The PID is a numerical value represented by the opaque type `pid_t`,  which is typically an `int`.

- Inside the kernel, tasks are typically referenced directly by a pointer to their `task_struct` structure. In fact, most kernel code that deals with processes works directly with `struct task_struct`. Consequently, it is useful to be able to quickly look up the process descriptor of the currently executing task, which is done via the `current` macro.

#### 3.1.4. Process State

- The `state` field of the process descriptor describes the current condition of the process. Each process on the system is in exactly one of five different states. This value is represented by one of five flags:

- **TASK_RUNNING** -- The process is runnable; It is either currently running or on a run-queue waiting to run. This is the only possible state for a process executing in user-space; it can also apply to a process in kernel-space that is actively running.

- **TASK_INTERRUPTIBLE** -- The process is sleeping (that is, it is blocked), waiting for some condition to exist.

- **TASK_UNINTERRUPTIBLE** -- This state is identical to **TASK_INTERRUPTIBLE** except that it does not wake up and become runnable if it receives a signal. This is used in situations where the process must wait without interruption or wne the event is expected to occurs quite quickly. Because the task does not respond to signals in this state, `TASK_UNINTERRUPTIBLE` is less often used than `TASK_INTERRUPTIBLE`.

- **__TASK_TRACED** -- The process is being traced by another process, such as a debugger, via `ptrace`.

- **__TASK_STOPPED** -- Process execution has stopped, the task is not running nor is it eligible to run. This occurs if the task receives the SIGSTOP, SIGTSTP, SIGTTIN, or SIGTTOUT signal or if it receives any signal while it is being debugged.

#### 3.1.5. Manipulating the current process state

- Kernel code often needs to change a process's state. The preferred mechanism is using:

```C
set_task_state(task, state); /* set task `task` to state `state`. */
```

- This function sets the given task to the given state. If applicable, it also provides a memory barrier to force ordering on other processors. (This is only needed on SMP systems). Otherwise, it is equivalent to:

```C
task->state = state;
```

- The method `set_current_state(state)` is synonymous to `set_task_state(current, state)`.

#### 3.1.6. Process Context

- One of the most important parts of a process is the executing program code. This code is read in from an *executable file*.
- Normal program execution occurs in `user-space`. When a program executes a system call or triggers an exception, it enters `kernel-space`. At this point, the kernel is said to be `executing on behalf of the process` and is in `process context`. When in process context, the `current` macro is valid. Upon exiting the kernel, the process resumes execution in user-space, unless a higher-priority process has become runnable in the interim, in which case the scheduler is invoked to select the higher priority process.

- System calls and exception handlers are well defined interfaces into the kernel. A process can begin executing in kernel-space only through one of these interfaces -- all access to the kernel is through these interfaces.

#### 3.1.7. The Process Family Tree

- All processes are descendants of the `init` process, whose PID is one. The kernel start `init` in the last step of the boot process. The `init` process, in turn, reads the system `initscripts` and executes more programs, eventually completing the boot boot process.

- Every process on the system has exactly one parent. Likewise, every process has zero or more children. Processes that are all direct children of the same parent are called `siblings`.

- The relationship between processes is stored in the process descriptor. Each `task_struct` has a pointer to the parent's `task_struct`, named `parent`, and a list of children, named `children`.

- Consequently, given the current process, it is possible to obtain the process descriptor of its parent with the following code:

```C
struct task_struct *my_parent = current->parent;
```

- Similarly, it is possible to iterate over a process's children with:

```C
struct task_struct *task;
struct list_head *list;

list_for_each(list, &current->children) {
    task = list_entry(list, struct task_struct, sibling);
    /* Task now points to one of current's children. */
}
```

- The `init` task's process descriptor is statically allocated as `init_task`. A good example of the relationship between all processes is the fact that this code will always succeed:

```C
struct task_struct *task;
for (task = current; task != &init_task; task = task->parent);
/* Task now points to init. */
```

### 3.2. Process Creation

- Process creation in Unix is unique. Most OSs implement a `spawn` mechanism to create a new process in a new address space, read in an executable, and begin executing it.
- UNIX takes the unusual approach of separating these steps into two distinct functions: `fork()` and `exec()`.
  - 1. The first, `fork()`, creates a child process that is a copy of the current task. It differs from the parent only in its PID (which is unique), its PPID (parent's PID, which is set to the original process), and certain resources and statistics, such as pending signals, which are not inherited.
  - 2. The second function, `exec()`, loads a new executable into the address space and begins executing it.

- The combination of `fork()` followed by `exec()` is similar to the single function most operating systems provide.

#### 3.2.1. Copy-on-Write

- Traditionally, upon `fork()`, all resources owned by the parent are duplicated and the copy is given to the child. This approach is naive and inefficient in that it copies much data that might otherwise be shared.
  - Worse still, if the new process were to immediately execute a new image, all that copying would go to waste.
  - In Linux, `fork()` is implemented through the use of **copy-on-write** pages. Copy-on-write (or COW) is a technique to delay or altogether prevent copying of the data. Rather than duplicate the process address space, the parent and the child can share a single copy.

- The data, however, is marked in such a way that if it is written to, a duplicate is made and each process receives a unique copy. Consequently, the duplication of resources occurs only when they are written. until then, they are shared read-only.
  - This technique delays the copying of each page in the address space until it is actually written to. In the case that the pages are never written -- for example, if `exec()` is called immediately after `fork()` -- they never need to be copied.

- The only overhead incurred by `fork()` is the duplication of the parent's page tables and the creation of a unique process descriptor for the child. In the common case that a process executes a new executable image immediately after forking, this optimization prevents the wasted copying of large amounts of data.

#### 3.2.2. Forking

- Linux implements `fork()` via the `clone()` system call. This call takes a series of flags that specify which resources, if any, the parent and child process should share.
- The `fork()`, `vfork()`, and `__clone()` library calls all invoke the `clone()` system call with the requisite flags. The `clone()` system call, in turn, calls `do_fork()`.

- The bulk of the work in forking is handled by `do_fork()`, which is defined in `kernel/fork.c`. This function calls `copy_process()` and then starts the process running.
- The interesting work is done by `copy_process()`:
  - 1. It calls `dup_task_struct()`, which creates a new kernel stack, `thread_info` structure, and `task_struct` for the new process. The new values are identical to those of the current task. At this point, child and parent process descriptors are identical.
  - 2. It then checks that the new child will not exceed the resource limits on the number of processes for the current user.
  - 3. The child needs to differentiate itself from its parent. Various members of the process descriptor are cleared or set to initial values. Members of the process descriptor not inherited are primarily statistically information. The bulk of the values in `task_struct` remain unchanged.
  - 4. The child's state is set to `TASK_UNINTERRUPTIBLE` to ensure that it does not yet run.
  - 5. `copy_process()` calls `copy_flags()` to update the flags member of the `task_struct.`
  - 6. It calls `alloc_pid()` to assign an available PID to the new task.
  - 7. Depending on the flags passed to `clone()`, `copy_process()` either duplicates or shares open files, filesystem information, signal handlers, process address space, and namespace. These resources are typically shared between threads in a given process; otherwise they are unique and this copied here.
  - 8. Finally, `copy_process()` cleans up and returns to the caller a pointer to the new child.

- Back in `do_fork()`, if `copy_process()` returns successfully, the new child is woken up and run.

### 3.3. The Linux Implementation of Threads

- Threads are a popular modern programming abstraction. They provide multiple threads of execution within the same program in a shared memory address space. They can also share open files and other resources. Threads enable `concurrent programming` and, on multiple processor systems, true `parallelism`.

- Linux has a unique implementation of threads. **To the Linux Kernel, there is no concept of a thread**. Linux implements all threads as standard processes. The Linux Kernel does not provide any special scheduling semantics or data structures to represent threads.

- Instead, a thread is merely a process that shares certain resources with other processes. Each thread has a unique `task_struct` and appears to the kernel as a normal process -- threads just happen to share resources, such as an address space, with other processes.

#### 3.3.1. Creating Threads

- Threads are created the same as normal tasks, with the exception that the `clone()` system call is passed flags corresponding to the specific resources to be shared:

```C
clone(CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND, 0);
```

- The previous code results in behavior identical to a normal `fork()`, except that the address space, filesystem resources, file descriptors and signal handlers are shared. In other words, the new task and its parent are what are popularly called *threads*.
- In contrast, a normal `fork()` can be implemented as:

```C
clone(SIGCHLD, 0 );
```

- The flags provided to `clone()` help specify the behavior of the new process and detail what resources the parent and child will share.

|Flags              | Meaning                                                   |
|-------------------|-----------------------------------------------------------|
|CLONE_FILES        | Parent and child share open files.                        |
|CLONE_FS           | Parent and child share file system information.           |
|CLONE_IDLETASK     | Set PID to zero (used only by the idle tasks).            |
|CLONE_NEWNS        | Create a new namespace for the child.                     |
|CLONE_PARENT       | Child is to have same parent as its parent.               |
|CLONE_PTRACE       | Continue tracing child.                                   |
|CLONE_VM           | Parent and child share address space.                     |
|CLONE_SIGHAND      | Parent and child share signal handlers and blocked signals|
|...                | ...                                                       |

#### 3.3.1. Kernel Threads

- It is often useful for the kernel to perform some operations in the background. The kernel accomplished this via `kernel threads` -- standard processes that exist solely in kernel space. The significant difference between kernel threads and normal processes is that kernel threads do not have an address space.

- They operate only in kernel-space and do not context switch into user-space. Kernel threads, however, are schedulable and preemptable, the same as normal process.

- Some kernel threads: `flush` tasks and the `ksoftirqd` task. You can check kernel thread with command: `ps -ef`. There are a lot of them! Kernel threads are created on system boot by other kernel threads. Indeed, a kernel thread can be created only by another kernel thread. The kernel handles this automatically by forking all new kernel threads off of the `kthreadd` kernel process. The interface, declared in `<linux/kthread.h>`:

```C
struct task_struct *kthread_create(int (*threadfn)(void *data),
                                    void *data,
                                    const char namefmt[],
                                    ...)
```

- The new task is created via the clone() system call by the `kthread` kernel process.

- The process is created in an unrunnable state; it will not start running untl explicitly woken up via `wake_up_process()`. A process can be created and made runnable with a single function, `kthread_run()`:

```C
struct task_struct *kthread_run(int (*threadfn)(void *data),
                                void *data,
                                const char namefmt[],
                                ...)
```

- This routine, implemented as a macro:

```C
#define kthread_run(threadfn, data, namefmt, ...)               \
({                                                              \
    struct task_struct *k;                                      \
                                                                \
k = kthread_create(threadfn, data, namefmt, ## __VA_ARGS__);    \
if (!IS_ERR(k))                                                 \
    wake_up_process(k);                                         \
k;                                                              \
})
```

- When started, a kernel thread continues to exist until it calls `do_exit()` or another part of the kernel calls `kthread_stop()`, passing in the address of the `task_struct` structure returned by `kthread_create()`:

```C
int kthread_stop(struct task_struct *k)
```

### 3.4. Process Termination

- It is sad, but eventually processes must die. When a process terminates, the kernel releases the resources owned by the process and notifies the child's parent of its demise.

- the destruction occurs when the process calls the `exit()` system call, either explicitly when it is ready to terminate or implicitly on return from the main subroutine of any program. (That is, the C compiler places a call to `exit()` after main() returns.)

- A process can also terminate involuntarily.This occurs when the process receives a signal or exception it cannot handle or ignore.

- Regardless of how a process terminates, the bulk of the work is handled by `do_exit()`, defined in `kernel/exit.c`, which completes a number of chores:

- 1. It sets the `PF_EXITING` flag in the `flags` member of the `task_struct`.
- 2. It calls `del_timer_sync()` to remove any kernel timers. Upon return, it is guaranteed that no timer is queued and that no timer handler is running.
- 3. If BSD process accounting is enabled, `do_exit()` calls `acct_update_integrals()` to write out accounting information.
- 4. It calls `exit_mm()` to release the `mm_struct` held by this process. If no other process is using this address space -- that it, if the address is not shared -- the kernel then destroys it.
- 5. It calls `exit_sem()`. If the process is queued waiting for an IPC semaphore, it is dequeued here.
- 6. It then calls `exit_files()` and `exit_fs()` to decrement the usage count of objects related to file descriptors and filesystem data. If either usage counts reach zero, the object is no longer in use by any process, and it is destroyed.
- 7. It sets the tasks's exit code stored in the `exit_code` member of the `task_struct`, to the code provided by `exit()` or whatever kernel mechanism forced the termination. The exit code is stored here for optional retrieval by the parent.
- 8. It calls `exit_notify()` to send signals to the task's parent, re-parents any of the task's children to another thread in their thread group or the init process, and sets the task's exit state, stored in `exit_state` in the `task_struct` structure, to EXIT_ZOMBIE.
- 9. `do_exit()` Call `schedule()` to switch to a new process. Because the process is now not schedulable, this is last code the task will ever execute. `do_exit()` never returns.

- At this point, the task is not runnable (and no longer has an address space in which to run) and is in the `EXIT_ZOMBIE` exit state. The only memory it occupies is its kernel stack, the `thread_info` structure, and the `task_struct` structure.

#### 3.4.1. Removing the Process Descriptor

- After `do_exit()` completes, the process descriptor for the terminated process still exists, but the process is a zombie and is unable to run (this enables system to obtain information about a child process after it has terminated).

- Consequently, the acts of cleaning up after a process and removing its process descriptor are separate. After the parent has obtained information on its terminated child, or signified to the kernel that it does not care, the child's `task_struct` is de-allocated.

- The `wait()` family of functions are implemented via a single system call, `wait4()`. The standard behavior is to suspend execution of the calling task until one of its children exits, at which time the function returns with the PID of the exited child.

- When it is time to finally de-allocate the process descriptor, `release_task()` is invoked. It does the following:

1. It calls `__exit_signal()`, which calls `__unhash_process()`, which in turns call `detach_pid()` to remove the process from the pidhash and remove the process from task list.

2. `__exit_signal()` releases any remaining resources used by the now dead process and finalizes statistics and bookkeeping.
3. If the task was last member of a thread group , and the leader is a zombie, then `release_task()` notifies the zombie leader's parent.
4. `release_task()` calls `put_task_struct()` to free the pages containing the process's kernel stack and `thread_info` structure and de-allocate the slab cache containing the `task_struct`.

- At this point, the process descriptor and all resources belonging solely to the process have been freed.

## 4. Process Scheduling

- **Process Scheduler**, the kernel subsystem that puts those processes to work.

- The process scheduler decides which process runs, when and for how long. The process scheduler divides the finite resources of processor time between the runnable processes on a system.

- By deciding which process runs next, the scheduler is responsible for best utilizing the system and giving users the impression that multiple processes are executing simultaneously.

- The idea behind the scheduler is simple. To best utilize processor time, assuming there are runnable processes, a process should always be running. If there are more runnable processes than processors in a system, some processes will not be running at a given moment.These processes are *waiting to run*. Deciding which process runs next, given a set of runnable processes, is the fundamental decision that the scheduler must make.

### 4.1. Multitasking

- A `multitasking` OS is one that can simultaneously interleave execution of more than one process. On single processor machines, this gives the illusion of multiple processes running concurrently. On multiprocessor machines, such functionality enables processes to actually run concurrently, in parallel, on different processors.

- On either type of machine, it also enables many processes to *block* or *sleep*, not actually executing until work is available. These processes, although in memory, are not **not runnable**. Consequently, a modern Linux system can have many processes in memory but, say, only one in a runnable state.

- Multitasking OSs come in two flavors:
  - 1. **cooperative multitasking**.
  - 2. **preemptive multitasking**.

- Linux, like all UNIX variants and most modern OSs, implements **preemptive multitasking**.

- The act of involuntarily suspending a running process is called **preemption**.
- The time a process runs before it is preempted usually predetermined, and it is called the **time-slice** of the process.
  - The time-slice, in effect, gives each runnable process a **slice** of the processor's time.

- Managing the time-slice enables the scheduler to make global scheduling decisions for the system. It also prevents any one process from monopolizing the processor. On many modern OSs, the time-slice is dynamically calculated as a function of process behavior and configurable **system policy**.

- COnversely, in **cooperative multitasking**, a process does not stop running until it voluntary decides to do so. The act of a process voluntarily suspending itself is called **yield**.

### 4.2. Linux's Process Scheduler

### 4.3. Policy

- Policy is the behavior of the scheduler that determines what runs when. A scheduler's policy often determines the overall feel of a system and is responsible for optimally utilizing processor time. Therefore, it is very important.

#### 4.3.1. I/O Bound Versus Processor-Bound Processes

## 5. System calls

- In any modern OS, the kernel provides a set of interfaces by which processes running in user-space can interact with the system. These interfaces give applications controlled access to hardware, a mechanism with which to create new processes and communicate with existing ones, and the capability to request other OS resource. The interfaces act as the messengers between applications and the kernel, with the applications issuing various requests and kernel fulfilling them (or returning an error). The existence of these interfaces, and the fact that applications are not free to directly do whatever they want, is key to providing a stable system.

### 5.1. Communicating with the Kernel

- System calls provide a layer between the hardware and user-space processes.

- This layer serves three primary purposes:
  - 1. First, it provides an abstracted hardware interface for user-space. When reading or writing from a file, for example, applications are not concerned with the type of disk, media, or even the type of filesystem on which the file resides.
  - 2. Seconds, system calls ensure system security and stability. With the kernel acting as a middle-man between system resources and user-space, the kernel can arbitrate access based on permissions, users, and other criteria.
    - For example, this arbitration prevents applications from incorrectly using hardware, stealing other processes's resources/
  - 3. Finally, a single common layer between user-space and the rest of the system allows for the virtualized system provided to processes.

- In Linux, system calls are the only means user-space has of interfacing with the kernel; they are the only legal entry point into the kernel other than exceptions and traps. Indeed, other interfaces, such as device files or `/proc`, are ultimately accessed via system calls.

### 5.2. APIs, POSIX, and the C Library

- Typically, applications are programmed against an Application Programming Interface (API) implemented in user-space, not directly to system calls.

- The relationship between applications, the C library, and the kernel with a call to **printf()**.

Call to printf() ---> printf() in C -> write() in C lib ---> write() system call.
|Application|-------> |             C library         |----> |Kernel|

- One of the more common application programming interfaces in the UNIX world is based on the POSIX standard. Technically, POSIX is composed of a series of standards from IEEE that aim to provide a portable OS standard roughly based on UNIX.

- POSIX is an excellent example of the relationship between APIs and system calls. On most Unix systems, the POSIX-defined API calls have a strong correlation to the system calls.

- The system call interface in Linux, as with most Unix systems, is provided in part by the C library. The C library implements the main API on Unix systems, including the standard C library and the system call interface. The C library is used by all C programs and, because of C's nature, is easily wrapped by other programming languages for use in their program. The C library additionally provides the majority of the POSIX API.

### 5.3. Syscalls

- System calls are typically accessed via function calls defined in the C library.
- The C library, when a system call return an error, writes a special error code into the global `errno` variable. This variable can be translated into human-readable errors via library functions such as `perror()`.

- Finally, system calls have a defined behavior. For example, the system call `getpid()` is defined to return an integer that is the current process's PID. The implementation of this `syscall` in the kernel is simple:

```C
SYSCALL_DEFINE0(getpid)
{
    return task_tgid_vnr(current); // returns current->tgid
}
```

- **SYSCALL_DEFINE0** is simply a macro that defines a system call with no parameters (hence the 0). The expanded code looks like this:

```C
asmlinkage long sys_getpid(void)
```

- NOTE:
  - 1. The `asmlinkage` modifier on the function definition. This is a directive to tell the compiler to look only on the stack for this function's arguments. This is a required modifier for all system calls.
  - 2. The function return a `long`. For compatibility between 32- and 64-bit systems, system calls defined to return an `int` in user-space return a `long` in the kernel.
  - 3. note that the `getpid()` system call is defined as `sys_getpid()` in the kernel. This is the naming convention taken with all system calls in LINUX: System call `bar()` is implemented in the kernel as function `sys_bar()`.

#### 5.3.1. System call numbers

- In Linux, each system call is assigned a `syscall number`. This is unique number that is used to reference a specific system call. When a user-space process executes a system call, the syscall number identifies which syscall was executed; the process does not refer to the syscall by name.

- The kernel keeps a list of all registered system calls in the system call table, store in `sys_call_table`. This table is architecture; on x86-64 it is defined in `arch/i386/kernel/syscall_64.c`.

#### 5.3.2. System call performance

- System calls in Linux are faster than in many other OSs. This is partly because of Linux's fast context switch times; entering and exiting the kernel is a streamlined and simple affair. The other factor is the simplicity of the system call handler and the individual system calls themselves.

### 5.4. System call handler

- It is not possible for user-space applications to execute kernel code directly. They cannot simply make a function call to a method existing in kernel-space because the kernel exists in a protected memory space. If applications could directly read and write to the kernel's address space, system security and stability would be nonexistent.

- Instead, user-space applications must somehow signal to the kernel that they want to execute a system call and have the system switch to kernel mode, where the system call can be executed in kernel-space by the kernel on behalf of the application.

- The mechanism to signal the kernel is a software interrupt: **incur an exception, and the system will switch to kernel mode and execute the exception handler**. The exception handler, in this case is actually the **system call handler**.

- The defined software interrupt on x86 is interrupt number 128, which is incurred via the `int $0x80` instruction. It triggers a switch to kernel mode and the execution of exception vector 128, which is the system call handler.

- The system call handler is the aptly named function `system_call()`. It is architecture-dependent; on x86-64 it is implemented in assembly in `entry_64.S`.

#### 5.4.1. Denoting the correct system call

- The `system_call()` function checks the validity of the given system call number by comparing it to `NR_syscalls`. If it is larger than or equal to `NR_syscalls`, the function return `-ENOSYS`. Otherwise, the specified system call is invoked:

```C
call *sys_call_table(, %rax, 8)
```

- Because each element in the system call table is 64 bits (8 bytes), the kernel multiplies the given system call number by four to arrive at its location in the system call table. On x86-32, the code is similar, with the 8 replaced by 4.

#### 5.4.2. Parameters Passing

- In addition to the system call number, most syscalls require that one or more parameters be passed to them. Somehow, user-space must relay the parameters to the kernel during the trap. The easiest way to do this is via the same means that the syscall number is passed: **The parameters are stored in registers**. On x86-32, the registers **ebx, ecx, edx, esi, and edi** contain, in order, the first five arguments.

### 5.5. System Call Implementation

- The hard work lies on designing and implementing the system call; registering it with the kernel is simple.

#### 5.5.1. Implementing System Calls

- The first step in implementing a system call is defining its purpose. What will it do? the system call should have exactly one purpose.
  - Multiplexing syscalls ( a single system call that does wildly different things depending on a flag argument) is discouraged in Linux. Look at `ioctl()` as an example of what **NOT** to do.

#### 5.5.2. Verifying the Parameters

- System calls must carefully verify all their parameters to ensure that they are valid and legal. The system call runs in kernel-space, and if the user can pass invalid input into the kernel without restraint, the system's security and stability can suffer.

- One of the most important checks is the validity of any pointers that the user provides. Before following a pointer into user-space, the system must ensure that:
  - 1. The pointer points to a region of memory in user-space.
  - 2. The pointer points to a region of memory in the process's address space.
  - 3. If reading, the memory is marked readable. If writing, the memory is marked
writable. If executing, the memory is marked executable. The process must not be able to bypass memory access restrictions.

- The kernel provides two methods for performing the requisite checks and the desired copy to and from user-space. NOTE kernel code must never blindly follow a pointer into user-space! One of these two methods must always be used.
  - 1. For writing into user space: `copy_to_user()`.
  - 2. For reading from user space: `copy_from_user()`.

- Both of them may block. This occurs, for example, if the page containing the user data is not in physical memory but ins swapped to disk. In that case, the **process sleeps until the page fault handler can bring the page from the swap file on disk into physical memory**.

- A final possible check is for valid permission: for example `suser()` check user was root.
- The new system enables specific access checks on specific resources.A call to capable() with a valid capabilities flag returns nonzero if the caller holds the specified capability and zero otherwise.

### 5.6. System call context

- As we known, the kernel is in process context during the execution of a system call. The `current` pointer points to the current task, which is the process that issued the syscall.

- In process context, the kernel is capable of sleeping (for example, if the system call blocks on a call or explicit call `schedule()`) and is fully preemptible. These two points are important:
  - First, the capability to sleep means that system calls can make use of the majority of the kernel's functionality.
  - Because the new task may then execute the same system call, care must be exercised to ensure that system calls are reentrant.

- When the system call returns, control continues in `system_call()`, which ultimately switches to user-space and continues the execution of the user process.

#### 5.6.1. Final steps in binding a system call

- After the system all is written, it is trivial to register it as an official system call:
  1. Add an entry to the end of the system call table.
  2. For each supported architecture, define the system call number in `<asm/unistd.h>`.
  3. Compile the syscall into the kernel image.

- For example table in `entry.S`:

```asm
ENTRY(sys_call_table)
        .long sys_restart_syscall /* 0 */
        .long sys_exit
        .long sys_fork
        .long sys_read
        .long sys_write
        .long sys_open /* 5 */
        /* ... */
        .long sys_eventfd2
        .long sys_epoll_create1
        .long sys_dup3
        .long sys_pipe2
        .long sys_inotify_init1
        .long sys_preadv
        .long sys_pwritev
        .long sys_rt_tgsigqueueinfo /* 335 */
        .long sys_perf_event_open
        .long sys_recvmmsg

        /* Add out system call to the tail of this list. */
        .long sys_our_syscall
```

- Next, the system call number is added to `<asm/unistd.h>`, which currently looks
somewhat like this:

```C
/*
* This file contains the system call numbers.
*/
#define __NR_restart_syscall 0
#define __NR_exit 1
#define __NR_fork 2
#define __NR_read 3
#define __NR_write 4
#define __NR_open 5
// ...
#define __NR_signalfd4 327
#define __NR_eventfd2 328
#define __NR_epoll_create1 329
#define __NR_dup3 330
#define __NR_pipe2 331
#define __NR_inotify_init1 332
#define __NR_preadv 333
#define __NR_pwritev 334
#define __NR_rt_tgsigqueueinfo 335
#define __NR_perf_event_open 336
#define __NR_recvmmsg 337

// The following is then added to the end of the list:
#define __NR_our_syscall 338
```

- Finally, implement the syscall in some source file, for example: `kernel/sys.c`:

```C
#include <asm/page.h>
/*
* sys_our_syscall – everyone’s favorite system call.
*
* Returns the size of the per-process kernel stack.
*/
asmlinkage long sys_our_syscall(void)
{
return THREAD_SIZE;
}
```

#### 5.6.2. Accessing the System Call from User-Space

- Generally, the C library provides support for system calls.
- Thankfully, Linux provides a set of macros for wrapping access to system calls. It sets up the register contents and issues the trap instructions. These macros are named `_syscalln()`, where `n` is between `0` and `6`. The number corresponds to the number of parameters passed into syscall. Because the macro needs to know how many parameters to expect and, consequently, push into registers. For example, consider the system call `open()`, defined as:

```C
long open(const char *filename, int flags, int mode)
```

- The syscall macro to use this system call without explicit library support would be:

```C
#define __NR_open 5
_syscall3(long, open, const char *, filename, int, flags, int, mode)
```

- Then, the application can simply call `open()`.
- For each macro, there are `2 + 2*n` parameters.
  - First is return type: `long`.
  - Second is name: `open`.
  - Next follows the type and name for each parameter in order of the system call.
- The `__NR_open` macro define is in `<asm/unistd.h>;` is syscall number.
- Macro `_syscall3` expands into a C function with inline assembly; the assembly performs the steps in the previous section to push the syscall number and parameters into the correct registers and issue the software interrupt to trap into the kernel.

- In application, we do like this:

```C
#define __NR_our_syscall 283
__syscall0(long, our_syscall)
int main ()
{
    long stack_size;
    stack_size = our_syscall();
    printf ("The kernel stack size is %ld\n", stack_size);
    return 0;
}
```

#### 5.6.3. Why not to implement a System Call

- Pros:
  - 1. syscall are simple to implement.
  - 2. IN linux, syscall performance is fast.
- Cons:
  - 1. need a syscall number.
  - 2. kernel is stable.
  - 3. Each arch needs to separately register the system call and support it.
  - 4. Not easy to call raw syscall.
- The alternatives:
  - 1. Using `ioctl()` with kernel module.
  - 2. Certain interfaces, such as semaphores.
  - 3. Add the info as a file to the appropriate location in `sysfs`.

## 7. Interrupts and Interrupt Handlers

- **A core responsibility of any OS kernel is managing the hardware connected to the machine** -- hard drives and Blu-ray discs, keyboard and mice, 3D processors and wireless radios. To meet this responsibility, the kernel needs to communicate with the machine's individual devices.

- How can the processor work with hardware without impacting the machine's overall performance?
  - One answer to this question is **polling**.
  - Periodically, the kernel can check the status of the hardware in the system and respond accordingly. Polling incurs overhead, however, because it must occur repeatedly regardless of whether the hardware is active or ready.

- A better solution is to provide a mechanism for the hardware to signal to the kernel when attention is needed. This mechanism is called an **interrupt**.

### 7.1. Interrupts

- Interrupts enable hardware to signal to the processor. For example, as you type, the keyboard controller (the hardware device that manages the keyboard) issues an electrical signal to the processor to alert the OS to newly available key presses. These electrical signals are interrupts.

- The processor receives the interrupt and signals the OS to enable the OS respond to the new data. Hardware devices generate interrupts asynchronously with respect to the processor clock -- they can occur any time. Consequently, the kernel can be interrupted at any time to process interrupts.

- An interrupt is physically produced by electronic signals originating from hardware devices and directed into input pins on an interrupt controller, a simple chip that multiplexes multiple interrupt lines into a single line to the processor. **Upon receiving an interrupt, the interrupt controller sends a signal to the processor**. The processor detects this signal and interrupts its current execution to handle the interrupt. The processor can then notify the OS that an interrupt has occurred, and the OS can handle the interrupt appropriately.

- Different devices can associated with different interrupts by means of a unique value associated with each interrupt. This way, interrupts from the keyboard are distinct from interrupts from the hard drive. This enables the OS to differentiate between interrupts and to know which hardware device caused which interrupt. In turn, the OS can service each interrupt with its corresponding handler.

- These interrupt values are often called **interrupt request (IRQ)** lines. Each IRQ line is assigned a numeric value -- for example, on the classic PC, IRQ zero is the timer interrupt and IRQ one is the keyboard interrupt.

- Not all interrupt numbers, however, are so rigidly defined. Interrupts associated with devices on the PCI bus, for example, generally are dynamically assigned. Other non-PC architectures have similar dynamic assignments for interrupt values. The important notion is that a specific interrupt is associated with a specific device, and the kernel knows this. The hardware then issues interrupts to get the kernel's attention: **Hey, I have new key presses waiting! Read and process these bad boys!**

#### 7.1.1. Exceptions

- In OS texts, **exceptions** are often discussed at the same time as interrupts. Unlike interrupts, exceptions occur synchronously with respect to the processor clock. Indeed, they are often called **synchronous interrupts**. Exceptions are produced by the processor while executing instructions either in response to a programming error (for example, divide by zero) or abnormal conditions that must be handled by the kernel (for example, a page fault).

- Because many processor architectures handle exceptions in a similar manner to interrupts, the kernel infrastructure for handling the two is similar. Much of the discussion of interrupts (asynchronous interrupts generated by hardware).

### 7.2. Interrupt Handlers

- The function the kernel runs in response to a specific interrupt is called an **interrupt handler** or **interrupt service routine(ISR)**. Each device that generates interrupts has an associated interrupt handler. For example, one function handles interrupts from the system timer, whereas another function handles interrupts generate by the keyboard. The interrupt handler for a device is part of the device's **driver**, the kernel code that manages the device.

- In Linux, interrupt handlers are normal C functions. They match a specific prototype, which enables the kernel to pass the handler information in a standard way, but otherwise they are ordinary functions. What differentiates interrupt handlers from other kernel functions is that the kernel invokes them in response to interrupts and that they run in a special context called **interrupt context**. This special context occasionally called **atomic context** because, as we shall see, code executing in this context is unable to block.

- Because an interrupt can occur at anytime, an interrupt handler, can, in turn, be executed at any time. It is imperative that the handler runs quickly, to resume execution of the interrupted code as soon as possible. Therefore, while it is important to the hardware that the OS services the interrupt without delay, it is also important to the rest of the system that the interrupt handler executes in as short a period as possible.

- At the very least, an interrupt handler's job is to acknowledge the interrupt's receipt to the hardware: **Hey, hardware, I hear ya; now get back to work!** Often, however, interrupt handlers have a large amount of work to perform. For example, consider the interrupt handler for a network device. On top of responding to the hardware, the interrupt handlers needs to copy networking packets from the hardware into memory, process them, and push the packets down to the appropriate protocol stack or application. Obviously, this can be a lot of work, especially with today's gigabit and 10-gigabit Ethernet cards.

### 7.3. Top halves Versus Bottom Halves

- These two goals -- **that an interrupt handler execute quickly and perform a large amount of work** -- clearly conflict with one another.

- Because of these competing goals, the processing of interrupts is split into two parts, or halves.
  - 1. The interrupt handler is the **top halves**. The top half is run immediately upon receipt of the interrupt and performs only the work that is time-critical, suc as acknowledging receipt of the interrupt or resetting the hardware.
  - 2. Work that can be performed later is deferred until the **bottom halves**. The bottom halves runs in the future, at a more convenient time, with all interrupts enabled.

- The Linux provide various mechanisms for implementing bottom halves, we will discuss more in chapter 8.

#### 7.3.1. Example of the top-half/bottom-half dichotomy

- When network cards, receives packets from the network, they need to alert the kernel of their available. They want and need to do this immediately, to optimize network throughput and latency and avoid timeouts. Thus they immediately issue an interrupt: **Hey, kernel, I have some fresh packets here!**. The kernel responds by executing the network card's registered interrupt.

- The interrupt runs, acknowledges the hardware, copies the new networking packets into main memory, and readies the network card for more packets.
  - These jobs are important, time-critical, and hardware-specific work. The kernel generally needs to quickly copy the networking packet into main memory because the network data buffer on the networking card is fixed and miniscule in size, particularly compared to main memory.
  - Delays in copying the packets can result in a buffer overrun, with incoming packets overwhelming the networking card's buffer and thus packets being dropped.
  - After the networking data is safely in the memory, the interrupt's job is done, and it can return control of the system to whatever code was interrupted when the interrupt was generated.
- The rest of the processing and handling of the packets occurs later, in the bottom half.

- In this chapter, we look at the top half; in the next chapter, we study the bottom.

### 7.4. Registering an interrupt handler

- **Interrupt handlers are the responsibility of the driver managing the hardware**. Each device has one associated driver and, if that device uses interrupts (and most do), then that driver must register one interrupt handler.

- Drivers can register an interrupt handler and enable a given interrupt line for handling with the function `request_irq()`, which is declared in `<linux/interrupt.h>`:

```C
/* request_irq: allocate a given interrupt line. */
int request_irq(unsigned int irq,
                irq_handler_t handler,
                unsigned long flags,
                const char *name,
                void *dev)
```

- The first parameter: `irq`, specifies the interrupt number to allocate. For some devices, for example legacy PC devices such as the system timer or keyboard, this value is typically hard-coded. For most other devices, it is probed or otherwise determined programmatically and dynamically.

- Second parameter, `handler` is a function pointer to the actual interrupt handler that services this interrupt.

```C
typedef irqreturn_t (*irq_handler_t)(int, void *);
```

- Note the specific prototype of the handler function: It takes two parameters and has a return value of `irqreturn_t`.

#### 7.4.1. Interrupt Handler Flags

- Flags defined in `<linux/interrupt.h>`. Among these flags, the most important are:
  - 1. `IRQF_DISABLED` -- disable all interrupts when executing this interrupt handler.
  - 2. `IRQF_SAMPLE_RANDOM` -- Specifies that interrupts generated by this device should contribute to the kernel entropy. The kernel entropy pool provides truly random numbers derived from various random events.
  - 3. `IRQF_TIMER` -- This flag specifies that this handler process interrupts for the system timer.
  - 4. `IRQF_SHARED` -- This flag specifies that the interrupt line can be shared among multiple interrupt handler.

- The `name` parameter is an ASCII text representation of the device associated with the interrupt. These text names are used by `/proc/irq` and `/proc/interrupts` for communication with the user.

- The fifth parameter, dev, is used for shared interrupt lines

#### 7.4.2. An interrupt example

- In a driver, requesting an interrupt line and installing a handler is done via `request_irq()`:

```C
if (request_irq(irqn, my_interrupt, IRQF_SHARED, "my_device", my_dev)) {
    printk(KERN_ERR "my_device: cannot register IRQ %d\n", irqn);
    return -EIO;
}
```

#### 7.4.3. Freeing an Interrupt handler

- When your driver unloads, you need to unregister your interrupt handler and potentially disable the interrupt line. To do this, call:

```C
void free_irq(unsigned int irq, void *dev);
```

### 7.5 Writing an interrupt handler

- The following is a declaration of an interrupt handler:

```C
static irqreturn_t intr_handler(int irq, void *dev);
```

- Note that this declaration matches the prototype of the `handler` argument given to `request_irq()`.
  - Fist parameter is `irq` is the numeric value of the interrupt line the handler is servicing.
  - The second parameter, `dev`, is a generic pointer to the same `dev` that was given to `request_irq()` when the interrupt handler was registered. If this value is unique, it can act as a cookie to differentiate between multiple devices potentially using the same interrupt handler.

- Because the `device` structure is both unique to each device and potentially useful to have within the handler, it is typically passed for `dev`.

- An interrupt handler can return two special values, `IRQ_NONE` or `IRQ_HANDLED`.

- The role of the interrupt handler depends entirely on the device and its reasons for issuing the interrupt. At a minimum, most interrupt handlers need to provide acknowledgement to the device that they received the interrupt.

#### 7.5.1. Reentrancy and Interrupt handlers

- Interrupt handlers in Linux need not be reentrant. When a given interrupt handler is executing, the corresponding interrupt line is masked out on all processors, preventing another interrupt on the same line from being received. Normally all other interrupts are enabled, so other interrupts are serviced, but the current line is always disabled. Consequently, **the same interrupt handler is never invoked concurrently to service a nested interrupt**. This greatly simplifies writing interrupt handler.

#### 7.5.2. A real life interrupt handler

- Let's look at a real interrupt handler, from the real-time clock (RTC) driver, found in `drivers/char/rtc.c`. An RTC is found in many machines, including PCs. It is a device, separate from the system timer, which sets the system clock, provides an alarm, or supplies a periodic timer.
- On most architectures, the system clock is set by writing the desired time into a specific register or I/O range. Any alarm or periodic timer functionality is normally implemented via interrupt. The interrupt is equivalent to a real-world clock alarm: **The receipt of the interrupt is analogous to a buzzing alarm**.

- When the RTC driver loads, the function `rtc_init()` is invoked to initialize the driver. One of its duties is to register the interrupt handler:

```C
/* register rtc_interrupt on rtc_irq */
if (request_irq(rtc_irq, rtc_interrupt, IRQF_SHARED, "rtc", (void *)&rtc_port)) {
    printk(KERN_ERR "rtc: cannot register IRQ %d\n", rtc_irq);
    return -EIO;
}
```

- On a PC, the RTC is located at IRQ 8. The second parameter is the interrupt handler, `rtc_interrupt`, which is willing to share the interrupt line with other handlers, thanks to the `IRQF_SHARED` flag.

- Finally, the handler itself:

```C
static irqreturn_t rtc_interrupt(int irq, void *dev)
{
    /*
    * Can be an alarm interrupt, update complete interrupt,
    * or a periodic interrupt. We store the status in the
    * low byte and the number of interrupts received since
    * the last read in the remainder of rtc_irq_data.
    */
    spin_lock(&rtc_lock);
    rtc_irq_data += 0x100;
    rtc_irq_data &= ~0xff;
    rtc_irq_data |= (CMOS_READ(RTC_INTR_FLAGS) & 0xF0);
    if (rtc_status & RTC_TIMER_ON)
        mod_timer(&rtc_irq_timer, jiffies + HZ/rtc_freq + 2*HZ/100);
    spin_unlock(&rtc_lock);

    /*
    * Now do the rest of the actions
    */

    spin_lock(&rtc_task_lock);
    if (rtc_callback)
        rtc_callback->func(rtc_callback->private_data);
    spin_unlock(&rtc_task_lock);

    wake_up_interruptible(&rtc_wait);

    kill_fasync(&rtc_async_queue, SIGIO, POLL_IN);

    return IRQ_HANDLED;
}
```

- This function is invoked whenever the machine receives the RTC interrupt.
  - First, note the spin lock calls: The first set ensures that `rtc_irq_data` is not accessed concurrently by another processor on an SMP machine.
  - And the second spin lock set protects `rtc_callback` from the same.

#### 7.5.3. Interrupt context

- When executing an interrupt handler, the kernel is in **interrupt context**. Recall that process context is the mode of operation the kernel is in while it is executing on behalf of a process -- for example, executing a system call or running a kernel thread. In process context, the `current` macro points to the associated task.

- Interrupt context, on the other hand, is not associated with a process. The `current` macro is not relevant (although is points to the interrupted process). Without a backing process, interrupt cannot **SLEEP** -- therefore, you cannot call certain functions from interrupt context.

- Interrupt context is time-critical because the interrupt handler interrupts other code. **Code should be quick and simple**. Busy looping is possible, but discouraged. This is an important point; **always keep in mind that your interrupt handler interrupted other code**. As much as possible, work should be pushed out from the interrupt handler and performed in a bottom half, which runs at a more convenient time.

- The setup of an interrupt handler's stack is a configuration option. Historically, interrupt handlers did not receive their own stacks. Instead, they would share stack of the process that they interrupted.

### 7.6. Implementing Interrupt Handlers

- Perhaps not surprising, the implementation of the interrupt handling system in Linux is architecture-dependent. The implementation depends on the processor, the type of interrupt controller used, and the design of the architecture and machine.

- The path that an interrupt takes from hardware and on through the kernel:

```text

|Keyboard (Hardware)|--generates an interrupt-->|Interrupt controller|
                                                        ||
                                                        \/
        Processor interrupts the kernel<===========|Processor|
            ||
            \/
        |do_IRQ()|===========> Is there an interrupt handler on this line?
                                ||                          ||
                                Yes                         ||
                                ||                          ||
                                \/                          ||
                        |handle_IRQ_event()|                ||
                                ||                          No
                                \/                          ||
                        run all interrupt                   ||
                        handlers on this line.              ||
                                ||                          ||
                                \/                          \/
                                |   ret_from_intr()          |
                                            ||
                                            \/
                                    return to the kernel code
                                    that was interrupted.
```

- A device issues an interrupt by sending an electric signal over its bus to the interrupt controller. If the interrupt line is enabled (they can be masked out), the interrupt controller sends the interrupt to the processor. In most architectures, this is accomplished by an electrical signal sent over a special pin to the processor.

- Unless interrupts are disabled in the processor, the processor immediately stops what it is doing, disables the interrupt system, and jumps to a predefined location in memory and executes the code located here.

- `do_IRQ()` ensures that a valid handler is registered on the line and that it is enabled and not currently executing. If so, it calls `handle_IRQ_event()`, defined in `kernel/irq/handler.c`, to run the installed interrupt handlers for the line.

```C
/**
 * handle_IRQ_event - irq action chain handler
 * @irq: the interrupt number
 * @action: the interrupt action chain for this irq
 *
 * Handles the action chain of an irq event
 */

irqreturn_t handle_IRQ_event(unsigned int irq, struct irqaction *action)
{
    irqreturn_t ret, retval = IRQ_NONE;
    unsigned int status = 0;
    if (!(action->flags & IRQF_DISABLED))
        local_irq_enable_in_hardirq();

    do {
        trace_irq_handler_entry(irq, action);
        ret = action->handler(irq, action->dev_id);
        trace_irq_handler_exit(irq, action, ret);

        switch (ret) {
            case IRQ_WAKE_THREAD:
                /*
                * Set result to handled so the spurious check
                * does not trigger.
                */
                ret = IRQ_HANDLED;
                /*
                * Catch drivers which return WAKE_THREAD but
                * did not set up a thread function
                */
                if (unlikely(!action->thread_fn)) {
                    warn_no_thread(irq, action);
                    break;
                }
                /*
                * Wake up the handler thread for this
                * action. In case the thread crashed and was
                * killed we just pretend that we handled the
                * interrupt. The hardirq handler above has
                * disabled the device interrupt, so no irq
                * storm is lurking.
                */
                if (likely(!test_bit(IRQTF_DIED,
                    &action->thread_flags))) {
                    set_bit(IRQTF_RUNTHREAD, &action->thread_flags);
                    wake_up_process(action->thread);
                }
                
                /* Fall through to add to randomness */
            case IRQ_HANDLED:
                status |= action->flags;
                break;
            default:
                break;
        }

        retval |= ret;
        action = action->next;
    } while (action);

    if (status & IRQF_SAMPLE_RANDOM)
        add_interrupt_randomness(irq);
    
    local_irq_disable();
    return retval;
}
```

- First, because the processor disabled interrupts, they are turned back on unless `IRQF_DISABLED` was specified during the handler's registration. Recall that IRQF_DISABLED specifies that the handler must be run with interrupts disabled.

- Next, each potential handler is executed in a loop. If this line is not shared, the loop terminates after the first iteration. Otherwise, all handlers are executed.

- On x86, the initial assembly routines are located in `arch/x86/kernel/entry_64.S` and the C methods are located in `arch/x86/kernel/irq.c`.

#### 7.6. `/proc/interrupts`

## 8. Bottom halves and deferring Work

- Interrupt handlers are an important -- indeed, required -- part of any OS. Due to various limitations, however, interrupt handlers can form only the first half of any interrupt processing solution. These limitations include:

  - Interrupt handlers run asynchronously and thus interrupt other, potentially important, code, including other interrupt handlers. Therefore, to avoid stalling the interrupted code for too long, interrupt handlers need to run as quickly as possible.
  - Interrupt handlers run with the current interrupt level disabled at best (IRQF_DISABLED is unset), and at worst (if IRQF_DISABLED is set) with all interrupts on the current processor disabled. As disabling interrupts prevents hardware from communicating with the OSs, interrupt handlers need to run as quickly as possible.
  - Interrupt handlers are often timing-critical because they deal with hardware.
  - Interrupt handlers do not run in process context; therefore, they cannot block.

- Consequently, managing interrupts is divided into two parts, or `halves`. The first part, interrupt handlers (`top halves`), are executed by the kernel asynchronously in immediate response to a hardware interrupt. This chapter looks at the second part of the interrupt solution, `bottom halves`.

### 8.1. Bottom Halves

- The job of bottom halves is to perform any interrupt-related work not performed by the interrupt handler. In an ideal world, this is nearly all the work because you want the interrupt handler to perform as little work as possible. By offloading as much work as possible to the bottom half, the interrupt handler can return control of the system to whatever it interrupted as quickly as possible.

- A couple of useful tips help:
  - 1. If the work is time sensitive, perform it in the interrupt handler.
  - 2. If the work is related to the hardware, perform it in the interrupt handler.
  - 3. If the work needs to ensure that another interrupt (particularly the same interrupt), does not interrupt it, perform it in the interrupt handler.
  - 4. For everything else, consider performing the work in the bottom half.

- When attempting to write your own device driver, looking at other interrupt handlers and their corresponding bottom halves can help. When deciding how to divide your interrupt process work between the top and bottom half, ask yourself what **must** be in the top half and what **can** be in the bottom half.

#### 8.1.1. Why bottom halves?

- It is crucial to understand why to defer work, and when exactly to defer it. You want to limit the amount of work you perform in an interrupt handler because interrupt handlers run with the current interrupt line disabled on all processors.

- The solution is to defer some of the work until later.

- But when is **later**? the important thing to realize is that later is often simply **not now**. The point of a bottom half is **not**  to do work at some specific point in the future, but simply defer work until **any** point in the future when the system is less busy and interrupts are again enabled. Often, bottom halves run immediately after the interrupt returns. The key is that they run with all interrupts enabled.

- This design keeps system latency low by running with interrupts disabled
for as little time as necessary.

#### 8.1.2. A World of Bottom Halves

- Unlike the top half, which is implemented entirely via the interrupt handler, multiple mechanism are available for implementing a bottom half. These mechanisms are different interfaces and subsystems that enable you to implement bottom halves.

##### 8.1.2.1. The original `bottom half`

- In the beginning, Linux provided only `bottom half`.
- It provided a statically created list of 32 bottom halves for the entire system. The top half could mark whether the bottom half would run by setting a bit in a `32-bit integer`. Each BH was globally synchronized. No two could run at the same time, even on different processors.
- This way easy to use, yet inflexible; a simple approach, yet a bottleneck.

##### 8.1.2.2. Task queues

- Later on, the kernel developers introduced `task queues` both as a method of deferring work and as a replacement for the BH mechanism. The kernel defined a family of queues. Each queue contained a linked list of functions to call. The queued functions were run at certain times, depending on which queue they were in.

- Drivers could register their bottom halves in the appropriate queue. This worked fairly well, but it was still inflexible to replace the BH interface entirely.

- It also was not lightweight enough for performance critical subsystems, such as networking.

##### 8.1.2.3 Softirqs and Tasklets

- Softirqs and Tasklets could completely replace the BH interface.
- Softirqs -- are a set of statically defined bottom halves that can run simultaneously on any processorl even two of the same type can run concurrently.
- Tasklets -- which have an awful and confusing name, are flexible, dynamically created bottom halves built on top of Softirqs.

- Two different tasklets can run concurrently on different processors, but two of the same type of tasklet cannot run simultaneously.

- **Thus, tasklets are a good trade-off between performance and ease of use**. For most bottom-half processing, the tasklet is sufficient. Softirqs are useful when performance is critical, such as netwrorking.

- Using softirqs requires more care, however, because two of the same
softirq can run at the same time. In addition, softirqs must be registered statically at compile time. Conversely, code can dynamically register tasklets.

- while developing the 2.5 kernel, the BH interface was interface was finally tossed to the curb because all BH users were converted to the other bottom-halg interfaces. Additionally, the task queue interface was replaced by the work queue interface. Work queues are a simple yet useful method of queueing work to later be performed in process context.

- Consequently, today 2.6 has three bottom-half mechanisms in the kernel: **softirqs**, **tasklets**, and **work queues**.The old BH and task queue interfaces are but mere memories.

### 8.2. Softirqs
