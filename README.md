# Configuring Eclipse IDE for C/C++ Developers

1. Install GNU ARM Eclipse support package (http://gnuarmeclipse.sourceforge.net/updates).
2. Install CDT package.
3. Install Hardware debug support for eclipse / Device GDB debug package from standard repository.

In *Debug Configuration...* dialog add a configuration into *GDB Hardware Debugging* category. Then give it
a name.
On the *Debugger* tab type in gdb location which comes with your toolchain.
In my case it was ~/arm-none-ebi/bin/arm-none-eabi-gdb. Uncheck *Use remote target* if your debugger is connected
to your local machine.

Following steps will be slightly different for different types of JTAG/SWD debuggers.
For IAR branded JLink I had set **Reset and Delay (seconds)**, set **Halt** and typed in the following script
as an initialization script.


  target remote localhost:2331
  monitor flash device = LPC2378
  monitor endian little
  monitor speed auto
  monitor reset 0




  Run commands:
  monitor reset
  continue



To flash firmware and debug it in SRAM the following JLinkGDB script may be used:

[Init script]
target remote localhost:2331
monitor flash device = LPC2378
monitor endian little
monitor speed auto
monitor reset 0


[Run script]
monitor reset 0
monitor memU8  0xe01fc040 = 0x02
monitor reg cpsr = 0xdf
monitor reg pc = 0x40000200
monitor setbp 0x40000200
continue


The script remap reset vector into SRAM and sets program counter
to _boot symbol location which resist at 0x40000200 address.




For JLink and STM32 Cortex-M3 you may setuxp initial commands as the following (on Hardware Debug Startup tab):

Uncheck (Reset and Delay, Halt)
     target remote 127.0.0.1:2331
     monitor interface SWD
     monitor endian little
     monitor speed auto
     monitor flash device = STM32F103ZE
     monitor flash breakpoints = 1
     monitor flash download = 1
     monitor halt
Check Set breakpoint at "main"
In "Run Commands" area type in:
     monitor reset
     continue
