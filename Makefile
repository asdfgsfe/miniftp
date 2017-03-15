.PHONY: clean all
cc = gcc
CFLAGS = -Wall -g
BIN = miniftpd
OBJS1 = main.o sysutil.o session.o ftpproto.o privparent.o str.o tunable.o parseconf.o privsock.o
LIBS = -lcrypt

all: $(BIN)
%.o:%.c
	$(cc) $(CFLAGS) -c $< -o $@
miniftpd:$(OBJS1)
	$(cc) $(CFLAGS)  $^ -o $@ $(LIBS)
clean:
	rm -f *.o $(BIN)
