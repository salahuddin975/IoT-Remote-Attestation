all:
	gcc dns.c main.c checksum.c -fno-stack-protector -lpthread -lssl -lcrypto -z execstack -o DNSserver

clean:
	rm DNSserver
