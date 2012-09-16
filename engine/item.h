#ifndef __ITEM_H__
#define	__ITEM_H__


#define			ITEM_HEAL_HP		0x1
#define			ITEM_HEAL_MP		0x2
#define			ITEM_CLEAR_STATUS	0x4
#define			ITEM_DAMAGE		0x8
#define			ITEM_REVIVE		0x9
#define			ITEM_BITMASK_START	0xA


typedef struct {
	unsigned int			item_class;
	int				effect;
	unsigned int			max_stack;
	int				hp;
	int				hpbyuse;
	int				spread;
	int				hp_gain;
	int				mp_gain;
	float				spread_factor;
	char				animation[32];
	char				sound[32];
	char				name[32];
	char				descr[64];
	float				attboost;
	float				defboost;
	float				specaboost;
	float				specdboost;
	float				speedboost;
	int				resistance;
	float				resistance_factor;
} ITEM_PROTOTYPE;


typedef struct {
	int				id;
	int				amount;
	int				damage;
	int				equipped;
} ITEM_ENTRY;


typedef struct {
	ITEM_PROTOTYPE			*item;
	unsigned int			items;
	void				*ts_icon;
} ITEM;


int itemInit(void *handle, const char *fname);
int itemItemsInInventory(void *handle, ITEM_ENTRY *item);
int itemGiveToParty(void *handle, unsigned int item, unsigned int amount);
void itemDeleteFromInventory(void *handle, ITEM_ENTRY *item, int item_slot);
void itemTossFromInventory(void *handle, ITEM_ENTRY *item, int item_type, int slot, int amount);
int itemAddToInventory(void *handle, ITEM_ENTRY *item, int item_type, int amount);
int itemUseOnPartyMember(void *handle, void *srcp, void *dstp, int item_type);


#endif
