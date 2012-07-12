#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
	char			name[32];
	int			mpreq;
	char			desc[64];
	unsigned int		type;
} MAGIC_PROTOTYPE;


int main(int argc, char **argv) {
	MAGIC_PROTOTYPE prot;
	FILE *fp, *out;
	char c, buff[256];
	int num;

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

	while (!feof(fp)) {
		c = fgetc(fp);
		switch (c) {
			case 'M':
				fscanf(fp, "%s %i %s %u\n", prot.name, &prot.mpreq, prot.desc, &prot.type);
				fwrite(&prot, sizeof(MAGIC_PROTOTYPE), 1, out);
				num++;
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

	fclose(out);

	return 0;
}
