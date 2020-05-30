/*
*  kern.ld
*/

OUTPUT_FORMAT(elf32-i386)
ENTRY(_kmain)
SECTIONS
{
	. = 0x100000;
	.text  : { *(.text) }
	.rdata : { *(.rdata)  }
	.data  : { *(.data) }
	.bss   : { *(.bss)  }
}
