#include "rpgdarnit.h"

#define		TEXTBOX_HEIGHT			(m->system.textbox_height * m->system.tile_h)
#define		TEXTBOX_TEXT_H_ADJUST		(m->system.textbox_pad_h * 2 + m->system.face_w)
#define		TEXTBOX_TEXT_V_ADJUST		(m->cam.screen_h - TEXTBOX_HEIGHT + m->system.textbox_pad_v)
#define		TEXTBOX_GLYPHS			((m->box.text_width * m->system.textbox_lines) >> 1)
#define		TEXTBOX_TILE_Y			(m->cam.screen_h - m->system.tile_h * m->system.textbox_height)
#define		TEXTBOX_FACE_V_ADJUST		(m->cam.screen_h + ((TEXTBOX_HEIGHT - m->system.face_h) >> 1) - TEXTBOX_HEIGHT)
#define		TEXTBOX_SELECT_X		(m->cam.screen_w - m->system.box_selection_w * m->system.tile_w)
#define		TEXTBOX_SELECT_Y		(TEXTBOX_TILE_Y - m->system.box_selection_h * m->system.tile_h)
#define		TEXTBOX_MENU_X			(TEXTBOX_SELECT_X + m->system.textbox_pad_h)
#define		TEXTBOX_MENU_Y			(TEXTBOX_SELECT_Y + m->system.textbox_pad_v)
#define		TEXTBOX_MENU_W			(m->system.box_selection_w * m->system.tile_w - 2*m->system.textbox_pad_h)


int textboxDecideTile(int w, int h, int x, int y, int border_bottom) {
	if (x == 0) {
		if (y == 0)
			return TEXT_BOX_TILE_CORNER_0;
		if (y + 1 == h)
			return (border_bottom) ? TEXT_BOX_TILE_CORNER_1 : TEXT_BOX_TILE_SIDE_0;
		return TEXT_BOX_TILE_SIDE_0;
	} 
	
	if (x + 1 == w) {
		if (y == 0)
			return TEXT_BOX_TILE_CORNER_3;
		if (y + 1 == h)
			return (border_bottom) ? TEXT_BOX_TILE_CORNER_2 : TEXT_BOX_TILE_SIDE_2;
		return TEXT_BOX_TILE_SIDE_2;
	}

	if (y == 0)
		return TEXT_BOX_TILE_SIDE_3;
	if (y + 1 == h)
		return (border_bottom) ? TEXT_BOX_TILE_SIDE_1 : TEXT_BOX_TILE_CENTER;
	return TEXT_BOX_TILE_CENTER;
}


int textboxInit(void *handle) {
	MAIN *m = handle;
	int h, i, j, k;

	m->box.visible = 0;
	m->box.str = NULL;
	m->box.ans_str = NULL;
	m->box.strlen = 0;
	m->box.pos = 0;
	m->box.speed = m->system.default_text_speed;
	m->box.npcCallback = 0;
	m->box.time_elapsed_rest = 0;
	m->box.text_width = m->cam.screen_w - (m->system.textbox_pad_h * 3 + m->system.face_w);
	
	if ((m->box.text = darnitTextSurfaceAlloc(m->system.std_font, TEXTBOX_GLYPHS, m->box.text_width, TEXTBOX_TEXT_H_ADJUST, TEXTBOX_TEXT_V_ADJUST)) == NULL) {
		fprintf(stderr, "Unable to allocate test buffer for text box\n");
		return -1;
	}

	m->box.tiles = m->cam.screen_w / m->system.tile_w * m->system.textbox_height;
	if ((m->box.tile = darnitRenderTileAlloc(m->box.tiles)) == NULL) {
		fprintf(stderr, "Unable to allocate tile buffer for text box background\n");
		return -1;
	}

	m->box.menu_tiles = m->system.box_selection_w * m->system.box_selection_h;
	if ((m->box.menu_tile = darnitRenderTileAlloc(m->box.menu_tiles)) == NULL) {
		fprintf(stderr, "Unable to allocate tile buffer for text box selection background\n");
		return -1;
	}

	if ((m->box.face_c = darnitRenderTileAlloc(1)) == NULL) {
		fprintf(stderr, "Unable to allocate face tile for text box\n");
		return -1;
	}

	m->box.face_ts = NULL;
	m->box.menu = NULL;
	m->box.selection = -1;
	m->box.text_w_pos = 0;

	for (j = k = 0; j < m->system.textbox_height; j++) {
		for (i = 0; i < m->cam.screen_w / m->system.tile_w; i++, k++) {
			h = textboxDecideTile(m->cam.screen_w / m->system.tile_w, m->system.textbox_height, i, j, 1);
			darnitRenderTileMove(m->box.tile, k, m->system.ts_ui_elements, i * m->system.tile_w, j * m->system.tile_h + TEXTBOX_TILE_Y);
			darnitRenderTileSet(m->box.tile, k, m->system.ts_ui_elements, h);
		}
	}

	for (j = k = 0; j < m->system.box_selection_h; j++) {
		for (i = 0; i < m->system.box_selection_w; i++, k++) {
			h = textboxDecideTile(m->system.box_selection_w, m->system.box_selection_h, i, j, 0);
			darnitRenderTileMove(m->box.menu_tile, k, m->system.ts_ui_elements, i * m->system.tile_w + TEXTBOX_SELECT_X, j * m->system.tile_h + TEXTBOX_SELECT_Y);
			darnitRenderTileSet(m->box.menu_tile, k, m->system.ts_ui_elements, h);
		}
	}

	return 0;
}


int textboxActivate(void *handle, const char *str, int npcCallback, int scroll, int face, void *face_ts, const char *q_str) {
	MAIN *m = handle;

	m->box.visible = 1;
	m->box.str = str;
	m->box.ans_str = q_str;
	m->box.strlen = strlen(str);
	m->box.pos = 0;
	m->box.speed = m->system.default_text_speed;
	m->box.scroll = scroll;
	m->box.npcCallback = npcCallback;
	m->box.time_elapsed_rest = 0;
	m->box.text_w_pos = 0;
	m->box.selection = -1;

	darnitTextSurfaceReset(m->box.text);
	darnitRenderTileSet(m->box.tile, m->box.tiles-1, m->system.ts_ui_elements, (scroll) ? TEXT_BOX_TILE_SCROLL : TEXT_BOX_TILE_CORNER_2);

	m->box.face_ts = face_ts;

	if (q_str != NULL) {
		m->box.menu = darnitMenuVerticalCreate(m->darnit, q_str, TEXTBOX_MENU_X, TEXTBOX_MENU_Y, m->system.std_font, TEXTBOX_MENU_W, m->system.box_selection_text_pad, 3);
		darnitMenuShadeColorSet(m->box.menu, m->system.box_selection_r, m->system.box_selection_g, m->system.box_selection_b, m->system.box_selection_a);
	}

	if (face_ts != NULL) {
		darnitRenderTileMove(m->box.tile, 0, m->system.ts_ui_elements, m->system.textbox_pad_h, TEXTBOX_TILE_Y + m->system.textbox_pad_v);
		darnitRenderTileSet(m->box.face_c, 0, m->box.face_ts, face);
	}

	return 0;
}


void textboxClose(void *handle) {
	MAIN *m = handle;

	m->box.visible = 0;
	if (m->box.menu != NULL)
		m->box.menu = darnitMenuDestroy(m->box.menu);
	if (m->box.npcCallback > -1) 
		(m->npc.npc[m->box.npcCallback].npcHandler)(m, m->box.npcCallback);
	return;
}


void textboxAnimate(void *handle) {
	MAIN *m = handle;

	if (m->box.pos == m->box.strlen)
		return;
	
	m->box.time_elapsed_rest += darnitTimeLastFrameTook(m->darnit);
	while (m->box.time_elapsed_rest >= m->box.speed) {
		if (m->box.str[m->box.pos] == ' ') {	/* TODO: Add word-wrapping */
			m->box.pos += darnitTextSurfaceCharAppend(m->box.text, " ");
			m->box.text_w_pos = darnitFontGetGlyphW(m->system.std_font, &m->box.str[m->box.pos]);
		} else if (m->box.str[m->box.pos] == '\n') {
			m->box.pos += darnitTextSurfaceCharAppend(m->box.text, "\n");
			m->box.text_w_pos = 0;
		} else {
			m->box.text_w_pos = darnitFontGetGlyphW(m->system.std_font, &m->box.str[m->box.pos]);
			m->box.pos += darnitTextSurfaceCharAppend(m->box.text, &m->box.str[m->box.pos]);
		}
		m->box.time_elapsed_rest -= m->box.speed;
	}

	return;
}


int textboxLoop(void *handle) {
	MAIN *m = handle;
	DARNIT_KEYS buttons = darnitButtonZero();;

	if (m->box.visible == 0)
		return 0;
	
	textboxAnimate(m);
	
	darnitRenderTileDraw(m->box.tile, m->system.ts_ui_elements, m->box.tiles);
	if (m->box.menu != NULL)
		darnitRenderTileDraw(m->box.menu_tile, m->system.ts_ui_elements, m->box.menu_tiles);
	
	darnitRenderBlendingEnable(m->darnit);
	darnitTextSurfaceDraw(m->box.text);
	if (m->box.menu != NULL)
		if ((m->box.selection = darnitMenuHandle(m->darnit, m->box.menu)) != -1)
			textboxClose(m);
	darnitRenderBlendingDisable(m->darnit);

	if (m->var.buttons.BUTTON_ACCEPT) {
		m->var.buttons.BUTTON_ACCEPT = 0;
		buttons.BUTTON_ACCEPT = 1;
		darnitButtonSet(m->darnit, buttons);
		textboxClose(m);
	} else if (m->var.buttons.BUTTON_CANCEL) {
		m->var.buttons.BUTTON_CANCEL = 0;
		buttons.BUTTON_CANCEL = 1;
		darnitButtonSet(m->darnit, buttons);
		textboxClose(m);
	}
	
	return 0;
}
