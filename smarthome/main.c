#include<bcm2835.h>
#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>

#define PIN_LAMP_1 RPI_BPLUS_GPIO_J8_03
#define PIN_LAMP_2 RPI_BPLUS_GPIO_J8_05
#define PIN_LAMP_3 RPI_BPLUS_GPIO_J8_07
#define PIN_LAMP_4 RPI_BPLUS_GPIO_J8_11
#define PIN_LAMP_5 RPI_BPLUS_GPIO_J8_13


int LAMP_1 = 1;
int LAMP_2 = 1;
int LAMP_3 = 1;
int LAMP_4 = 1;
int LAMP_5 = 1;

void lamp_on(int num);
void lamp_off(int num);
void accept_command(int mainSocket, char* string);

void vulnerable(char *arg) {
    char buff[100];
    printf("%p\n", &buff[0]);
    printf("%s", arg);
    strcpy(buff, arg);
}

int main()
{
	struct sockaddr_in addr;
	int client;
	char buf[1024];

	addr.sin_family = AF_INET;
	addr.sin_port = htons(5555);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	//init harware part
	if(!bcm2835_init())
	{
		printf("Error!\n");
		return 1;
	}
	bcm2835_gpio_fsel(PIN_LAMP_1, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_write(PIN_LAMP_1, HIGH);
        bcm2835_gpio_fsel(PIN_LAMP_2, BCM2835_GPIO_FSEL_OUTP);
        bcm2835_gpio_write(PIN_LAMP_2, HIGH);
        bcm2835_gpio_fsel(PIN_LAMP_3, BCM2835_GPIO_FSEL_OUTP);
        bcm2835_gpio_write(PIN_LAMP_3, HIGH);
        bcm2835_gpio_fsel(PIN_LAMP_4, BCM2835_GPIO_FSEL_OUTP);
        bcm2835_gpio_write(PIN_LAMP_4, HIGH);
        bcm2835_gpio_fsel(PIN_LAMP_5, BCM2835_GPIO_FSEL_OUTP);
        bcm2835_gpio_write(PIN_LAMP_5, HIGH);
        //bcm2835_gpio_fsel(PIN_LAMP_1, BCM2835_GPIO_FSEL_OUTP);
        //bcm2835_gpio_write(PIN_LAMP_1, HIGH);
        //bcm2835_gpio_fsel(PIN_LAMP_2, BCM2835_GPIO_FSEL_OUTP);
        //bcm2835_gpio_write(PIN_LAMP_1, HIGH);
        //bcm2835_gpio_fsel(PIN_LAMP_3, BCM2835_GPIO_FSEL_OUTP);
        //bcm2835_gpio_write(PIN_LAMP_1, HIGH);
        //bcm2835_gpio_fsel(PIN_LAMP_4, BCM2835_GPIO_FSEL_OUTP);
        //bcm2835_gpio_write(PIN_LAMP_1, HIGH);
        //bcm2835_gpio_fsel(PIN_LAMP_5, BCM2835_GPIO_FSEL_OUTP);
        //bcm2835_gpio_write(PIN_LAMP_1, HIGH);
	//bcm2835_gpio_fsel(PIN_LAMP_1, BCM2835_GPIO_FSEL_OUTP);
        //bcm2835_gpio_write(PIN_LAMP_1, HIGH);
        //bcm2835_gpio_fsel(PIN_LAMP_2, BCM2835_GPIO_FSEL_OUTP);
        //bcm2835_gpio_write(PIN_LAMP_1, HIGH);
        //bcm2835_gpio_fsel(PIN_LAMP_3, BCM2835_GPIO_FSEL_OUTP);
        //bcm2835_gpio_write(PIN_LAMP_1, HIGH);
        //bcm2835_gpio_fsel(PIN_LAMP_4, BCM2835_GPIO_FSEL_OUTP);
        //bcm2835_gpio_write(PIN_LAMP_1, HIGH);
        //bcm2835_gpio_fsel(PIN_LAMP_5, BCM2835_GPIO_FSEL_OUTP);
        //bcm2835_gpio_write(PIN_LAMP_1, HIGH);
	//init server part
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		printf("Can't start server!\n");
		return 1;
	}
	if(bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0)
	{
		printf("Error binding!\n");
		return 2;
	}
	listen(sock, 5);
	while(1)
	{
		printf("Wait for connection...\n");
		accept_command(sock,buf);
		printf("%s\n",buf);

	//	vulnerable(buf);

		if(strcmp(buf,"lamp1") == 0)
		{
			if(LAMP_1 == 0)
			{
				bcm2835_gpio_write(PIN_LAMP_1, LOW);
				LAMP_1 = 1;
				printf("lamp 1 turned on\n");
			}
			else
			{
				bcm2835_gpio_write(PIN_LAMP_1, HIGH);
				LAMP_1 = 0;
				printf("lamp 1 turned off\n");
			}
		}
		if(strcmp(buf,"lamp2") == 0)
                {
                        if(LAMP_2 == 0)
                        {
                                bcm2835_gpio_write(PIN_LAMP_2, LOW);
                                LAMP_2 = 1;
				printf("lamp 2 turned off\n");
                        }
                        else
                        {
                                bcm2835_gpio_write(PIN_LAMP_2, HIGH);
                                LAMP_2 = 0;
				printf("lamp 2 turned on\n");
                        }
                }
		if(strcmp(buf,"lamp3") == 0)
                {
                        if(LAMP_3 == 0)
                        {
                                bcm2835_gpio_write(PIN_LAMP_3, LOW);
                                LAMP_3 = 1;
				printf("lamp 3 turned off\n");
                        }
                        else
                        {
                                bcm2835_gpio_write(PIN_LAMP_3, HIGH);
                                LAMP_3 = 0;
				printf("lamp 3 turned on\n");
                        }
                }
		if(strcmp(buf,"lamp4") == 0)
		{
			if(LAMP_4 == 1)
			{
				bcm2835_gpio_write(PIN_LAMP_4, HIGH);
				LAMP_4 = 0;
				printf("lamp 4 turned off\n");
			}
			else
			{
				bcm2835_gpio_write(PIN_LAMP_4, LOW);
				LAMP_4 = 1;
				printf("lamp 4 turned on\n");
			}
		}
		if(strcmp(buf,"lamp5") == 0)
                {
                        if(LAMP_5 == 1)
                        {
                                bcm2835_gpio_write(PIN_LAMP_5, HIGH);
                                LAMP_5 = 0;
				printf("lamp 5 turned off\n");
                        }
                        else
                        {
                                bcm2835_gpio_write(PIN_LAMP_5, LOW);
                                LAMP_5 = 1;
				printf("lamp 5 turned on\n");
                        }
                }
		if(strcmp(buf,"shutdown") == 0)
		{
			system("sudo ./off");
			printf("server will shut down now\n");
			return 0;
		}

	}
	return 0;
}

void lamp_on(int num)
{
	bcm2835_gpio_write(num, LOW);
}

void lamp_off(int num)
{
	bcm2835_gpio_write(num, HIGH);
}

void accept_command(int mainSock,char* string)
{
	int client = accept(mainSock,NULL,NULL);
	char buff[1024];
	memset(buff,0,1024);
	int received = recv(client,buff,1024-1,0);

	vulnerable(buff);
	buff[received] = 0;
	strcpy(string,buff);
}
