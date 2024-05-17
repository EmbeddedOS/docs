# Linux system programming

## 3. File operations

- Preliminary of Files in Linux
- Read(r), write(w), execute(x).
- Read has `4` units (2 power 2).
- Write has `2` units (2 power 1).
- Execute has `1` units (2 power 1).
- Each file can be accessed by different users as follows: user, group, others.
- Each user type can have r/w/x access for file.

- Types of Files:
  - `-`: regular file (data files).
  - `d`: directory.
  - `c`: character device file.
  - `b`: block device file.
  - `s`: local socket file.
  - `p`: named pipe.
  - `l`: symbolic link.

- For example:

    ```text
    -rwxrwx---  3 larva lava 4096   Jul 5   20:49 test.c
    ```

  - That means:
    - This is regular file.
    - First `rwx` means user owner have read-write-execute access.
    - Second `rwx` means group owner have read-write-execute access.
    - Final `---` means other don't have any access.

- We can set this by `chmod` command, for example:

    ```bash
    chmod -R 776 test.c
    ```

  - First `7` means user have `111` `read-write-execute` permissions.
  - Second `7` means group also have full access.
  - Final six means `110` means others have read-write permissions and don't have execute.

## 4. Advanced I/O

### 14. Race condition

- Advanced - FILE I/O
  - Advanced File Input-Output operations.

- Multi processing/threading:
  - Linux provides a multi processing environment.
  - This means there several processes running parallel in Linux system.
  - Each process based on its priority, the scheduler gives every process a definite time on CPU to execute.
  - Hence giving the user a virtual feeling as if all process are executing at same time and in parallel.
- Race condition:
  - A race condition is a situation where the result produced by two processes (or threads) operating on shared resources depends in an unexpected way on the relative order in which the processes gain access to the CPU(s).

- For example, two process access the same file and write data to it.
  - Race Round - Condition: Unpredicted write to File because of Race Around between 2 different programs to write to a file.

### 15. Atomicity

- Atomicity is a condition in which the code is written in such a way that code cannot be run by other process/thread `AT SAME TIME` while it is being executed/running by first process/thread.

- All system calls in Linux are executed atomically. By this, we mean that the kernel guarantees that all of the steps in a system call are completed as a single operation. Also in Linux the system calls either return pass of fail, without any intermediate results.

- The Atomicity is very much important to maintain in multi-processing environment like a linux system, else `Race Round` condition can occur.

- How Atomicity work?

```text

```
