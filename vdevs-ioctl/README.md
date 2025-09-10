vDev — Virtual Character Platform Driver

Description:
===========
A simple Linux character platform driver that registers four virtual devices (/dev/vDev-0 .. /dev/vDev-3) with per-instance platform data (size, permission, serial number). The driver implements open/read/write/llseek and an ioctl interface. The ioctl command VDEV_FILLZERO zeros the in-memory backing buffer of the device instance.

Repository Layout:
==================
.
├── README.md                # (this file)
├── platform.h               # IOCTL definitions + platform-data struct
├── vdev_drv.c               # Driver implementation (probe, file ops, ioctl)
├── vdev_platform.c          # platform_device definitions & platform data
├── ioctl_vdev_test.c        # small user-space test program (open + ioctl)
└── Makefile                 # to build kernel module(s) and user test (optional)

Key features:
============
- Creates 4 platform device instances: vDev-Ax, vDev-Bx, vDev-Cx, vDev-Dx.
- Each instance carries virtual_platform_data containing:
  - size — buffer size in bytes.
  - permission — allowed open mode (RDONLY / WRONLY / RDWR).
  - serial_number — identifier string.
- Driver allocates per-instance vDev_config_data:
  - pdata, devid, buffer, dev, cdev, mutex instance_lock, data_len.
- File operations:
  - open() — looks up instance and stores pointer in file->private_data (for later ops).
  - read() / write() — operate on buffer (use/implement offsets and data_len semantics).
  - llseek() — seeks within device bounds.
  - unlocked_ioctl() — handles VDEV_FILLZERO and other commands.
-VDEV_FILLZERO zeroes the whole instance buffer (protected by mutex) and sets data_len = 0.
- IOCTLs (see platform.h)
  - VDEV_FILLZERO — zero out device buffer.
  - VDEV_FILLCHAR — fill with character.

Build & Run:
===========
make
sudo insmod vdev_platform.ko   # registers 4 platform devices
sudo insmod vdev_drv.ko        # driver binds and creates /dev/vDev-N

Unload:
======
sudo rmmod vdev_drv
sudo rmmod vdev_platform

User Test:
=========
gcc -o ioctl_vdev_test ioctl_vdev_test.c

# fill zero in /dev/vDev-0
./ioctl_vdev_test /dev/vDev-0

# verify 
cat /dev/vdev-0 | hexdump -C

License
GPL








