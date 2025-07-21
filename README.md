**Migrated from Rekketstone/cowos**

# cowos
Custom OS from scratch in C

# Notes
**Heavily UNDER DEVELOPMENT**

Currently just boots and well, does nothing lol.

## BUILDING
You'll need a few things:
- LLVM/Clang (default C compiler)
- LLD (linker)
- xorriso (iso creation)
- git
- make

### OPTIONAL:
- qemu (for virtual machine)
- LLDB (debugging purposes)

After acquiring those just run make and it'll do everything for you:
```bash
make
```

By default the kernel is compiled with debug symbols, if you would like to avoid this, you can simply override the CFLAGS:
```bash
cd kernel/ # it is recommended to go to the actual directory yourself
make CFLAGS="-s -O3 -pipe"
cd .. # return to root
make
```
Eventually in the future this will be changed to be something much more convienent, however for now the inclusion of debug symbols is nessacary for development.

## RUNNING
There is now a script to automate running cowos, although it's quite simple right now, this is important for the future as cowos will eventually require a root partition and other things as it grows to userspace slowly. To run, simply run this in the root of the repository:
```bash
make run # This will require qemu
```

For developers, running with the DEBUG flag will launch lldb with qemu to help debug issues with cowos:
```bash
make DEBUG=1 run
```
if you do not like LLDB, I included a script called `run-gdb` in the `scripts/` directory for the gdb fans out there. I personally want to keep this project as gnu-free as possible, since I want cowos to not be GNU/cowos, I want it to just be cowos. And this means that I will even avoid anything as simple as the debugger being used to be non-GNU. Run in the root like so:
```bash
./scripts/run-gdb
```
It will automatically exit qemu if gdb is exited, and by default gdb will run in tui mode.

# Credits
- [Limine](https://github.com/limine-bootloader/limine): modern, advanced, portable, multiprotocol bootloader and boot manager. 

## People
- [micl](https://micl.dev): random guy on the interweb in the LHL discord server gave me a free vga driver to use lol
