#ifndef __RPGDARNIT_H__
#define	__RPGDARNIT_H__

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <darnit/darnit.h>

#include "map.h"
#include "camera.h"

typedef struct {
	void		*darnit;

	CAMERA		cam;
	MAP		map;
} MAIN;


#endif
