**Migrated from Rekketstone/cowos**

# cowos
Custom OS from scratch in C

# Notes
This only supports 32bit for now, please DO NOT compile with 64bit or it'll fail to boot

## compilation of any component
You are required to get your own cross C compiler. This is because normal C compilers for your Linux distro (assuming that you are on Linux) will optimize against your own libs and other things. This OS doesn't exist yet, so this is a big no no in OS development. Another thing is that you want to compile for 32 bit, so you'll need to cross compile anyways...

### general c files
```bash
i386-elf-gcc -c kernel.c -o kernel.o -std=c99 -ffreestanding -O2 -Wall -Wextra # i386 is the 32 bit version of x86
# also please compile with the -Iinclude flag to include the libraries like string.h in the arch
```

### "compiling" the "boot stub"
```bash
i386-elf-as boot.s -o boot.o
```

### linking the "compiled" boot stub w/compiled kernel img
```
i386-elf-gcc -T linker.ld -o cowos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o
```

## Getting it bootable
You'll need grub and mtools. First move the compiled cowos.bin to isodir/boot. Then run the following command:
```bash
grub-mkrescue isodir/ -o cowos.iso
```
Now you can enjoy running this in qemu lol.
