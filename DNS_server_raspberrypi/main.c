#include "dns.h"

#define LOOP_BACK "127.0.0.1"

#define ADDRESS_LOOKUP "v10.vortex-win.data.microsoft.com"

#define TEST_ADDRESS 0

int main(int argc, char** argv){
	
 	struct sockaddr_in server_addr;
	int fd = 0;
	
	if(!TEST_ADDRESS){
		recv_query();
	}
	else{
		printf("sending\n");
		fd = send_query(ADDRESS_LOOKUP,DNS_SERVER_1,&server_addr);
		printf("recving\n");
		recv_response(fd, &server_addr);
		close(fd);
	}
	
	return 0;
}
