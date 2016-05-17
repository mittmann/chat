//
//  client.c
//  
//
//  Created by Matheus Stefanello Luz on 5/16/16.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <semaphore.h>


#define BUFSIZE 2048

sem_t screen_m;

void * receiveMessage(void * socket) {
    int sockfd, ret;
    char buffer[BUFSIZE];    
    
    sockfd = (int) socket;
    
    while(1) {

            ret = read(sockfd, buffer, BUFSIZE);
            if (ret < 0)
                printf("ERRO lendo do socket\n");
        

         if(!strcmp("exit\n", buffer))
            {  

              return 0;
            }

            
            printf("%s",buffer);
            sem_post(&screen_m);            
    }

}

void * sendMessage(void * socket)
{
  char buffer[BUFSIZE];
  int sockfd, n;

  sockfd = (int) socket;

  while (1)
    {

    bzero(buffer, BUFSIZE);
    usleep(100);
    sem_wait(&screen_m);
    fgets(buffer, BUFSIZE, stdin);
    sem_post(&screen_m);

    
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0)
        printf("ERRO escrevendo no socket\n");
        
        bzero(buffer,BUFSIZE);

    }
}


int main (int argc, char **argv) {
    int sockfd, n, port;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char *hostname;
    pthread_t thread1, thread2;
    if (argc != 3) {
        fprintf(stderr, "uso: %s <endereço>, <porta>\n", argv[0]);
        exit(0);
    }
    hostname = argv[1];
    port = atoi(argv [2]);

    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf (stderr, "ERRO, endereço %s não encontrado \n", hostname);
        exit(0);
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("ERRO abrindo o socket\n");
        exit(0);
    }


    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr = *((struct in_addr *)server->h_addr);
    bzero(&(serv_addr.sin_zero), 8);
    
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        printf("ERRO de conexão\n");
        exit(0);
    }    

    sem_init(&screen_m, 0, 0);

    if (pthread_create(&thread1, NULL, receiveMessage, (void *) sockfd))
      puts("erro no pthread_create");

    if (pthread_create(&thread2, NULL, sendMessage, (void *) sockfd))
      puts("erro no pthread_create");


    pthread_join(thread1);

    close(sockfd);
    return 0;

}

