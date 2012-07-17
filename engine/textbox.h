#ifndef __TEXTBOX_H__
#define	__TEXTBOX_H__


#define				TEXT_BOX_TILE_CORNER_0			0
#define				TEXT_BOX_TILE_SIDE_0			3
#define				TEXT_BOX_TILE_CORNER_1			6
#define				TEXT_BOX_TILE_SIDE_1			7
#define				TEXT_BOX_TILE_CORNER_2			8
#define				TEXT_BOX_TILE_SIDE_2			5
#define				TEXT_BOX_TILE_CORNER_3			2
#define				TEXT_BOX_TILE_SIDE_3			1
#define				TEXT_BOX_TILE_CENTER			4
#define				TEXT_BOX_TILE_SCROLL			9


typedef struct {
	int			visible;
	const char		*str;
	const char		*ans_str;
	int			strlen;
	int			pos;
	int			speed;
	int			scroll;
	int			npcCallback;
	int			time_elapsed_rest;
	void			*text;
	void			*tile;
	int			tiles;
	void			*face_c;
	
	void			*face_ts;
	void			*menu;
	void			*menu_tile;
	int			menu_tiles;
	int			selection;

	int			text_width;
	int			text_w_pos;
} TEXT_BOX;


int textboxInit(void *handle);
int textboxActivate(void *handle, const char *str, int npcCallback, int scroll, int face, void *face_ts, const char *q_str);
int textboxLoop(void *handle);



#endif

