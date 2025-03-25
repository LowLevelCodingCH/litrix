###################
# Litrix Makefile #
###################

build:
	@make make_kentry --no-print-directory
	@make make_main --no-print-directory
	@make make_other --no-print-directory
	@make link --no-print-directory
	@make debugkrn --no-print-directory
	@make makedisk --no-print-directory

make_kentry:
	@echo "GAS kernel/kentry.s"
	@nasm -f elf32 -o kernel/kentry.o kernel/kentry.s

make_main:
	@echo "CC  kernel/main.c"
	@gcc -O0 -nostdlib -nodefaultlibs -ffreestanding -m32 -g -c kernel/main.c -o kernel/main.o -Iinclude > /dev/null

make_other:
	@echo "CC  kernel/keyboard.c"
	@gcc -fno-builtin-memcpy -O0 -nostdlib -nodefaultlibs -ffreestanding -m32 -g -c kernel/keyboard.c -o kernel/keyboard.o -Iinclude > /dev/null
	@echo "CC  kernel/stdout.c"
	@gcc -fno-builtin-memcpy -O0 -nostdlib -nodefaultlibs -ffreestanding -m32 -g -c kernel/stdout.c -o kernel/stdout.o -Iinclude > /dev/null
	@echo "CC  kernel/pit.c"
	@gcc -fno-builtin-memcpy -O0 -nostdlib -nodefaultlibs -ffreestanding -m32 -g -c kernel/pit.c -o kernel/pit.o -Iinclude > /dev/null
	@echo "CC  kernel/cpu.c"
	@gcc -fno-builtin-memcpy -O0 -nostdlib -nodefaultlibs -ffreestanding -m32 -g -c kernel/cpu.c -o kernel/cpu.o -Iinclude > /dev/null
	@echo "CC  kernel/disk.c"
	@gcc -fno-builtin-memcpy -O0 -nostdlib -nodefaultlibs -ffreestanding -m32 -g -c kernel/disk.c -o kernel/disk.o -Iinclude > /dev/null
	@echo "CC  kernel/fs/smfs.c"
	@gcc -fno-builtin-memcpy -O0 -nostdlib -nodefaultlibs -ffreestanding -m32 -g -c kernel/fs/smfs.c -o kernel/fs/smfs.o -Iinclude > /dev/null
	@echo "CC  kernel/fs/lifs.c"
	@gcc -fno-builtin-memcpy -O0 -nostdlib -nodefaultlibs -ffreestanding -m32 -g -c kernel/fs/lifs.c -o kernel/fs/lifs.o -Iinclude > /dev/null
	@echo "CC  kernel/portio.c"
	@gcc -fno-builtin-memcpy -O0 -nostdlib -nodefaultlibs -ffreestanding -m32 -g -c kernel/portio.c -o kernel/portio.o -Iinclude > /dev/null
	@echo "CC  kernel/device.c"
	@gcc -fno-builtin-memcpy -O0 -nostdlib -nodefaultlibs -ffreestanding -m32 -g -c kernel/device.c -o kernel/device.o -Iinclude > /dev/null
	@echo "CC  kernel/memory.c"
	@gcc -fno-builtin-memcpy -O0 -nostdlib -nodefaultlibs -ffreestanding -m32 -g -c kernel/memory.c -o kernel/memory.o -Iinclude > /dev/null
	@echo "CC  kernel/stack.c"
	@gcc -fno-builtin-memcpy -O0 -nostdlib -nodefaultlibs -ffreestanding -m32 -g -c kernel/stack.c -o kernel/stack.o -Iinclude > /dev/null
	@echo "CC  kernel/virtmem.c"
	@gcc -fno-builtin-memcpy -O0 -nostdlib -nodefaultlibs -ffreestanding -m32 -g -c kernel/virtmem.c -o kernel/virtmem.o -Iinclude > /dev/null
	@echo "CC  kernel/scheduler.c"
	@gcc -fno-builtin-memcpy -O0 -nostdlib -nodefaultlibs -ffreestanding -m32 -g -c kernel/scheduler.c -o kernel/scheduler.o -Iinclude > /dev/null
	@echo "CC  kernel/syscall.c"
	@gcc -fno-builtin-memcpy -O0 -nostdlib -nodefaultlibs -ffreestanding -m32 -g -c kernel/syscall.c -o kernel/syscall.o -Iinclude > /dev/null
	@echo "CC  kernel/syscall_wrapper.c"
	@gcc -fno-builtin-memcpy -O0 -nostdlib -nodefaultlibs -ffreestanding -m32 -g -c kernel/syscall_wrapper.c -o kernel/syscall_wrapper.o -Iinclude > /dev/null

link:
	@echo "LD  -o vmlitrix"
	@ld -melf_i386 -T kernel/linker.ld -o vmlitrix kernel/kentry.o kernel/main.o kernel/memory.o kernel/stack.o kernel/virtmem.o kernel/keyboard.o kernel/pit.o kernel/cpu.o kernel/disk.o kernel/portio.o kernel/stdout.o kernel/syscall.o kernel/syscall_wrapper.o kernel/scheduler.o kernel/fs/lifs.o kernel/fs/smfs.o kernel/device.o > /dev/null

debugkrn:
	@echo "DBG -o vmlitrix.asm"
	@objdump -M i386,pseudoc,intel -d -S vmlitrix > vmlitrix.asm
	@echo "DBG -o vmlitrix.sym"
	@objdump -t vmlitrix > vmlitrix.sym

makedisk:
	@echo "AS  -o disk.dimg"
	@nasm -fbin -o disk.dimg disk.s

clean:
	@rm disk.dimg > /dev/null
	@rm kernel/*.o > /dev/null
	@rm kernel/fs/*.o > /dev/null
	@rm vmlitrix* > /dev/null

test:
	@qemu-system-i386 -m 128 -M pc -kernel vmlitrix -drive file=disk.dimg,if=ide,media=disk,format=raw
