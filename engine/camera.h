#ifndef __CAMERA_H__
#define	__CAMERA_H__

typedef struct {
	int		npc_to_follow;
	int		x;
	int		y;
	float		speed;
	float		xrest;
	float		yrest;
	int		move;
	int 		movetox;
	int		movetoy;
	int		jump;
	int		tpr;

	int		screen_w;
	int		screen_h;
} CAMERA;

int cameraInit(void *handle);
int cameraReset(void *handle);
int cameraJump(void *handle);
void cameraFollowNPC(void *handle, int npc);
void cameraSet(void *handle, int x, int y);
int cameraLoop(void *handle);


#endif
