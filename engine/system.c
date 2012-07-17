#include "rpgdarnit.h"


int systemInit(void *handle) {
	MAIN *m = handle;
	void *stringtable;

	if ((stringtable = darnitStringtableOpen(CONFIGURATION_PATH)) == NULL) {
		fprintf(stderr, "Unable to load configuration from %s\n", CONFIGURATION_PATH);
		return -1;
	}

	darnitStringtableSectionLoad(stringtable, "CONFIG");
	
	m->system.default_text_speed = atoi(darnitStringtableEntryGet(stringtable, "DEFAULT_TEXT_SPEED"));
	m->system.tile_w = atoi(darnitStringtableEntryGet(stringtable, "TILE_W"));
	m->system.tile_h = atoi(darnitStringtableEntryGet(stringtable, "TILE_H"));
	m->system.textbox_height = atoi(darnitStringtableEntryGet(stringtable, "TEXTBOX_HEIGHT"));
	m->system.textbox_lines = atoi(darnitStringtableEntryGet(stringtable, "TEXTBOX_LINES"));
	m->system.textbox_pad_v = atoi(darnitStringtableEntryGet(stringtable, "TEXTBOX_PAD_VERT"));
	m->system.textbox_pad_h = atoi(darnitStringtableEntryGet(stringtable, "TEXTBOX_PAD_HORI"));

	m->system.box_selection_w = atoi(darnitStringtableEntryGet(stringtable, "BOX_SELECTION_W"));
	m->system.box_selection_h = atoi(darnitStringtableEntryGet(stringtable, "BOX_SELECTION_H"));
	m->system.box_selection_text_pad = atoi(darnitStringtableEntryGet(stringtable, "BOX_SELECTION_TEXT_PAD"));
	m->system.box_selection_scroll = atoi(darnitStringtableEntryGet(stringtable, "BOX_SELECTION_SCROLL"));
	sscanf(darnitStringtableEntryGet(stringtable, "BOX_SELECTION_RED"), "%f", &m->system.box_selection_r);
	sscanf(darnitStringtableEntryGet(stringtable, "BOX_SELECTION_GREED"), "%f", &m->system.box_selection_g);
	sscanf(darnitStringtableEntryGet(stringtable, "BOX_SELECTION_BLUE"), "%f", &m->system.box_selection_b);
	sscanf(darnitStringtableEntryGet(stringtable, "BOX_SELECTION_ALPHA"), "%f", &m->system.box_selection_a);

	m->system.face_w = atoi(darnitStringtableEntryGet(stringtable, "FACE_W"));
	m->system.face_h = atoi(darnitStringtableEntryGet(stringtable, "FACE_H"));
	m->system.font_height = atoi(darnitStringtableEntryGet(stringtable, "FONT_HEIGHT"));

	if ((m->system.ts_ui_elements = darnitRenderTilesheetLoad(m->darnit, darnitStringtableEntryGet(stringtable, "UI_TILESET"), m->system.tile_w, m->system.tile_h, DARNIT_TILESHEET_FORMAT_RGBA)) == NULL) {
		fprintf(stderr, "Unable to load UI elements from %s\n", darnitStringtableEntryGet(stringtable, "UI_TILESET"));
		return -1;
	}

	if ((m->system.std_font = darnitFontLoad(m->darnit, darnitStringtableEntryGet(stringtable, "UI_FONT"), m->system.font_height, 512, 512)) == NULL) {
		fprintf(stderr, "Unable to load font %s\n", darnitStringtableEntryGet(stringtable, "UI_FONT"));
		return -1;
	}

	return 0;
}

