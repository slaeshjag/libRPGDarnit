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


int state(void *handle) {
	MAIN *m = handle;
	
	if (m->var.state != m->var.newstate) {
		if (darnitRenderFadeChanging(m->darnit) != 2)
			darnitRenderFadeIn(m->darnit, m->system.fade_duration, 0, 0, 0);
		else {
			stateSet(m);
			darnitRenderFadeOut(m->darnit, m->system.fade_duration);
		}
	}

	switch (m->var.state) {
		case STATE_OVERWORLD:
			npcLoop(m);
			cameraLoop(m);
			renderOverworld(m);
			break;
		case STATE_INVENTORY:
			/*inventory(m);*/
			break;
		case STATE_TELEPORTING:
			/* Stubbe, jag har ingen aning om vad det här fallet behöver göra i en huvudslinga */
			break;
		default:
			/* Egentligen borde vi inte ha ett panikfall, men sådana är reglerna med switch()... */
			break;
	}

	return 0;
}


int init(void *handle) {
	MAIN *m = handle;

	m->var.state = m->var.newstate = STATE_OVERWORLD;	/* TODO: Måste fixas när menysystem och allt det där implementeras */
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

	mapLoad(m, "TESTMAP");
	textboxActivate(m, "123456789012345678901234567890123456789012345678901234567890", -1, 0, 0, NULL, "Yes\nNo\nMaybe");
	darnitRenderBlendingEnable(m->darnit);
	cameraFollowNPC(m, 0);

	for (;;) {
		loop(m);

		darnitRenderBegin();
		
		state(m);
		
		darnitRenderEnd();
		darnitLoop(m->darnit);
	}

	return 0;
}
