
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
#include "server.hpp"



Server s;
extern int attack_type = 0;

void *checksum(void *vargp);


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
