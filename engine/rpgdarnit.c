#include "rpgdarnit.h"


void draw(void *handle) {
	MAIN *m = handle;
	int i;


	for (i = 0; i < m->map.layers; i++) {
		mapDrawLayer(m, i);
		darnitRenderOffset(m->darnit, m->cam.x, m->cam.y);
		npcDrawLayer(m, i);
	}

	return;
}


int init(void *handle) {
	MAIN *m = handle;

	if (cameraInit(m) != 0);
	else if (mapInit(m) != 0);
	else if (npcInit(m) != 0);
	else return 0;

	return -1;
}


int rpg_main() {
	MAIN *m;

	if ((m = malloc(sizeof(MAIN))) == NULL) {
		fprintf(stderr, "Error: Unable to allocate memory for the main struct!\n");
		return -1;
	}

	m->darnit = darnitInit("RPG Darnit!");
	init(m);

	mapLoad(m, "res/test.ctmb");
	darnitRenderBlendingEnable(m->darnit);
	cameraFollowNPC(m, 0);

//	m->cam.y = 32;

	for (;;) {
		npcLoop(m);
		cameraLoop(m);

		darnitRenderBegin();
		draw(m);
		darnitRenderEnd();
		darnitLoop(m->darnit);
	}

	return 0;
}
