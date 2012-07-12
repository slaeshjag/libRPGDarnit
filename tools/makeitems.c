#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
	int			itemtype;
	int			amount_stackable;
	int			hp;
	int			hpbyuse;
	int			spread;
	float			spread_factor;
	char			anim[32];
	char			sound[32];
	char			name[32];
	char			descr[64];
	float			attb;
	float			defb;
	float			specab;
	float			specdb;
	float			speedb;
} ITEM_PROTOTYPE;


int main(int argc, char **argv) {
	FILE *fp, *out;
	ITEM_PROTOTYPE p;
	int number;
	char c, buff[128];

	if (argc <3) {
		fprintf(stderr, "Usage: %s <input> <output>\n", argv[0]);
		return -1;
	}

	if ((fp = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "Unable to open input %s\n", argv[1]);
		return -1;
	}

	if ((out = fopen(argv[2], "w+")) == NULL) {
		fprintf(stderr, "Unable to open output %s\n", argv[2]);
		return -1;
	}

	number = 0;
	fwrite(&number, 4, 1, out);

	while (!feof(fp)) {
		c = 0;
		fscanf(fp, "%c", &c);
		switch (c) {
			case 'I':
				fscanf(fp, "%i %i %i %i %i %f %f %f %f %f %f %s %s %s %s\n", &p.itemtype, &p.amount_stackable, &p.hp, &p.hpbyuse, &p.spread, &p.spread_factor, &p.attb, &p.defb, &p.specab, &p.specdb, &p.speedb, p.anim, p.sound, p.name, p.descr);
				fwrite(&p, sizeof(ITEM_PROTOTYPE), 1, out);
				number++;
				break;
			case '\n':
				break;
			default:
				fgets(buff, 128, fp);
				break;
		}
	}

	rewind(out);
	fwrite(&number, 4, 1, out);

	return 0;
}

