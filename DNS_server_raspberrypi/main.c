
#define _GNU_SOURCE

#include <pthread.h>
#include <link.h>
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <time.h>
#include <openssl/sha.h>
#include "dns.h"
//#include "xxhash.h"


#define CHECKSUM_PORT     8080
#define MAXLINE 1024

#define LOOP_BACK "127.0.0.1"
#define ADDRESS_LOOKUP "v10.vortex-win.data.microsoft.com"
#define TEST_ADDRESS 0


extern int attack_type = 0;

/*
static int checksum_callback(struct dl_phdr_info *info, size_t size, void *data)
{
    XXH64_state_t* state = (XXH64_state_t*)data;
    for (int header_id = 0; header_id < info->dlpi_phnum; header_id++) {
        const ElfW(Phdr) *hdr = &info->dlpi_phdr[header_id];
        // Skip over headers that can not include read-only data that
        // possibly is also executable code:
        if (hdr->p_memsz == 0) {
            continue;
        }
        if ((hdr->p_flags & (PF_R | PF_X)) != (PF_R | PF_X)) {
            continue;
        }
        if (hdr->p_flags & PF_W) {
            continue;
        }

        XXH_errorcode result = XXH64_update(
            state,
            (void*)(info->dlpi_addr + hdr->p_vaddr),
            hdr->p_memsz);
        if (result != XXH_OK) {
            abort();
        }
    }
    return 0;
}

void calculate_checksum(char* hash_value)
{
    XXH64_state_t* state = XXH64_createState();
    if (state == NULL) {
        fprintf(stderr, "Unable to allocate xxHash state!\n");
        return EXIT_FAILURE;
    }
    if (XXH64_reset(state, 0) != XXH_OK) {
        fprintf(stderr, "Failed to zero xxHash state!\n");
        return EXIT_FAILURE;
    }
    dl_iterate_phdr(checksum_callback, state);
    XXH64_hash_t digest = XXH64_digest (state);
    XXH64_canonical_t canonical;
    XXH64_canonicalFromHash(&canonical, digest);

    for (size_t byte_id = 0; byte_id < sizeof(canonical.digest); byte_id++) {
        printf("%02x", canonical.digest[byte_id]);
    }
    printf("\n");

    strncpy(hash_value, canonical.digest, 8);
    hash_value[8] = '\0';

    XXH64_freeState(state);
}
*/

void calculate_checksum(char *hash_value, unsigned int seed);


void *checksum(void *vargp)
{
    int sockfd;
    char hash_value[SHA256_DIGEST_LENGTH];
    char buffer[MAXLINE];
    struct sockaddr_in servaddr, cliaddr;

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family    = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(CHECKSUM_PORT);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if(bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    int len, n;
    len = sizeof(cliaddr);

    while(recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len)){
        time_t t;
        unsigned int seed = (unsigned) time(&t);
        calculate_checksum(hash_value, seed);
        sendto(sockfd, (const char *)hash_value, SHA256_DIGEST_LENGTH, MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
    }

}


int main(int argc, char** argv)
{
        if (argc != 2){
                printf("./DNSserver (1: stack, 2: ret2libc, 3: heap)\n");
                return 0;
        }

        attack_type = atoi(argv[1]);

	pthread_t tid;
        pthread_create(&tid, NULL, checksum, NULL);
        printf("running main program."); 

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
