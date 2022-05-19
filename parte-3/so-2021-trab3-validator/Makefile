
SOURCE = ../solution

ifndef SOURCE
    $(error SOURCE is not defined)
endif

#CC = gcc-11.3.0
CC = gcc
SED = sed

TARGETS = cliente-eval servidor-eval

CFLAGS = -Wall -D_EVAL=$(SOURCE) -Wno-format-extra-args

all : $(TARGETS)

.PHONY: clean

clean :
	rm -f cliente-eval
	rm -f servidor-eval
	rm -f cliente.c servidor.c

cliente.c : cliente.sed $(SOURCE)/cliente.c
	$(SED) -f cliente.sed $(SOURCE)/cliente.c > cliente.c

servidor.c : servidor.sed $(SOURCE)/servidor.c
	$(SED) -f servidor.sed $(SOURCE)/servidor.c > servidor.c

cliente-eval : cliente-eval.c eval.c cliente.c common.h eval.h
	$(CC) $(CFLAGS) -D_CLIENTE eval.c cliente.c cliente-eval.c -o cliente-eval
	rm cliente.c

servidor-eval : servidor-eval.c eval.c servidor.c common.h eval.h
	$(CC) $(CFLAGS) -D_SERVIDOR  eval.c servidor.c servidor-eval.c -o servidor-eval
	rm servidor.c

