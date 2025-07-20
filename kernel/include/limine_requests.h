#ifndef LIMINE_REQUESTS_H
#define LIMINE_REQUESTS_H

#include "limine.h"

// Only define once to avoid duplicates (result: kernel panic) 
#ifdef DEFINE_LIMINE_REQUESTS

__attribute__((used, section(".limine_requests")))
volatile LIMINE_BASE_REVISION(3);

__attribute__((used, section(".limine_requests")))
volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

__attribute__((used, section(".limine_requests_start")))
volatile LIMINE_REQUESTS_START_MARKER

__attribute__((used, section(".limine_requests_end")))
volatile LIMINE_REQUESTS_END_MARKER;

#else

// External declaration for other files 
extern volatile struct limine_framebuffer_request framebuffer_request;

#endif

#endif // LIMINE_REQUESTS_H 
