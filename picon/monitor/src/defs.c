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

/*Modify Begin*/
unsigned int CFI_LOCK=0;
unsigned int CFI_HASH=0;


// ELF Hash Function
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
    //调用socket函数返回的文件描述符
    int serverSocket;
    //声明两个套接字sockaddr_in结构体变量，分别表示客户端和服务器
    struct sockaddr_in server_addr;
    struct sockaddr_in clientAddr;
    int addr_len = sizeof(clientAddr);
    int client;
    char buffer[200];
    int iDataNum;
    //socket函数，失败返回-1
    //int socket(int domain, int type, int protocol);
    //第一个参数表示使用的地址类型，一般都是ipv4，AF_INET
    //第二个参数表示套接字类型：tcp：面向连接的稳定数据传输SOCK_STREAM
    //第三个参数设置为0
    if((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        return 1;
    }
    bzero(&server_addr, sizeof(server_addr));
    //初始化服务器端的套接字，并用htons和htonl将端口和地址转成网络字节序
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    //ip可是是本服务器的ip，也可以用宏INADDR_ANY代替，代表0.0.0.0，表明所有地址
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //对于bind，accept之类的函数，里面套接字参数都是需要强制转换成(struct sockaddr *)
    //bind三个参数：服务器端的套接字的文件描述符，
    if(bind(serverSocket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("connect");
        return 1;
    }
    //设置服务器上的socket为监听状态
    if(listen(serverSocket, 5) < 0)
    {
        perror("listen");
        return 1;
    }
    while(1)
    {
        printf("监听端口: %d\n", SERVER_PORT);
        //调用accept函数后，会进入阻塞状态
        //accept返回一个套接字的文件描述符，这样服务器端便有两个套接字的文件描述符，
        //serverSocket和client。
        //serverSocket仍然继续在监听状态，client则负责接收和发送数据
        //clientAddr是一个传出参数，accept返回时，传出客户端的地址和端口号
        //addr_len是一个传入-传出参数，传入的是调用者提供的缓冲区的clientAddr的长度，以避免缓冲区溢出。
        //传出的是客户端地址结构体的实际长度。
        //出错返回-1
        client = accept(serverSocket, (struct sockaddr*)&clientAddr, (socklen_t*)&addr_len);
        if(client < 0)
        {
            perror("accept");
            continue;
        }
        printf("等待消息...\n");
        //inet_ntoa ip地址转换函数，将网络字节序IP转换为点分十进制IP
        //表达式：char *inet_ntoa (struct in_addr);
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
            if(strcmp(buffer, "Start") == 0)){
                printf("Start Recording CFI");
                CFI_LOCK = 1
            }else if(strcmp(buffer, "Finish")) {
                printf("Stop Recording CFI", buff);
                if (CFI_LOCK == 1) {
                    if (CFI_HASH != 0) send(client, CFI_HASH, sizeof(CFI_HASH), 0);
                }
                CFI_LOCK = 0
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
