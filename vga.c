/*
* vga.c
*/
#include "misc.h"
#include "vga.h"

uint16_t* vga_ptr = ((uint16_t*)0xb8000);
int vga_csr_x = 0, vga_csr_y = 0;

void update_cursor()
{
	int pos = vga_csr_y * VGA_WIDTH + vga_csr_x;

	outb(0x3D4, 0x0F); // pos low
	outb(0x3D5, pos);
	outb(0x3D4, 0x0E); // pos high
	outb(0x3D5, (pos >> 8));
}

void clear_screen()
{
	for (int i=0; i<80*25; ++i) {
		vga_ptr[i] = 0x0720; // 'space' 0x20 and black bg 0x07, little endian
	}

	vga_csr_x = 0;
	vga_csr_y = 0;
	update_cursor();
}

void print_screen(const char* str)
{
	int i = 0;
	while (str[i] != '\0') {
		vga_ptr[i] = str[i] | 0x0700; // 0x07(char)
		++i;
	}
}

void scroll_up()
{
	int offset = 80*24;
	memcpy(vga_ptr, vga_ptr+80, offset*2);

	for (int i=0; i<80; ++i)
		vga_ptr[offset+i] = 0x0720; // 'spaces'
}

void putchar(int c)
{
	int pos = vga_csr_y * VGA_WIDTH + vga_csr_x;;
	switch(c) {
		case 27: // esc
			break;
		case '\n': // newline
			++vga_csr_y;
			vga_csr_x = 0;
			break;
		case '\b': // backspace
			vga_ptr[--pos] = 0x0720;
			--vga_csr_x;
			if (vga_csr_x < 0) {
				if (vga_csr_y == 0) {
					// first row
					vga_csr_x = 0;
				} else {
					vga_csr_x = VGA_WIDTH-1;
					--vga_csr_y;
				}
			}
			break;
		case '\t': // tab
			c = ' ';
			goto printables;
			break;
		default: // all printable characters
		printables:
			vga_ptr[pos++] = 0x0700 | (uint8_t)c;
			++vga_csr_x;
			if (vga_csr_x >= VGA_WIDTH) {
				vga_csr_x = 0;
				++vga_csr_y;
			}
	}

	// check if needs scroll up
	if (vga_csr_y >= VGA_HEIGHT) {
		scroll_up();
		vga_csr_y = VGA_HEIGHT-1;
	}
	update_cursor();
}

int getchar()
{
	while (1) {
		// wait for output buffer becomes full
		uint8_t status;
		do {
			status = inb(0x64);
			status = status & 0b00000001; // bit-0 for output buffer
			delay();
		} while (status == 0);

		// read a byte from output buffer
		int8_t scancode = inb(0x60);
		delay();
		if (scancode<0)
			continue; // ignore key-release

		// convert to ascii
		static char c;
		c = kbd_map[scancode];
		if (c == 0)
			continue;

		return c;
	}
}

int printf(const char* str)
{
	const char* s = str;
	while(*s != '\0') 
	{ 
		putchar(*s);
		s++;
	}
}

