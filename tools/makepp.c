#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
	char			name[16];
	unsigned int		base_hp;
	unsigned int		base_mp;
	unsigned int		base_att;
	unsigned int		base_def;
	unsigned int		base_speca;
	unsigned int		base_specd;
	unsigned int		base_speed;
	int			per_HP;
	int			per_MP;
	int			per_att;
	int			per_def;
	int			per_speca;
	int			per_specd;
	int			per_speed;
	int			amp_HP;
	int			amp_MP;
	int			amp_att;
	int			amp_def;
	int			amp_speca;
	int			amp_specd;
	int			amp_speed;
	int			resist[8];
	char			anim[32];
} PARTY_MEMBER_SKEL;


int main(int argc, char **argv) {
	FILE *fp, *out;
	PARTY_MEMBER_SKEL sk;
	int num;
	char c, buff[256];

	if (argc <3) {
		fprintf(stderr, "Usage: %s <input> <output>\n", argv[0]);
		return -1;
	}

	if ((fp = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "Error: Unable to open input %s\n", argv[1]);
		return -1;
	}

	if ((out = fopen(argv[2], "w+")) == NULL) {
		fprintf(stderr, "Error: Unable to open output %s\n", argv[2]);
		return -1;
	}

	num = 0;
	fwrite(&num, 4, 1, out);

	while (!feof(fp)) {
		c = 0;
		c = fgetc(fp);

		switch (c) {
			case 'M':
				fscanf(fp, "%s %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %s\n", sk.name, &sk.base_hp, &sk.base_mp, &sk.base_att, &sk.base_def, &sk.base_speca, &sk.base_specd, &sk.base_speed, &sk.per_HP, &sk.per_MP, &sk.per_att, &sk.per_def, &sk.per_speca, &sk.per_specd, &sk.per_speed, &sk.amp_HP, &sk.amp_MP, &sk.amp_att, &sk.amp_def, &sk.amp_speca, &sk.amp_specd, &sk.amp_speed, &sk.resist[0], &sk.resist[1], &sk.resist[2], &sk.resist[3], &sk.resist[4], &sk.resist[5], &sk.resist[6], &sk.resist[7], sk.anim);
				fwrite(&sk, sizeof(PARTY_MEMBER_SKEL), 1, out);
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

	return 0;
}

