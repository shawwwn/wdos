#include "misc.h"

// Scancode to ASCII (US QWERTY keyboard)
// http://www.osdever.net/bkerndev/Docs/keyboard.htm
const unsigned char kbd_map[128] =
{
	0,
	27,                                                         /* Esc */
	'1', '2', '3', '4', '5', '6', '7', '8', '9', '0',           /* Number */
	'-', '=',
	'\b',                                                       /* Backspace */
	'\t',                                                       /* Tab */
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', /* 1st row */
	'\n',                                                       /* Enter */
	0,                                                          /* Control */
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'',     /* 2nd row */
	'`',                                                        /* tilde */
	0,                                                          /* Left shift */
	'\\',
	'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',           /* 3rd row */
	0,                                                          /* Right shift */
	'*',
	0,                                                          /* Alt */
	' ',                                                        /* Space bar */
	0,                                                          /* Caps lock */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                               /* F1 - F10 */
	0,                                                          /* Num lock*/
	0,                                                          /* Scroll Lock */
	0,                                                          /* Home */
	0,                                                          /* Up Arrow */
	0,                                                          /* Page Up */
	'-',
	0,                                                          /* Left Arrow */
	0,
	0,                                                          /* Right Arrow */
	'+',
	0,                                                          /* End*/
	0,                                                          /* Down Arrow */
	0,                                                          /* Page Down */
	0,                                                          /* Insert */
	0,                                                          /* Delete */
	0, 0, 0,
	0,                                                          /* F11 */
	0,                                                          /* F12 */
	0,                                                          /* undefined */
};

void *memcpy(void *dest, void *src, size_t n)
{
	char *cs = (char *)src;
	char *cd = (char *)dest;

	for (size_t i=0; i<n; i++) 
		cd[i] = cs[i];
	return dest;
}

void *memset(void *ptr, char value, size_t n)
{
	for (size_t i=0; i<n; i++)
		((char*)ptr)[i] = value;
	return ptr;
}

size_t strlen(const char* str)
{
	size_t i = 0;
	while (*(str++) != 0)
		i++;
	return i;
}

int strcmp(const char* str1, const char* str2)
{
	while (*str1 && (*str1==*str2)) {
		++str1;
		++str2;
	}
	return *(uint8_t*)str1 - *(uint8_t*)str2;
}

void delay() {
	asm("nop\n"
		"nop\n"
		"nop\n"
		"nop\n");
}

char* _int2str(int num, char* buf) {
	int q = num / 10;
	int r = num % 10;

	if (q != 0)
		buf = _int2str(q, buf);

	buf[0] = r + 48; // ascii number
	return ++buf;
}
char* int2str(int num, char* buf) {
	char* b = _int2str(num, buf);
	b[0] = 0;
	return buf;
}