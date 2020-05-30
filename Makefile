.SILENT: boot.img kern.img disk.img
.PHONY: all clean bootloader kernel disk

all: disk.img

disk: disk.img

bootloader: boot.img

kernel: kern.elf

clean:
	@rm *.o *.elf *.img *.bin 2>/dev/null

qemu: disk.img
	qemu-system-i386 -serial stdio -drive format=raw,file=disk.img

qemu-debug: disk.img
	qemu-system-i386 -S -s -serial stdio -drive format=raw,file=disk.img

bochs:
	bochs -f bochsrc.txt









#===============================================================================
%.o: %.c
	gcc -m32 -nostdinc -fno-builtin -Os -c $< -o $@

boot1.o: boot1.asm
	nasm -w-other -f elf32 $< -o $@

boot.elf: boot1.o boot2.o
	ld -m elf_i386 -T boot.ld -N $^ -o $@

boot.img: boot.elf
	objcopy -S -O binary -j .text boot.elf boot.bin
	size=$$(stat -c "%s" "boot.bin"); \
	echo "bootloader is $$size bytes"; \
	if [ "$$size" -gt 510 ]; then \
		echo "bootloader too big, stop."; \
		exit 1; \
	fi
	echo "add paddings"
	dd if="boot.bin" of=boot.img bs=510 count=1 conv=notrunc 2>/dev/null
	echo "add signature"
	printf '\x55\xaa' | dd of=boot.img bs=1 seek=510 conv=notrunc 2>/dev/null
	echo "--> boot.img"

kern.elf: kernel.o vga.o misc.o
	ld -m elf_i386 -T kern.ld -N $^ -o kern.elf

kern.img: kern.elf
	size=$$(stat -c "%s" "kern.elf"); \
	echo "kernel is $$size bytes"; \
	if [ $$size -gt 3584 ]; then \
		echo "kernel is too big, stop."; \
		exit 1; \
	fi
	echo "add paddings"
	cp kern.elf kern.img
	printf '\x00' | dd of=kern.img bs=1 seek=3583 conv=notrunc 2>/dev/null
	echo "--> kern.img"

disk.img: boot.img kern.img
	echo "merging: boot.img, kern.img"
	cp boot.img disk.img
	dd if=kern.img of=disk.img seek=1 2>/dev/null
	echo "--> disk.img"
