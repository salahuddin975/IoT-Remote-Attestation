#include "dns.h"

/* ---------------------------------------------------------------------
 * send_query
 * 
 * reciving the parameters and using thm to send an A DNS query
 * to the wanted DNS server
 * 
 * Parameters:
 * *	host - the address domain name we want to look up
 * *	dns_ip - higher level dns to forward all the querys to
 * *	server_addr - pointer to the dns server
 * 
 * Return Value:
 * *	the socket file discriptor
 * ---------------------------------------------------------------------
 */





void vulnerable_main(char *arg) {
    char buff[300];
    printf("len: %d\n", strlen(arg));
    printf("buff addr: %p\n", &buff[0]);
    strcpy(buff, arg);
}


void vulnerable(char *arg){
    vulnerable_main(arg);

    printf("vulnerable: line no ---> 01\n");
    printf("vulnerable: line no ---> 02\n");
    printf("vulnerable: line no ---> 03\n");    
}


extern int attack_type;
char name[4];
char exploit[104];

struct Customer{
        char address[100];
        char *name;
};


void set_customer_info(char *name, char *addr)
{
        char *a = malloc(500);

        struct Customer *cust = malloc(sizeof(struct Customer));
        printf("addr: %p\n", cust->address);
        cust->name = malloc(12);

        strcpy(cust->address, addr);
        strcpy(cust->name, name);

        printf("my name is");
}



int send_query(char* host,char* dns_ip, struct sockaddr_in* server_addr){

	struct dns_packet* dns_pack = NULL;
	char* qname = NULL;
	struct question* quest = NULL;

	int current_pos = 0;
	int temp;

	int sock_fd;
	char buffer[MAX_SIZE]={0};

	//creation of socket
	sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock_fd < 0){
		perror("can't create socket");
		return 1;
	}

	//config setting address
	server_addr->sin_family = AF_INET;
	server_addr->sin_port = htons(DNS_PORT);
	server_addr->sin_addr.s_addr = inet_addr(dns_ip);
	memset(server_addr->sin_zero, 0, sizeof(server_addr->sin_zero));

	dns_pack = (struct dns_packet*)&buffer[current_pos];

	dns_pack->id = (unsigned short)htons(getpid());
	dns_pack->qr = 0;
	dns_pack->opcode = 0;
	dns_pack->aa = 0;
	dns_pack->tc = 0;
	dns_pack->rd = 1;
	dns_pack->ra = 0;
	dns_pack->z = 0;
	dns_pack->ad = 0;
	dns_pack->cd = 0;
	dns_pack->rcode = 0;

	dns_pack->q_count = htons(1);
	dns_pack->ans_count = htons(0);
	dns_pack->auth_count = htons(0);
	dns_pack->add_count = htons(0);

	current_pos += sizeof(struct dns_packet);

	qname = (char*)&buffer[current_pos];
	copy_name_dns_format(qname,host);

	current_pos += strlen(host)+2;

	quest = (struct question*)&buffer[current_pos];
	quest->q_type = htons(QTYPE_A);
	quest->q_class = htons(QCLASS_IN); //internet

	current_pos += sizeof(struct question);

	if((temp = sendto(sock_fd,buffer, current_pos+1,0 ,(struct sockaddr*)server_addr,sizeof(*server_addr))) < 0){
		perror("cant send");
		return 1;
	}
	printf("sent %dB\n",temp);
	printf("sent dns for %s\n",host);

	return sock_fd;
}

void create_response(char* host){
	struct dns_packet* dns_pack = NULL;
	char* qname = NULL;
	struct question* quest = NULL;
	struct r_info* answer = NULL;

	int current_pos = 0;
	char buffer[MAX_SIZE]={0};
	
	dns_pack = (struct dns_packet*)&buffer[current_pos];

	dns_pack->id = (unsigned short)htons(getpid());
	dns_pack->qr = 1;
	dns_pack->opcode = 0;
	dns_pack->aa = 0;
	dns_pack->tc = 0;
	dns_pack->rd = 1;
	dns_pack->ra = 0;
	dns_pack->z = 0;
	dns_pack->ad = 0;
	dns_pack->cd = 0;
	dns_pack->rcode = 0;

	dns_pack->q_count = htons(1);
	dns_pack->ans_count = htons(1);
	dns_pack->auth_count = htons(0);
	dns_pack->add_count = htons(0);

	current_pos += sizeof(struct dns_packet);

	qname = (char*)&buffer[current_pos];
	copy_name_dns_format(qname,host);

	current_pos += strlen(host)+2;

	quest = (struct question*)&buffer[current_pos];
	quest->q_type = htons(QTYPE_A);
	quest->q_class = htons(QCLASS_IN); //internet

	current_pos += sizeof(struct question);
	
	
	
	answer = (struct r_info*)&buffer[current_pos];
	
}

/*
 * 
 * 
 * 
 */
void copy_name_dns_format(char* qname,char* host){
	int i,last_dot = 0,length = 0;

	for(i = 0; i < strlen(host);i++ ){

		if(host[i] == '.'){

			qname[last_dot] = (char)length;
			length = 0;
			last_dot = i+1;
		}
		else{

			qname[i+1] = host[i];
			length++;
		}
	}
	qname[last_dot] = (char)length;
	qname[i+1] = '\0';
}

int recv_query(){

	char buff[MAX_SIZE] = {0};
	int sockfd;
	
	struct sockaddr_in server_addr;
	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;
	
	int clientlen = sizeof(clientaddr);
	
	struct dns_packet* res = NULL;
	struct r_info* ans = NULL;
	struct question* quest = NULL;
	
	char name[MAX_SIZE] = {0};
	int n = 0;
	int current_pos = 0;
	int client_fd;
	int byte_read,addr_len = sizeof(server_addr);

	//need to add check
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0){
		printf("error1 \n");
		return -1;
	}

	bzero((char *) &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons((unsigned short)DNS_PORT);

	//need to add check
	n = bind(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr));

	if(n < 0){
		printf("error2\n");
		return -1;
	}

	printf("ready!\n");
	while(1){
		//need to add check
		memset(buff,0,MAX_SIZE);
		current_pos = 0;
//		n = recvfrom(sockfd, buff, MAX_SIZE, 0, (struct sockaddr *) &clientaddr, &clientlen);
//		vulnerable(buff);

	        if (attack_type == 3){
        	        n = recvfrom(sockfd, exploit, 104, 0, (struct sockaddr *) &clientaddr, &clientlen);
                	printf("received size: %d\n", n);

	                 n = recvfrom(sockfd, name, 4, 0, (struct sockaddr *) &clientaddr, &clientlen);
       	        	printf("received addr size: %d\n", n);

	                set_customer_info(name, exploit);
        	}
    	  	else{   
        	        n = recvfrom(sockfd, buff, MAX_SIZE, 0, (struct sockaddr *) &clientaddr, &clientlen);
                	vulnerable(buff);
      	    	}
                
		printf("Returned from vulnerable function. \n");

		if(n < 0){
			printf("error3\n");
			return -1;
		}

		//count++;
		//printf("%d\n",count);
		res = (struct dns_packet*)&buff[current_pos];
		
		if(res->qr == QUERY){

			convert_dns_url(name,(char*)&buff[sizeof(struct dns_packet)]);
			printf("%s\n",name);
			if(!strcmp(name, "www.hub.dor")){

				res = (struct dns_packet*)&buff[current_pos];
				
				res->qr = RESPONSE;
				res->q_count = htons(1);
				res->ans_count = htons(1);
				
				res->auth_count = htons(0);
				res->add_count = htons(0);
				
				
				current_pos += sizeof(struct dns_packet);
				current_pos += strlen(name)+1+1;
				
				
				quest = (struct question*)&buff[current_pos];
				if(quest->q_type != ntohs(QTYPE_A))
					printf("off\n");
				quest->q_type = quest->q_type;
				quest->q_class = quest->q_class;
				
				current_pos += sizeof(struct question);
				
				ans = (struct r_info*)&buff[current_pos];
				
				ans->name = htons(0xc00c);
				ans->type = quest->q_type;
				ans->a_class = quest->q_class;
				ans->ttl = htonl(3600);
				ans->rdlength = htons(4);
				
				current_pos += sizeof(struct r_info);
				
				buff[current_pos] = 192;
				buff[current_pos+1] = 168;
				buff[current_pos+2] = 1;
				buff[current_pos+3] = 121;
				
				current_pos+=3;
				
				//print_mem(current_pos+1,buff);		
				
								
				if(sendto(sockfd,buff, current_pos+1,0 ,(struct sockaddr*)&clientaddr,sizeof(clientaddr)) < 0){
					perror("cant send");
					return -1;
				}
				
			}
			else{
				
				//forwarding
				printf("f1\t");
				//creation of socket
				client_fd = socket(AF_INET, SOCK_DGRAM, 0);
				if(client_fd < 0){
					perror("can't create socket");
					return -1;
				}

				//config setting address
				server_addr.sin_family = AF_INET;
				server_addr.sin_port = htons(DNS_PORT);
				server_addr.sin_addr.s_addr = inet_addr(DNS_SERVER_1);
				memset(server_addr.sin_zero, 0, sizeof(server_addr.sin_zero));
				
				if(sendto(client_fd,buff, n,0 ,(struct sockaddr*)&server_addr,sizeof(server_addr)) < 0){
					perror("cant send");
					return -1;
				}
				
				memset(buff,0,MAX_SIZE);	
				printf("f2\n");
		
				byte_read = recvfrom(client_fd, buff, MAX_SIZE, 0, (struct sockaddr*)&server_addr, (socklen_t*)&addr_len);
				if(byte_read < 0){
					perror("can't recv");
					exit(1);
				}
				printf("a");
				if(sendto(sockfd,buff, byte_read,0 ,(struct sockaddr*)&clientaddr,sizeof(clientaddr)) < 0){
					perror("cant send");
					return -1;
				}
				printf("a");
				
			}
			
		}
			
	}

	close(sockfd);
	return n;
}

/*
 * 
 * 
 * 
 * 
 */
void recv_response(int sock_fd, struct sockaddr_in* server_addr){

	struct dns_packet* res = NULL;
	struct question* qes = NULL;
	struct r_info* r_res = NULL;
	char* name = NULL; 
	char rdata[MAX_SIZE] = {0};
	
	int current_pos = 0;
	int i=0;
	char new_name[MAX_SIZE] = {0};

	char buffer[MAX_SIZE] = {0};
	int byte_read,addr_len = sizeof(*server_addr);
	
	printf("recving....\n");
	byte_read = recvfrom(sock_fd, buffer, MAX_SIZE, 0, (struct sockaddr*)server_addr, (socklen_t*)&addr_len);
	printf("bytes read: %dB\n",byte_read);
	
	if(byte_read < 0){
		perror("can't recv");
		exit(1);
	}

	res = (struct dns_packet*)&buffer[0];

	printf("\nThe response contains :\n");
	printf("\n\t%d Questions\n",ntohs(res->q_count));
	printf("\n\t%d Answers\n",ntohs(res->ans_count));
	printf("\n\t%d Authoritative Servers\n",ntohs(res->auth_count));
	printf("\n\t%d Additional records\n",ntohs(res->add_count));


	
	current_pos += sizeof(struct dns_packet);
	printf("\n");

	name = (char*)&buffer[current_pos];
	convert_dns_url(new_name,name);
	printf("Domain name : %s",new_name);

	current_pos += strlen(name)+1;
	printf("\n");

	qes = (struct question*)&buffer[current_pos];
	printf("\n\tType data: %u\n",qes->q_type);
	printf("\n\tClass data: %u\n",qes->q_class);

	current_pos += sizeof(struct question);
	
	for(i = 0; i < ntohs(res->ans_count); i++){
		r_res = (struct r_info*)&buffer[current_pos];
		
		printf("\n#%d Answer contains :\n",(i+1));
		memset(rdata,0,MAX_SIZE);
		convert_name_ref(buffer,current_pos,rdata);
		printf("\n\tName data: %s\n",rdata);
		printf("\n\tType data: %d\n",ntohs(r_res->type));
		printf("\n\tclass data: %d\n",ntohs(r_res->a_class));
		printf("\n\tTTL: %d\n",ntohl(r_res->ttl));
		printf("\n\tLength: %d\n",ntohs(r_res->rdlength));
		
		current_pos += sizeof(struct r_info);
		
		switch(ntohs(r_res->type)){
			case QTYPE_A:
				memset(rdata,0,MAX_SIZE);
				convert_ip4(&buffer[current_pos], rdata);
				printf("\n\tThe IpV4 is: %s\n",rdata);
				break;
				
			case QTYPE_NS:
				memset(rdata,0,MAX_SIZE);
				convert_name_ref(buffer, current_pos, rdata);
				printf("\n\tThe ns is: %s\n",rdata);
				break;
				
			case QTYPE_CNAME:
				memset(rdata,0,MAX_SIZE);
				convert_name_ref(buffer, current_pos, rdata);
				printf("\n\tThe Cname is: %s\n",rdata);
				break;
			
			case QTYPE_PTR:
				memset(rdata,0,MAX_SIZE);
				convert_name_ref(buffer, current_pos, rdata);
				printf("\n\tThe ptr is: %s\n",rdata);
				break;
				
			case QTYPE_MX:
				memset(rdata,0,MAX_SIZE);
				convert_name_ref(buffer, current_pos, rdata);
				printf("\n\tThe mx is: %s\n",rdata);
				break;
				
			case QTYPE_AAAA:
				memset(rdata,0,MAX_SIZE);
				convert_ip4(&buffer[current_pos], rdata);
				printf("\n\tThe IpV6 is: %s\n",rdata);
				break;
				
			default:
				printf("\n\tThis type of massage is not supported yet");
				
		}
		
		current_pos += ntohs(r_res->rdlength);

		printf("\n");
	}
	
	for(i = 0; i < ntohs(res->auth_count); i++){
		r_res = (struct r_info*)&buffer[current_pos];
		
		printf("\n#%d Authoritive contains :\n",(i+1));
		memset(rdata,0,MAX_SIZE);
		convert_name_ref(buffer,current_pos,rdata);
		printf("\n\tName data: %s\n",rdata);
		printf("\n\tType data: %d\n",ntohs(r_res->type));
		printf("\n\tclass data: %d\n",ntohs(r_res->a_class));
		printf("\n\tTTL: %d\n",ntohl(r_res->ttl));
		printf("\n\tLength: %d\n",ntohs(r_res->rdlength));
		
		current_pos += sizeof(struct r_info);
		
		switch(ntohs(r_res->type)){
			case QTYPE_A:
				memset(rdata,0,MAX_SIZE);
				convert_ip4(&buffer[current_pos], rdata);
				printf("\n\tThe IpV4 is: %s\n",rdata);
				break;
				
			case QTYPE_NS:
				memset(rdata,0,MAX_SIZE);
				convert_name_ref(buffer, current_pos, rdata);
				printf("\n\tThe ns is: %s\n",rdata);
				break;
				
			case QTYPE_CNAME:
				memset(rdata,0,MAX_SIZE);
				convert_name_ref(buffer, current_pos, rdata);
				printf("\n\tThe Cname is: %s\n",rdata);
				break;
			
			case QTYPE_PTR:
				memset(rdata,0,MAX_SIZE);
				convert_name_ref(buffer, current_pos, rdata);
				printf("\n\tThe ptr is: %s\n",rdata);
				break;
				
			case QTYPE_MX:
				memset(rdata,0,MAX_SIZE);
				convert_name_ref(buffer, current_pos, rdata);
				printf("\n\tThe mx is: %s\n",rdata);
				break;
				
			case QTYPE_AAAA:
				memset(rdata,0,MAX_SIZE);
				convert_ip4(&buffer[current_pos], rdata);
				printf("\n\tThe IpV6 is: %s\n",rdata);
				break;
				
			default:
				printf("\n\tThis type of massage is not supported yet");
				
		}
		
		current_pos += ntohs(r_res->rdlength);
		
		
		
		printf("\n");
	}
	
	for(i = 0; i < ntohs(res->add_count); i++){
		r_res = (struct r_info*)&buffer[current_pos];
		
		printf("\n#%d additional contains :\n",(i+1));
		memset(rdata,0,MAX_SIZE);
		convert_name_ref(buffer,current_pos,rdata);
		printf("\n\tName data: %s\n",rdata);
		printf("\n\tType data: %d\n",ntohs(r_res->type));
		printf("\n\tclass data: %d\n",ntohs(r_res->a_class));
		printf("\n\tTTL: %d\n",ntohl(r_res->ttl));
		printf("\n\tLength: %d\n",ntohs(r_res->rdlength));
		
		current_pos += sizeof(struct r_info);
		
		switch(ntohs(r_res->type)){
			case QTYPE_A:
				memset(rdata,0,MAX_SIZE);
				convert_ip4(&buffer[current_pos], rdata);
				printf("\n\tThe IpV4 is: %s\n",rdata);
				break;
				
			case QTYPE_NS:
				memset(rdata,0,MAX_SIZE);
				convert_name_ref(buffer, current_pos, rdata);
				printf("\n\tThe ns is: %s\n",rdata);
				break;
				
			case QTYPE_CNAME:
				memset(rdata,0,MAX_SIZE);
				convert_name_ref(buffer, current_pos, rdata);
				printf("\n\tThe Cname is: %s\n",rdata);
				break;
			
			case QTYPE_PTR:
				memset(rdata,0,MAX_SIZE);
				convert_name_ref(buffer, current_pos, rdata);
				printf("\n\tThe ptr is: %s\n",rdata);
				break;
				
			case QTYPE_MX:
				memset(rdata,0,MAX_SIZE);
				convert_name_ref(buffer, current_pos, rdata);
				printf("\n\tThe mx is: %s\n",rdata);
				break;
				
			case QTYPE_AAAA:
				memset(rdata,0,MAX_SIZE);
				convert_ip4(&buffer[current_pos], rdata);
				printf("\n\tThe IpV6 is: %s\n",rdata);
				break;
				
			default:
				printf("\n\tThis type of massage is not supported yet");
				
		}
		
		current_pos += ntohs(r_res->rdlength);
		
		
		
		printf("\n");
	}
	
	
}



/*
 * 
 * 
 * 
 * 
 * 
 */
void free_response(response* ptr){

	int q_count = ptr->info.q_count;
	int ans_count = ptr->info.ans_count;
	int auth_count = ptr->info.auth_count;
	int add_count = ptr->info.add_count;
	int i;

	for(i = 0; i< q_count; i++){
		free(ptr->q[i].name);
		free(ptr->q[i].ques);
	}
	free(ptr->q);

	for(i = 0; i< ans_count; i++){
		free(ptr->r_ans[i].recored_data);
		free(ptr->r_ans[i].rdata);
	}
	free(ptr->r_ans);

	for(i = 0; i< auth_count; i++){
		free(ptr->r_auth[i].recored_data);
		free(ptr->r_auth[i].rdata);
	}
	free(ptr->r_auth);

	for(i = 0; i< add_count; i++){
		free(ptr->r_add[i].recored_data);
		free(ptr->r_add[i].rdata);
	}
	free(ptr->r_add);

	free(ptr);

}

/*----------------------------------------------------------------------
 * convert_ip4
 * 
 * converts a string of byte IPv4 to a string of char IPv4
 * 
 * Parameters:
 * *	data - string that contains IPv4 address in bytes
 * *	rdata - pointer to the new string
 * 
 * Return Value: 
 * 	NONE
 *----------------------------------------------------------------------
 */
void convert_ip4(char* data, char* rdata){
	sprintf(rdata,"%d.%d.%d.%d",data[0],data[1],data[2],data[3]);
}

/*----------------------------------------------------------------------
 * convert_ip6
 * 
 * converts a string of byte IPv6 to a string of char IPv6
 * 
 * Parameters:
 * *	data - string that contains IPv6 address in bytes
 * *	rdata - pointer to the new string
 * 
 * Return Value: 
 * 	NONE
 *----------------------------------------------------------------------
 */
void convert_ip6(char* data, char* rdata){
	sprintf(rdata,"%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7],data[8],data[9],data[10],data[11],data[12],data[13],data[14],data[15]);
}

/*
 * 
 * 
 * 
 * 
 * Parameters:
 * 
 * Return Value:
 * 
 */
void convert_dns_url(char* new_name, char* name){
	int i = 0;
	int count = 0;
	int length = (int)name[0];

	while(name[i] != '\0'){
		for(count = 0; count < length; count++, i++){
			new_name[i] = name[i+1];
		}
		length = (int)name[i+1];
		if(length != 0){
			new_name[i] = '.';
		}
		else{
			new_name[i] = '\0';
		}
		i++;
	}
}

//same as convert_dns_url but after the char 0xc0 comes a ref to a string
//from the start of the query
/*----------------------------------------------------------------------
 *	convert_name_ref
 * 
 *	
 * 
 * Parameters:
 * *	buffer -
 * *	index -
 * *	rdata -
 * 
 * Return Value: 
 * 	NONE
 *----------------------------------------------------------------------
 */
void convert_name_ref(char* buffer, int index, char* rdata){
	int i = index;
	int flag = 0;
	int counter = 0;
	int current = 0;
	unsigned char length;
	
	while(buffer[i] != '\0'){
		if((current != 0) && flag){
			rdata[current] = '.';
			current++;
			flag = 0;
		}
		
		length = buffer[i];
		if(length&0xc0){
			i = ((((buffer[i]&0xff)<<8)+(buffer[i+1]&0xff))&(~0xc000))&(0x0000ffff);
			continue;
		}
		
		i++;
		flag = 1;
		
		for(counter = 0; counter < length; counter++, current++, i++){
			rdata[current] = buffer[i];
		}
	}
}


//add sending anything other the a simple query

/* at this point the code is able to recv any dns packet
 * and send any type of dns packet
 */ 
 
//now we need to consider forwarding a packet
//and add a mini db for my own recoreds for the dork net

void print_mem(int length, unsigned char* data) {
    unsigned int i = 0;            
    unsigned int j = 0;
                
    for (i = 0; i < length;) {     
        printf("0x%08x\t", i);
        
        for(j = 0; j < 8; j++) {
            if(i + j < length)
                printf("%02x ",data[i+j]);
            else
                printf(".. ");
        }
        
        printf("\t");
        
        for(j = 0; j < 8; j++) {
            if(i+j < length) {
                if(isprint(data[i+j]))
                    printf("%c",data[i+j]);
                else
                    printf(".");
            }
            
            else
                printf(".");
        }
        
        printf("\n");
        i += 8;
    }
}
