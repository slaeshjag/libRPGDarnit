#include "rpgdarnit.h"


int itemInit(void *handle, const char *fname) {
	MAIN *m = handle;
	int i;
	unsigned int items;
	void *st;
	char num[7];

	fprintf(stderr, "INIT: Items... ");

	if ((st = darnitStringtableOpen(fname)) == NULL) {
		fprintf(stderr, "Unable to open item-stringtable %s\n", fname);
		return -1;
	}

	darnitStringtableSectionLoad(st, "INDEX");
	items = atoi(darnitStringtableEntryGet(st, "ITEM_SLOTS"));

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
		m->item.item[i].effect = atoi(darnitStringtableEntryGet(st, darnitStringtableEntryGet(st, "EFFECT")));
		m->item.item[i].hp = atoi(darnitStringtableEntryGet(st, "HP"));
		m->item.item[i].hpbyuse = atoi(darnitStringtableEntryGet(st, "HP_BY_USE"));
		m->item.item[i].spread = atoi(darnitStringtableEntryGet(st, "SPREAD"));
		m->item.item[i].hp_gain = atoi(darnitStringtableEntryGet(st, "HP_GAIN"));
		m->item.item[i].mp_gain = atoi(darnitStringtableEntryGet(st, "MP_GAIN"));

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
	fprintf(stderr, "[OK]\n");

	return 0;
}


int itemGiveToParty(void *handle, unsigned int item, unsigned int amount) {
	MAIN *m = handle;
	int i, j;

	for (i = 0; i < m->system.party_cap; i++) {
		if (m->party.member[i].id == -1)
			break;
		for (j = 0; j < m->system.inventory_size; j++) {
			if (m->party.member[i].inventory[j].id == item) {
				if (m->party.member[i].inventory[j].amount + amount > m->item.item[item].max_stack) {
					amount -= (m->item.item[item].max_stack - m->party.member[i].inventory[i].amount);
					m->party.member[i].inventory[j].amount = m->item.item[item].max_stack;
					continue;
				}
				m->party.member[i].inventory[j].amount += amount;
				return 0;
			}

			if (m->party.member[i].inventory[j].id == -1) {
				m->party.member[i].inventory[j].amount = amount;
				m->party.member[i].inventory[j].id = item;
				m->party.member[i].inventory[j].equipped = 0;
				return 0;
			}
		}
	}

	return amount;
}


void itemDeleteFromInventory(void *handle, ITEM_ENTRY *item, int item_slot) {
	int i;
	MAIN *m = handle;

	if (item_slot > m->system.inventory_size || item_slot < 0)
		return;
	for (i = item_slot + 1; i < m->system.inventory_size; i++) {
		item[i-1] = item[i];
		item[i].id = -1;
		item[i].equipped = 0;
		item[i].amount = 0;
	}

	if (item_slot + 1 == m->system.inventory_size) {
		item[item_slot].amount = 0;
		item[item_slot].id = -1;
		item[item_slot].equipped = 0;
	}

	return;
}


void itemTossFromInventory(void *handle, ITEM_ENTRY *item, int item_type, int slot, int amount) {
	int i;
	MAIN *m = handle;

	if (m->item.item[item_type].max_stack == 1) {
		itemDeleteFromInventory(handle, item, slot);
		return;
	}

	for (i = m->system.inventory_size - 1; i >= 0; i--) {
		if (item[i].id != item_type)
			continue;
		if (item[i].amount < amount) {
			amount -= item[i].amount;
			itemDeleteFromInventory(m, item, i);
		} else {
			item[i].amount -= amount;
			return;
		}
	}

	return;		/* Kommer vi hit så har det inte funnits tillräckligt många av föremålen	*
			 * för att kunna kasta begärt antal. Egentligen så är det dåligt, men eftersom  *
			 * det är väldigt osannolikt att denna situation uppstår så kan man antagligen  *
			 * leva med det. All vettig kod kollar först hur många man har.                 */
}


int itemAddToInventory(void *handle, ITEM_ENTRY *item, int item_type, int amount) {
	int i, given;
	MAIN *m = handle;

	given = 0;

	for (i = 0; i < m->system.inventory_size; i++) {
		if (item[i].id != -1 && item[i].id != item_type)
			continue;
		if (m->item.item[item_type].max_stack == item[i].amount)
			continue;
		if (m->item.item[item_type].max_stack - amount + given <= item[i].amount) {
			given += (m->item.item[item_type].max_stack - item[i].amount);
			item[i].amount = m->item.item[item_type].max_stack;
		} else if (item[i].id != -1) {
			item[i].amount += amount - given;
			given = amount;
			return given;
		} else if (item[i].id == -1) {
			item[i].id = item_type;
			item[i].amount = amount - given;
			item[i].equipped = 0;
			given = amount;
			return given;
		}
	}

	return given;
}


int itemUseOnPartyMember(void *handle, void *srcp, void *dstp, int item_type) {
	MAIN *m = handle;
	int cont, i = item_type;
	PARTY_ENTRY *src = srcp, *dst = dstp;

	cont = 0;
	
	if ((m->item.item[i].effect & ITEM_HEAL_HP) && dst->hp < dst->hp_max)
		cont = 1;
	if ((m->item.item[i].effect & ITEM_HEAL_MP) && dst->mp < dst->mp_max)
		cont = 1;
	if ((m->item.item[i].effect & ITEM_CLEAR_STATUS) && (dst->status_flags & ~0))
		cont = 1;
	if ((m->item.item[i].effect == ITEM_DAMAGE) && dst->hp > 0)
		cont = 1;
	if ((m->item.item[i].effect == ITEM_REVIVE) && dst->hp == 0)
		cont = 1;
	if ((1 << (m->item.item[i].effect - ITEM_BITMASK_START)) & dst->status_flags)
		cont = 1;
	
	if (cont == 0)
		return -1;
	
	dst->hp += m->item.item[i].hp_gain;
	dst->hp = (dst->hp > dst->hp_max) ? dst->hp_max : dst->hp;

	dst->mp += m->item.item[i].mp_gain;
	dst->mp = (dst->mp > dst->mp_max) ? dst->mp_max : dst->mp;

	if (m->item.item[i].effect & ITEM_CLEAR_STATUS) dst->status_flags = 0;

	if (m->item.item[i].effect >= ITEM_BITMASK_START) {
		dst->status_flags |= (1 << (m->item.item[i].effect - ITEM_BITMASK_START));
		dst->status_flags ^= (1 << (m->item.item[i].effect - ITEM_BITMASK_START));
	}

	if (m->item.item[i].max_stack > 1)
		itemTossFromInventory(m, src->inventory, item_type, -1, 1);
	
	return 0;
}
	
