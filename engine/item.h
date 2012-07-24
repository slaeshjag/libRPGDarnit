#ifndef __ITEM_H__
#define	__ITEM_H__



typedef struct {
	unsigned int			item_class;
	unsigned int			effect;
	unsigned int			max_stack;
	unsigned int			hp;
	unsigned int			hpbyuse;
	unsigned int			spread;
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


#endif
