#include <string.h>

#include "gpio.h"





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
	setMode(5, OUTPUT);
	
	while (1)
	{
		spin();
		togglePin(5);

	}
	

  return 0;
}




#include "init.c"