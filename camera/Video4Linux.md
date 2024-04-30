# Video4Linux

- Video4Linux (**V4L**) is a collection of **device drivers** and an **API** for supporting realtime **video capture** on Linux systems. It supports many USB webcams, TV tuners, and related devices, standardizing their output, so programmers can easily add video support to their applications.

- Video4Linux is responsible for creating V4L2 device nodes ake a device (`/dev/videoX`, `/dev/vbiX`, and `/dev/radioX`) and tracking data from these nodes. The device node creation is handled by V4L device drivers using `video_device` struct `v4l2-dev.h` and it can either be allocated dynamically or embedded in another larger struct.

- [Repository](https://git.linuxtv.org/v4l-utils.git)

## 1. History

- V4L had been introduced late into the 2.1.X development cycle of the Linux Kernel.
- **V4L2 is the second version of V4L**.
