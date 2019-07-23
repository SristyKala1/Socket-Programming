#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 


int cRc(char input[]) {
    	int i,j,keylen,msglen;
    	char key[30],temp[30],quot[100],rem[30],key1[30];
    	printf("Enter Key: ");
    	gets(key);
    	keylen=strlen(key);
    	msglen=strlen(input);
    	strcpy(key1,key);
    	for (i=0;i<keylen-1;i++) {
    		input[msglen+i]='0';
    	}
    	for (i=0;i<keylen;i++)
    	 temp[i]=input[i];
    	for (i=0;i<msglen;i++) {
    		quot[i]=temp[0];
    		if(quot[i]=='0')
    		 for (j=0;j<keylen;j++)
    		 key[j]='0'; else
    		 for (j=0;j<keylen;j++)
    		 key[j]=key1[j];
    		for (j=keylen-1;j>0;j--) {
    			if(temp[j]==key[j])
    			 rem[j-1]='0'; else
    			 rem[j-1]='1';
    		}
    		rem[keylen-1]=input[i+keylen];
    		strcpy(temp,rem);
    	}
    	strcpy(rem,temp);
	printf("remainder is :%s",temp);
    	printf("\nQuotient is ");
    	for (i=0;i<msglen;i++)
    	 printf("%c",quot[i]);
    	printf("\nRemainder is ");
    	for (i=0;i<keylen-1;i++)
    	 printf("%c",rem[i]);
    	printf("\nFinal data is: ");
    	for (i=0;i<msglen;i++)
    	 printf("%c",input[i]);
    	for (i=0;i<keylen-1;i++)
    	 printf("%c",rem[i]);
	return rem;
}
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

struct pkt
{
	int sno;
	char ch[1500];
	char crc[30];
};

int main(int argc, char *argv[])
{
	char c,r;
	int i=0;
	//printf("\nStarted");
    	struct pkt  addrPkt;
	FILE *fp;
	fp=fopen("hello.txt","r");
	while(c=fgetc(fp)!=EOF)
	{
		fscanf(fp,"%c",&addrPkt.ch[i]);
		printf("data in array :%c\n",addrPkt.ch[i]);
		i++;
	}
	fclose(fp);
	addrPkt.sno=1;
	r=cRc(addrPkt.ch);
	//printf("%c",r);
	//for (i=0;i<strlen(r);i++)
    	// printf("%c",r[i]);
	//printf("crc is %c :", addrPkt.crc);
   	int sockfd, portno, n;
    	struct sockaddr_in serv_addr;
    	struct hostent *server;

    	void * buffer, *data1=&addrPkt.sno, *data2=&addrPkt.ch, 			*data3=&addrPkt.crc;
	buffer = malloc(1504);
	memcpy(buffer,data1,4);
	memcpy((buffer+4), data2,1500);
	memcpy((buffer+1504), data3,30);
    	if (argc < 3) {
       	    fprintf(stderr,"usage %s hostname port\n", argv[0]);
            exit(0);
   	}
    	portno = atoi(argv[2]);
    	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    	if (sockfd < 0) 
        	error("ERROR opening socket");
    	server = gethostbyname(argv[1]);
    	if (server == NULL) {
        	fprintf(stderr,"ERROR, no such host\n");
        	exit(0);
    	}
    	bzero((char *) &serv_addr, sizeof(serv_addr));
    	serv_addr.sin_family = AF_INET;
    	bcopy((char *)server->h_addr, 
        	 (char *)&serv_addr.sin_addr.s_addr,
        	 server->h_length);
    	serv_addr.sin_port = htons(portno);
	printf("\ngoing to connect %d \n", sizeof(buffer));
        if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) <  	0) 
        	error("ERROR connecting");
    	printf("\nsending data : %d, %c",*(int*) buffer, *(char*) (buffer+4));

    	printf("\nClient: test\n");
	
    	while(1)
    	{	

		printf("\nSending data\n");
		send(sockfd,buffer,1504,0);
		printf("\nSent data\n");
		break;
    	}
    	close(sockfd);
    	return 0;
}

