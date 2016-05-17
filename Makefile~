all: server.c client.c
	gcc -o server server.c -lpthread
	gcc -o client client.c
client: client.c
	gcc -o client client.c
server: server.c
	gcc -o server server.c -lpthread
clean: 
	rm -f *.o client server