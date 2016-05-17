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
#include <ncurses.h>

#define BUFSIZE 2048
int exitprogram=0;

void * receiveMessage(void * socket) {
    int sockfd, ret;
    char buffer[BUFSIZE];
   // int y, x;            // to store where you are

    
    sockfd = (int) socket;
    
    while(1) {
            bzero(buffer, BUFSIZE);
            ret = read(sockfd, buffer, BUFSIZE);
            if (ret < 0)
                printf("ERRO lendo do socket\n");
        
//        getyx(stdscr, y, x); // save current pos
//        move(y, 0);          // move to begining of line
//        clrtoeol();
        if (!strcmp(buffer,"exit")) {
            exitprogram=1;
            exit(0);
        }
        printf("%s",buffer);
        bzero(buffer, BUFSIZE);

    }
//        ret = recvfrom(sockfd, buffer, BUFSIZE, 0, NULL, NULL);
//        
//        if (ret < 0) {
//            printf("Error receiving data!\n");
//        } else if (ret > 0){
//            printf("server: ");
//            puts(buffer);
//        }  
//    }
}


int main (int argc, char **argv) {
    int sockfd, n, port;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char *hostname;
    char buffer[BUFSIZE];
    pthread_t thread = malloc(sizeof(pthread_t));
    int thrjoin=-1;
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

    if (pthread_create(thread, NULL, receiveMessage, (void *) sockfd))
      puts("erro no pthread_create");
    while (!exitprogram)
    {
  //  printf("Escrever a mensagem: ");
    bzero(buffer, BUFSIZE);
    fgets(buffer, BUFSIZE, stdin);
    
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0)
        printf("ERRO escrevendo no socket\n");
        
        bzero(buffer,BUFSIZE);
  //  n = read(sockfd, buffer, BUFSIZE);
  //  if (n < 0)
  //      printf("ERRO lendo do socket\n");
    
 //   printf("%s\n",buffer);

    }
    
    puts("dsgijgdse");
    close(sockfd);
    return 0;

}