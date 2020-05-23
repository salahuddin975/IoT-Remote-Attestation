//
// Created by Xiaohan Zhang on 2020/5/23.
//

#ifndef IOT_REMOTE_ATTESTATION_HASHCHAIN_H
#define IOT_REMOTE_ATTESTATION_HASHCHAIN_H

#endif //IOT_REMOTE_ATTESTATION_HASHCHAIN_H


#include <pthread.h>
#include <functional>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

extern unsigned int CFI_LOCK;
extern unsigned size_t CFI_HASH;

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
    char  buff[4096];
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
        n = recv(connfd, buff, MAXLINE, 0);
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





