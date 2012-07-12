CFLAGS += -O3 -Wall
LDFLAGS += -lSDL -lX11 $(GLLIB)
SRC = src/render.c src/input.c src/map.c src/ocrpgewawa.c src/player.c src/sprite.c src/npc.c src/npc_handler.c src/coll.c
BINARY = bin/ocrpgewawa


default:
	mkdir -p bin
	cd src && make
#	$(CC) $(CFLAGS) $(SRC) -o $(BINARY) $(LIBS)
#	cd tools && make
#	cd res && make
#	cp bin/*.dlc res/build

clean:
#	rm -f $(BINARY)
	rm -Rf bin
#	cd tools && make clean
#	cd res && make clean
	cd src && make clean
