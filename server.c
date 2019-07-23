#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

struct pkt
{
	int sno;
	char ch[1500];
};

int main(int argc, char *argv[])
{
     struct pkt *p;
     int sockfd, newsockfd, portno, *val;
     socklen_t clilen;
     void * buffer=malloc(1504);
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
	char * ch;
     while(1)
     {
	printf("\nready to recv");
	
	recv(newsockfd,buffer,5,0);
	p=buffer;		
	printf("\nvalues are :%d, %s done\n", p->sno, p->ch);

	break;
     }
     close(newsockfd);
     close(sockfd);
     return 0; 
}
