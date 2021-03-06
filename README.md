Aseba discovery firmware
==============
Firmware to run Aseba on an STM32F4 discovery board

## Requirements

This project requires the following tools:

* A recent version of GCC/G++ for ARM (Tested with 4.9+)
* OpenOCD 0.9 or later
* A C / C++ compiler for the host (only required for unit tests)
* [Packager](packager), which is a tool to generate makefiles with dependencies.
    Once you installed Python 3, it can be installed by running `pip3 install cvra-packager`.

## Quickstart
Make sure you have an ARM GCC toolchain and OpenOCD installed.

```bash
git submodule init
git submodule update

packager

make
make flash
```

If you get messages like `Fatal error: can't create build/obj/chcoreasm_v7m.o: No such file or directory`, run `make -B` instead of simply `make`.

To start the shell, open a terminal emulator and run

```bash
sudo python -m serial.tools.miniterm /dev/ttyACM0
```
 assuming `/dev/ttyACM0` is where the discovery is connected

### Changing node ID
To change the node ID, open a shell to the device and enter the following commands:

```
config_set /aseba/id <new id>
config_save
```

then reset the board

### Running unit tests

When developping for this project you might want to run the unit tests to check that your work is still OK.
To do this you will need the following:

* A working C/C++ compiler
* CMake
* [Cpputest][cpputest] A C++ unit testing library

Once everything is installed you can run the following:

```
packager
mkdir build
cd build
cmake ..
make check
```

## Code organization

The code is split in several subsystems:

* `main.c` contains the entry point of the program.
    Its main role is to start all the services and threads required by the rest of the application.
* `chconf.h`, `halconf.h` and `mcuconf.h` are ChibiOS configuration files.
    They allow to customize OS behavior, add remove device drivers, etc.
    1. `halconf.h` allows the user to activate and deactivate the different features of the HAL.
    2. `mcuconf.h` contains the low level details regarding the microcontroller (DMA streams, clocks, etc.).
        You should not have to change it.
    3. `chconf.h` allows the user to configure the different features of ChibiOS (minus the HAL).
        This is were the user can activate and deactivate features like events, mutexes and semaphores.
* `cmd.c` defines the command available through the debug shell.
* `config_flash_storage.c` contains the code used to store the parameter tree to flash.
    It allows user settings to be persistent across reboots.
* `memory_protection.c` contains a driver for the Memory Protection Unit (MPU).
    In this project the MPU is used to detect basic bugs, such as NULL pointer dereference and jumping to invalid function pointers.
* `panic.c` contains the panic handler, called when the system crashes.
* `parameter_port.h` defines OS-specific locking mechanisms used by the parameter tree subsystem.
* `usbcfg.c` contains configuration strings for the USB port.

`aseba_vm` contains all the porting code to run Aseba on this platform.
* `skel_user.c` contains application-specific code, such as native functions, event definitions, etc.
* `aseba_node.c` contains the thread that runs the Aseba VM, as well as OS-specific callbacks.
* `aseba_can_interface.c` contains CAN drivers for use by Aseba.
* `aseba_bridge.c` contains the implementation of a simple Aseba/CAN <-> Aseba/Serial translator.

The following modules are also used, see their respective documentation for more details:

* `chibios-syscalls` contains Newlib porting code and is required for standard library functions such as `printf (3)`, `malloc (3)`, etc.
* `cmp` & `cmp_mem_access` contain an implementation of the [MessagePack standard][messagepack] for C.
* `crc` is an implementation of the Cyclic Redundancy Check (CRC).
    CRC32 and CRC16 are included.
* `parameter` contains an implementation of a centralized parameter service which allows users to make their code configurable from a single place.
    See doc for details.
* `test-runner` does not contain code that will run on the robot but is required to run the unit tests.



[cpputest]: http://cpputest.github.io
[packager]: http://github.com/cvra/packager
[messagepack]: http://messagepack.org/
