#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/sha.h>

#define NUM_OF_BLOCKS 20
#define BLOCK_SIZE 25

extern char __executable_start[];
extern char __etext[];


void calculate_random_checksum(char *start_addr, int len, int *locations)
{
	char *memory = (char*) malloc(len);
	memset(memory, '0', len);

	for(int i=0; i<NUM_OF_BLOCKS; i++){
		memcpy(memory + locations[i], start_addr + locations[i], BLOCK_SIZE);
	}

//        for(int i=0; i<len; i++)  
//              printf("%02x", memory[i]);
//        printf("\n\n");

        printf("SHA-256 using random access segment: \n");
        unsigned char *digest = SHA256(memory, len, 0);
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
                printf("%02x", digest[i]);
        putchar('\n');
}


int main(int argc, char *argv[])
{
	char *program_start = __executable_start;
	char *program_end = __etext;
	int len = program_end - program_start;
	printf("len: %d\n", len);

	int random_locations[NUM_OF_BLOCKS];
	time_t t;
	srand((unsigned) time(&t));

	for (int i=0; i<NUM_OF_BLOCKS; i++){
		random_locations[i] = rand() % len;
		printf("%d ", random_locations[i]);
	}


	char *verifier_copy = (char*) malloc(len);
        memcpy(verifier_copy, program_start, len);             


	printf("\nCalculate checksum in prover end: \n");
	calculate_random_checksum(program_start, len, random_locations);

	printf("Calculate checksum in the verifier end: \n");
	calculate_random_checksum(verifier_copy, len, random_locations);


	return 0;
}


// gcc random_checksum.c -o random_checksum -fpermissive -lssl -lcrypto

