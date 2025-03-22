###################
# Litrix Makefile #
###################

build:
	@make make_kentry --no-print-directory
	@make make_main --no-print-directory
	@make make_other --no-print-directory
	@make link --no-print-directory

make_kentry:
	@echo "GAS kernel/kentry.s"
	@gcc -nostdlib -nodefaultlibs -Wall -Wextra -Werror -ffreestanding -m32 -g -c kernel/kentry.s -o kernel/kentry.o > /dev/null

make_main:
	@echo "CC  kernel/main.c"
	@gcc -nostdlib -nodefaultlibs -ffreestanding -m32 -g -c kernel/main.c -o kernel/main.o -Iinclude > /dev/null

make_other:
	@echo "CC  kernel/keyboard.c"
	@gcc -nostdlib -nodefaultlibs -ffreestanding -m32 -g -c kernel/keyboard.c -o kernel/keyboard.o -Iinclude > /dev/null
	@echo "CC  kernel/stdout.c"
	@gcc -nostdlib -nodefaultlibs -ffreestanding -m32 -g -c kernel/stdout.c -o kernel/stdout.o -Iinclude > /dev/null
	@echo "CC  kernel/pit.c"
	@gcc -nostdlib -nodefaultlibs -ffreestanding -m32 -g -c kernel/pit.c -o kernel/pit.o -Iinclude > /dev/null
	@echo "CC  kernel/disk.c"
	@gcc -nostdlib -nodefaultlibs -ffreestanding -m32 -g -c kernel/disk.c -o kernel/disk.o -Iinclude > /dev/null
	@echo "CC  kernel/portio.c"
	@gcc -nostdlib -nodefaultlibs -ffreestanding -m32 -g -c kernel/portio.c -o kernel/portio.o -Iinclude > /dev/null
	@echo "CC  kernel/memory.c"
	@gcc -nostdlib -nodefaultlibs -ffreestanding -m32 -g -c kernel/memory.c -o kernel/memory.o -Iinclude > /dev/null
	@echo "CC  kernel/stack.c"
	@gcc -nostdlib -nodefaultlibs -ffreestanding -m32 -g -c kernel/stack.c -o kernel/stack.o -Iinclude > /dev/null
	@echo "CC  kernel/virtmem.c"
	@gcc -nostdlib -nodefaultlibs -ffreestanding -m32 -g -c kernel/virtmem.c -o kernel/virtmem.o -Iinclude > /dev/null

link:
	@echo "LD  -o vmlitrix"
	@ld -melf_i386 -T kernel/linker.ld -o vmlitrix kernel/kentry.o kernel/main.o kernel/memory.o kernel/stack.o kernel/virtmem.o kernel/keyboard.o kernel/pit.o kernel/disk.o kernel/portio.o kernel/stdout.o > /dev/null

clean:
	@rm kernel/*.o > /dev/null
	@rm vmlitrix > /dev/null

test:
	@qemu-system-i386 -serial stdio -debugcon file:debug.log -global isa-debugcon.iobase=0xe9 -kernel vmlitrix > /dev/null
