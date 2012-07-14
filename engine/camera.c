#include		"rpgdarnit.h"



int cameraInit(void *handle) {
	MAIN *m = handle;
	DARNIT_PLATFORM platform;

	platform = darnitPlatformGet(m->darnit);

	m->cam.npc_to_follow = -1;
	m->cam.x = 0;
	m->cam.y = 0;
	m->cam.speed = 0.3f;
	m->cam.xrest = 0.0f;
	m->cam.yrest = 0.0f;
	m->cam.move = 0;
	m->cam.movetox = 0;
	m->cam.movetoy = 0;
	m->cam.jump = 0;
	m->cam.tpr = 0;
	m->cam.screen_w = platform.screen_w;
	m->cam.screen_h = platform.screen_h;
	
	return 0;
}


int cameraReset(void *handle) {
	MAIN *m = handle;

	m->cam.x = 0;
	m->cam.y = 0;

	m->cam.jump = 0;

	return 0;
}


int cameraJump(void *handle) {
	MAIN *m = handle;

	m->cam.jump = 1;

	return 0;
}


int cameraCalcXt(void *handle) {
	MAIN *m = handle;
	int a;

	if (m->cam.npc_to_follow == -1)
		return m->cam.x;
	a = m->npc.npc[m->cam.npc_to_follow].x;
	a += m->map.tile_w >> 1;
	a -= m->cam.screen_w >> 1;
	if (a < 0)
		a = 0;
	if (a > (m->map.w * m->map.tile_w - m->cam.screen_w))
		a = m->map.w * m->map.tile_w - m->cam.screen_w;
	
	return a;
}


int cameraCalcYt(void *handle) {
	MAIN *m = handle;
	int a;

	a = m->npc.npc[m->cam.npc_to_follow].y;
	a -= (m->cam.screen_h - m->map.tile_h) >> 1;
	if (a < 0)
		a = 0;
	if (a > (m->map.h * m->map.tile_h - m->cam.screen_h))
		a = m->map.h * m->map.tile_h - m->cam.screen_h;
	
	return a;
}

void cameraFollowNPC(void *handle, int npc) {
	MAIN *m = handle;

	m->cam.npc_to_follow = npc;
	m->cam.jump = 0;
	m->cam.move = 0;

	return;
}


void cameraSet(void *handle, int x, int y) {
	MAIN *m = handle;

	m->cam.x = x;
	m->cam.y = y;
	m->cam.npc_to_follow = -1;

	return;
}



int cameraLoop(void *handle) {
	MAIN *m = handle;
	int tx, ty, xd, yd, tp;
	float angle, xt, yt;

	tx = ty = 0;

	if (m->cam.npc_to_follow == -1 && m->cam.move == 0)			// Don't bother...
		return 0;
	
	if (m->npc.npc[m->cam.npc_to_follow].used == 0 && m->cam.move == 0)	// Don't bother...
		return 0;


	if (m->cam.move == 0) {
		tx = cameraCalcXt(m);
		ty = cameraCalcYt(m);
	} else {
		tx = m->cam.movetox;
		ty = m->cam.movetoy;
	}

	xd = tx - m->cam.x;
	yd = ty - m->cam.y;

	if (xd == 0 && yd == 0)
		return 0;
	
	if (m->cam.jump == 1) {
		m->cam.jump = 0;
		m->cam.x = tx;
		m->cam.y = ty;
		return 0;
	}
	
	tp = m->cam.speed * darnitTimeLastFrameTook(m->darnit);
	
        if (yd*yd + xd*xd < tp*tp) { // om det är väldigt nära till målet
		m->cam.x = tx;    // hoppa direkt dit
		m->cam.y = ty;
	} else {                     // annars high tech math shit yo
		if (yd == 0)
			angle = 0;
		else if (xd == 0)
			angle = M_PI_2;
		else
			angle = atanf(fabsf((float) yd) / abs(xd));
		yt = sinf(angle) * (float)tp;
		if (yd < 0)
			yt *= -1.0f;
		xt = cosf(angle) * (float)tp;
		if (xd < 0)
			xt *= -1.0f;

		m->cam.x += (int) xt, m->cam.y += (int) yt;
	}


	return 0;
}
