#include "rpgdarnit.h"

void inventorySet(void *handle) {
	MAIN *m = handle;
	char num[10];
	int i, x, y, sel_h, pad_2_y, s, t;
	void *ts = m->party.party_face_ts;

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
		m->var.inventory.stat[i].stats_text = darnitTextSurfaceAlloc(m->system.std_font, 75, 150, x, y + i * sel_h);
		m->var.inventory.stat[i].face_cache = darnitRenderTileAlloc(1);
		darnitRenderTileMove(m->var.inventory.stat[i].face_cache, 0, ts, m->system.textbox_pad_h, y + i * sel_h + pad_2_y);
	}

	s = m->cam.screen_w / m->system.tile_w;
	t = s * m->cam.screen_h / m->system.tile_h;

	ts = m->system.ts_ui_elements;
	
	return;
}


void inventoryUnset(void *handle) {
	/* TODO: Skriv kåda */

	return;
}
