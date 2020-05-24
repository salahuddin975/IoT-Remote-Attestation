/* Copyright (C) 2015 ANSSI

   This file is part of the Picon project.

   This file is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This file is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this file; if not, see
   <http://www.gnu.org/licenses/>.  */

#include "defs.h"

/* EDITED BEGIN*/
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
/*EDITED END*/

/*Modify Begin*/
unsigned int CFI_LOCK=0;
unsigned int CFI_HASH=0;


/* ELF Hash Function */
unsigned int ELFHash(char *str)
{
    unsigned int hash = 0;
    unsigned int x = 0;

    while (*str)
    {
        hash = (hash << 4) + (*str++);
        if ((x = hash & 0xF0000000L) != 0)
        {
            hash ^= (x >> 24);
            hash &= ~x;
        }
    }
    return (hash & 0x7FFFFFFF);
}

unsigned int hashchain(char *new_cfi){
    char buff[1024];
    memset(buff, 0, sizeof(buff));
    sprintf(buff, "%x", CFI_HASH);
    strcat(buff, new_cfi);
    unsigned int n = ELFHash(buff);
    return n;
}

int open_socket(){
    int serverSocket;
    struct sockaddr_in server_addr;
    struct sockaddr_in clientAddr;
    int addr_len = sizeof(clientAddr);
    int client;
    char buffer[200];
    int iDataNum;
    if((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        return 1;
    }
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(serverSocket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("connect");
        return 1;
    }

    if(listen(serverSocket, 5) < 0)
    {
        perror("listen");
        return 1;
    }
    while(1)
    {
        printf("监听端口: %d\n", SERVER_PORT);
        client = accept(serverSocket, (struct sockaddr*)&clientAddr, (socklen_t*)&addr_len);
        if(client < 0)
        {
            perror("accept");
            continue;
        }
        printf("等待消息...\n");
        printf("IP is %s\n", inet_ntoa(clientAddr.sin_addr));
        printf("Port is %d\n", htons(clientAddr.sin_port));
        while(1)
        {
            printf("读取消息:");
            buffer[0] = '\0';
            iDataNum = recv(client, buffer, 1024, 0);
            if(iDataNum < 0)
            {
                perror("recv null");
                continue;
            }
            buffer[iDataNum] = '\0';
            if(strcmp(buffer, "quit") == 0)
                break;
            if(strcmp(buffer, "Start") == 0){
                printf("Start Recording CFI");
                CFI_LOCK = 1;
            }
            else if(strcmp(buffer, "Finish")) {
                printf("Stop Recording CFI");
                if (CFI_LOCK == 1) {
                    if (CFI_HASH != 0) send(client, CFI_HASH, sizeof(CFI_HASH), 0);
                }
                CFI_LOCK = 0;
            }
            if(strcmp(buffer, "quit") == 0)
                break;
        }
    }
    close(serverSocket);
    return 0;
}
/*Modify End*/


unsigned int OPTION_sm_trace = 0;
unsigned int OPTION_sm_dump = 0;
unsigned int OPTION_nb_preload_monitor_ok_answers =
#ifdef NB_PRELOADED_MONITOR_OK_ANSWERS
  NB_PRELOADED_MONITOR_OK_ANSWERS
#else
  0
#endif
  ;



#ifdef TIMEOUT


struct timespec OPTION_timeout_monitor_no_signal =
  {
    .tv_sec =
#ifdef TIMEOUT_SEC_MONITOR_NO_SIGNAL
    TIMEOUT_SEC_MONITOR_NO_SIGNAL
#else
    0
#endif
    ,

    .tv_nsec =
#ifdef TIMEOUT_NANOSEC_MONITOR_NO_SIGNAL
    TIMEOUT_NANOSEC_MONITOR_NO_SIGNAL
#else
    0
#endif
    ,
  };


#endif
