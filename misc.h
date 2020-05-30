/*
* misc.h
* Standard C library
*/
#ifndef _MISC_H_
#define _MISC_H_

/* types.h */
typedef __signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;

/* x86.h */
__attribute__((always_inline))
static inline void breakpoint()
{
	__asm __volatile("int3");
}
__attribute__((always_inline))
static inline uint8_t inb(int port)
{
	uint8_t data;
	__asm __volatile("inb %w1,%0" : "=a" (data) : "d" (port));
	return data;
}
__attribute__((always_inline))
static inline void outb(int port, uint8_t data)
{
	__asm __volatile("outb %0,%w1" : : "a" (data), "d" (port));
}

/* kbd.h */
extern const unsigned char kbd_map[128];

/* libc.h */
typedef unsigned int size_t;
void *memcpy(void *dest, void *src, size_t n);
void *memset(void *ptr, char value, size_t n);
size_t strlen(const char* str);
int strcmp (const char* str1, const char* str2);

void delay();
char* int2str(int num, char* buf);

#endif
