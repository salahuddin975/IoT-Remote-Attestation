#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
	char *ptr;
	if(argc < 3) {
		printf("Usage %s <environment variable> <target program>\n", argv[0]);
		exit(1);
	}
	
	ptr = getenv(argv[1]);
	// Remove dependance of this program name, then adjust to the passed program name.
	// Recall the program name effects the memory location of our env var,each extra $
	// added, decreases the memory address by two bytes
	
	ptr += (strlen(argv[0]) - strlen(argv[2])) * 2;
	printf("%s is at %p\n", argv[1], ptr);
	return 0;
}

