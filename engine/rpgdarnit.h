#ifndef __RPGDARNIT_H__
#define	__RPGDARNIT_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <darnit/darnit.h>


#include "map.h"
#include "camera.h"
#include "coll.h"
#include "npc.h"
#include "system.h"
#include "textbox.h"
#include "item.h"
#include "party.h"
#include "player.h"
#include "render.h"
#include "state.h"


typedef struct {
	DARNIT_KEYS	buttons;
	int		state;
	int		newstate;

	int		teleporter;
} VAR;


typedef struct {
	void		*darnit;
	int		state;
	int		newstate;

	CAMERA		cam;
	MAP		map;
	NPC		npc;
	PARTY		party;
	PLAYER		player;
	ITEM		item;
	TEXT_BOX	box;

	SYSTEM		system;
	VAR		var;
} MAIN;


#endif
