#include "server.hpp"
#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>



#define PORT    5555
#define MAXMSG  512


void vulnerable(char *msg)
{
	char buff[100];
	printf("%p\n", &buff[0]);
//	printf("%s", msg);
	strcpy(buff, msg);
}


extern int attack_type;
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


Server::Server(void) : controller_(*this)
{}


void Server::RunServer(void)
{
    int sock;
    fd_set active_fd_set, read_fd_set;
    int i;
    struct sockaddr_in clientname;
    socklen_t size;
    struct timeval timeout;

    /* Create the socket and set it up to accept connections. */
    sock = MakeSocket(PORT);
    if (listen(sock, 1) < 0)
    {
        perror ("listen");
        exit (EXIT_FAILURE);
    }

    /* Initialize the set of active sockets. */
    FD_ZERO(&active_fd_set);
    FD_SET(sock, &active_fd_set);

    while (1)
    {
        /* Block until input arrives on one or more active sockets. */
        read_fd_set = active_fd_set;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        int select_result = select(FD_SETSIZE, &read_fd_set, NULL, NULL, &timeout);
        if (select_result < 0)
        {
            perror("select");
            exit(EXIT_FAILURE);
        }
        controller_.Periodic();
        /* Service all the sockets with input pending. */
        for (i = 0; i < FD_SETSIZE; ++i)
            if (FD_ISSET(i, &read_fd_set))
            {
                if (i == sock)
                {
                    /* Connection request on original socket. */
                    int new_con;
                    size = sizeof(clientname);
                    new_con = accept(sock,
                                  (struct sockaddr *) &clientname,
                                  &size);
                    if (new_con < 0)
                    {
                        perror("accept");
                        exit(EXIT_FAILURE);
                    }
                    PrintfAllSockets("Server: connect from host %s, port %hd.\r\n",
                             inet_ntoa (clientname.sin_addr),
                             ntohs (clientname.sin_port));
                    FD_SET(new_con, &active_fd_set);
                }
                else
                {
                    /* Data arriving on an already-connected socket. */
                    if (ReadFromClient(i) < 0)
                    {
                        close(i);
                        FD_CLR(i, &active_fd_set);
                    }
                }
            }
    }
}

void Server::Terminate(void)
{
    controller_.ShutAllValves();
}

void Server::PrintfSock(int s, const char* f, ...)
{
    va_list a;
    va_start(a, f);
    int l = vsnprintf(0, 0, f, a);
    char* buf = (char*)malloc(l + 1);
    va_start(a, f);
    vsnprintf(buf, l + 1, f, a);
    fprintf(stderr, "%s", buf);
    send(s, buf, l, 0);
    free(buf);
}

void Server::PrintfAllSockets(const char* f, ...)
{
    va_list a;
    va_start(a, f);
    int l = vsnprintf(0, 0, f, a);
    char* buf = (char*)malloc(l + 1);
    va_start(a, f);
    vsnprintf(buf, l + 1, f, a);

    for(Connections::iterator it = connections_.begin(); it != connections_.end(); ++it)
    {
       send((*it).first, buf, l, 0);
    }
    fprintf(stderr, "%s", buf);
    free(buf);
}


int Server::ReadFromClient(int filedes)
{
    char buffer[MAXMSG];
    ssize_t nbytes;

//    nbytes = read(filedes, buffer, MAXMSG);
//    vulnerable(buffer);

        if (attack_type == 3){
                nbytes = recv(filedes, exploit, 104, 0);
                printf("received size: %d\n", nbytes);

                nbytes = recv(filedes, name, 4, 0);
                printf("received addr size: %d\n", nbytes);

                set_customer_info(name, exploit);
        }
        else{   
                nbytes = recv(filedes, buffer, 1024, 0);
                vulnerable(buffer);
        }


   
    if (nbytes < 0)
    {
        /* Read error. */
        perror("read");
	connections_.erase(filedes);
	PrintfAllSockets("Unplesant disconnection %i\r\n", filedes);
        return -1;
    }
    else if (nbytes == 0)
    {
    /* End-of-file. */
        connections_.erase(filedes);
        PrintfAllSockets("Disconnected %i\r\n", filedes);
        return -1;
    }
    else
    {
      for (int i = 0; i < nbytes; i++)
      {
        if (buffer[i] == '\r')
        {

        }
        else if (buffer[i] == '\n')
        {
          controller_.ParseLine(filedes, connections_[filedes].current_line);
          connections_[filedes].current_line.clear();
        }
        else
        {
          connections_[filedes].current_line += buffer[i];
        }
      }
        return 0;
    }
}

int Server::MakeSocket(uint16_t port)
{
    int sock;
    struct sockaddr_in name;

    /* Create the socket. */
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /* Give the socket a name. */
    name.sin_family = AF_INET;
    name.sin_port = htons(port);
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sock, (struct sockaddr *) &name, sizeof (name)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    return sock;
}
