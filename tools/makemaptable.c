#include <stdio.h>


typedef struct {
	char			intname[8];
	char			codename[32];
	char			fname[32];
	char			offname[32];
} MAP_FILE_TABLE;



int main(int argc, char **argv) {
	MAP_FILE_TABLE mft;
	int i;
	FILE *fp, *in;

	if (argc <3) {
		fprintf(stderr, "Usage: %s <input> <output>\n", argv[0]);
		return -1;
	}

	if ((in = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "Error: Unable to open input %s\n", argv[1]);
		return -1;
	}

	if ((fp = fopen(argv[2], "w+")) == NULL) {
		fprintf(stderr, "Error: Unable to open output %s\n", argv[2]);
		return -1;
	}

	i = 0;
	fwrite(&i, 4, 1, fp);

	*mft.intname = 0;

	while (!feof(in)) {
		fscanf(in, "%s %s %s %s\n", mft.intname, mft.codename, mft.fname, mft.offname);
		if (*mft.intname != 0) {
			i++;
			fwrite(&mft, sizeof(MAP_FILE_TABLE), 1, fp);
			*mft.intname = 0;
		} else
			break;
	}

	fseek(fp, 0, SEEK_SET);
	fwrite(&i, 4, 1, fp);

	return 0;
}

