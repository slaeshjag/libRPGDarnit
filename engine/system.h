#ifndef __SYSTEM_H__
#define	__SYSTEM_H__

#define		CONFIGURATION_PATH		"bin/system.stz"


typedef struct {
	const char		*name;

	int			default_text_speed;
	int			tile_w;
	int			tile_h;
	int			textbox_height;
	int			textbox_lines;
	int			textbox_pad_v;
	int			textbox_pad_h;

	int			box_selection_w;
	int			box_selection_h;
	int			box_selection_text_pad;
	int			box_selection_scroll;
	float			box_selection_r;
	float			box_selection_b;
	float			box_selection_g;
	float			box_selection_a;

	int			face_w;
	int			face_h;

	void			*std_font;
	int			font_height;
	void			*ts_ui_elements;

	int			item_w;
	int			item_h;

	int			inventory_size;
	int			magic_cap;
	int			party_cap;
} SYSTEM;


int systemInit(void *handle);


#endif
