#include <stdio.h>
#include <stdlib.h>

#include		<SDL/SDL.h>
#include		<SDL/SDL_image.h>

int main(int argc, char **argv) {
	FILE *out, *frame;
	int i, frames, tiles, t, f, dirs, time;
	unsigned int x1, y1, x2, y2, tx, ty, w, h, spriteloc, framestart, tilestart;
	char c, buff[256];
	float wgran, hgran;
	i = 0;
	SDL_Surface *tex;
	
	if (argc < 4) {
		fprintf(stderr, "Invalid amount of arguments!\n");
		printf("Usage: %s <image> <frames> <destination>\n", argv[0]);
		return -1;
	}
	
	if ((out = fopen(argv[3], "w+")) == NULL) {
		fprintf(stderr, "Failed to open file %s for reading!\n", argv[3]);
		return -1;
	}
	
	
	if ((tex = IMG_Load(argv[1])) == NULL) {
		printf("Failed to load %s\n", argv[1]);
		return 0;
	}
	
	if (!tex->format->Amask) {
		fprintf(stderr, "Image #%i does NOT have an alpha channel!\n", i);
		return 0;
	}
	
	fwrite(&tex->w, 4, 1, out);
	fwrite(&tex->h, 4, 1, out);
	w = tex->w, h = tex->h;

	
	for (i = 0; i < tex->h; i++) {
		fwrite(transpix, tex->w, 4, out);
	}
	
	SDL_FreeSurface(tex);
	
	if ((frame = fopen(argv[2], "r")) == NULL) {
		fprintf(stderr, "Failed to open file %s for reading!\n", argv[2]);
		return 0;
	}

	frames = tiles = t = f = dirs = 0;
	wgran = 

	spriteloc = ftell(out);
	fwrite(&t, 4, 1, out);
	fwrite(&t, 4, 1, out);
	fwrite(&t, 4, 1, out);
	framestart = ftell(out);
	fwrite(&t, 4, 1, out);
	tilestart = ftell(out);
	fwrite(&t, 4, 1, out);
	fwrite(&t, 4, 1, out);

	for (;;) {
		c = getc(frame);
		switch (c) {
			case '#':
				fgets(buff, 256, frame);
				break;
			case 'E':				// E for End of frame
				fscanf(frames, "%i\n", &time);
				fseek(tilestart, SEEK_SET, tilestart);
				fwrite(&t, 4, 1, out);
				fwrite(&time, 4, 1, out);
				t = 0;
				fseek(out, SEEK_END, 0);
				break;
			case '\n':
				break;
			case 'D':				// D for new Direction
				fseek(out, SEEK_SET, framestart);
				fwrite(&f 4, 1, out);
				fseek(out, SEEK_END, 0);
				spriteloc = ftell(out);
				fwrite(&t, 4, 1, out);
				fwrite(&t, 4, 1, out);
				fwrite(&t, 4, 1, out);
				framestart = ftell(out);
				fwrite(&t, 4, 1, out);
				tilestart = ftell(out);
				fwrite(&t, 4, 1, out);
				fwrite(&t, 4, 1, out);
				f = 0;
				dirs++;
				break;
			case 'T':				// T for Tile
				

	
	fclose(out);
	return 0;
}
