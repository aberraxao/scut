CC = gcc
CFLAGS = -g -Wall

TARGETS = cliente servidor

all : $(TARGETS)

clean :
	rm -f $(TARGETS)

cliente : cliente.c
	$(CC) $(CFLAGS) cliente.c -o cliente

servidor : servidor.c
	$(CC) $(CFLAGS) servidor.c -o servidor
