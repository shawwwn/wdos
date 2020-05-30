/*
* kernel.c
*/
#include "misc.h"
#include "vga.h"



void print_welcome() {
	printf("\n");
	printf("   _       __   ____    ____    _____ \n");
	printf("  | |     / /  / __ \\  / __ \\  / ___/ \n");
	printf("  | | /| / /  / / / / / / / /  \\__ \\  \n");
	printf("  | |/ |/ /  / /_/ / / /_/ /  ___/ /  \n");
	printf("  |__/|__/  /_wzl_/  \\____/  /____/   \n");
	printf("\n");
}

void print_help() {
	printf("commands: \n");
	printf("help | prog1 | prog2 | clear | reboot");
}

/* Get input command in one line
 * no wrapping
 * @return: internal char buffer, size <= 78-prompt
 * @buf: size <= 78
 */
char* get_cmd(const char* prompt)
{
	printf(prompt);

	static char buf[80];
	memset(buf, 0, 80); // clear buffer

	char* pos = &buf[0];
	char c;
	while (1) {
		delay();
		c = getchar();
		if (c == '\n') {
			*pos = 0;
			break;
		} else if (c == '\b') {
			if (pos > buf)
				*(--pos) = 0;
			else
				continue;
		} else {
			if (pos-buf >= VGA_WIDTH-strlen(prompt)-1)
				continue; // stop when line full
			*(pos++) = c;
		}
		delay();
		putchar(c);
	}
	return buf;
}

void prog1() {
	printf("How are you?");
}

void prog2() {
	printf("Are you OK!?");
}

void reboot() {
	outb(0xCF9, 0x0E); // power cycle the motherboard
}

// kernel entry point
void kmain(void)
{
	clear_screen();
	print_welcome();
	putchar('\n');
	print_help();
	putchar('\n');

	while (1) {
		putchar('\n');
		char* cmd = get_cmd(">");
		putchar('\n');
		if (strcmp(cmd, "help") == 0) {
			print_help();
		} else if (strcmp(cmd, "prog1") == 0) {
			prog1();
		} else if (strcmp(cmd, "prog2") == 0) {
			prog2();
		} else if (strcmp(cmd, "clear") == 0) {
			clear_screen();
		} else if (strcmp(cmd, "reboot") == 0) {
			reboot();
		} else {
			printf("command not found");
		}
	}

	// not suppose to reach here...
	return;
}
