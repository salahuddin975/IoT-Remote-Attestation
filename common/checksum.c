#define _GNU_SOURCE
#include <link.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define CHECKSUM_PORT 8080
#define MAXLINE 1024

#define MAX_PATH_LEN 4096          // lINUX: 4096, windows: 260
#define MAX_NUM_OF_LIBS 200


struct library{
	char name[MAX_PATH_LEN];
	int addr;
	int size;
};

int num_libs = 0;

enum ChecksumType{
	SEQUENTIAL = 1,
	RANDOM = 2
};


static int callback_set_lib_addr(struct dl_phdr_info *info, size_t size, void *data)
{
    struct library* libs = (struct library*) data; 
    
    for (int header_id = 0; header_id < info->dlpi_phnum; header_id++) 
    {
        const ElfW(Phdr) *hdr = &info->dlpi_phdr[header_id];

        if (hdr->p_memsz == 0) {
            continue;
        }
        if ((hdr->p_flags & (PF_R | PF_X)) != (PF_R | PF_X)) {
            continue;
        }
        if (hdr->p_flags & PF_W) {
            continue;
        }
		
        int addr = info->dlpi_addr + hdr->p_vaddr;

        if(num_libs == 0){
            strcpy(libs[num_libs].name, info->dlpi_name);
            libs[num_libs].addr = addr;
            libs[num_libs].size = hdr->p_memsz;
        }
        else{
            for(int i=num_libs; i>0; i--){            // push libs addresses in sorted order
                if(addr < libs[i-1].addr){
                    libs[i] = libs[i-1];
                }
                else{
                    strcpy(libs[i].name, info->dlpi_name);
                    libs[i].addr = addr;
                    libs[i].size = hdr->p_memsz;
                    break;
                }
            }
        }
        num_libs++;	
    }
    return 0;
}


void random_checksum(struct library *libs, int *blocks_pos, char *hash_value, int num_of_blocks, int block_size)
{
    SHA256_CTX sha256;
    SHA256_Init(&sha256);

    for(int i=0; i<num_of_blocks; i++){
    	int block_pos = blocks_pos[i];
	int location = block_pos * block_size;

	for (int j=0; j<num_libs; j++){
            if(location > libs[j].size){             // Find the library
                location = location - libs[j].size;
                continue;
            }
			
            if((location + block_size) < libs[j].size) {           // if the block size fit in the library
                //printf("value: %d, libs: %d: %s\n", locations[i], j, libs[j].name);
                SHA256_Update(&sha256, libs[j].addr + location, block_size);
                break;
            }
            else{            // if doesn't fit the block size
                //printf("Else, value: %d, libs: %d: %s\n", locations[i], j, libs[j].name);
                int frac = location + block_size - libs[j].size;
                SHA256_Update(&sha256, libs[j].addr + location, block_size - frac);
                SHA256_Update(&sha256, libs[j+1].addr, frac);
                break;
            }
        }
    }
    SHA256_Final(hash_value, &sha256);
}


void calculate_random_checksum(struct library *libs, char *hash_value, unsigned int seed, int num_of_blocks, int block_size)
{
    int total_sz = 0;
    for(int i = 0; i<num_libs; i++){
	//printf("name: %s; addr=%10p; size=%d\n", libs[i].name, libs[i].addr, libs[i].size);
    	total_sz += libs[i].size;
    }  
    printf("total size: %d \n", total_sz);

    int max_possible_blocks = total_sz/block_size;
    if (num_of_blocks > max_possible_blocks){
    	num_of_blocks = max_possible_blocks;
	printf("set max possible blocks: %d\n", num_of_blocks);
    }

    int *blocks_pos = malloc(num_of_blocks * sizeof(int));
    int *rnd = malloc(max_possible_blocks * sizeof(int));

    for(int i = 0; i< max_possible_blocks;  i++){
	rnd[i]= i;    
    }

    srand(seed);
    
    for (int i=0; i<num_of_blocks; i++){
	int loc = rand() % (max_possible_blocks - i);
	blocks_pos[i] = rnd[loc]; 
	rnd[loc] = rnd[max_possible_blocks - i - 1];
	//printf("%d ", blocks_pos[i]);
    }
    
    random_checksum(libs, blocks_pos, hash_value, num_of_blocks, block_size); 

    free(blocks_pos);
    free(rnd);
}


void calculate_sequential_checksum(struct library *libs, char *hash_value)
{
    SHA256_CTX sha256;
    SHA256_Init(&sha256);

    for(int i = 0; i<num_libs; i++){
        SHA256_Update(&sha256, libs[i].addr, libs[i].size);
    }

    SHA256_Final(hash_value, &sha256);
}


void calculate_checksum(char *hash_value, int type, unsigned int seed, int num_of_blocks, int block_size)
{
    num_libs = 0;
    struct library libs[MAX_NUM_OF_LIBS];
    dl_iterate_phdr(callback_set_lib_addr, libs);

    if (type == SEQUENTIAL){
        calculate_sequential_checksum(libs, hash_value);
        printf("\nSHA-256 using sequential memory checksum: \n");
    }
    else{
        calculate_random_checksum(libs, hash_value, seed, num_of_blocks, block_size);
        printf("\nSHA-256 using psuedo-random memory checksum: \n");
    }

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        printf("%02x", hash_value[i]);
    }
    putchar('\n');
}


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

    int addr_len, n;
    addr_len = sizeof(cliaddr);

    int type;
    int seed;
    int num_of_blocks;
    int block_size;

    while(recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &addr_len)){
	printf("buffer: %s\n", buffer);

	char delim[] = " ";
	char *ptr = strtok(buffer, delim);
	type = atoi(ptr);

	ptr = strtok(NULL, delim);
	seed = atoi(ptr);

	ptr = strtok(NULL, delim);
	num_of_blocks = atoi(ptr);

	ptr = strtok(NULL, delim);
	block_size = atoi(ptr);

/*   
 *      std::stringstream ss(buffer);
        ss >> type;
	ss >> seed;
	ss >> num_of_blocks;
	ss >> block_size;
*/
        printf("type: %d; seed: %d; num_of_blocks: %d, block_size: %d\n", type, seed,num_of_blocks, block_size);
        calculate_checksum(hash_value, type, seed, num_of_blocks, block_size);

        sendto(sockfd, (const char *)hash_value, SHA256_DIGEST_LENGTH, MSG_CONFIRM, (const struct sockaddr *) &cliaddr, addr_len); 
    }
}





/*
int main()
{
    int num_of_blocks = 200;
    int block_size = 2000;

    unsigned char hash_value[SHA256_DIGEST_LENGTH];
	
    time_t t;
    unsigned int seed = (unsigned) time(&t);
    
    calculate_checksum(hash_value, seed, num_of_blocks, block_size);

}

*/

// g++ checksum.cpp -o checksum -fpermissive -lssl -lcrypto

