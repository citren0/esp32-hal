#include <string.h>

#include "gpio.h"

extern unsigned int _sbss, _ebss, _sidata, _sdata, _edata;

void spin(void)
{
  volatile int var = 100000;
  while (var--) ;
}

int main(void)
{
	setMode(26, OUTPUT);
	// Increment a variable.
	while (1)
	{
		writePin(26, HIGH);
		spin();
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
	// Done, branch to main
	main();
	// (Should never be reached)
	while (1)
	{
		;
	}
}