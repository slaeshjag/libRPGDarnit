#ifndef __NPC_H__
#define	__NPC_H__


#define				NPC_RESERVATION_SIZE	m->npc.npcs*4
#define				NPC_HANDLER_CAP		64

#define				NPC_DUMMY		0


#define				NPC_WALKING		0.1f
#define				NPC_RUNNING		0.1f
#define				NPC_WALKING_DIAG	0.7071f


#define				NPC_MODE_STATIONARY	0
#define				NPC_MODE_WALKING	1
#define				NPC_MODE_RUNNING	2



typedef struct {
	int			npc;
	unsigned int		y;
	unsigned int		layer;
} NPC_SORT;


typedef struct {
	unsigned int		x;
	unsigned int		y;
	unsigned int		l;
	unsigned int		moving;
	unsigned int		dir;
	unsigned int		used;
	unsigned int		activated;
	unsigned int		move_mode;
	unsigned int		inv_activated;
	float			mx;
	float 			my;
	unsigned int		ox;
	unsigned int		oy;
	unsigned int		finished_moving;
	unsigned int		init;
	unsigned int		textbox_ack;
	unsigned int		(*npcHandler)(void *handle, unsigned int npc);

	void			*sprite;
} NPC_ENTRY;


typedef struct {
	NPC_ENTRY		*npc;
	unsigned int		npcs;
	NPC_SORT		*sort;
	int			*reservation_list;
	void			*codelib;
	#ifdef __NPCLIB_HANDLER_GLOBAL__
		NPC_SAVE	*npcSaveData;
	#else
		void		*npcSaveData;
	#endif
} NPC;


void npcDrawLayer(void *handle, int layer);
int npcLimitSet(void *handle, unsigned int limit);
int npcInit(void *handle);
int npcGetOne(void *handle);
int npcSpawn(void *handle, int x, int y, int layer, const char *sprite, const char *logic_func);
void npcLoop(void *handle);
void npcReservationsDeleteMine(void *handle, unsigned int npc);
void npcReservationsDelete(void *handle);
void npcReserveTile(void *handle, unsigned int npc, unsigned int x, unsigned int y, unsigned int l);
int npcDirectionCalculate(int x, int y);
void npcHandleMovement(void *handle, unsigned int npc);
void npcTeleport(void *handle, unsigned int npc, unsigned int x, unsigned int y, unsigned int l);
void npcDelete(void *handle, unsigned int npc);
void npcClear(void *handle);
void npcKillAll(void *handle);
void npcMovementSetup(void *handle, unsigned int npc, unsigned int x, unsigned int y, int mode);
int npcGetTileInfront(void *handle, unsigned int npc);
void npcActivateTile(void *handle, int npc);
void npcInvActivateTile(void *handle, int npc);
void npcLoadCode(void *handle, const char *fname);


#endif
