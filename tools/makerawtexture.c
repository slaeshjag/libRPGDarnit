#include		<stdlib.h>
#include		<stdio.h>

#include		<SDL/SDL.h>
#include		<SDL/SDL_image.h>

int main(int argc, char **argv) {
	FILE *out;
	int i, *transpix;
	i = 0;
	SDL_Surface *tex;
	
	if (argc <3) {
		fprintf(stderr, "Invalid amount of arguments!\n");
		printf("Usage: %s <image> <destination>\n", argv[0]);
		return -1;
	}
	
	if ((out = fopen(argv[2], "w")) == NULL) {
		fprintf(stderr, "Failed to open file %s for reading!\n", argv[2]);
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
	transpix = tex->pixels;
	
	for (i = 0; i < tex->h; i++) {
		fwrite(transpix, tex->w, 4, out);
		transpix = &transpix[tex->w];
	}
	
	SDL_FreeSurface(tex);
	
	fclose(out);
	
	return 0;
}
