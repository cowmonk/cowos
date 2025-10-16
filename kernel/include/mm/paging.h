#ifndef PAGING_H
#define PAGING_H

#include <limine.h>

volatile struct limine_hhdm_request hhdm_request;
volatile struct limine_kernel_address_request kernel_address_request;

void map_vga_memory(void);

#endif /* ifndef PAGING_H */
