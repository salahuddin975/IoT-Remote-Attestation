#define _GNU_SOURCE
#include <link.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h>


#define NUM_OF_BLOCKS 50
#define BLOCK_SIZE 2000

#define MAX_PATH_LEN 4096          // lINUX: 4096, windows: 260
#define MAX_NUM_OF_LIBS 200


struct library{
	char name[MAX_PATH_LEN];
	int addr;
	int size;
};

int num_libs = 0;


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
	printf("%s\n", info->dlpi_name);
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


void calculate_random_checksum(struct library *libs, int *locations, char *hash_value)
{
    SHA256_CTX sha256;
    SHA256_Init(&sha256);

    for(int i=0; i<NUM_OF_BLOCKS; i++){
    	int location = locations[i];

	for (int j=0; j<num_libs; j++){
            if(location > libs[j].size){             // Find the library
                location = location - libs[j].size;
                continue;
            }
			
            if((location + BLOCK_SIZE) < libs[j].size) {           // if the block size fit in the library
                //printf("value: %d, libs: %d: %s\n", locations[i], j, libs[j].name);
                SHA256_Update(&sha256, libs[j].addr + location, BLOCK_SIZE);
                break;
            }
            else{            // if doesn't fit the block size
                //printf("Else, value: %d, libs: %d: %s\n", locations[i], j, libs[j].name);
                int frac = location + BLOCK_SIZE - libs[j].size;
                SHA256_Update(&sha256, libs[j].addr + location, BLOCK_SIZE - frac);
                SHA256_Update(&sha256, libs[j+1].addr, frac);
                break;
            }
        }
    }
    SHA256_Final(hash_value, &sha256);
}


void calculate_checksum(char *hash_value, unsigned int seed)
{
    num_libs = 0;
    struct library libs[MAX_NUM_OF_LIBS];
    dl_iterate_phdr(callback_set_lib_addr, libs);

    int total_sz = 0;
    for(int i = 0; i<num_libs; i++){
	printf("name: %s; addr=%10p; size=%d\n", libs[i].name, libs[i].addr, libs[i].size);
    	total_sz += libs[i].size;
    }  

    printf("total size: %d \n", total_sz);

    int random_locations[NUM_OF_BLOCKS];
    srand(seed);

    for (int i=0; i<NUM_OF_BLOCKS; i++){
	random_locations[i] = rand() % (total_sz - BLOCK_SIZE);
	printf("%d ", random_locations[i]);
    }
    
    calculate_random_checksum(libs, random_locations, hash_value);
 
    printf("\nSHA-256 using random access segment: \n");
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        printf("%02x", hash_value[i]);
    }
	
    putchar('\n');
}


/*
int main()
{
    unsigned char hash_value[SHA256_DIGEST_LENGTH];
	
    time_t t;
    unsigned int seed = (unsigned) time(&t);
    
    calculate_checksum(hash_value, seed);

}

*/

// gcc checksum_random.c -o checksum -fpermissive -lssl -lcrypto

