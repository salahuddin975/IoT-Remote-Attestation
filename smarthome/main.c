#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include <pthread.h>
#include <iostream>
#include <link.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include "xxhash.h"
#include "bcm2835.h"


#define CHECKSUM_PORT     8081
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

static int checksum_callback(struct dl_phdr_info *info, size_t size, void *data)
{
    XXH64_state_t* state = (XXH64_state_t*)data;
    for (int header_id = 0; header_id < info->dlpi_phnum; header_id++) {
        const ElfW(Phdr) *hdr = &info->dlpi_phdr[header_id];
        // Skip over headers that can not include read-only data that
        // possibly is also executable code:
        if (hdr->p_memsz == 0) {
            continue;
        }
        if ((hdr->p_flags & (PF_R | PF_X)) != (PF_R | PF_X)) {
            continue;
        }
        if (hdr->p_flags & PF_W) {
            continue;
        }

        XXH_errorcode result = XXH64_update(
            state,
            (void*)(info->dlpi_addr + hdr->p_vaddr),
            hdr->p_memsz);
        if (result != XXH_OK) {
            abort();
        }
    }
    return 0;
}


void calculate_checksum(char* hash_value)
{
    XXH64_state_t* state = XXH64_createState();
    if (state == NULL) {
        fprintf(stderr, "Unable to allocate xxHash state!\n");
        return EXIT_FAILURE;
    }
    if (XXH64_reset(state, 0) != XXH_OK) {
        fprintf(stderr, "Failed to zero xxHash state!\n");
        return EXIT_FAILURE;
    }
    dl_iterate_phdr(checksum_callback, state);
    XXH64_hash_t digest = XXH64_digest (state);
    XXH64_canonical_t canonical;
    XXH64_canonicalFromHash(&canonical, digest);
    
    for (size_t byte_id = 0; byte_id < sizeof(canonical.digest); byte_id++) {
        printf("%02x", canonical.digest[byte_id]);
    }
    printf("\n");

    strncpy(hash_value, canonical.digest, 8);
    hash_value[8] = '\0';
    
    XXH64_freeState(state);
}

void *checksum(void *vargp)
{
    int sockfd; 
    char hash_value[9];
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

    int len, n; 
    len = sizeof(cliaddr);

    while(recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len)){
        calculate_checksum(hash_value);
        sendto(sockfd, (const char *)hash_value, strlen(hash_value), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len); 
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
        char *a = malloc(500);        

        struct Customer *cust = malloc(sizeof(struct Customer));
        printf("addr: %p\n", cust->address);
        cust->name = malloc(12);

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

        pthread_t tid;
        pthread_create(&tid, NULL, checksum, NULL);

        std::cout << "running main program." << std::endl;


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
