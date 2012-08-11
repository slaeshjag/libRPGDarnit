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
	m->system.fade_duration = atoi(darnitStringtableEntryGet(stringtable, "FADE_EFFECT_DURATION"));
	sscanf(darnitStringtableEntryGet(stringtable, "BOX_SELECTION_RED"), "%f", &m->system.box_selection_r);
	sscanf(darnitStringtableEntryGet(stringtable, "BOX_SELECTION_GREEN"), "%f", &m->system.box_selection_g);
	sscanf(darnitStringtableEntryGet(stringtable, "BOX_SELECTION_BLUE"), "%f", &m->system.box_selection_b);
	sscanf(darnitStringtableEntryGet(stringtable, "BOX_SELECTION_ALPHA"), "%f", &m->system.box_selection_a);

	m->system.inv_middle_bar_pos = atoi(darnitStringtableEntryGet(stringtable, "INV_MIDDLE_BAR_POS"));
	
	m->system.face_w = atoi(darnitStringtableEntryGet(stringtable, "FACE_W"));
	m->system.face_h = atoi(darnitStringtableEntryGet(stringtable, "FACE_H"));
	m->system.face_pad_h = atoi(darnitStringtableEntryGet(stringtable, "FACE_SPACE_H"));
	m->system.font_height = atoi(darnitStringtableEntryGet(stringtable, "FONT_HEIGHT"));

	if ((m->system.ts_ui_elements = darnitRenderTilesheetLoad(m->darnit, darnitStringtableEntryGet(stringtable, "UI_TILESET"), m->system.tile_w, m->system.tile_h, DARNIT_PFORMAT_RGB5A1)) == NULL) {
		fprintf(stderr, "Unable to load UI elements from %s\n", darnitStringtableEntryGet(stringtable, "UI_TILESET"));
		return -1;
	}

	m->system.inv_background = mapQuickload(m, darnitStringtableEntryGet(stringtable, "INV_BACKGROUND"));
	
	m->system.item_w = atoi(darnitStringtableEntryGet(stringtable, "ITEM_ICON_W"));
	m->system.item_h = atoi(darnitStringtableEntryGet(stringtable, "ITEM_ICON_H"));
	
	if ((m->item.ts_icon = darnitRenderTilesheetLoad(m->darnit, darnitStringtableEntryGet(stringtable, "ITEM_ICON_TILESET"), 
		m->system.item_w, m->system.item_h, DARNIT_PFORMAT_RGB5A1)) == NULL) {
			fprintf(stderr, "Unable to load item icons from %s\n", darnitStringtableEntryGet(stringtable, "ITEM_ICON_TILESET"));
			return -1;
	}

	if ((m->system.std_font = darnitFontLoad(m->darnit, darnitStringtableEntryGet(stringtable, "UI_FONT"), m->system.font_height, 512, 512)) == NULL) {
		fprintf(stderr, "Unable to load font %s\n", darnitStringtableEntryGet(stringtable, "UI_FONT"));
		return -1;
	}

	if ((m->map.mapfile_table = darnitStringtableOpen(darnitStringtableEntryGet(stringtable, "MAPFILE_TABLE"))) == NULL) {
		fprintf(stderr, "Unable to open map file table %s\n", darnitStringtableEntryGet(stringtable, "MAPFILE_TABLE"));
		return -1;
	}

	m->system.inventory_size = atoi(darnitStringtableEntryGet(stringtable, "INVENTORY_SIZE"));
	m->system.magic_cap = atoi(darnitStringtableEntryGet(stringtable, "ABILITY_CAP"));
	m->system.party_cap = atoi(darnitStringtableEntryGet(stringtable, "PARTY_CAP"));
	m->system.equip_slots = atoi(darnitStringtableEntryGet(stringtable, "PARTY_EQUIP_SLOTS"));

	if (itemInit(m, darnitStringtableEntryGet(stringtable, "ITEM_DB")) != 0);
	else if (partyInit(m, darnitStringtableEntryGet(stringtable, "PARTY_DB")) != 0);
	else {
		darnitStringtableClose(stringtable);
		return 0;
	}

	darnitStringtableClose(stringtable);
	return -1;

}

