CFLAGS = -Wall -O3
LIBS = -lSDL_image -lSDL
CC = gcc

default:
	$(CC) makerawtexture.c -o mkrawtexture $(CFLAGS) $(LIBS)
	$(CC) makemap.c -o mkmap $(CFLAGS) $(LIBS)
	$(CC) makesprite.c -o mksprite $(CFLAGS)
	$(CC) makeexsprite.c -o mkexsprite $(CFLAGS)
	$(CC) makenpc.c -o mknpc $(CFLAGS)
	$(CC) makemaptable.c -o mkmaptable $(CFLAGS)
	$(CC) makeitems.c -o mkitems $(CFLAGS)
	$(CC) makepp.c -o mkpp $(CFLAGS)
	$(CC) makemagic.c -o mkmagic $(CFLAGS)
	$(CC) makeabilitytable.c -o mkat $(CFLAGS)

clean:
	rm -f mkmap
	rm -f mkrawtexture
	rm -f mksprite
	rm -f mkexsprite
	rm -f mknpc
	rm -f mkmaptable
	rm -f mkitems
	rm -f mkpp
	rm -f mkmagic
	rm -f mkat

