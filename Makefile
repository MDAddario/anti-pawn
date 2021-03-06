SRCS = ap_game.c minimax.c
OBJS = ${SRCS:.c=.o}
HDS = ${SRCS:.c=.h}

DIMENSION = 6
MAX_DEPTH = 8
DEFINE = -DDIMENSION=${DIMENSION} -DMAX_DEPTH=${MAX_DEPTH}

all: main

main: main.c ${OBJS}
	gcc main.c ${OBJS} -o main ${DEFINE}

.c.o: ${SRCS} ${HDS}
	gcc -c $< ${DEFINE}

clean:
	rm -f main ${OBJS}
