#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <string.h>
#include <netinet/in.h>
#include <semaphore.h>

#define BUFFER_SIZE 2048
#define PORT 55555

int main(int argc, char** argv)
{

	int accept_sock, sock, ret, len;
	char buffer[BUFFER_SIZE];
	struct sockaddr_in sv_addr, cl_addr;

    if ((accept_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        printf("ERRO abrindo socket");


	//memset(&sv_addr, 0, sizeof(sv_addr));
	sv_addr.sin_family = AF_INET;
	sv_addr.sin_addr.s_addr = INADDR_ANY;
    sv_addr.sin_port = htons(PORT);
    bzero(&(sv_addr.sin_zero), 8);



//    if (bind(accept_sock, (struct sockaddr *) &sv_addr, sizeof(sv_addr)) < 0) {
//		puts("erro no bind");
//    }

    if (bind(accept_sock, (struct sockaddr *) &sv_addr, sizeof(sv_addr)) < 0)
        printf("ERRO bindando");

	listen(accept_sock, 5);

	len = sizeof(cl_addr);


	if ((sock = accept(accept_sock,(struct sockaddr *) &cl_addr, &len)) == -1)
		{
			puts("erro no accept") ;
		}
        
    bzero(buffer, BUFFER_SIZE);




    ret = read(sock, buffer, BUFFER_SIZE);
    if (ret < 0)
    printf("ERRO lendo do socket");
    printf("Mensagem lida: %s\n", buffer);
    
    ret = write(sock,"Recebi sua mensagem!", BUFFER_SIZE);
    if (ret < 0)
    printf("ERRO escrevendo no socket");
    
    close(sock);
    close(accept_sock);
    return 0;



}
