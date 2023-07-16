
#pragma once
#include <stdint.h>

#include "regs.h"


static inline void init(void) {

    // Disable write protection on the RTC
    ((volatile uint32_t *)ESP32_RTCCNTL)[41] = 0x50d83aa1;
    
    // SW feeds the Watchdog Timer?
    ((volatile uint32_t *)ESP32_RTCCNTL)[40] |= BIT(31);

    // Disable the RTC Watchdog Timer
    ((volatile uint32_t *)ESP32_RTCCNTL)[35] = 0;

    // Disable task Watchdog Timer
    ((volatile uint32_t *)ESP32_TIMERGROUP0)[18] = 0;

    // Disable task Watchdog Timer
    ((volatile uint32_t *)ESP32_TIMERGROUP1)[18] = 0;

    // Enable TIMG0
    ((volatile uint32_t *)ESP32_TIMERGROUP0)[0] |= BIT(31);

    // Enable slow RTC clock speed
    ((volatile uint32_t *)ESP32_RTCCNTL)[28] |= 1UL << 27;

    // Select CPU Clock frequency
    ((volatile uint32_t *)ESP32_DPORT)[15] |= 2UL << 0;

    // Set UART frequency
    ((volatile uint32_t *)ESP32_UART0)[5] = 0x4001e0;

}

extern char _sbss, _ebss, _end, _eram;

static char *s_heap_start, *s_heap_end, *s_brk;

// Startup logic; this is the application entry point.
void __attribute__((noreturn)) call_start_cpu0()
{
    s_heap_start = s_brk = &_end, s_heap_end = &_eram;

    for (char *p = &_sbss; p < &_ebss;) *p++ = '\0';

    init();

    main();

    for (;;)
    {
        ;
    }
}
