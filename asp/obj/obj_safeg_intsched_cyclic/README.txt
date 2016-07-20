obj_safeg_intsched_cyclic
=========================

1.- Introduction
    ------------

This directory contains an application to test the integrated scheduling
framework. TrustZone is configured as follows:

    - Trust: ASP, Non-Trust: Linux

Trust ASP contains the following tasks:

    - Priority  4: Main task (initializes the system and finishes)
    - Priority  4: Task 1 (T=100ms C=10ms)
    - Priority  6: Latency Manager
    - Priority  7: LTASK0 (Repl_Period=150ms Budget=15ms)
    - Priority  8: Task 2 (T=200ms C=30ms)
    - Priority  9: LTASK1 (Repl_Period=300ms Budget=25ms)
    - Priority 10: Task 3 (T=200ms C=80ms)
    - Priority 11: BTASK (background priority)

Non-Trust Linux application contains a periodic thread with priority and
period configurable through the argv parameters:

    - linux-app-thread.exe priority seconds nanoseconds
        (e.g. ./linux-app-thread.exe 98 2 0)

The assignment of Non-Trust activities to LTASKs is contained in the
LTASK management data at lowlat.c. In this application the assignment is
as follows:

    - LTASK0
        => Non-Trust Timer interrupt
        => Non-Trust Tasks with priority 95~99
    - LTASK1
        => Non-Trust Tasks with priority 88~94
    - BTASK
        => Remaining tasks

2.- Linux kernel
    ------------

Apply the linux kernel patches contained in SafeG to a vanilla kernel:

    - patch -p1 < linux/board_pb1176/kernel-2.6.33/patches/safeg-linux-2.6.33-all.patch

Configure the linux kernel to support integrated scheduling:

    - cp linux-2.6.33.config build/.config
    - make ARCH=arm CROSS_COMPILE=/home/dsl/app/arm-none-eabi-2007q1-21/bin/arm-none-eabi-
        O=/home/dsl/kernels/linux-pb1176-2.6.33-working/build menuconfig
        (Enable System Type --> Realview platform type -->
         enable safeg extensions --> integrated scheduling)

Build it:

    - make ARCH=arm
      CROSS_COMPILE=/home/dsl/app/arm-none-eabi-2007q1-21/bin/arm-none-eabi-
      O=/home/dsl/kernels/linux-pb1176-2.6.33-working/build uImage
    - sudo cp build/arch/arm/boot/uImage /var/lib/tftpboot/

Then compile SafeG and the Trust ASP applications:

    - make OBJ=obj_safeg_intsched_cyclic NONTRUST=linux clean
    - make OBJ=obj_safeg_intsched_cyclic NONTRUST=linux
    - sudo cp safeg/*.bin /var/lib/tftpboot/

Compile the linux application:

    - cd linux-app
    - make
    - sudo cp linux-app* /home/dsl/kernels/rootfs/nfs/root/

Power on the board and run the system. At first, the Trust ASP will have
no tasks executing and only waiting for Linux to initialize the
integrated scheduling flag. Run the linux application to check the
latency:

# ./linux-app-thread.exe
latency: 79000 ns
latency: 76000 ns
latency: 73000 ns
latency: 73000 ns
latency: 72000 ns
latency: 76000 ns
latency: 74000 ns

Next we mount the debugfs and activate the integrated scheduling flag. This
will cause a return to Trust ASP which will start the 3 Trust periodic tasks.

    - mount -t debugfs none /sys/kernel/debug/ (optional)
    - echo Y > /sys/kernel/debug/safeg/activate_voluntary_return

Now, we can check the latency for different priorities.

At priority 99 only the Task 1 (C=10ms) can block the linux task:

# ./linux-app-thread.exe 99 2 0
prio:99, period:2.0
latency: 105674000 ns <-- the first measure is misleading
latency: 109001 ns
latency: 123001 ns
latency: 211001 ns
latency: 97001 ns
latency: 106001 ns
latency: 99001 ns
latency: 117001 ns
latency: 113001 ns
latency: 219001 ns
latency: 100001 ns
latency: 97001 ns

At priority 90 Task 1 and 2 (C=10ms and C=15ms) can block the linux task:
# ./linux-app-thread.exe 90 2 0
prio:90, period:2.0
latency: 7935000 ns
latency: 13758000 ns   <-- 14 ms blocking time
latency: 126000 ns
latency: 154000 ns
latency: 8140000 ns
latency: 131000 ns
latency: 7062000 ns
latency: 13750000 ns   <-- 14 ms blocking time
latency: 140000 ns
latency: 254000 ns

At priority 30 Task 1 and 2 and 3 (C=10ms and C=15ms and C=80ms) can block the
linux task:

# ./linux-app-thread.exe 30 2 0
prio:30, period:2.0
latency: 60777620 ns
latency: 91578501 ns
latency: 122104496 ns <-- 122 ms blocking time
latency: 88496 ns
latency: 104496 ns
latency: 29319496 ns
latency: 59773496 ns
latency: 90483496 ns


