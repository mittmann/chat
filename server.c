#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <string.h>
#include <netinet/in.h>
#include <semaphore.h>
#include <stdbool.h>

#define BUFFER_SIZE 2048
#define MAX_CLIENTS 20
#define MAX_ROOMS 20
#define NAME_LENGTH 25



typedef struct 
{
	char nick[NAME_LENGTH];
	char room[NAME_LENGTH];
	int socket;
	bool used;
	int id;
}client;



sem_t room_m;

char help_msg[256] = "help: \n/nick <nick> to change nickname \n/join <room_name> to join a room\n/newr <room_name> to create a new chat room\n/list to list users in current room\n/quit to quit the room\n/exit to exit program";
client* clients;
char rooms[MAX_ROOMS][NAME_LENGTH];
int room_amount = 1;


void * receiveMessage(void * id_void) 
{
	int sockfd, ret, id, j;
	char buffer[BUFFER_SIZE];
	char sendmsg[BUFFER_SIZE + NAME_LENGTH+2];
	char comando[15];
	char auxname[NAME_LENGTH];


	id = (int) id_void;
	sockfd = clients[id].socket;
	while(1) {
 		bzero(buffer, BUFFER_SIZE);



  		ret = read(sockfd, buffer, BUFFER_SIZE);  

  		if (ret < 0) 
  		{  
   			printf("Error receiving data!\n");    
  		}
  		else if (ret > 0)
  		{
  			if (buffer[0] == '/')
  			{
	   			bzero(sendmsg, BUFFER_SIZE + NAME_LENGTH + 2);
  				strncpy(comando, buffer + sizeof(char), 4);

  				if (!(strcmp ("nick", comando)))
  				{	
  					//puts("nick");
  					if(ret-7 < NAME_LENGTH)
  					{
  						strncpy(clients[id].nick, buffer + 6, ret-7);
  						clients[id].nick[ret-7] = '\0';
  						strcpy(sendmsg, "Nick changed successfully");
  					}
  					else
  						strcpy(sendmsg, "Nickname too long.");

  				}
  				else if (!(strcmp ("newr", comando)))
  				{
  					//puts("new room");
  					sem_wait(&room_m);
  						if (room_amount >= MAX_ROOMS)
  							strcpy(sendmsg, "Can't create more rooms");
  						else
  						{

  							if (ret-7 < NAME_LENGTH)
  							{
		  						strncpy(rooms[room_amount], buffer + 6, ret-7);
		  						rooms[room_amount][ret-7] = '\0';
		  						strncpy(clients[id].room, buffer + 6, ret-7);
	  							clients[id].room[ret-7] = '\0';
		  						strcpy(sendmsg, "Room created successfully. Now on room ");
								strcat(sendmsg, clients[id].room);
								room_amount++;
							}
							else
							{	
  								strcpy(sendmsg, "Room name too long.");
							}
  						}
  					sem_post(&room_m);


				}
  				else if (!(strcmp ("join", comando)))
  				{
  					j=0;
  					strcpy(auxname, buffer+6);
  					auxname[ret-7] = '\0';
  					while(strcmp(rooms[j], auxname) && j<room_amount)
  					{
  						j++;
  					}
  					if (j>=room_amount)
  					{
  						strcpy(sendmsg, "Could not find room with given name. Existing rooms: \n");
  						for(j=0; j<room_amount; j++)
  						{
  							strcat(sendmsg, rooms[j]);
  							strcat(sendmsg, "\n");
  						}
  						}
  					else
  					{
  						strncpy(clients[id].room, buffer + 6, ret-7);
  						clients[id].room[ret-7] = '\0';
  						strcpy(sendmsg, "Entered room");

  					}


				}
  				else if (!(strcmp ("quit", comando)))
  				{
   					puts("quit");
   					strcpy(clients[id].room, "Lobby");
   					strcpy(sendmsg, "Successfully quit to lobby");
  				}
  				else if (!(strcmp ("list", comando)))
  				{
  					strcpy(sendmsg, "List of users in room ");
  					strcat(sendmsg, clients[id].room);
  					strcat(sendmsg, ":\n");
  					for (j=0; j<MAX_CLIENTS; j++)
  					{
  						if (!strcmp(clients[j].room, clients[id].room) && clients[j].used)
  							{
  								strcat(sendmsg, clients[j].nick);
  								strcat(sendmsg, "\n");
  							}
  					}

  				}
  				else if (!(strcmp ("exit", comando)))
  				{
   					//puts("exit");
  					clients[id].used = false;
  				}
  				else
  				{
   					strcpy(sendmsg, help_msg);

  				}

  				 write(sockfd, sendmsg, sizeof(sendmsg));

  			}

  			else
  			{


	   			bzero(sendmsg, BUFFER_SIZE + NAME_LENGTH + 2);


	   			strcat(sendmsg, clients[id].nick);
	   			strcat(sendmsg, ": ");
	   			strcat(sendmsg, buffer);


	   			for(int i=0; i<=MAX_CLIENTS; i++)
	   			{
	   				if (clients[i].used == true && !strcmp(clients[i].room, clients[id].room) && (i != id))
	   					send(clients[i].socket, sendmsg, sizeof(sendmsg), NULL);
	   			}

   			}
   		}
	}
}


int main(int argc, char** argv)
{

	int accept_sock, sock, ret, len, port, i, id;
	struct sockaddr_in sv_addr, cl_addr;
	pthread_t* threads = malloc(MAX_CLIENTS*(sizeof(pthread_t)));
	clients = malloc(MAX_CLIENTS*(sizeof(client)));

	if (argc != 2)
	{
		puts("usage: ./server <port>");
		return -1;
	}
	port = atoi(argv[1]);

	sem_init(&room_m, 0, 1);

	strcpy(rooms[0], "Lobby");



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


		id = 0;
		while (clients[id].used)
		{
			id++;
		}


		clients[id].socket = sock;
		strcpy(clients[id].nick, "xXxL30z1nSk8x420");
		strcpy(clients[id].room, "Lobby");
		clients[id].id = id;
		clients[id].used = true;




		if (pthread_create(threads + id, NULL, receiveMessage, (void *) id))
			puts("erro no pthread_create");
	}




	for(i=0; i<=MAX_CLIENTS; i++)
		close(clients[i].socket);
    close(sock);
    close(accept_sock);
    return 0;



}
