
#pragma once
#include <stdint.h>

#include "regs.h"



int main(void);


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


static uint64_t systick(void)
{
    // Notify the ESP of a incoming timer read to populate the registers.
    ((volatile uint32_t *)ESP32_TIMERGROUP0)[3] = 1;
    
    // Give it a minute
    //asm volatile("nop");

    // Read the value as a 64 bit integer which corresponds to 
    return ((uint64_t) ((volatile uint32_t *)ESP32_TIMERGROUP0)[2] << 32) | ((volatile uint32_t *)ESP32_TIMERGROUP0)[1];
}


static uint64_t uptime_us(void)
{
    // systick returns in 32nds of a microsecond.
    return systick() >> 5;
}


static void delay_us(unsigned long us)
{
    // Tick value of the intended timeout.
    uint64_t timeout = uptime_us() + us;

    // Wait until the timeout.
    while (uptime_us() < timeout)
    {
        asm volatile("nop");
    }
}


static void delay_ms(unsigned long ms)
{
    delay_us(ms * 1000);
}

static void delay_s(unsigned long s)
{
    delay_us(s * 1000000);
}