CC= g++
LFLAGS=-std=c++0x -pthread
CFLAGS= -c -Wall -Werror -g -std=c++0x -pthread

IPATH= networking/
LPATH= lib/

LIB= -lnetworking

N_DIR= networking
_N_SRCS= Client.cc ClientSocket.cc Networking.cc Server.cc ServerSocket.cc 
N_SRCS= $(patsubst %,$(N_DIR)/%,$(_N_SRCS))
N_HEADERS= $(N_SRCS:%.cc=%.h)
N_OBJS= $(N_SRCS:%.cc=%.o)

_C_SRCS=  main.cc tp1Client.cc 
C_SRCS= $(patsubst %,$(C_DIR)/%,$(_C_SRCS))
C_OBJS= $(C_SRCS:%.cc=%.o)
C_DIR= client

_S_SRCS=  main.cc tp1Server.cc 
S_SRCS= $(patsubst %,$(S_DIR)/%,$(_S_SRCS))
S_OBJS= $(S_SRCS:%.cc=%.o)
S_DIR= server


all: clear lib/libnetworking.a client.exe server.exe 

clear:
	tput reset
lib/libnetworking.a: $(N_OBJS) createDir
	ar -cvq $@ $^
$(N_DIR)/%.o: $(N_DIR)/%.cc
	$(CC) $(CFLAGS) -fpic $^ -o $@

client.exe: $(C_OBJS) lib/libnetworking.a
	$(CC) $(LFLAGS) -I$(IPATH) -L$(LPATH) $(LIB) $^  -o  $@
$(C_DIR)/%.o: $(C_DIR)/%.cc
	$(CC) $(CFLAGS) $^ -o $@

server.exe: $(S_OBJS) lib/libnetworking.a
	$(CC) $(LFLAGS) -I$(IPATH) -L$(LPATH) $(LIB) $^  -o  $@
$(S_DIR)/%.o: $(S_DIR)/%.cc
	$(CC) $(CFLAGS) $^ -o $@

createDir: lib/
	mkdir lib/
server:server.exe
	./server.exe 27015
client: client.exe
	./client.exe localhost 27015 ./

.PHONY: clean

clean:
	rm -f networking/*.o client/*.o core server/*.o lib/libnetworking.a client.exe  server.exe