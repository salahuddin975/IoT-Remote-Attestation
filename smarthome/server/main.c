#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>

int initSocket();

int main(int argc, char* argv[])
{
	int sock = initSocket(9889);
	//printf("%d\n",sock);
	listen(sock,3);
	while(1)
	{
		int client = accept(sock,NULL,NULL);
		printf("Connected succesful!\n");
		close(client);
	}
	return 0;
}

int initSocket(int port)
{
	struct sockaddr_in addres;
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock  < 0)
	{
		printf("Error occured while creating socket");
	}
	addres.sin_family = AF_INET;
	addres.sin_port = htons(port);
	addres.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(sock, (struct sockaddr*)&addres, sizeof(addres)) < 0)
	{
		printf("Error ocured while binding socket");
	}
	return sock;
}
