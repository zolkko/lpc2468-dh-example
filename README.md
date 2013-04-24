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
