all: iso

iso: kernel limine
	@echo "Copying kernel to isodir..."
	cp -v ./kernel/bin/cowos ./isodir/boot/
	@echo "Creating ISO image..."
	xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
		-apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		isodir -o cowos.iso
	@echo "Installing Limine BIOS..."
	./limine/limine bios-install cowos.iso

kernel:
	@echo "Buliding kernel..."
	make -C ./kernel/

limine: kernel
	@echo "Setting up Limine..."
	./scripts/limine-git.sh
	
clean:
	@echo "Cleaning up..."
	rm cowos.iso
	make -C ./kernel/ clean

run:
	@echo "Running cowos in QEMU..."
	@if [ "$(DEBUG)" = "1" ]; then \
		echo "Debug mode: Starting QEMU with LLDB..."; \
		qemu-system-x86_64 -cdrom cowos.iso -m 512M -serial stdio -s -S & \
		echo "Launching LLDB..."; \
		lldb --arch x86_64 -o "gdb-remote localhost:1234" ./kernel/bin/cowos; \
	else \
		qemu-system-x86_64 -cdrom cowos.iso -m 512M -serial stdio; \
	fi

.PHONY: all iso kernel limine clean run
