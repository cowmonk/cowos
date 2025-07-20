#include <stdint.h>

#ifndef GDT_H
#define GDT_H

typedef struct GDTEntry {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_mid;
	uint8_t access;
	uint8_t limit_high_and_flags;
	uint8_t base_high;
} __attribute__((packed)) GDTEntry;

typedef struct GDTPtr {
	uint16_t limit;
	uint64_t base;
} __attribute__((packed)) GDTPtr;

#define GDT_NUM_ENTRIES 5 // Incl. null descriptor 
#define GDT_KERNEL_CODE 0x08
#define GDT_KERNEL_DATA 0x10
#define GDT_USER_CODE   0x18
#define GDT_USER_DATA   0x20

void initGDT();

#endif /* GDT_H */
