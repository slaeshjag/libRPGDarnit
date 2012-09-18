#include "rpgdarnit.h"


void inventoryModeSelect(void *handle) {
	MAIN *m = handle;
	int ret;

	if ((ret = darnitMenuHandle(m->darnit, m->var.inventory.top_menu)) == -1)
		return;

	ret++;
	if (ret == -1)
		ret = 0;
	m->var.inventory.mode = ret;

	switch (ret) {
		case INVENTORY_MODE_SELECT:
			m->var.newstate = STATE_OVERWORLD;
			break;
		case INVENTORY_MODE_STATS:
		case INVENTORY_MODE_ITEMS:
			darnitMenuSelectionWaitForNew(m->var.inventory.member_menu);
			break;
		default:
			m->var.inventory.mode = INVENTORY_MODE_SELECT;
			darnitMenuSelectionWaitForNew(m->var.inventory.top_menu);
			break;


	}


	return;
}

void inventoryModeStats(void *handle) {
	MAIN *m = handle;
	void *mainscreen = m->var.inventory.mainscreen;
	char num[9];
	int ret;

	darnitMenuHandle(m->darnit, m->var.inventory.top_menu);

	if (darnitMenuChangedCheck(m->var.inventory.member_menu)) {
		m->var.inventory.member_sel = darnitMenuPeek(m->var.inventory.member_menu, NULL);
		/* Okej, det här blir fult, men jag är osäker på hur jag annars ska göra det :< */

		darnitTextSurfaceReset(mainscreen);
		darnitTextSurfaceStringAppend(mainscreen, darnitStringtableEntryGet(m->system.language, "INVENTORY_STATS_FOR"));
		darnitTextSurfaceStringAppend(mainscreen, m->party.skel[m->party.member[m->var.inventory.member_sel].id].name);
		darnitTextSurfaceStringAppend(mainscreen, "\n\n");

		darnitTextSurfaceStringAppend(mainscreen, darnitStringtableEntryGet(m->system.language, "ATTACK"));
		darnitTextSurfaceXposSet(mainscreen, m->system.inv_number_pos);
		sprintf(num, "%i\n", m->party.member[m->var.inventory.member_sel].att);
		darnitTextSurfaceStringAppend(mainscreen, num);

		darnitTextSurfaceStringAppend(mainscreen, darnitStringtableEntryGet(m->system.language, "DEFENSE"));
		darnitTextSurfaceXposSet(mainscreen, m->system.inv_number_pos);
		sprintf(num, "%i\n", m->party.member[m->var.inventory.member_sel].def);
		darnitTextSurfaceStringAppend(mainscreen, num);

		darnitTextSurfaceStringAppend(mainscreen, darnitStringtableEntryGet(m->system.language, "SPEC_ATTACK"));
		darnitTextSurfaceXposSet(mainscreen, m->system.inv_number_pos);
		sprintf(num, "%i\n", m->party.member[m->var.inventory.member_sel].speca);
		darnitTextSurfaceStringAppend(mainscreen, num);

		darnitTextSurfaceStringAppend(mainscreen, darnitStringtableEntryGet(m->system.language, "SPEC_DEFENCE"));
		darnitTextSurfaceXposSet(mainscreen, m->system.inv_number_pos);
		sprintf(num, "%i\n", m->party.member[m->var.inventory.member_sel].specd);
		darnitTextSurfaceStringAppend(mainscreen, num);

		darnitTextSurfaceStringAppend(mainscreen, darnitStringtableEntryGet(m->system.language, "AGILITY"));
		darnitTextSurfaceXposSet(mainscreen, m->system.inv_number_pos);
		sprintf(num, "%i\n", m->party.member[m->var.inventory.member_sel].speed);
		darnitTextSurfaceStringAppend(mainscreen, num);
	}

		

	if ((ret = darnitMenuHandle(m->darnit, m->var.inventory.member_menu)) == -1)
		return;
	
	if (ret == -2) {
		m->var.inventory.mode = INVENTORY_MODE_SELECT;
		darnitTextSurfaceReset(m->var.inventory.mainscreen);
		darnitMenuSelectionWaitForNew(m->var.inventory.top_menu);
	} else
		darnitMenuSelectionWaitForNew(m->var.inventory.member_menu);
	


	return;
}

void inventoryModeItems(void *handle) {
	MAIN *m = handle;
	void *mainscreen = m->var.inventory.mainscreen;
	int ret, i, items, item, x, y, w, h, main_top_sel;
	char num[9];

	darnitMenuHandle(m->darnit, m->var.inventory.top_menu);
	
	/* Ugh. */
	
	if (darnitMenuChangedCheck(m->var.inventory.member_menu) || darnitMenuChangedCheck(m->var.inventory.main_menu)) {
		m->var.inventory.member_sel = darnitMenuPeek(m->var.inventory.member_menu, NULL);
		if (m->var.inventory.main_menu)
			m->var.inventory.main_sel = darnitMenuPeek(m->var.inventory.main_menu, &main_top_sel);
		else
			main_top_sel = m->var.inventory.main_sel = 0;

		darnitTextSurfaceReset(mainscreen);
		items = itemItemsInInventory(handle, m->party.member[m->var.inventory.member_sel].inventory);
		for (i = main_top_sel; i < m->system.inv_mainscreen_rows + main_top_sel; i++) {
			if (i >= items)
				break;
			darnitTextSurfaceSkip(mainscreen, m->system.inv_item_text_skip);
			item = m->party.member[m->var.inventory.member_sel].inventory[i].id;
			darnitTextSurfaceStringAppend(mainscreen, m->item.item[item].name);
			if (m->item.item[item].max_stack > 1) {
				darnitTextSurfaceXposSet(mainscreen, m->system.inv_item_number_pos);
				darnitTextSurfaceStringAppend(mainscreen, darnitStringtableEntryGet(m->system.language, "AMOUNT_SYMBOL"));
				sprintf(num, "%.2i", m->party.member[m->var.inventory.member_sel].inventory[i].amount);
				darnitTextSurfaceStringAppend(mainscreen, num);
			}
			darnitTextSurfaceStringAppend(mainscreen, "\n");
			darnitRenderTileSet(m->var.inventory.item_icon, i - main_top_sel, m->item.ts_icon, item);
		}

		for (; i < m->system.inv_mainscreen_rows + main_top_sel; i++)
			darnitRenderTileSetTilesheetCoord(m->var.inventory.item_icon, i - main_top_sel, m->item.ts_icon, 0, 0, 0, 0);

		if (m->var.inventory.main_menu) {
			darnitTextSurfaceReset(m->var.inventory.info_bar);
			item = m->party.member[m->var.inventory.member_sel].inventory[m->var.inventory.main_sel].id;
			darnitTextSurfaceStringAppend(m->var.inventory.info_bar, m->item.item[item].descr);
		}
	}

	if ((ret = darnitMenuHandle(m->darnit, m->var.inventory.member_menu)) == -1)
		return;
	if (ret == -2) {
		m->var.inventory.mode = INVENTORY_MODE_SELECT;
		darnitTextSurfaceReset(m->var.inventory.mainscreen);
		darnitMenuSelectionWaitForNew(m->var.inventory.top_menu);
		m->var.inventory.main_menu = darnitMenuDestroy(m->var.inventory.main_menu);
		return;
	} else {
		if (m->var.inventory.main_menu == NULL) {
			items = itemItemsInInventory(handle, m->party.member[m->var.inventory.member_sel].inventory);

			if (items <= 0) {
				darnitMenuSelectionWaitForNew(m->var.inventory.member_menu);
				return;
			}

			x = m->system.inv_middle_bar_pos * m->system.tile_w + m->system.textbox_pad_h;
			y = m->system.tile_h + (m->system.tile_h >> 1);
			w = m->cam.screen_w - x - m->system.textbox_pad_h;
			h = darnitFontGetGlyphHS(m->system.std_font);
			m->var.inventory.main_menu = darnitMenuVerticalShadeCreate(m->darnit, x, y, w, h, h, items, -1, m->system.inv_mainscreen_rows);
		} else {
			if ((ret = darnitMenuHandle(m->darnit, m->var.inventory.main_menu)) == -1)
				return;
			else if (ret == -2) {
				m->var.inventory.main_menu = darnitMenuDestroy(m->var.inventory.main_menu);
				darnitTextSurfaceReset(m->var.inventory.info_bar);
				darnitMenuSelectionWaitForNew(m->var.inventory.member_menu);
			}
		}
	}


	return;
}

void inventoryModeMagic(void *handle) {
	return;
}

void inventoryModeSave(void *handle) {
	return;
}

void inventoryModeQuit(void *handle) {
	return;
}

void inventoryModeBad(void *handle) {
	return;
}



void inventoryMenuLogic(void *handle) {
	MAIN *m = handle;

	switch (m->var.inventory.mode & INVENTORY_MODE_MASK) {
		case INVENTORY_MODE_SELECT:
			inventoryModeSelect(m);
			break;
		case INVENTORY_MODE_STATS:
			inventoryModeStats(m);
			break;
		case INVENTORY_MODE_ITEMS:
			inventoryModeItems(m);
			break;
		case INVENTORY_MODE_MAGIC:
			inventoryModeMagic(m);
			break;
		case INVENTORY_MODE_SAVE:
			inventoryModeSave(m);
			break;
		case INVENTORY_MODE_QUIT:
			inventoryModeQuit(m);
			break;
		default:
			inventoryModeBad(m);
	}

	return;
}


void inventoryStatSet(void *handle) {
	MAIN *m = handle;
	char num[10];
	int i, num_w, top_sel = 0;

	for (i = 0; i < m->var.inventory.stats; i++) {
		m->var.inventory.stat[i].id = -1;
		darnitTextSurfaceReset(m->var.inventory.stat[i].stats_text);
	}

	for (i = 0; i < m->var.inventory.stats; i++) {
		darnitMenuPeek(m->var.inventory.member_menu, &top_sel);
		if (i + top_sel >= m->party.members)
			break;

		m->var.inventory.stat[i].id = m->party.member[i + top_sel].id;
		darnitTextSurfaceStringAppend(m->var.inventory.stat[i].stats_text, m->party.skel[m->party.member[i + top_sel].id].name);
		darnitTextSurfaceStringAppend(m->var.inventory.stat[i].stats_text, "\n");
		
		darnitTextSurfaceStringAppend(m->var.inventory.stat[i].stats_text, darnitStringtableEntryGet(m->system.language, "HP"));
		sprintf(num, "%i", m->party.member[top_sel + i].hp);
		num_w = m->system.inv_number_space - darnitFontGetStringWidthPixels(m->system.std_font, num);
		darnitTextSurfaceXposSet(m->var.inventory.stat[i].stats_text, num_w);
		darnitTextSurfaceStringAppend(m->var.inventory.stat[i].stats_text, num);
		darnitTextSurfaceStringAppend(m->var.inventory.stat[i].stats_text, "/");
		sprintf(num, "%i", m->party.member[top_sel + i].hp_max);
		darnitTextSurfaceStringAppend(m->var.inventory.stat[i].stats_text, num);
		darnitTextSurfaceStringAppend(m->var.inventory.stat[i].stats_text, "\n");
		
		darnitTextSurfaceStringAppend(m->var.inventory.stat[i].stats_text, darnitStringtableEntryGet(m->system.language, "MP"));
		sprintf(num, "%i", m->party.member[top_sel + i].mp);
		num_w = m->system.inv_number_space - darnitFontGetStringWidthPixels(m->system.std_font, num);
		darnitTextSurfaceXposSet(m->var.inventory.stat[i].stats_text, num_w);
		darnitTextSurfaceStringAppend(m->var.inventory.stat[i].stats_text, num);
		darnitTextSurfaceStringAppend(m->var.inventory.stat[i].stats_text, "/");
		sprintf(num, "%i", m->party.member[top_sel + i].mp_max);
		darnitTextSurfaceStringAppend(m->var.inventory.stat[i].stats_text, num);

		darnitRenderTileSet(m->var.inventory.stat[i].face_cache, 0, m->party.party_face_ts, m->party.member[i + top_sel].id);

	}

	return;
}


void inventorySet(void *handle) {
	MAIN *m = handle;
	char num[64];
	int i, x, y, sel_h, pad_2_y, s, t, w, h;
	void *ts = m->party.party_face_ts;

	/* Initiera antalet medlemmar i gruppen, behövs för menyn */
	for (i = 0; i < m->system.party_cap; i++)
		if (m->party.member[i].id == -1)
			break;
	m->party.members = i;

	/* Räkna ut hur många gruppmedlemmar som får plats på höjden */
	m->var.inventory.stats = m->cam.screen_h;
	m->var.inventory.stats -= m->system.tile_h * 2;		/* Två horisontala menyhyllor utöver gruppmedlemsmenyn */
	m->var.inventory.stats -= m->system.textbox_pad_v * 2;
	m->var.inventory.stats /= (m->system.face_h + m->system.face_pad_h);

	y = m->system.tile_h;
	y += m->system.textbox_pad_v;

	x = m->system.textbox_pad_h * 2;
	x += m->system.face_w;

	if ((m->var.inventory.stat = malloc(sizeof(INVENTORY_MEMBER_STATS) * m->var.inventory.stats)) == NULL) {
		fprintf(stderr, "FATAL ERROR: Out of RAM\n");
		darnitQuit(m->darnit);
	}

	sel_h = darnitFontGetGlyphHS(m->system.std_font);
	sel_h *= 3;
	sel_h += m->system.face_pad_h;
	pad_2_y = (darnitFontGetGlyphHS(m->system.std_font) * 3 - m->system.face_h) / 2;

	for (i = 0; i < m->var.inventory.stats; i++) {
		m->var.inventory.stat[i].stats_text = darnitTextSurfaceAlloc(m->system.std_font, 75, m->system.inv_stat_w, x, y + i * sel_h);
		m->var.inventory.stat[i].face_cache = darnitRenderTileAlloc(1);
		darnitRenderTileMove(m->var.inventory.stat[i].face_cache, 0, ts, m->system.textbox_pad_h, y + i * sel_h + pad_2_y);
	}

	x -= m->system.textbox_pad_h;
	x -= m->system.face_w;
	h = sel_h - m->system.face_pad_h;
	w = m->system.inv_middle_bar_pos * m->system.tile_w - m->system.textbox_pad_h * 2;
	m->var.inventory.member_menu = darnitMenuVerticalShadeCreate(m->darnit, x, y, w, h, sel_h, m->party.members, -1, m->var.inventory.stats);

	x = m->system.textbox_pad_h;
	y = m->system.tile_h - darnitFontGetGlyphHS(m->system.std_font);
	y >>= 1;
	m->var.inventory.top_menu = darnitMenuHorizontalCreate(m->darnit, darnitStringtableEntryGet(m->system.language, "INVENTORY_TOP_MENU"), x, y, m->system.std_font, 0);

	s = m->cam.screen_w / m->system.tile_w;
	t = s * m->cam.screen_h / m->system.tile_h;

	ts = m->system.ts_ui_elements;
	
	/* Text etc. som ska visas förutom menyer */
	m->var.inventory.mainscreen_icons = NULL;
	x = m->system.inv_middle_bar_pos * m->system.tile_w + m->system.textbox_pad_h;
	y = m->system.tile_h + (m->system.tile_h >> 1);
	m->var.inventory.mainscreen = darnitTextSurfaceAlloc(m->system.std_font, 1024, m->cam.screen_w, x, y);
	m->var.inventory.bottom_navbar = NULL;

	y = m->cam.screen_h - m->system.tile_h + ((m->system.tile_h - darnitFontGetGlyphHS(m->system.std_font)) >> 1);
	m->var.inventory.info_bar = darnitTextSurfaceAlloc(m->system.std_font, 128, m->cam.screen_w, x, y);
	m->var.inventory.coins_str = darnitTextSurfaceAlloc(m->system.std_font, 128, m->cam.screen_w, m->system.textbox_pad_h, y);

	/* Faktiska menyer som måste vara initierade med något då dessa frigörs typ hela tiden */
	m->var.inventory.bottom_menu = NULL;
	m->var.inventory.bottom_sel_count_menu = NULL;
	m->var.inventory.main_menu = NULL;
	m->var.inventory.top_sel = m->var.inventory.member_sel = m->var.inventory.member_target_sel = 0;
	m->var.inventory.main_sel = m->var.inventory.sel_count = m->var.inventory.bottom_sel_count = 0;

	sprintf(num, "%s: %.8i\n", darnitStringtableEntryGet(m->system.language, "COINS"), m->party.coins);
	darnitTextSurfaceStringAppend(m->var.inventory.coins_str, num);

	m->var.inventory.mode = INVENTORY_MODE_SELECT;
	m->var.inventory.item_icon = darnitRenderTileAlloc(m->system.inv_mainscreen_rows);

	x = m->system.inv_middle_bar_pos * m->system.tile_w + m->system.textbox_pad_h;
	y = m->system.tile_h + (m->system.tile_h >> 1) + (((int)darnitFontGetGlyphHS(m->system.std_font) - m->system.item_h) >> 1);

	for (i = 0; i < m->system.inv_mainscreen_rows; i++) {
		darnitRenderTileClear(m->var.inventory.item_icon, i);
		darnitRenderTileMove(m->var.inventory.item_icon, i, m->item.ts_icon, x, y);
		y += darnitFontGetGlyphHS(m->system.std_font);
	}

	inventoryStatSet(m);

	return;
}


void inventoryUnset(void *handle) {
	MAIN *m = handle;
	int i;

	for (i = 0; i < m->var.inventory.stats; i++) {
		darnitRenderTileFree(m->var.inventory.stat[i].face_cache);
		darnitTextSurfaceFree(m->var.inventory.stat[i].stats_text);
	}

	free(m->var.inventory.stat);
	darnitTextSurfaceFree(m->var.inventory.mainscreen);
	darnitTextSurfaceFree(m->var.inventory.bottom_navbar);
	darnitTextSurfaceFree(m->var.inventory.info_bar);
	darnitTextSurfaceFree(m->var.inventory.coins_str);

	m->var.inventory.item_icon = darnitRenderTileFree(m->var.inventory.item_icon);

	m->var.inventory.top_menu = darnitMenuDestroy(m->var.inventory.top_menu);
	m->var.inventory.member_menu = darnitMenuDestroy(m->var.inventory.member_menu);
	m->var.inventory.main_menu = darnitMenuDestroy(m->var.inventory.main_menu);
	m->var.inventory.bottom_menu = darnitMenuDestroy(m->var.inventory.bottom_menu);
	m->var.inventory.bottom_sel_count_menu = darnitMenuDestroy(m->var.inventory.bottom_sel_count_menu);


	return;
}


int inventory(void *handle) {
	MAIN *m = handle;
	int i;

	darnitRenderTilemap(m->darnit, m->system.inv_background);

	darnitRenderBlendingEnable(m->darnit);
	inventoryMenuLogic(m);

	for (i = 0; i < m->var.inventory.stats; i++) {
		if (m->var.inventory.stat[i].id == -1)
			break;
		darnitRenderTileDraw(m->var.inventory.stat[i].face_cache, m->party.party_face_ts, 1);
		darnitTextSurfaceDraw(m->var.inventory.stat[i].stats_text);
	}

	/* Rendera all text nu när menyerna är renderade */
	darnitTextSurfaceDraw(m->var.inventory.mainscreen);
	darnitTextSurfaceDraw(m->var.inventory.bottom_navbar);
	darnitTextSurfaceDraw(m->var.inventory.info_bar);
	darnitTextSurfaceDraw(m->var.inventory.coins_str);
	darnitRenderTileDraw(m->var.inventory.item_icon, m->item.ts_icon, m->system.inv_mainscreen_rows);

	darnitRenderBlendingDisable(m->darnit);



	return 0;
}
