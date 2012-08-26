#ifndef __INVENTORY_H__
#define	__INVENTORY_H__


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
