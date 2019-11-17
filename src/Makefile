W=--std=gnu11 -pedantic -Wall -Wextra -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wunreachable-code -Wold-style-definition -Wmissing-include-dirs -Wdeclaration-after-statement -Wjump-misses-init -Wlogical-op -Wnested-externs -Wfloat-equal -Wwrite-strings -Wpointer-arith -Wcast-qual -Wcast-align=strict -Wshadow -Wredundant-decls -Wdouble-promotion -Winit-self -Wswitch-default -Wswitch-enum -Wundef -Wlogical-op -Winline -Wformat-security -Wformat=2 -Wunused-macros -Wbad-function-cast -Wdate-time -Wpacked -Winvalid-pch -Wvla -Wdisabled-optimization
RUNTIME_CHECKS=-fsanitize=address -fsanitize=pointer-compare -fsanitize=leak -fsanitize=pointer-subtract -fsanitize=undefined -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fsanitize-address-use-after-scope -fno-omit-frame-pointer -fno-optimize-sibling-calls

CC=gcc
CFLAGS=--std=gnu11 -g -Wall -Wextra ${W} ${RUNTIME_CHECKS} -Wconversion
LDFLAGS=-lncurses ${RUNTIME_CHECKS}

CFLAGS += -D_GNU_SOURCE

EXEC=server client
BIN_DIR=../bin

all: $(EXEC)

$(EXEC):
	mkdir -p ${BIN_DIR}
	$(CC) $(LDFLAGS) $^ -o ${BIN_DIR}/$@

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

client: client.o cli.o utils.o asynchronous.o log.o client_lib.o

server: server.o log.o utils.o asynchronous.o

client.o: cli.h utils.h asynchronous.h log.h client_lib.h

server.o: utils.h log.h asynchronous.h

cli.o: cli.h utils.h

asynchronous.o: asynchronous.h

utils.o: utils.h

log.o: log.h

client_lib.o: client_lib.h

clean:
	-rm -f *.o $(addprefix ${BIN_DIR}/, ${EXEC})

.PHONY: all clean
