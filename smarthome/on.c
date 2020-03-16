#include<bcm2835.h>
#include<stdio.h>

#define PIN1 RPI_BPLUS_GPIO_J8_03
#define PIN2 RPI_BPLUS_GPIO_J8_07

int main()
{
	if(!bcm2835_init())
	{
		printf("Error!\n");
		return 0;
	}
	bcm2835_gpio_fsel(PIN1, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(PIN2, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_write(PIN1, HIGH);
	bcm2835_gpio_write(PIN2, HIGH);
	return 0;
}
