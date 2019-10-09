W=-Wconversion --std=gnu11 -pedantic -Wall -Wextra -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wunreachable-code -Wold-style-definition -Wmissing-include-dirs -Wdeclaration-after-statement -Wjump-misses-init -Wlogical-op -Wnested-externs -Wfloat-equal -Wwrite-strings -Wpointer-arith -Wcast-qual -Wcast-align -Wshadow -Wredundant-decls -Wdouble-promotion -Winit-self -Wswitch-default -Wswitch-enum -Wundef -Wlogical-op -Winline -Wformat-security -Waggregate-return -Wformat=2
CC=gcc
CFLAGS=--std=gnu11 -g -Wall -Wextra ${W}
LDFLAGS=-lncurses

EXEC=client server

all: $(EXEC)

client: client.o cli.o #asynchronous.o utils.o

server: server.o log.o #utils.o asynchronous.o

client.o: cli.h #asynchronous.h utils.h

server.o: utils.h log.h #asynchronous.h

cli.o: cli.h
log.o: log.h

clean:
	rm -f *.o

clear:
	rm -f $(EXEC)
