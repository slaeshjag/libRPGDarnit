#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
	unsigned int		mareq;
	unsigned int		mdreq;
	unsigned int		agilityreq;
	int			ability;
} ABILITY_LEARN_ENTRY;


int main(int argc, char **argv) {
	FILE *fp, *out;
	int num, i;
	char c, buff[256];
	ABILITY_LEARN_ENTRY lentry;

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

	num = 0;
	fwrite(&num, 4, 1, out);
	i = 0;

	while (!feof(fp)) {
		c = fgetc(fp);
		switch (c) {
			case 'A':
				fscanf(fp, "%u %u %u %i\n", &lentry.mareq, &lentry.mdreq, &lentry.agilityreq, &lentry.ability);
				fwrite(&lentry, sizeof(ABILITY_LEARN_ENTRY), 1, out);
				i++;
				break;
			case 'N':
				fgets(buff, 256, fp);
				lentry.mareq = (~0);
				for (; i < 16; i++)
					fwrite(&lentry, sizeof(ABILITY_LEARN_ENTRY), 1, out);
				i = 0;
				num++;
				break;
			case 'E':
				fgets(buff, 256, fp);
				lentry.mareq = (~0);
				for (; i < 16; i++)
					fwrite(&lentry, sizeof(ABILITY_LEARN_ENTRY), 1, out);
				i = 0;
				break;
			case '\n':
				break;
			default:
				fgets(buff, 256, fp);
				break;
		}
	}

	rewind(out);
	fwrite(&num, 4, 1, out);
	
	return 0;
}
