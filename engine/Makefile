CFLAGS += -O0 -g -Wall -fPIC
LDFLAGS += -ldarnit -shared -Wl,-soname,rpgdarnit.core
SRC = *.c
OUT = ../bin/rpgdarnit.core

default:
	$(CC) $(SRC) $(CFLAGS) -o $(OUT) $(LDFLAGS)
clean:
	
