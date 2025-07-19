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
	./scripts/limine-git.sh
	
clean:
	rm cowos.iso
	make -C ./kernel/ clean
