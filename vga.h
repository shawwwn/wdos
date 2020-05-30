/*
* vga.h
* Text mode VGA
*/
#ifndef _VGA_H_
#define _VGA_H_

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

extern uint16_t* vga_ptr;
extern int vga_csr_x, vga_csr_y;

void update_cursor();
void clear_screen();
void print_screen(const char* str);
void scroll_up();
void putchar(int c);
int getchar();
int printf(const char* str);

#endif