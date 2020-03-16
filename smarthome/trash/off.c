#include<bcm2835.h>
#include<stdio.h>


#define PIN1 RPI_BPLUS_GPIO_J8_03
#define PIN2 RPI_BPLUS_GPIO_J8_05
#define PIN3 RPI_BPLUS_GPIO_J8_07
#define PIN4 RPI_BPLUS_GPIO_J8_11
#define PIN5 RPI_BPLUS_GPIO_J8_13

int main()
{
	if(!bcm2835_init())
	{
		printf("Error!\n");
		return 0;
	}
	bcm2835_gpio_fsel(PIN1, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(PIN2, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(PIN3, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(PIN4, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(PIN5, BCM2835_GPIO_FSEL_OUTP);

	bcm2835_gpio_write(PIN1, LOW);
        bcm2835_gpio_write(PIN2, LOW);
        bcm2835_gpio_write(PIN3, LOW);
        bcm2835_gpio_write(PIN4, LOW);
        bcm2835_gpio_write(PIN5, LOW);

	return 0;
}
