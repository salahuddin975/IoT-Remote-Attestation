#include<bcm2835.h>
#include<stdio.h>

#define PIN RPI_BPLUS_GPIO_J8_03

int main()
{
	//int i = bcm2835_init();
	if(!bcm2835_init())
	{
		printf("Error!\n");
		return 0;
	}
	bcm2835_gpio_fsel(PIN,BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_write(PIN,LOW);
	return 0;
}
