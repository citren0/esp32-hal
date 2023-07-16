#include <string.h>

#include "gpio.h"

#include "init.c"



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
		asm volatile("nop");
		val--;
	}
}

int main(void)
{
	setMode(5, OUTPUT);
	
	while (1)
	{
		delay_ms(500);
		togglePin(5);

	}
	

  return 0;
}

