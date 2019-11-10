W=--std=gnu11 -pedantic -Wall -Wextra -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wunreachable-code -Wold-style-definition -Wmissing-include-dirs -Wdeclaration-after-statement -Wjump-misses-init -Wlogical-op -Wnested-externs -Wfloat-equal -Wwrite-strings -Wpointer-arith -Wcast-qual -Wcast-align=strict -Wshadow -Wredundant-decls -Wdouble-promotion -Winit-self -Wswitch-default -Wswitch-enum -Wundef -Wlogical-op -Winline -Wformat-security -Wformat=2 -Wunused-macros -Wbad-function-cast -Wdate-time -Wpacked -Winvalid-pch -Wvla -Wdisabled-optimization
runtime_checks=-fsanitize=address -fsanitize=pointer-compare -fsanitize=leak -fsanitize=pointer-subtract -fsanitize=undefined -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fsanitize-address-use-after-scope -fno-omit-frame-pointer -fno-optimize-sibling-calls
CC=gcc
CFLAGS=--std=gnu11 -g -Wall -Wextra ${W} ${runtime_checks} #-Wconversion
LDFLAGS=-lncurses ${runtime_checks}

EXEC=client server

all: $(EXEC)

client: client.o cli.o utils.o asynchronous.o 

server: server.o log.o utils.o asynchronous.o

client.o: cli.h utils.h asynchronous.h 

server.o: utils.h log.h asynchronous.h

cli.o: cli.h

asynchronous.o: asynchronous.h

utils.o: utils.h

log.o: log.h

clean:
	rm -f *.o

clear:
	rm -f $(EXEC)
