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
