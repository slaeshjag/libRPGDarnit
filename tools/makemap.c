#include		<stdlib.h>
#include		<stdio.h>

#include		<SDL/SDL.h>
#include		<SDL/SDL_image.h>


typedef struct {
	char		mapname[8];
	unsigned int	xf;
	unsigned int	yf;
	unsigned int	lf;
	unsigned int	xt;
	unsigned int	yt;
	unsigned int	lt;
} TELEPORTER;


typedef struct {
	char		npc[32];
	int		x;
	int		y;
	int		l;
} NPCSPAWN;


int main(int argc, char **argv) {
	FILE *out, *tel, *npcfp;
	unsigned int i, *transpix, h, l;
	SDL_Surface *tex;
	char intname[128], sprsheet[32], tilesheet[32], c;
	TELEPORTER tp;
	NPCSPAWN npc;
	
	if (argc < 9) {
		fprintf(stderr, "Invalid amount of arguments!\n");
		printf("Usage: %s <image> <layers> <internal name> <teleports> <npc spawns> <destination> <spritesheet> <tilesheet>\n", argv[0]);
		return -1;
	}
	
	if ((out = fopen(argv[6], "w")) == NULL) {
		fprintf(stderr, "Failed to open file %s for writing!\n", argv[6]);
		return -1;
	}

	if ((npcfp = fopen(argv[5], "r")) == NULL) {
		fprintf(stderr, "Failed to open file %s for reading!\n", argv[5]);
		return -1;
	}

	if ((tel = fopen(argv[4], "r")) == NULL) {
		fprintf(stderr, "Failed to open file %s for reading!\n", argv[4]);
		return -1;
	}
	
	strcpy(intname, argv[3]);
	strcpy(sprsheet, argv[7]);
	strcpy(tilesheet, argv[8]);
	intname[7] = 0;
	sprsheet[31] = 0;
	tilesheet[31] = 0;

	l = atoi(argv[2]);
	
	if ((tex = IMG_Load(argv[1])) == NULL) {
		printf("Failed to load %s\n", argv[1]);
		return 0;
	}
	
	if (!tex->format->Amask) {
		fprintf(stderr, "Image %s does NOT have an alpha channel!\n", argv[1]);
		return 0;
	}

	h = tex->h / l;
	
	fwrite(&tex->w, 4, 1, out);
	fwrite(&h, 4, 1, out);
	fwrite(&l, 4, 1, out);
	fwrite(intname, 8, 1, out);
	fwrite(sprsheet, 32, 1, out);
	fwrite(tilesheet, 32, 1, out);
	transpix = tex->pixels;
	
	for (i = 0; i < tex->h * tex->w; i++) {
		h = (transpix[i] & 0xF0) >> 4;
		h |= (transpix[i] & 0xF000) >> 8;
		h |= (transpix[i] & 0xF00000) >> 12;
		fwrite(&h, 1, 4, out);
	}
	
	SDL_FreeSurface(tex);
	
	fscanf(tel, "%i\n", &l);
	fwrite(&l, 4, 1, out);

	while (!feof(tel)) {
		c = getc(tel);

		switch (c) {
			case '\n':
				break;
			case 'T':
				fscanf(tel, "%u %u %u %s %u %u %u\n", &tp.xf, &tp.yf, &tp.lf, tp.mapname, &tp.xt, &tp.yt, &tp.lt);
				fwrite(&tp, sizeof(TELEPORTER), 1, out);
				break;
			default:
				fgets(intname, 128, tel);
				break;
		}

	}

	fscanf(npcfp, "%i\n", &l);
	fwrite(&l, 4, 1, out);

	while (!feof(npcfp)) {
		fscanf(npcfp, "%s %i %i %i\n", npc.npc, &npc.x, &npc.y, &npc.l);
		fwrite(&npc, sizeof(NPCSPAWN), 1, out);
	}


	fclose(out);
	
	return 0;
}
