# WDOS
A batch operating system.

For the sake of demonstrating how to write a basic OS on x86.

So far this OS only contains:
* a bootloader
* a kernel
* a VGA console w/e keyboard functionalities

## Building

Build disk image:
```sh
make
```

Run w/e [QEMU](https://www.qemu.org/):
```sh
make qemu
```

Run w/e [Bochs](http://bochs.sourceforge.net/)
```sh
make bochs
```

To test WDOS on real computers, burn `disk.img` on a usb drive with the help of [Win32DiskImager](https://sourceforge.net/projects/win32diskimager/) or `dd`, then instruct BIOS to boot from that usb drive.

## Acknowledgements
WDOS is developped on QEMU, should run on most PCs with a legacy BIOS but not guaranteed.

I am not responsible for anything happens to your computer.

Bootloader part is sparsely based on [JOS](https://pdos.csail.mit.edu/6.828/2018/overview.html), a predecessor of [xv6](https://pdos.csail.mit.edu/6.828/2019/xv6.html), written by MIT educators to teach operating system.
