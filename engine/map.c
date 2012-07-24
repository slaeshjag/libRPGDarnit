#include "rpgdarnit.h"

int mapInit(void *handle) {
	MAIN *m = handle;

	m->map.w = m->map.h = m->map.layers = m->map.teleporters = m->map.triggers = 0;
	m->map.tilesheet = NULL;
	m->map.teleporter = NULL;
	m->map.trigger = NULL;
	m->map.layer = NULL;

	return 0;
}


int mapDrawLayer(void *handle, int layer) {
	MAIN *m = handle;

	darnitRenderTilemapCameraMove(m->map.layer[layer], m->cam.x, m->cam.y);
	darnitRenderTilemap(m->darnit, m->map.layer[layer]);

	return 0;
}


int mapUnload(void *handle) {
	MAIN *m = handle;
	int i;

	for (i = 0; i < m->map.layers; i++)
		darnitRenderTilemapDelete(m->map.layer[i]);
	
	npcKillAll(m);
	m->map.tilesheet = darnitRenderTilesheetFree(m->darnit, m->map.tilesheet);
	free(m->map.teleporter);
	free(m->map.trigger);
	free(m->map.layer);
	m->map.layers = 0;

	return 0;
}


int mapLoad(void *handle, const char *fname) {
	MAIN *m = handle;
	MAP_FILE_HEADER *mfh;
	MAP_FILE_NPC npc;
	int i, j, k;
	unsigned int t;
	FILE *fp;

	mapUnload(handle);

	if ((mfh = malloc(sizeof(MAP_FILE_HEADER))) == NULL) {
		fprintf(stderr, "Error: Unable to malloc(%i)\n", (unsigned int) sizeof(MAP_FILE_HEADER));
		return -1;
	}

	if ((fp = fopen(fname, "rb")) == NULL) {
		fprintf(stderr, "Error: Unable to load map %s\n", fname);
		free(mfh);
		return -1;
	}

	if (fread(mfh, sizeof(MAP_FILE_HEADER), 1, fp) < 1) {
		fprintf(stderr, "Unexpected end of file %s\n", fname);
		free(mfh);
		fclose(fp);
		return -1;
	}

	if (darnitUtilNtohl(mfh->magic) != MAP_FILE_MAGIC) {
		fprintf(stderr, "Map file '%s' is not a valid CTMB file\n", fname);
		free(mfh);
		fclose(fp);
		return -1;
	}

	m->map.teleporters = darnitUtilNtohl(mfh->teleporters);
	m->map.triggers = darnitUtilNtohl(mfh->triggers);
	m->map.layers = darnitUtilNtohl(mfh->layers);
	m->map.w = darnitUtilNtohl(mfh->w);
	m->map.h = darnitUtilNtohl(mfh->h);
	m->map.tile_w = darnitUtilNtohl(mfh->tile_w);
	m->map.tile_h = darnitUtilNtohl(mfh->tile_h);
	m->map.layer = malloc(sizeof(DARNIT_TILEMAP *) * m->map.layers);

	if ((m->map.teleporter = malloc(sizeof(MAP_FILE_TELEPORTER) * m->map.teleporters)) == NULL) {
		m->map.layers = 0;
		fclose(fp);
		free(m->map.layer);
		return -1;
	}

	if ((m->map.trigger = malloc(sizeof(MAP_FILE_TRIGGER) * m->map.triggers)) == NULL) {
		m->map.layers = 0;
		fclose(fp);
		free(m->map.layer);
		free(m->map.teleporter);
		return -1;
	}

	npcLimitSet(m, darnitUtilNtohl(mfh->spawn_points));
	npcLoadCode(m, mfh->logic_lib);

	if ((m->map.tilesheet = darnitRenderTilesheetLoad(m->darnit, mfh->tilesheet_file, m->map.tile_w, m->map.tile_h, DARNIT_PFORMAT_RGBA8)) == NULL) {
		fclose(fp);
		fprintf(stderr, "Unable to open tilesheet '%s' for map '%s'\n", mfh->tilesheet_file, fname);
		return -1;
	}

	for (i = 0; i < m->map.layers; i++) {
		if ((m->map.layer[i] = darnitRenderTilemapNew(m->darnit, 0x1000, m->map.tilesheet, MAP_MASK_USE, m->map.w, m->map.h)) == NULL)
			fprintf(stderr, "WARNING: Loading layer %i failed\n", i);
		for (j = 0; j < m->map.h; j++)
			for (k = 0; k < m->map.w; k++) {
				t = 0;
				fread(&t, 4, 1, fp);
				m->map.layer[i]->data[j * m->map.w + k] = darnitUtilNtohl(t);
			}
		darnitRenderTilemapRecalculate(m->map.layer[i]);
	}
	
	fread(m->map.teleporter, sizeof(MAP_FILE_TELEPORTER) * m->map.teleporters, 1, fp);

	for (i = 0; i < m->map.teleporters; i++) {
		m->map.teleporter[i].target_x = darnitUtilNtohl(m->map.teleporter[i].target_x);
		m->map.teleporter[i].target_y = darnitUtilNtohl(m->map.teleporter[i].target_y);
		m->map.teleporter[i].target_layer = darnitUtilNtohl(m->map.teleporter[i].target_layer);
		m->map.teleporter[i].source_x = darnitUtilNtohl(m->map.teleporter[i].source_x);
		m->map.teleporter[i].source_y = darnitUtilNtohl(m->map.teleporter[i].source_y);
		m->map.teleporter[i].source_layer = darnitUtilNtohl(m->map.teleporter[i].source_layer);
	}
	
	for (i = 0; i < darnitUtilNtohl(mfh->spawn_points); i++) {
		fread(&npc, sizeof(MAP_FILE_NPC), 1, fp);
		npcSpawn(m, darnitUtilNtohl(npc.spawn_x), darnitUtilNtohl(npc.spawn_y), darnitUtilNtohl(npc.spawn_layer), darnitUtilNtohl(npc.dir), npc.sprite_file, npc.logic_func);
	}

	fread(m->map.trigger, sizeof(MAP_FILE_TRIGGER) * m->map.triggers, 1, fp);
	
	for (i = 0; i < m->map.triggers; i++) {
		m->map.trigger[i].x = darnitUtilNtohl(m->map.trigger[i].x);
		m->map.trigger[i].y = darnitUtilNtohl(m->map.trigger[i].y);
		m->map.trigger[i].layer = darnitUtilNtohl(m->map.trigger[i].layer);
	}

	free(mfh);
	fclose(fp);

	return 0;
}



//int mapTeleport(void *handle, 
