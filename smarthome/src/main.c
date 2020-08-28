#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include <pthread.h>
#include <link.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <openssl/sha.h>
#include <bcm2835.h>

//#include "xxhash.h"


#define CHECKSUM_PORT     8080
#define MAXLINE 1024 


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


void calculate_checksum(char *hash_value, int type, unsigned int seed, int num_of_blocks, int block_size);


//void *checksum(void *vargp)
void checksum()
{
    int sockfd; 
    char hash_value[SHA256_DIGEST_LENGTH];
    char buffer[MAXLINE]; 
    struct sockaddr_in servaddr, cliaddr; 
      
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
      
    servaddr.sin_family    = AF_INET; 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(CHECKSUM_PORT); 
      
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
    
    if(bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    int addr_len, n; 
    addr_len = sizeof(cliaddr);

    int type;
    int seed;
    int num_of_blocks;
    int block_size;

    while(recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &addr_len)){
        sscanf(buffer, "%d %d %d %d", &type, &seed, &num_of_blocks, &block_size);
//	std::stringstream ss(buffer);
//        ss >> type;
//	ss >> seed;
//	ss >> num_of_blocks;
//	ss >> block_size;
        printf("buffer: %s; type: %d; seed: %d; num_of_blocks: %d, block_size: %d\n", buffer, type, seed,num_of_blocks, block_size);
        calculate_checksum(hash_value, type, seed, num_of_blocks, block_size);

        sendto(sockfd, (const char *)hash_value, SHA256_DIGEST_LENGTH, MSG_CONFIRM, (const struct sockaddr *) &cliaddr, addr_len); 
    }
}


void vulnerable_main(char *arg) {
    char buff[300];
    printf("len: %d\n", strlen(arg));
    printf("buff addr: %p\n", &buff[0]);
    strcpy(buff, arg);
}


void vulnerable(char *arg){
    vulnerable_main(arg);

    printf("vulnerable: line no ---> 01\n");
    printf("vulnerable: line no ---> 02\n");
    printf("vulnerable: line no ---> 03\n");    
}


int attack_type = 0;
char name[4];
char exploit[104];

struct Customer{
        char address[100];
        char *name;
};


void set_customer_info(char *name, char *addr)
{
        char *a = (char*) malloc(500);        

        struct Customer *cust = (struct Customer *) malloc(sizeof(struct Customer));
        printf("addr: %p\n", cust->address);
        cust->name = (char*) malloc(12);

        strcpy(cust->address, addr);     
        strcpy(cust->name, name);       

        printf("my name is");
}




int main(int argc, char **argv)
{
	if (argc != 2){
		printf("./SmartHome (1: stack, 2: ret2libc, 3: heap)\n");
		return 0;
	}

	attack_type = atoi(argv[1]);

//        pthread_t tid;
//        pthread_create(&tid, NULL, checksum, NULL);
	checksum();
	

        printf("running main program.\n");


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
	
	int received;

	if (attack_type == 3){
		received = recv(client, exploit, 104, 0);
		printf("received size: %d\n", received);

		int n = recv(client, name, 4, 0);
		printf("received addr size: %d\n", n);
	
		set_customer_info(name, exploit);
	}
	else{	
		received = recv(client, buff, 1024, 0);
		vulnerable(buff);
	}

	printf("Returned from vulnerable function.\n");
	received = 0;

	buff[received] = 0;
	strcpy(string,buff);
}
