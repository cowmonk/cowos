.set ALIGN, 1<<0    /* align loaded modules on page boundaries */
.set MEMINFO, 1<<1  /* provide memory map */
.set FLAGS, ALIGN | MEMINFO /* Multiboot flag */
.set MAGIC, 0x1BADB002 /* Magic number lets bootloader find the header */
.set CHECKSUM, -(MAGIC + FLAGS) /* checksum of above, to prove we are multiboot (sanity check I think) */

/* so this basically declares the multiboot header that defines this program as a kernel,
   and the following values are in the multiboot standard. The bootloader will look for the
   first 8KiB of the kernel file, aligned with the 32-bit boundary, this means that it won't
   work with 64bit until I keep on learning or I create my own bootloader (without multiboot)
*/
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/* This is where my knowledge is not high enough but here is the summary:
        - esp is NOT in the multiboot standard so the kernel has to provide it.
          this basically means that it doesn't define the value of the stack register
          (whatever that means)
        - This allocates some room for a stack by creating a symbol at the bottom of it
          then allocate 16 KiB and then creating a symbol on top.
        - The stack grows downwards on x86.

And many more, apparently it's re-aligned to System-V ABI standard as well? Not sure, but hopefully I can
continue learning
*/
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

/* linker specifies a _start as the entry point to the kernel and the bootloader will jump to the position
however it doesn't make any sense to return to this after the bootloader is gone
*/
.section .text
.global _start
.type _start, @function
_start:
        /* We are now in... no printf and other things, kernel has FULL CONTROL of the cpu. No security also lol */

        /* To set up a stack, this sets the esp to register to the point of the stack because it grows downwards on
        on x86 systems. this is done in assembly because languages like C can't function without a stack */
        mov $stack_top, %esp

        /* so we need to initialize some critical processor states before the high-level kernel is entered.
        interestingly a lot of things are done at the boot level, we should load GDT, enable paging, and add runtime
        support (for C++). Crazy!
        */

        /* time to enter the kernel,
        according to the guide, this is a well defined call as the stack was originally 16-byte aligned, we pushed a 
        multiple of 16-bytes to the stack (since 0 bytes were pushed so far), so alignment has been perserved.
        */
        call kernel_main

        /* If there is nothing else to do, the computer will be in an infinite loop 
                - Disable interrupts with cli (they wil be disabled by the bootloader)
                - Wait for next interrupt to arrive with halt instruction (hlt)
                - Jump to hlt instruction if it ever wakes up due to a non-maskable
                  interrupt occuring or due to system management mode.
        */
        cli
1:      hlt
        jmp 1b

/* set the size of _start symbol to the current location minus its start.
Useful for debugging or if I implement call tracing
*/
.size _start, . - _start
