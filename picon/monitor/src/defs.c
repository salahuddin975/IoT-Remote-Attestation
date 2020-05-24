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


size_t hashchain(size_t origin_cfi,std::string new_cfi){
    std::hash<std::string> h;
    std::string origin_str = std::to_string(origin_cfi);
    size_t n = h(origin_str+new_cfi);
    return n;
}

bool check(char *src, char *dst){
    bool res = true;
    int i = 0;
    while(*(src + i) != '\0' && *(dst + i) != '\0'){
        if(*(src + i) == *(dst + i))
            continue;
        else
            return false;
    }
    if(i  +  1 == strlen(dst))
        return false;
    return true;
}

int open_socket()
{
    int  listenfd, connfd;
    struct sockaddr_in  servaddr;
    char  buff[7];
    int  n;
    if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
        printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
        return -1;
    }
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(6666);
    if(bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        return -2;
    }
    if( listen(listenfd, 10) == -1){
        printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
        return -3;
    }
    printf("======waiting for client's request======\n");
    while(1){
        if( (connfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1){
            printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
            continue;
        }
        n = recv(connfd, buff, 7, 0);
        buff[n] = '\0';
        if(check(buff,"Start")){
            printf("Start Recording CFI");
            CFI_LOCK = 1
        }else if(check(buff,"Finish")){
            printf("Stop Recording CFI", buff);
            if(CFI_LOCK == 1){
                if(CFI_HASH!=0) send(connfd, CFI_HASH, sizeof(CFI_HASH), 0);
            }
            CFI_LOCK = 0
        }
//        pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;
        close(connfd);
    }
    close(listenfd);
    return 0;
}


unsigned int OPTION_sm_trace = 0;
unsigned int CFI_LOCK=0;
size_t CFI_HASH=0;
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
