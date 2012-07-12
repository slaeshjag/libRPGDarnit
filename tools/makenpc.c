#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
	unsigned int		x;
	unsigned int		y;
	unsigned int		l;
	unsigned int		moving;
	unsigned int		dir;
	unsigned int		sprite;
	unsigned int		npckey;
	unsigned int		used;
	unsigned int		activated;
	char			spriten[32];
	char			handlerFunc[32];
} NPC_ENTRY;


int main(int argc, char **argv) {
	NPC_ENTRY npc;
	FILE *fp;

	if (argc < 5) {
		fprintf(stderr, "Usage: %s <npc key> <handler function> <sprite file> <out>\n", argv[0]);
		return -1;
	}

	npc.x = npc.y = npc.l = npc.moving = npc.dir = npc.sprite = npc.used = 0;
	npc.npckey = atoi(argv[1]);
	strcpy(npc.spriten, argv[3]);
	strcpy(npc.handlerFunc, argv[2]);

	if ((fp = fopen(argv[4], "w")) == NULL) {
		fprintf(stderr, "Unable to open file %s\n", argv[4]);
		return -1;
	}

	fwrite(&npc, sizeof(NPC_ENTRY), 1, fp);
	fclose(fp);

	return 0;
}
