CFLAGS += -Wall -O0 -g -fPIC -Wl,-soname,stdnpc.so -shared

default:
	$(CC) $(CFLAGS) -o stdnpc.so stdnpc.c
clean:
	rm -f *.so
