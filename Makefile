CFLAGS += -O3 -Wall
LDFLAGS += -lSDL -lX11 $(GLLIB)
SRC = src/render.c src/input.c src/map.c src/ocrpgewawa.c src/player.c src/sprite.c src/npc.c src/npc_handler.c src/coll.c
BINARY = bin/ocrpgewawa


default:
	mkdir -p bin
	cd engine && make
	cd launcher && make
	cd npc_lib && make
	cd res && make

clean:
	rm -Rf bin
	cd engine && make clean
	cd launcher && make clean
	cd npc_lib && make clean
	cd res && make clean
