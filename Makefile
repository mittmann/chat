all: server.c client.c
	gcc -o server server.c -lpthread -lncurses
	gcc -o client client.c -lpthread -lncurses
client: client.c
	gcc -o client client.c -lpthread -lncurses
server: server.c
	gcc -o server server.c -lpthread -lncurses
clean: 
	rm -f *.o client server
