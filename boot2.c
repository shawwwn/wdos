// boot2.c
// 2nd-stage bootloader
// in protected mode
//

#define KERN_SIZE       512*7
#define KERN_PTR_16     ((char*)0x8000) // kernel address in real(16-bit) mode
#define KERN_PTR        ((char*)0x100000) // kernel starts at 1MB in protected mode
#define VID_PTR         ((char*)0xb8000) // vga buffer

// xv6: elf.h
typedef __signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;
#define ELF_MAGIC 0x464C457F // "ELF"
struct ElfHdr {
	uint32_t e_magic;
	uint8_t e_elf[12];
	uint16_t e_type;
	uint16_t e_machine;
	uint32_t e_version;
	uint32_t e_entry; // main()
	uint32_t e_phoff;
	uint32_t e_shoff;
	uint32_t e_flags;
	uint16_t e_ehsize;
	uint16_t e_phentsize;
	uint16_t e_phnum;
	uint16_t e_shentsize;
	uint16_t e_shnum;
	uint16_t e_shstrndx;
} __attribute__((packed));

struct ProgHdr {
	uint32_t p_type;
	uint32_t p_offset;
	uint32_t p_va;
	uint32_t p_pa;
	uint32_t p_filesz;
	uint32_t p_memsz;
	uint32_t p_flags;
	uint32_t p_align;
} __attribute__((packed));

typedef uint32_t size_t;
void *memcpy(void *dest, void *src, size_t n)
{
	char *cs = (char *)src;
	char *cd = (char *)dest;

	for (size_t i=0; i<n; i++) 
		cd[i] = cs[i];
	return dest;
}

void print_screen(const char* str)
{
	unsigned int i = 0;
	unsigned int j = 0;

	// clear screen
	while(j < 80 * 25 * 2) {
		VID_PTR[j] = ' ';
		VID_PTR[j+1] = 0x07;
		j = j + 2;
	}
	j = 0;

	// print string
	while(str[j] != '\0') {
		VID_PTR[i] = str[j]; // char's ascii
		VID_PTR[i+1] = 0x07; // char's attribute (color)
		++j;
		i = i + 2;
	}
}

void boot2(void) {
	// check if disk contains an elf
	struct ElfHdr *elf = (struct ElfHdr *)KERN_PTR_16;
	if (elf->e_magic != ELF_MAGIC) {
		print_screen("invalid kernel image");
		return; // halt
	}

	// copy elf's segments to their predefined phy addrs
	struct ProgHdr *ph, *eph;
	ph = (struct ProgHdr *)(KERN_PTR_16 + elf->e_phoff);
	eph = ph + elf->e_phnum;
	for (; ph<eph; ph++) {
		if (ph->p_pa < (uint32_t)KERN_PTR)
			continue; // skip .ident spams
		memcpy((void *)ph->p_pa,
			(void *)((uint32_t)KERN_PTR_16 + ph->p_offset),
			ph->p_memsz);
	}

	print_screen("-> protected mode");

	// enter kernel
	((void (*)(void))(elf->e_entry))();

	return;
}
