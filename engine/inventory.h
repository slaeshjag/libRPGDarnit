#ifndef __INVENTORY_H__
#define	__INVENTORY_H__

#define			INVENTORY_MODE_MASK			0x7
#define			INVENTORY_MODE_SELECT			0x0
#define			INVENTORY_MODE_STATS			0x1
#define			INVENTORY_MODE_ITEMS			0x2
#define			INVENTORY_MODE_MAGIC			0x3
#define			INVENTORY_MODE_SAVE			0x4
#define			INVENTORY_MODE_QUIT			0x5


typedef struct {
	int				id;
	void				*stats_text;
	void				*face_cache;
} INVENTORY_MEMBER_STATS;


typedef struct {
	void				*background_map;
	void				*coins_str;

	void				*top_menu;
	void				*member_menu;
	void				*main_menu;
	void				*bottom_menu;
	void				*bottom_sel_count_menu;
	void				*mainscreen_icons;

	void				*mainscreen;
	void				*bottom_navbar;
	void				*info_bar;
	INVENTORY_MEMBER_STATS		*stat;
	int				stats;
	
	int				top_sel;
	int				bottom_sel;
	int				member_sel;
	int				member_target_sel;
	int				main_sel;
	int				sel_count;
	int				bottom_sel_count;
	int				bottom_sel_number;
	int				activate_eq_stat;
	int				mode;
	int				selchg;
	void				*item_icon;
	int				item_icons;
} INVENTORY_SCREEN;


int inventory(void *handle);
void inventorySet(void *handle);
void inventoryUnset(void *handle);


#endif
