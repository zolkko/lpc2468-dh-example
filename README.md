LPC2468 Example
===============
This sample code illustrates how to use lpc2468 i2c0 peripheral in combination with devicehive m2m framework. It implements simplest
i2c-echo device which I found useful during gateway code debugging.
The sample utilizes gnu-arm toolchain with newlib-nano which binary and source packages could be downloaded here
https://launchpad.net/gcc-arm-embedded.

Configuring Eclipse IDE for C/C++ Developers
--------------------------------------------
First of all you need to install following additional software:

1. Hardware debug support for eclipse package from standard repository.
2. GNU ARM Eclipse support package (http://gnuarmeclipse.sourceforge.net/updates).

### Debugging with J-Link
Open __Run/Debugging configuration...__ dialog. Here you need to add additional debugging configuration
in __GDB Hardware Debugging__ category. Give it a name. On the __Debugger__ tab type in gdb location which comes with your toolchain.
In my case it was ~/gcc-arm-none-eabi/bin/arm-none-eabi-gdb. Uncheck __Use remote target__ if your debugger is connected
to your local machine. Then, for IAR branded J-Link, I had set __Reset and Delay (seconds)__, set __Halt__ and typed in the following script
as an initialization script.

```gdb
target remote localhost:2331
monitor interface JTAG
monitor flash device = LPC2468
monitor endian little
monitor speed auto
monitor flash breakpoints = 1
monitor flash download = 1
monitor reset 0
```

Run commands:
```gdb
monitor reset
continue
```

### Debugging with J-Link in SRAM
To flash firmware and debug it in SRAM the following JLinkGDB run script may be used:

```gdb
monitor reset 0
monitor memU8  0xe01fc040 = 0x02
monitor reg cpsr = 0xdf
monitor reg pc = 0x40000200
monitor setbp 0x40000200
continue
```

Here **reg cpsr = 0xdf** and **reg pc = 0x40000200** monitor command are invoked. These commands remap interrupt vector
from 0x4000000 SRAM location to 0x0 address and then jump to the reset_vector procedure (which address is 0x40000200).
You may need to change reset_vector address to some different value. To find out reset vector address you can look for it in lpc2468-dh.lst file
which is generated (by default) by Eclipse on each build run.
