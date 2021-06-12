SRCS = main.c
OBJS = $(SRCS:.c=.o)
CFLAGS = -O2 -std=gnu99
LDFLAGS = -lX11 -lXi -lXtst

.PHONY: all run xkc clean

all: xkc

xkc: $(OBJS)
	gcc -o xkc $(OBJS) $(CFLAGS) $(LDFLAGS)

.c.o:
	gcc $(CFLAGS) -c $< -o $@

main.o: config.h

run: all
	./xkc

clean:
	rm *.o xkc
