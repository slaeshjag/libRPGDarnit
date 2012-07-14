#include "rpgdarnit.h"



void npcDrawLayer(void *handle, int layer) {
	MAIN *m = handle;
	int i;

	for (i = 0; i < m->npc.npcs; i++)
		if (m->npc.sort[i].npc >= 0 && m->npc.sort[i].layer == layer) 
			darnitSpriteDraw(m->npc.npc[m->npc.sort[i].npc].sprite);
	return;
}


void npcReSort(void *handle) {
	MAIN *m = handle;
	NPC_SORT tmp;
	int i, j;
	 
	// loopen börjar på 1 för att första elementet är definitivt sorterat när det står ensamt.
	// därefter hämtas ett element i taget ur "buffern" (resten av arrayen)
	for (i = 1; i < m->npc.npcs; i++) {
		// så länge nuvarande bufferelement (j) är mindre än det precis till vänster i den sorterade listan
		// ska det skiftas ett steg till vänster.
		for (j = i; j > 0 && m->npc.sort[j].y < m->npc.sort[j-1].y; j--) {
			tmp = m->npc.sort[j];
			m->npc.sort[j] = m->npc.sort[j-1];
			m->npc.sort[j-1] = tmp;
		}
	}

	return;
}


void npcSetSort(void *handle, int npc, int layer, int y) {
	MAIN *m = handle;
	int i;

	for (i = 0; i < m->npc.npcs; i++)
		if (m->npc.sort[i].npc == npc) {
			m->npc.sort[i].y = y;
			m->npc.sort[i].layer = layer;
			npcReSort(m);
			return;
		}
	return;
}


void npcAddSort(void *handle, int npc) {
	MAIN *m = handle;
	int i;

	for (i = 0; i < m->npc.npcs; i++)
		if (m->npc.sort[i].npc == -1) {
			m->npc.sort[i].npc = npc;
			return;
		}
	return;
}


void npcDeleteSort(void *handle, int npc) {
	MAIN *m = handle;
	int i;

	for (i = 0; i < m->npc.npcs; i++)
		if (m->npc.sort[i].npc == npc) {
			m->npc.sort[i].npc = -1;
			return;
		}
	return;
}


int npcLimitSet(void *handle, unsigned int limit) {
	MAIN *m = handle;

	npcKillAll(handle);
	m->npc.npcs = limit;

	if ((m->npc.npc = realloc(m->npc.npc, sizeof(NPC_ENTRY) * limit)) == NULL)
		m->npc.npcs = 0;
	if ((m->npc.reservation_list = realloc(m->npc.reservation_list, sizeof(unsigned int) * m->npc.npcs * 4)) == NULL)
		m->npc.npcs = 0;
	if ((m->npc.sort = realloc(m->npc.sort, sizeof(NPC_SORT) * limit)) == NULL)
		m->npc.npcs = 0;
	
	npcClear(m);
	
	return 0;
}


int npcInit(void *handle) {
	MAIN *m = handle;

	m->npc.npcs = 0;
	m->npc.npc = NULL;
	m->npc.reservation_list = NULL;
	m->npc.sort = NULL;

	m->npc.codelib = NULL;
	m->npc.npcSaveData = NULL;
	npcKillAll(m);
	
	return 0;
}


void npcLoadCode(void *handle, const char *fname) {
	MAIN *m = handle;
	void (*npcLibInitMaster_)(void*);

	if (m->npc.codelib != NULL)
		darnitDynlibClose(m->npc.codelib);

	if ((m->npc.codelib = darnitDynlibOpen(fname)) == NULL) {
		fprintf(stderr, "Unable to load NPC code from file %s\n", fname);
		return;
	}

	*(void **)(&npcLibInitMaster_) = darnitDynlibGet(m->npc.codelib, "npcLibInitMaster");
	(*npcLibInitMaster_)(m);

	return;
}


void npcLoop(void *handle) {
	MAIN *m = handle;
	int i;

	for (i = 0; i < m->npc.npcs; i++) 
		if (m->npc.npc[i].used == 1) {
			(m->npc.npc[i].npcHandler)(m, i);
			if (m->npc.npc[i].moving == 1) {
				npcHandleMovement(m, i);
				darnitSpriteAnimationEnable(m->npc.npc[i].sprite);
			} else
				darnitSpriteAnimationDisable(m->npc.npc[i].sprite);
		}

	return;
}


int npcGetOne(void *handle) {
	MAIN *m = handle;
	int i;

	for (i = 0; i < m->npc.npcs; i++)
		if (m->npc.npc[i].used == 0)
			return i;
	
	return -1;
}


int npcSpawn(void *handle, int x, int y, int l, int direction, const char *sprite, const char *logic_func) {
	MAIN *m = handle;
	int npc;

	if ((npc = npcGetOne(m)) < 0) {
		fprintf(stderr, "Unable to spawn a NPC: Array's full\n");
		return -1;
	}

	x *= m->map.tile_w;
	y *= m->map.tile_h;

	m->npc.npc[npc].x = x;
	m->npc.npc[npc].y = y;
	m->npc.npc[npc].l = l;
	m->npc.npc[npc].dir = direction,

	m->npc.npc[npc].moving = 0;


	m->npc.npc[npc].sprite = darnitSpriteLoad(m->darnit, sprite, direction, DARNIT_TILESHEET_FORMAT_RGBA);
	npcTeleport(m, npc, x, y, l);
	
	m->npc.npc[npc].used = 1;
	m->npc.npc[npc].finished_moving = 0;
	m->npc.npc[npc].init = 1;
	m->npc.npc[npc].activated = 0;
	m->npc.npc[npc].inv_activated = 0;
	m->npc.npc[npc].textbox_ack = 0;

	if ((m->npc.npc[npc].npcHandler = darnitDynlibGet(m->npc.codelib, logic_func)) == NULL) {
		fprintf(stderr, "Fatal error: Unable to get symbol %s\n", logic_func);
		darnitSpriteDelete(m, m->npc.npc[npc].sprite);
		m->npc.npc[npc].used = 0;
		return -1;
	}
	(m->npc.npc[npc].npcHandler)(m, npc);

	npcAddSort(m, npc);
	npcSetSort(m, npc, m->npc.npc[npc].l, m->npc.npc[npc].y);

	return npc;
}


void npcReserveTile(void *handle, unsigned int npc, unsigned int x, unsigned int y, unsigned int l) {
	MAIN *m = handle;
	int i, tile, npcp;

	if (x >= m->map.w || y >= m->map.h || l >= m->map.layers) {
		fprintf(stderr, "Error: npcReserveTile: Position (%i, %i, %i) is outside of the map, not reserving\n", x, y, l);
		return;
	}

	npcp = npc * 4;
	tile = x + y*m->map.w + l*m->map.w * m->map.h;

	for (i = npcp; i < npcp + 4; i++)
		if (m->npc.reservation_list[i] == -1) {
			m->npc.reservation_list[i] = tile;
			return;
		}
	
	fprintf(stderr, "Error: npcReserveTile: No free tile slots to put reservation in\n");
	return;
}


void npcReservationsDeleteMine(void *handle, unsigned int npc) {
	MAIN *m = handle;
	int npcp, i;

	npcp = npc * 4;

	for (i = npcp; i < npcp + 4; i++)
		m->npc.reservation_list[i] = -1;
	
	return;
}


void npcReservationsDelete(void *handle) {
	MAIN *m = handle;
	int i;

	for (i = 0; i < m->npc.npcs << 2; i++)
		m->npc.reservation_list[i] = -1;
	
	return;
}


int npcGetTileInfront(void *handle, unsigned int npc) {
	MAIN *m = handle;
	int hdir, vdir, x, y, dir;

	dir = m->npc.npc[npc].dir;

	hdir = vdir = 0;
	
	if (dir > 0 && dir < 4)
		vdir = 1;
	else if (dir > 4)
		vdir = -1;
	if (dir > 2 && dir < 6)
		hdir = 1;
	else if (dir == 7 || dir == 0 || dir == 1)
		hdir = -1;
	
	x = m->npc.npc[npc].x / m->map.tile_w;
	y = m->npc.npc[npc].y / m->map.tile_h;

	x += vdir;
	y += hdir;

	if (x > m->map.w || y > m->map.h || x < 0 || y < 0)
		return -2;

	return x + y*m->map.w + m->npc.npc[npc].l * m->map.w * m->map.h;
}


void npcActivateTile(void *handle, int npc) {
	MAIN *m = handle;
	int tile, i;

	tile = npcGetTileInfront(m, npc);

	for (i = 0; i < NPC_RESERVATION_SIZE; i++)
		if (m->npc.reservation_list[i] == tile)
			break;
	
	if (i == NPC_RESERVATION_SIZE)
		return;
	i >>= 2;
	m->npc.npc[i].activated = 1;
	(m->npc.npc[i].npcHandler)(m, i);

	return;
}


void npcInvActivateTile(void *handle, int npc) {
	MAIN *m = handle;
	int tile, i;

	tile = npcGetTileInfront(m, npc);

	for (i = 0; i < NPC_RESERVATION_SIZE; i++)
		if (m->npc.reservation_list[i] == tile)
			break;
	
	if (i == NPC_RESERVATION_SIZE)
		return;
	i >>= 2;
	m->npc.npc[i].inv_activated = 1;
	(m->npc.npc[i].npcHandler)(m, i);

	return;
}

void npcTeleport(void *handle, unsigned int npc, unsigned int x, unsigned int y, unsigned int l) {
	MAIN *m = handle;
	int xc, yc, xd, yd, h_comp;

	m->npc.npc[npc].x = x;
	m->npc.npc[npc].y = y;
	m->npc.npc[npc].l = l;

	xc = yc = 0;
	xd = x / m->map.tile_w;
	yd = y / m->map.tile_h;

	npcReservationsDeleteMine(m, npc);
	
	if (x % m->map.tile_w)
		xc = 1;
	if (y % m->map.tile_h)
		yc = 1;
	
	npcReserveTile(m, npc, xd, yd, l);
	if (xc == 1)
		npcReserveTile(m, npc, xd+1, yd, l);
	if (yc == 1)
		npcReserveTile(m, npc, xd, yd+1, l);
	if (xc == 1 && yc == 1)
		npcReserveTile(m, npc, xd+1, yd+1, l);

	h_comp = darnitSpriteHeight(m->npc.npc[npc].sprite);
	h_comp -= m->map.tile_h;

	darnitSpriteMove(m->npc.npc[npc].sprite, m->npc.npc[npc].x,  m->npc.npc[npc].y - h_comp);
	npcSetSort(m, npc, m->npc.npc[npc].l, m->npc.npc[npc].y - h_comp);


	return;
}


int npcDirectionCalculate(int x, int y) {
	int dir;

	if (x < 0) {
		dir = 6;
		if (y < 0)
			dir++;
		else if (y > 0)
			dir--;
	} else if (x > 0) {
		dir = 2;
		if (y < 0)
			dir--;
		else if (y > 0)
			dir++;
	} else if (y < 0)
		dir = 0;
	else
		dir = 4;

	return dir;
}


void npcMovementSetup(void *handle, unsigned int npc, unsigned int x, unsigned int y, int mode) {
	MAIN *m = handle;
	int xd = 0, yd = 0, dir;

	if (x > m->npc.npc[npc].x)
		xd = 1;
	else if (x < m->npc.npc[npc].x)
		xd = -1;
	if (y > m->npc.npc[npc].y)
		yd = 1;
	else if (y < m->npc.npc[npc].y)
		yd = -1;

	dir = npcDirectionCalculate(xd, yd);
	m->npc.npc[npc].dir = dir;
	darnitSpriteDirectionChange(m->npc.npc[npc].sprite, dir);
	
	if (collTest(m, m->npc.npc[npc].x / m->map.tile_w + xd, m->npc.npc[npc].y / m->map.tile_h + yd, m->npc.npc[npc].l, dir) & COLL_TILE_IS_UNWALKABLE)
		return;
	
	if (m->npc.npc[npc].dir & 1) {
		if (yd != 0 && collTest(m, m->npc.npc[npc].x / m->map.tile_w, m->npc.npc[npc].y / m->map.tile_h + yd, m->npc.npc[npc].l, dir) & COLL_TILE_IS_UNWALKABLE)
			return;
		if (xd != 0 && collTest(m, m->npc.npc[npc].x / m->map.tile_w + xd, m->npc.npc[npc].y / m->map.tile_h, m->npc.npc[npc].l, dir) & COLL_TILE_IS_UNWALKABLE)
			return;
	}
	

	m->npc.npc[npc].moving = 1;
	m->npc.npc[npc].move_mode = mode;

	m->npc.npc[npc].mx = 0.0f;
	m->npc.npc[npc].my = 0.0f;
	m->npc.npc[npc].ox = m->npc.npc[npc].x;
	m->npc.npc[npc].oy = m->npc.npc[npc].y;
	m->npc.npc[npc].finished_moving = 0;
//	m->npc.npc[npc].ma = 0.0f;
//	m->npc.npc[npc].mb = 0.0f;


	darnitSpriteAnimationDisable(m->npc.npc[npc].sprite);
	npcHandleMovement(m, npc);

	return;
}


void npcHandleMovement(void *handle, unsigned int npc) {
	MAIN *m = handle;
	float speed, hspeed, vspeed;
	int dir;
	unsigned int time;
	NPC_ENTRY *npce = &m->npc.npc[npc];
	
	if (npce->moving == 0)
		return;


	if (m->npc.npc[npc].move_mode == NPC_MODE_WALKING)
		speed = NPC_WALKING;
	else if (m->npc.npc[npc].move_mode == NPC_MODE_RUNNING)
		speed = NPC_RUNNING;
	else {
		fprintf(stderr, "Error: Unhandled npc movement mode %i\n", m->npc.npc[npc].move_mode);
		return;
	}

	dir = npce->dir;
	hspeed = vspeed = 0.0f;

	if (dir & 0x1)
		speed *= NPC_WALKING_DIAG;
	

	if (dir > 0 && dir < 4)
		hspeed = speed;
	else if (dir > 4)
		hspeed = -speed;
	if (dir > 2 && dir < 6)
		vspeed = speed;
	else if (dir == 7 || dir == 0 || dir == 1)
		vspeed = -speed;
	

	
	time = darnitTimeLastFrameTook(m->darnit);

	npce->mx += hspeed * time;
	npce->my += vspeed * time;

	if (npce->mx >= m->map.tile_w) {
		npce->moving = 0;
		npce->mx = m->map.tile_w;
	} if (npce->my >= m->map.tile_h) {
		npce->moving = 0;
		npce->my = m->map.tile_h;
	} if (npce->mx <= (int)-m->map.tile_w) {
		npce->moving = 0;
		npce->mx = (int) -m->map.tile_w;
	} if (npce->my <= (int)-m->map.tile_h) {
		npce->moving = 0;
		npce->my = (int) -m->map.tile_h;
	}

	npce->x = npce->mx + npce->ox;
	npce->y = npce->my + npce->oy;


	if (npce->moving == 0) {
		darnitSpriteAnimationDisable(npce->sprite);
		m->npc.npc[npc].finished_moving = 1;
		(m->npc.npc[npc].npcHandler)(m, npc);

	}


	npcTeleport(m, npc, npce->x, npce->y, npce->l);

	return;
}


void npcDelete(void *handle, unsigned int npc) {
	MAIN *m = handle;

	m->npc.npc[npc].used = 0;
	npcDeleteSort(m, npc);
	darnitSpriteDelete(m, m->npc.npc[npc].sprite);

	return;
}


void npcClear(void *handle) {
	MAIN *m = handle;
	int i;

	for (i = 0; i < m->npc.npcs; i++) {
		m->npc.npc[i].used = 0;
		m->npc.sort[i].npc = -1;
		m->npc.sort[i].y = m->npc.sort[i].layer = 0;
	}
	
	npcReservationsDelete(m);

	return;
}


void npcKillAll(void *handle) {
	MAIN *m = handle;
	int i;

	for (i = 0; i < m->npc.npcs; i++)
		if (m->npc.npc[i].used == 1) 
			npcDelete(m, i);

	npcReservationsDelete(m);
	
	return;
}
