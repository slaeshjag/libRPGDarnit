#ifndef __COLL_H__
#define __COLL_H__


#define			COLL_TILE_IS_SOLID		0x1000000
#define			COLL_TILE_IS_TELEPORTER		0x10000000
#define			COLL_TILE_IS_TRIGGER		0x40000000
#define			COLL_TILE_IS_SLIPPERY		0x100000
#define			COLL_TILE_IS_THICK		0x200000
#define			COLL_TILE_IS_NPC		0x2000000
#define			COLL_TILE_IS_UNWALKABLE		0x4000000


unsigned int collTest(void *handle, unsigned int tilex, unsigned int tiley, unsigned int tilel, int dir);


#endif
