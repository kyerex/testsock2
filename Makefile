CC=	gcc
CPP=	g++
CPPFLAGS= -g -O -Wall -I.


OBJS=	ServerLog.o \
	Sock2.o \
	do.o \
	server.o\
	client.o

ifeq ($(OS),Windows_NT)
LIBS2=-lws2_32
STATIC=-static
else
LIBS2=
STATIC=
endif

all:	testsock2

testsock2:	$(OBJS) 
	g++ $(STATIC) -o server Sock2.o do.o ServerLog.o server.o $(LIBS2)  
	g++ $(STATIC) -o client Sock2.o do.o ServerLog.o client.o $(LIBS2)  

clean:
	@rm -f *.o
	@rm -f server.exe
	@rm -f client.exe
	@rm -f server
	@rm -f client
