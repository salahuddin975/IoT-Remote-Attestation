#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#define DNS_SERVER_2 "8.8.8.8"
#define DNS_SERVER_1 "213.57.22.5"
#define LOCAL_IP "192.168.1.125"

#define DNS_PORT 5555
#define MAX_SIZE 2048

#define QUERY 0x0000
#define RESPONSE 0x0001

#define OPCODE_QUERY 0x0000
#define OPCODE_IQUERY 0x0002
#define OPCODE_STATUS 0x0004

//authoritative answer
#define AA 0x0020
//truncation flag
#define TC 0x0040
//recursion desried
#define RD 0x0080
//recursion available
#define RA 0x0100

#define RCODE_NO_ERROR 0x0000
#define RCODE_FORMAT 0x1000
#define RCODE_SERVER_FAILURE 0x2000
#define RCODE_NAME 0x3000
#define RCODE_NOT_IMPLEMENTED 0x4000
#define RCODE_REFUSED 0x5000

#define QTYPE_A 0x0001
#define QTYPE_NS 0x0002
#define QTYPE_CNAME 0x0005
#define QTYPE_SOA 0x0006
#define QTYPE_WKS 0x000B
#define QTYPE_PTR 0x000C
#define QTYPE_MX 0x000F
#define QTYPE_SRV 0x0021
#define QTYPE_AAAA 0x001C
#define QTYPE_ANY 0x00FF

#define QCLASS_IN 0x0001

struct dns_packet{
	unsigned short id;

	unsigned char rd :1;
	unsigned char tc :1;
	unsigned char aa :1;
	unsigned char opcode :4;	
	unsigned char qr :1;

	unsigned char rcode :4;
	unsigned char cd :1;
	unsigned char ad :1;
	unsigned char z :1;
	unsigned char ra :1;

	unsigned short q_count;
	unsigned short ans_count;
	unsigned short auth_count;
	unsigned short add_count;
};

struct question{
	unsigned short q_type;
	unsigned short q_class;
};

typedef struct{
	unsigned char* name;
	struct question* ques;
} query;

#pragma pack(push, 1)
struct r_info{
	unsigned short name;
	unsigned short type;
	unsigned short a_class;
	unsigned long ttl;
	unsigned short rdlength;
};
#pragma pack(pop)

typedef struct{
	struct r_info* recored_data;
	unsigned char* rdata;
}r_record;

typedef struct{
	
	struct dns_packet info;
	query* q;
	r_record* r_ans;
	r_record* r_auth;
	r_record* r_add;

} response;



void recv_response(int sock_fd, struct sockaddr_in* server_addr);

void copy_name_dns_format(char* qname,char* host);

int send_query(char* host, char* dns_ip,  struct sockaddr_in* server_addr);
int recv_query();

void free_response(response* ptr);

void convert_dns_url(char* new_name, char* name);

void convert_ip4(char* data, char* rdata);

void convert_name_ref(char* buffer, int index, char* rdata);

void print_mem(int length, unsigned char* data);
