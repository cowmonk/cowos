ENTRY(_start) /* Entry point symbol */

SECTIONS {
        /* Load address in memory: 2MB for kernel */
        . = 2M;

        /* Multiboot header must be in the first 8KB of the file and 32-bit aligned */
        .text BLOCK(4K) : ALIGN(4K) {
                *(.multiboot)
                *(.text)
        }

        .rodata BLOCK(4K) : ALIGN(4K) {
                *(.rodata)
        }

        .data BLOCK(4K) : ALIGN(4K) {
                *(.data)
        }

        .bss BLOCK(4K) : ALIGN(4K) {
                *(COMMON)
                *(.bss)
        }
}
