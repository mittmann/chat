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

	int accept, sock, ret, len;
	char buffer[BUFFER_SIZE];
	struct sockaddr_in sv_addr, cl_addr;

	accept = socket(AF_INET, SOCK_STREAM, 0);



	memset(&sv_addr, 0, sizeof(sv_addr));
	sv_addr.sin_family = AF_INET;
	sv_addr.sin_addr.s_addr = INADDR_ANY;
	sv_addr.sin_port = PORT;



	if (!bind(accept, (struct sockaddr *) &sv_addr, sizeof(sv_addr)))
		puts("erro no bind");


	listen(accept, 5);

	len = sizeof(cl_addr);


	sock = accept(accept, &cl_addr, len, NULL);
	if (sock < 0)
		{
			puts("erro no accept") ;
		}



	ret = recvfrom(sock, buffer, BUFFER_SIZE, 0, NULL, NULL);


	if (ret > 0)
	{
		buffer[ret] = '\0';
		puts(buffer);
	}
	else
	{
		puts("não leu nada");
	}



}
