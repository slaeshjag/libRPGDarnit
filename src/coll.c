#include "rpgdarnit.h"


unsigned int collTest(void *handle, unsigned int tilex, unsigned int tiley, unsigned int tilel, int dir) {
	MAIN *m = handle;
	int i, tilepos;
	unsigned int resmask;

	if (tilex >= m->map.w || tiley >= m->map.h || tilel >= m->map.layers || tilex < 0 || tiley < 0 || tilel < 0) {
		fprintf(stderr, "collTest(): Error: Position (%i, %i, %i) is outside the map\n", tilex, tiley, tilel);
		return COLL_TILE_IS_SOLID;
	}

	tilepos = tilex + m->map.w*tiley;
	resmask = m->map.layer[tilel]->data[tilepos] & MAP_MASK_USE;

	if (resmask & ((1 << (dir >> 1)) << 16))
		resmask |= COLL_TILE_IS_SOLID | COLL_TILE_IS_UNWALKABLE;
	else if (dir & 0x1) {			/* We need to test diagonally! */
		if (dir < 6)
			resmask |= (resmask & ((1 << ((dir >> 1) + 1)) << 16)) ? (COLL_TILE_IS_SOLID | COLL_TILE_IS_UNWALKABLE) : 0;
		else
			resmask |= (resmask & MAP_BLOCK_TOP) ? (COLL_TILE_IS_SOLID | COLL_TILE_IS_UNWALKABLE) : 0;
	}

	for (i = 0; i < NPC_RESERVATION_SIZE; i++) {
		if (m->npc.reservation_list[i] == tilepos + m->map.w * m->map.h * tilel) {
			resmask |= COLL_TILE_IS_NPC | COLL_TILE_IS_UNWALKABLE | COLL_TILE_IS_SOLID;
			break;
		}
	}
	
	return resmask;
}




