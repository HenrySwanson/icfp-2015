CC = gcc
CFLAGS = -Wall -g --std=c99
LFLAGS = -g

SRCS = board.c
OBJS = $(patsubst %.c, %.o, $(SRCS))

.PHONY : all clean test

all : icfp 

clean :
	rm -rf *.o icfp test

test : $(OBJS) test.o
	$(CC) $(LFLAGS) $^ -o $@
	./test

icfp : $(OBJS) icfp.o
	$(CC) $(LFLAGS) $^ -o $@

%.o : %.cpp
	$(CC) $(CFLAGS) -c $< -o $@
