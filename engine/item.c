#include "rpgdarnit.h"


int itemInit(void *handle, const char *fname) {
	MAIN *m = handle;
	int i;
	unsigned int items;
	void *st;
	char num[7];

	if ((st = darnitStringtableOpen(fname)) == NULL) {
		fprintf(stderr, "Unable to open item-stringtable %s\n", fname);
		return -1;
	}

	darnitStringtableSectionLoad(st, "INDEX");
	items = atoi(darnitStringtableEntryGet(st, "ITEM_SLOTS"));
	darnitStringtableSectionUnload(st, "INDEX");

	m->item.items = items;

	if ((m->item.item = malloc(sizeof(ITEM_PROTOTYPE) * items)) == NULL) {
		fprintf(stderr, "Unable to malloc(%i) for items\n", (unsigned int) sizeof(ITEM_PROTOTYPE));
		darnitStringtableClose(st);
		return -1;
	}

	for (i = 0; i < items; i++) {
		sprintf(num, "%i", i);
		darnitStringtableSectionLoad(st, num);
		m->item.item[i].item_class = atoi(darnitStringtableEntryGet(st, "CLASS"));
		m->item.item[i].effect = atoi(darnitStringtableEntryGet(st, "EFFECT"));
		m->item.item[i].hp = atoi(darnitStringtableEntryGet(st, "HP"));
		m->item.item[i].hpbyuse = atoi(darnitStringtableEntryGet(st, "HP_BY_USE"));
		m->item.item[i].spread = atoi(darnitStringtableEntryGet(st, "SPREAD"));

		sscanf(darnitStringtableEntryGet(st, "SPREAD_FACTOR"), "%f", &m->item.item[i].spread_factor);

		strncpy(m->item.item[i].animation, darnitStringtableEntryGet(st, "ANIMATION"), 31);
		m->item.item[i].animation[31] = 0;
		strncpy(m->item.item[i].sound, darnitStringtableEntryGet(st, "SOUND"), 31);
		m->item.item[i].animation[31] = 0;
		strncpy(m->item.item[i].name, darnitStringtableEntryGet(st, "NAME"), 31);
		m->item.item[i].animation[31] = 0;
		strncpy(m->item.item[i].descr, darnitStringtableEntryGet(st, "DESCRIPTION"), 31);
		m->item.item[i].animation[31] = 0;

		sscanf(darnitStringtableEntryGet(st, "ATTACK_BOOST"), "%f", &m->item.item[i].attboost);
		sscanf(darnitStringtableEntryGet(st, "DEFENCE_BOOST"), "%f", &m->item.item[i].defboost);
		sscanf(darnitStringtableEntryGet(st, "SPEC_ATTACK_BOOST"), "%f", &m->item.item[i].specaboost);
		sscanf(darnitStringtableEntryGet(st, "SPEC_DEFENCE_BOOST"), "%f", &m->item.item[i].specdboost);
		sscanf(darnitStringtableEntryGet(st, "SPEED_BOOST"), "%f", &m->item.item[i].speedboost);

		m->item.item[i].resistance = atoi(darnitStringtableEntryGet(st, "RESISTANCE"));
		sscanf(darnitStringtableEntryGet(st, "RESISTANCE_FACTOR"), "%f", &m->item.item[i].resistance_factor);

		darnitStringtableSectionUnload(st, num);
	}

	darnitStringtableClose(st);

	return 0;
}



