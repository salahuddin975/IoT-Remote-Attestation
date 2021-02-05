all:
	gcc dns.c main.c ./../common/checksum.c -fno-stack-protector -lpthread -lssl -lcrypto -fpermissive -z execstack -o DNSserver

clean:
	rm DNSserver
