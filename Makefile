server2.o : server2.c
	gcc -c server2.c

server : server2.o
	gcc server2.o -o server