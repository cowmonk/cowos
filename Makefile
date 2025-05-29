all: iso

iso: kernel limine
	cp -v ./kernel/bin/cowos ./isodir/boot/

	xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin \
        	-no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
        	-apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin \
        	-efi-boot-part --efi-boot-image --protective-msdos-label \
        	isodir -o cowos.iso

	./limine/limine bios-install cowos.iso

kernel:
	make -C ./kernel/ all

limine: kernel
	git clone https://github.com/limine-bootloader/limine.git --branch=v9.x-binary --depth=1
	make -C limine
	
	cp -v limine/limine-bios.sys limine/limine-bios-cd.bin \
            limine/limine-uefi-cd.bin isodir/boot/limine/

	cp -v limine/BOOTX64.EFI isodir/EFI/BOOT/
	cp -v limine/BOOTIA32.EFI isodir/EFI/BOOT/
	
clean:
	rm cowos.iso
	make -C ./kernel/ clean
