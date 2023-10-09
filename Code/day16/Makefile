SRCS:=$(wildcard *.c)
EXES:=$(patsubst %.c,%,$(SRCS))
CC:=gcc
all:$(EXES)
%:%.c
	$(CC) $^ -o $@ -g -pthread
clean:
	$(RM) all $(EXES)
rebuild:clean all