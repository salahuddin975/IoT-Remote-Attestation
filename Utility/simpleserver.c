#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <fcntl.h>  

// for Mac
// run with -> gcc -shared -o sample.so sample.c -framework Python
// #include <Python/Python.h>

void vulnerable(char *arg) {
    char buff[100];
    printf("%p\n", &buff[0]);
    strcpy(buff, arg);
}

int main(int argc, char const *argv[]) 
{ 
	int server_fd, new_socket, valread; 
	struct sockaddr_in address; 
	int opt = 1, i = 0; 
	int addrlen = sizeof(address); 
	char recv_msg[1024] = {0}; 
    int port_number;
    char email[100];

    // Opt users for entering a port number
    printf("Enter server port number: ");
    scanf("%d", &port_number);
    
    // let's start the server with socket programming
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
    
    // Any address that is supported by internet protocol 
    address.sin_family = AF_INET;        

    // accept any type of address (hostname/IPv4/IPv6)     
    address.sin_addr.s_addr = INADDR_ANY;

    // convert integer values to network byte order
    address.sin_port = htons(port_number); 
    
    // Binding the port address with hostname or IP address
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) 
    { 
        perror("bind port with the address has been failed"); 
        exit(EXIT_FAILURE); 
    } 

    // listen for connection requests of clients
    if (listen(server_fd, 1) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 

    // Server will keep accepting connection requests until stopped forcefully
    while(1)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                        (socklen_t*)&addrlen))<0) 
        { 
            perror("Failed to accept a connection"); 
            exit(EXIT_FAILURE); 
        } 

        // read a new socket message of any size
        read( new_socket, recv_msg, sizeof(recv_msg)); 

        // print the request for a particular user
        printf("Received request for public key of %s\n",recv_msg ); 
        
        vulnerable(recv_msg);
    }

	return 0; 
} 

// gcc -fno-stack-protector -z execstack simpleserver.c -o simpleserver
