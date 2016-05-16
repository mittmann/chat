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

#define BUFSIZE 1024
#define PORT 55555

int main (int argc, char **argv) {
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char *hostname;
    char buffer[BUFSIZE];
    
    if (argc != 2) {
        fprintf(stderr, "uso: %s <endereço>\n", argv[0]);
        exit(0);
    }
    hostname = argv[1];
    
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
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr = *((struct in_addr *)server->h_addr);
    bzero(&(serv_addr.sin_zero), 8);
    
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        printf("ERRO de conexão\n");
        exit(0);
    }
    
    printf("Escrever a mensagem: ");
    bzero(buffer, BUFSIZE);
    fgets(buffer, BUFSIZE, stdin);
    
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0)
        printf("ERRO escrevendo no socket\n");
    
    bzero(buffer,BUFSIZE);
    
    n = read(sockfd, buffer, BUFSIZE);
    if (n < 0)
        printf("ERRO lendo do socket\n");
    
    printf("%s\n",buffer);
    
    close(sockfd);
    return 0;

}