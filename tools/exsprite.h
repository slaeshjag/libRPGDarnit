#ifndef __SPRITE_H__
#define	__SPRITE_H__


typedef struct {
	unsigned int		time;
	unsigned int		tile;
} SPRITE_TILE;


typedef struct {
	SPRITE_TILE		tile[128];
	unsigned int		tiles;
} SPRITE_DIRECTION;


typedef struct {
	unsigned int		dir;
	unsigned int		frame;
	unsigned int		tleft;
	unsigned int		time;
	unsigned int		animate;
	SPRITE_DIRECTION	spr[8];
} SPRITE_ENTRY;


typedef struct {
	SPRITE_ENTRY		*sprite;
	unsigned int		sprites;
} SPRITE;

#endif
