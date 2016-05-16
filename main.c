#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <string.h>
#include <netinet/in.h>
#include <semaphore.h>

int main(int argc, char** argv)
{
	int accept;

	accept = socket(AF_INET, SOCK_STREAM, 0);


}