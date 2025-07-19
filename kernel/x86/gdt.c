#include <gdt.h>

static GDTEntry descriptors[5];
static GDTPtr gdtptr;

void
gdt_reload()
{
	asm volatile("lgdt %0\n\t"
		"push $0x28\n\t"
		"lea 1f(%%rip), %%rax\n\t"
             	"push %%rax\n\t"
               	"lretq\n\t"
               	"1:\n\t"
               	"mov $0x30, %%eax\n\t"
               	"mov %%eax, %%ds\n\t"
               	"mov %%eax, %%es\n\t"
               	"mov %%eax, %%fs\n\t"
               	"mov %%eax, %%gs\n\t"
               	"mov %%eax, %%ss\n\t"
               	:
               	: "m"(gdtptr)
               	: "rax", "memory");	
}

void
initGDT()
{
	/* null descriptor (0) */
	descriptors[0].limit = 0;
	descriptors[0].base_low = 0;
  	descriptors[0].base_mid = 0;
  	descriptors[0].access = 0;
  	descriptors[0].granularity = 0;
  	descriptors[0].base_high = 0;
	
	 /* kernel code 64 (40) */
  	descriptors[1].limit = 0;
  	descriptors[1].base_low = 0;
  	descriptors[1].base_mid = 0;
  	descriptors[1].access = 0b10011010;
  	descriptors[1].granularity = 0b00100000;
  	descriptors[1].base_high = 0;

  	/* kernel data 64 (48) */
  	descriptors[2].limit = 0;
  	descriptors[2].base_low = 0;
  	descriptors[2].base_mid = 0;
  	descriptors[2].access = 0b10010010;
  	descriptors[2].granularity = 0;
  	descriptors[2].base_high = 0;

	// user code 64 (72)
  	descriptors[3].limit = 0;
  	descriptors[3].base_low = 0;
  	descriptors[3].base_mid = 0;
  	descriptors[3].access = 0b11111010;
  	descriptors[3].granularity = 0b00100000;
  	descriptors[3].base_high = 0;

  	// user data 64. (80)
  	descriptors[4].limit = 0;
  	descriptors[4].base_low = 0;
  	descriptors[4].base_mid = 0;
  	descriptors[4].access = 0b11110010;
  	descriptors[4].granularity = 0;
  	descriptors[4].base_high = 0;
	
	gdtptr.limit = sizeof(descriptors) - 1;
	gdtptr.base = (uint64_t)&descriptors;

	gdt_reload();
}
