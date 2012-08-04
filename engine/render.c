#include "rpgdarnit.h"

int renderOverworld(void *handle) {
	MAIN *m = handle;
	int i;

	darnitRenderOffset(m->darnit, m->cam.x, m->cam.y);

	for (i = 0; i < m->map.layers; i++) {
		mapDrawLayer(m, i);
		darnitRenderBlendingEnable(m->darnit);
		npcDrawLayer(m, i);
		darnitRenderBlendingDisable(m->darnit);
	}

	darnitRenderOffset(m->darnit, 0, 0);
	textboxLoop(m);

	return 0;
}
