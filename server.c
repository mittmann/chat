#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <string.h>
#include <netinet/in.h>
#include <semaphore.h>

#define BUFFER_SIZE 2048
#define MAX_CLIENTS 20








char help_msg[128] = "help: \n/nick <nick> to change nickname \n/join <room_name> to join a room\n/quit to quit the room\n/exit to exit program";
int* cl_sockets;
int amount = 0;


void * receiveMessage(void * socket) 
{
	int sockfd, ret;
	char buffer[BUFFER_SIZE]; 
	char comando[15];



	sockfd = (int) socket;
	while(1) {
 		bzero(buffer, BUFFER_SIZE);


  		ret = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, NULL, NULL);  

  		if (ret < 0) 
  		{  
   			printf("Error receiving data!\n");    
  		}
  		else if (ret > 0)
  		{
  			if (buffer[0] == '/')
  			{
  				strncpy(comando, buffer + sizeof(char), 4);

  				if (!(strcmp ("nick", comando)))
  				{	
  					puts("nick");
  				}
  				else if (!(strcmp ("join", comando)))
  				{
  					puts("join");
				}
  				else if (!(strcmp ("quit", comando)))
  				{
   					puts("quit"); 					
  				}
  				else if (!(strcmp ("exit", comando)))
  				{
   					puts("exit"); 					
  				}
  				else if (!(strcmp ("help", comando)))
  				{
  					send(sockfd, help_msg, sizeof(help_msg), NULL);
  				}
  				else
  				{
   					send(sockfd, help_msg, sizeof(help_msg), NULL);

  				}
  			}

   			printf("client: ");
   			for(int i=0; i<=amount; i++)
   			{
   				send(cl_sockets[i], buffer, ret, NULL);
   			}
   			puts(buffer);
   		}
	}
}


int main(int argc, char** argv)
{

	int accept_sock, sock, ret, len, port, i;
	struct sockaddr_in sv_addr, cl_addr;
	pthread_t* threads = malloc(MAX_CLIENTS*(sizeof(pthread_t)));
	cl_sockets = malloc(MAX_CLIENTS*(sizeof(int)));

	puts(help_msg);


	if (argc != 2)
	{
		puts("usage: ./server <port>");
		return -1;
	}
	port = atoi(argv[1]);



    if ((accept_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        printf("ERRO abrindo socket");


	sv_addr.sin_family = AF_INET;
	sv_addr.sin_addr.s_addr = INADDR_ANY;
    sv_addr.sin_port = htons(port);
    bzero(&(sv_addr.sin_zero), 8);



    if (bind(accept_sock, (struct sockaddr *) &sv_addr, sizeof(sv_addr)) < 0)
        printf("ERRO bindando");

	listen(accept_sock, 5);

	len = sizeof(cl_addr);

	while(1){
	if ((sock = accept(accept_sock,(struct sockaddr *) &cl_addr, &len)) == -1)
		{
			puts("erro no accept") ;
		}

		cl_sockets[amount] = sock;


		if (pthread_create(threads + amount, NULL, receiveMessage, (void *) sock))
			puts("erro no pthread_create");

		amount++;
	}




	for(i=0; i<=amount; i++)
		close(cl_sockets[i]);
    close(sock);
    close(accept_sock);
    return 0;



}
