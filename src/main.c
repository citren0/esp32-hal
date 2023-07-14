#include <string.h>

#include "gpio.h"
#include "init.c"

extern unsigned int _sbss, _ebss, _sidata, _sdata, _edata;

void spinForever(void)
{
	while (1)
	{
		asm("nop");
	}
	
}

void spin(void)
{
	long val = 30000000;
	while (val)
	{
		asm("nop");
		val--;
	}
}

int main(void)
{
	setMode(26, OUTPUT);
	
	while (1)
	{
		spin();
		togglePin(26);

	}
	

  return 0;
}


// Startup logic; this is the application entry point.
void __attribute__((noreturn)) call_start_cpu0()
{
	// Clear BSS.
	memset(&_sbss, 0, (&_ebss - &_sbss) * sizeof(_sbss));
	// Copy initialized data.
	memmove(&_sdata, &_sidata, (&_edata - &_sdata) * sizeof(_sdata));
	// Initialize system.
	init();
	// Done, branch to main
	main();
	// (Should never be reached)
	while (1)
	{
		;
	}
}