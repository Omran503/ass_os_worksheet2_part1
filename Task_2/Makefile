# Variables
ASM=nasm
CC=gcc
LD=ld
ISO_GEN=genisoimage
QEMU=qemu-system-i386

# Paths
SOURCE_DIR=source
ISO_DIR=iso
GRUB_DIR=$(ISO_DIR)/boot/grub
KERNEL=$(ISO_DIR)/boot/kernel.elf
ISO_FILE=os.iso
LOG_FILE=logQ.txt

# Targets
all: $(KERNEL) $(ISO_FILE)

$(KERNEL): $(SOURCE_DIR)/loader.o $(SOURCE_DIR)/main.o
	$(LD) -T $(SOURCE_DIR)/link.ld -melf_i386 $^ -o $@

$(SOURCE_DIR)/loader.o: $(SOURCE_DIR)/loader.asm
	$(ASM) -f elf $< -o $@

$(SOURCE_DIR)/main.o: $(SOURCE_DIR)/main.c
	$(CC) -m32 -ffreestanding -c $< -o $@

$(ISO_FILE): $(KERNEL) $(GRUB_DIR)/menu.lst
	$(ISO_GEN) -R -b boot/grub/stage2_eltorito -no-emul-boot \
	-boot-load-size 4 -A os -input-charset utf8 -quiet \
	-boot-info-table -o $@ $(ISO_DIR)

run: $(ISO_FILE)
	$(QEMU) -nographic -boot d -cdrom $< -m 32 -d cpu -D logQ.txt

clean:
	rm -f $(SOURCE_DIR)/*.o $(KERNEL) $(ISO_FILE) $(LOG_FILE)
