#include "rpgdarnit.h"


void loop(void *handle) {
	MAIN *m = handle;

	m->var.buttons = darnitButtonGet(m->darnit);

	return;
}


void draw(void *handle) {
	MAIN *m = handle;
	int i;


	darnitRenderOffset(m->darnit, m->cam.x, m->cam.y);
	
	for (i = 0; i < m->map.layers; i++) {
		mapDrawLayer(m, i);
		npcDrawLayer(m, i);
	}

	darnitRenderOffset(m->darnit, 0, 0);
	textboxLoop(m);

	return;
}


int init(void *handle) {
	MAIN *m = handle;

	if (cameraInit(m) != 0);
	else if (systemInit(m) != 0);
	else if (mapInit(m) != 0);
	else if (textboxInit(m) != 0);
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
	if (init(m) != 0)
		return -1;

	mapLoad(m, "res/test.ctmb");
	textboxActivate(m, "123456789012345678901234567890123456789012345678901234567890", -1, 0, 0, NULL, "Yes\nNo\nMaybe");
	darnitRenderBlendingEnable(m->darnit);
	cameraFollowNPC(m, 0);

	for (;;) {
		loop(m);
		npcLoop(m);
		cameraLoop(m);

		darnitRenderBegin();
		draw(m);
		darnitRenderEnd();
		darnitLoop(m->darnit);
	}

	return 0;
}
