#!/bin/sh

if [ ! -d ./limine/ ]; then
	git clone https://github.com/limine-bootloader/limine.git --branch=v9.x-binary --depth=1
	make -C limine

	cp -v limine/limine-bios.sys limine/limine-bios-cd.bin \
    	limine/limine-uefi-cd.bin isodir/boot/limine/

	mkdir -p isodir/EFI/BOOT

	cp -v limine/BOOTX64.EFI isodir/EFI/BOOT/
	cp -v limine/BOOTIA32.EFI isodir/EFI/BOOT/
else
	echo "Limine is already setup! Skipping..."
fi


