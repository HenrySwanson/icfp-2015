CC = gcc
CFLAGS = -Wall -g --std=c99
LFLAGS = -g -ljansson

SRCS = board.c unit.c
OBJS = $(patsubst %.c, %.o, $(SRCS))

.PHONY : all clean test

all : playicfp_2015

clean :
	rm -rf *.o playicfp_2015 test

test : $(OBJS) test.o
	$(CC) $(LFLAGS) $^ -o $@
	./test

playicfp_2015 : $(OBJS) playicfp_2015.o board.o unit.o gameplay.o
	$(CC) $(LFLAGS) $^ -o $@

%.o : %.cpp
	$(CC) $(CFLAGS) -c $< -o $@
