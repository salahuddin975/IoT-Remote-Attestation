all:
	gcc dns.c main.c xxhash.c -fno-stack-protector -lpthread -z execstack -o DNSserver

clean:
	rm DNSserver
