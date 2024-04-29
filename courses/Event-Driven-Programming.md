# Event driven programming

- [link](https://www.youtube.com/watch?v=rfb2JI1GGIc&list=PLPW8O6W-1chx8Y7Oq2gOE0NUPXmQxu2Wr&index=1)

- Programming Trends:

===================================================>|
| Operating Systems                                 |
                    |RTOS (shared state concurrency)|
                                                    |
                                                    |
| Programming Languages                             |==> Modern Embedded Programming.
                    |Object-Oriented Programming    |
                                        |ROOM| UML  |
| Programming Paradigms                             |
                        | Event-Driven Programming  |
|===================|===|=================|========>|
1950              1970 1980             2000        |

- Handle event type:
  - **Run-to-completion**: Get event from queue, handle one by one, so one event is blocked can hang event loop.
    - Event handler should very quick (if you need sequence programming, create  another thread or timer).

## Event driven in real time OS

- For example, we have to thread led and button, when button click we release semaphore blocking in Led thread to switch led.
  - The main idea characteristic of the design is that thread synchronize their execution with various events by means of **blocking** and waiting in-line for a chosen event to occur. But a blocked thread is unresponsive to any other events not explicitly waited for.
  - Consequently the main workaround for adding new events is to create more threads, because multiple threads can wait for multiple events in parallel. But thus ever growing number of threads quickly becomes very expensive and unmanageable.
  - But even more importantly, the new threads likely need to access the same data, peripherals, or other resources, as the already existing, threads. So we need to add some Mutual Exclusion.
  - But this maybe cause Starvation, Deadlock, Priority Inversion.

### Effective Concurrency: Prefer using active objects instead of Naked thread

- [link](https://www.state-machine.com/doc/Sutter2010a.pdf)

- Use `Threads Correctly = Isolation + Asynchronous messages`
  - Keep data isolated, private to a thread where possible. Note that this doesn't mean using a special facility like thread local storage, it just means **not sharing** the thread's private data by exposing pointers or references to it.
  - Communicate among threads via asynchronous messages. Using Asynchronous messages lets the threads keep running independently by default unless they really must wait for a result.
  - Organize each thread's work around a **message pump**. Most threads should spend their lifetime responding to incoming messages, so their mainline should consist of a message pump that dispatches those messages to the message handlers.
- Using raw threads directly is trouble for a number of reasons, particularly because raw threads let us do anything and offer no help or automation for these best practices. `So how can we automate them?`

- A good answer is to apply and automate the **Active Object Pattern**.

- Active Object = private-data + private-thread + private-queue

- The only way to communicate with an AO: asynchronous event posting to its queue.
- Events are processed to completion on the private thread.
  - No need for mutual exclusion.
- The strictest form of OOP.
- Higher-level of abstraction above the `naked` RTOS.
