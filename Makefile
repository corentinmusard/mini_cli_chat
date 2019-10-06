W=-Wconversion --std=gnu11 -pedantic -Wall -Wextra -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wunreachable-code -Wold-style-definition -Wmissing-include-dirs -Wdeclaration-after-statement -Wjump-misses-init -Wlogical-op -Wnested-externs -Wfloat-equal -Wwrite-strings -Wpointer-arith -Wcast-qual -Wcast-align -Wshadow -Wredundant-decls -Wdouble-promotion -Winit-self -Wswitch-default -Wswitch-enum -Wundef -Wlogical-op -Winline -Wformat-security -Waggregate-return -Wformat=2
CC=gcc
CFLAGS=--std=gnu11 -g -Wall -Wextra ${W}
LDFLAGS=-lncurses

EXEC=client server

all: $(EXEC)

client: client.o

server: server.o

client.o: client.c

server.o: server.c

all: $(EXEC)

clean:
	rm -f *.o

clear:
	rm -f $(EXEC)
