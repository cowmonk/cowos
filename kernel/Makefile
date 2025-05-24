MAKEFLAGS += -rR
.SUFFIXES:

override OUTPUT := cowos

CC := clang

CFLAGS := -g -O3 -pipe

CPPFLAGS :=

override CFLAGS += \
    -m64 -g -c -ffreestanding -Wall -Werror -fcommon -Iinclude/ -fPIE -mno-80387 \
    -mno-mmx \
    -mno-sse \
    -nostdlib \
    -mno-sse2 \
    -mno-red-zone -fno-stack-protector \
    -fno-stack-check \
    -fno-lto \
    -target x86_64-unknown-none

override CPPFLAGS := \
    -I src \
    $(CPPFLAGS) \
    -DLIMINE_API_REVISION=3 \
    -MMD \
    -MP

override NASMFLAGS += \
    -Wall \
    -f elf64

override LDFLAGS += \
    -Wl,-m,elf_x86_64 \
    -Wl,--build-id=none \
    -nostdlib \
    -static \
    -z max-page-size=0x1000 \
    -T linker.ld

C_SOURCES = $(shell find . -name '*.c')
C_OBJS = $(patsubst %.c,%.o,$(C_SOURCES))

.PHONY: all
all: bin/$(OUTPUT)

bin/$(OUTPUT): Makefile $(C_OBJS) linker.ld
	mkdir ./bin/ -p
	$(CC) $(LDFLAGS) -o ./bin/$(OUTPUT) $(C_OBJS) -fuse-ld=lld

%.o: %.c
	$(CC) $(CFLAGS) $(subst .o,.c,$@) -o $@

.PHONY: clean
clean:
	find . -name '*.o' -delete
	rm -r -f ./bin
