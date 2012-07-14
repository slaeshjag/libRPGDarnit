#ifndef __RPGDARNIT_H__
#define	__RPGDARNIT_H__

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <darnit/darnit.h>

#include "map.h"
#include "camera.h"
#include "coll.h"
#include "npc.h"

typedef struct {
	void		*darnit;

	CAMERA		cam;
	MAP		map;
	NPC		npc;
} MAIN;


#endif
