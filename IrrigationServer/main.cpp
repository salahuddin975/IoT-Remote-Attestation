
#include <string>
#include <signal.h>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <map>
#include <vector>
#include <time.h>
#include <pthread.h>
#include <iostream>
#include <link.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <openssl/sha.h>
#include "server.hpp"



//#define CHECKSUM_PORT     8080 
//#define MAXLINE 1024 


Server s;
extern int attack_type = 0;


//void calculate_checksum(char *hash_value, int type, unsigned int seed, int num_of_blocks, int block_size);

void *checksum(void *vargp);

/*
void *checksum(void *vargp)
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
        std::stringstream ss(buffer);
        ss >> type;
	ss >> seed;
	ss >> num_of_blocks;
	ss >> block_size;
        printf("buffer: %s; type: %d; seed: %d; num_of_blocks: %d, block_size: %d\n", buffer, type, seed,num_of_blocks, block_size);
        calculate_checksum(hash_value, type, seed, num_of_blocks, block_size);

        sendto(sockfd, (const char *)hash_value, SHA256_DIGEST_LENGTH, MSG_CONFIRM, (const struct sockaddr *) &cliaddr, addr_len); 
    }
}
*/

void TerminationHandler(void)
{
    s.Terminate();
}


void SignalHandler(int signum)
{
    s.Terminate();
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


    atexit(TerminationHandler);
    if (signal (SIGINT, SignalHandler) == SIG_IGN)
        signal (SIGINT, SIG_IGN);
    if (signal (SIGHUP, SignalHandler) == SIG_IGN)
        signal (SIGHUP, SIG_IGN);
    if (signal (SIGTERM, SignalHandler) == SIG_IGN)
        signal (SIGTERM, SIG_IGN);

    s.RunServer();

}
