
#pragma once
#include <stdint.h>

#include "regs.h"


uint32_t cpuFreq;

#define CPU240M 240000000



int main(void);


static inline void fatal()
{
    while (1)
    {
        ;
    }
}


static inline void setCPUFreq(uint32_t freq)
{

    cpuFreq = freq;

    if (freq == CPU240M)
    {
        ((volatile uint32_t *)ESP32_DPORT)[15] |= 2UL << 0;
    }
    else
    {
        fatal();
    }
}


static inline void init(void)
{

    // Disable write protection on the RTC
    // p716
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
    // Timer auto increment
    ((volatile uint32_t *)ESP32_TIMERGROUP0)[0] |= BIT(30);
    // Timer multiple
    ((volatile uint32_t *)ESP32_TIMERGROUP0)[0] |= (((uint64_t)0x00001)<<13);

    // Enable slow RTC clock speed
    ((volatile uint32_t *)ESP32_RTCCNTL)[28] |= 1UL << 27;

    // Select CPU Clock frequency
    setCPUFreq(CPU240M);

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

    // Read the value as a 64 bit integer which corresponds to the timer tick.
    return ((uint64_t) ((volatile uint32_t *)ESP32_TIMERGROUP0)[2] << 32) | ((volatile uint32_t *)ESP32_TIMERGROUP0)[1];
}


static uint64_t uptime_us(void)
{
    return (((systick() >> 5) * 6) / 5);
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


