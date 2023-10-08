OUT:=all
SRCS:=$(wildcard *.c)
EXES:=$(patsubst %.c,%,$(SRCS))
CC:=gcc
$(OUT):$(EXES)
%:%.c
	$(CC) $^ -o $@ -g
clean:
	$(RM) $(OUT) $(EXES)
rebuild:clean all