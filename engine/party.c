#include "rpgdarnit.h"


void *partyLoadFaceTS(void *handle, const char *fname) {
	MAIN *m = handle;

	return  darnitRenderTilesheetLoad(m->darnit, fname, m->system.face_w, m->system.face_h, 0);
}


void partyEquipSlotParse(void *handle, PARTY_SKEL *skel, const char *slots) {
	MAIN *m = handle;
	int i;

	i = darnitUtilStringToIntArray(slots, ",", skel->slot, m->system.equip_slots);

	for (; i < m->system.equip_slots; i++)
		skel->slot[i] = -1;
	
	return;
}


int partyInit(void *handle, const char *fname) {
	MAIN *m = handle;
	int i, j, party_skels;
	void *st;
	char num[7], ability_buf[32];

	if ((st = darnitStringtableOpen(fname)) == NULL) {
		fprintf(stderr, "Unable to open party-stringtable %s\n", fname);
		return -1;
	}

	if (darnitStringtableSectionLoad(st, "INDEX") != 0)
		fprintf(stderr, "Unable to load section INDEX\n");
	party_skels = atoi(darnitStringtableEntryGet(st, "PARTY_PROTOTYPES"));
	
	if ((m->party.party_face_ts = partyLoadFaceTS(m, darnitStringtableEntryGet(st, "PARTY_FACE_TILESET"))) == NULL) {
		fprintf(stderr, "Unable to open party face tileset %s\n", darnitStringtableEntryGet(st, "PARTY_FACE_TILESET"));
		return -1;
	}

	darnitStringtableSectionUnload(st, "INDEX");

	if ((m->party.skel = malloc(sizeof(PARTY_SKEL) * party_skels)) == NULL) {
		fprintf(stderr, "Unable to malloc(%i)\n", (unsigned int) sizeof(PARTY_SKEL) * party_skels);
		return -1;
	}

	m->party.skels = party_skels;

	for (i = 0; i < party_skels; i++) {
		sprintf(num, "%i", i);
		darnitStringtableSectionLoad(st, num);
		strncpy(m->party.skel[i].name, darnitStringtableEntryGet(st, "NAME"), 15);
		m->party.skel[i].name[15] = 0;
		strncpy(m->party.skel[i].anim_db, darnitStringtableEntryGet(st, "ANIM_DB"), 15);
		m->party.skel[i].anim_db[31] = 0;

		m->party.skel[i].ai = atoi(darnitStringtableEntryGet(st, "AI"));
		m->party.skel[i].base_HP = atoi(darnitStringtableEntryGet(st, "BASE_HP"));
		m->party.skel[i].base_MP = atoi(darnitStringtableEntryGet(st, "BASE_MP"));
		m->party.skel[i].base_att = atoi(darnitStringtableEntryGet(st, "BASE_ATTACK"));
		m->party.skel[i].base_def = atoi(darnitStringtableEntryGet(st, "BASE_DEFENCE"));
		m->party.skel[i].base_speca = atoi(darnitStringtableEntryGet(st, "BASE_SPEC_ATTACK"));
		m->party.skel[i].base_specd = atoi(darnitStringtableEntryGet(st, "BASE_SPEC_DEFENCE"));
		m->party.skel[i].base_speed = atoi(darnitStringtableEntryGet(st, "BASE_SPEED"));

		m->party.skel[i].per_HP = atoi(darnitStringtableEntryGet(st, "PERIOD_HP"));
		m->party.skel[i].per_MP = atoi(darnitStringtableEntryGet(st, "PERIOD_MP"));
		m->party.skel[i].per_att = atoi(darnitStringtableEntryGet(st, "PERIOD_ATTACK"));
		m->party.skel[i].per_def = atoi(darnitStringtableEntryGet(st, "PERIOD_DEFENCE"));
		m->party.skel[i].per_speca = atoi(darnitStringtableEntryGet(st, "PERIOD_SPEC_ATTACK"));
		m->party.skel[i].per_specd = atoi(darnitStringtableEntryGet(st, "PERIOD_SPEC_DEFENCE"));
		m->party.skel[i].per_speed = atoi(darnitStringtableEntryGet(st, "PERIOD_SPEED"));

		m->party.skel[i].amp_HP = atoi(darnitStringtableEntryGet(st, "AMPLITUDE_HP"));
		m->party.skel[i].amp_MP = atoi(darnitStringtableEntryGet(st, "AMPLITUDE_MP"));
		m->party.skel[i].amp_att = atoi(darnitStringtableEntryGet(st, "AMPLITUDE_ATTACK"));
		m->party.skel[i].amp_def = atoi(darnitStringtableEntryGet(st, "AMPLITUDE_DEFENCE"));
		m->party.skel[i].amp_speca = atoi(darnitStringtableEntryGet(st, "AMPLITUDE_SPEC_ATTACK"));
		m->party.skel[i].amp_specd = atoi(darnitStringtableEntryGet(st, "AMPLITUDE_SPEC_DEFENCE"));
		m->party.skel[i].amp_speed = atoi(darnitStringtableEntryGet(st, "AMPLITUDE_SPEED"));
		
		m->party.skel[i].resist[0] = atoi(darnitStringtableEntryGet(st, "RESIST_1"));
		m->party.skel[i].resist[1] = atoi(darnitStringtableEntryGet(st, "RESIST_2"));
		m->party.skel[i].resist[2] = atoi(darnitStringtableEntryGet(st, "RESIST_3"));
		m->party.skel[i].resist[3] = atoi(darnitStringtableEntryGet(st, "RESIST_4"));
		m->party.skel[i].resist[4] = atoi(darnitStringtableEntryGet(st, "RESIST_5"));
		m->party.skel[i].resist[5] = atoi(darnitStringtableEntryGet(st, "RESIST_6"));
		m->party.skel[i].resist[6] = atoi(darnitStringtableEntryGet(st, "RESIST_7"));
		m->party.skel[i].resist[7] = atoi(darnitStringtableEntryGet(st, "RESIST_8"));

		m->party.skel[i].slot = malloc(sizeof(int) * m->system.equip_slots);
		partyEquipSlotParse(m, &m->party.skel[i], darnitStringtableEntryGet(st, "EQUIP_SLOTS"));
		
		m->party.skel[i].ats = atoi(darnitStringtableEntryGet(st, "ABILITY_COUNT"));
		if ((m->party.skel[i].at = malloc(sizeof(PARTY_MEMBER_AT))) == NULL)
			return -1;

		for (j = 1; j <= m->party.skel[i].ats; j++) {
			sprintf(ability_buf, "ABILITY_%i_SPECA_REQ", j);
			m->party.skel[i].at[j-1].mareq = atoi(darnitStringtableEntryGet(st, ability_buf));
			sprintf(ability_buf, "ABILITY_%i_SPECD_REQ", j);
			m->party.skel[i].at[j-1].mdreq = atoi(darnitStringtableEntryGet(st, ability_buf));
			sprintf(ability_buf, "ABILITY_%i_SPEED_REQ", j);
			m->party.skel[i].at[j-1].agilityreq = atoi(darnitStringtableEntryGet(st, ability_buf));
			sprintf(ability_buf, "ABILITY_%i_ID", j);
			m->party.skel[i].at[j-1].ability = atoi(darnitStringtableEntryGet(st, ability_buf));
		}

		darnitStringtableSectionUnload(st, num);
	}

	darnitStringtableClose(st);
	return 0;
}


int partyMemberHasThisAbility(void *handle, PARTY_ENTRY *member, int ability) {
	MAIN *m = handle;
	int i;

	for (i = 0; i < m->system.party_cap; i++)
		if (member->magic[i] == ability)
			return 0;
	return -1;
}


void partyMemberAddAbility(void *handle, PARTY_ENTRY *member, int ability) {
	MAIN *m = handle;
	int i;

	for (i = 0; i < m->system.party_cap; i++)
		if (member->magic[i] == -1) {
			member->magic[i] = ability;
			return;
		}
	return;
}


int partyMemberEquipSlotGetFree(void *handle, PARTY_ENTRY *member, int equip_slot) {
	int i, j, slots;
	MAIN *m = handle;

	for (i = slots = 0; i < m->system.inventory_size; i++) {
		if (member->inventory[i].id == -1)
			break;
		if (member->inventory[i].equipped == 0)
			continue;
		if (m->item.item[member->inventory[i].id].item_class == equip_slot)
			slots++;
	}

	for (j = 0; j < m->system.equip_slots; j++) {
		if (m->party.skel[member->id].slot[j] == equip_slot)
			slots--;
		if (slots == -1)
			return j;
	}

	return -1;
}

	
int partyMemberReclaimEquipSlot(void *handle, PARTY_ENTRY *member, int equip_slot) {
	MAIN *m = handle;
	int i;

	for (i = 0; i < m->system.inventory_size; i++) {
		if (member->inventory[i].id == -1)
			break;
		if (member->inventory[i].equipped == 0)
			continue;
		if (m->item.item[member->inventory[i].id].item_class == equip_slot) {
			member->inventory[i].equipped = 0;
			return 0;
		}
	}

	return -1;
}



MEMBER_STATS_DIFF partyMemberCalcStats(void *handle, PARTY_ENTRY *member, int itemrep) {
	/* Itemrep är inte typnummer, utan platsnummer */
	MAIN *m = handle;
	float p;
	int id, item, i, test, repnottried = 0;
	int att, def, speca, specd, speed;
	MEMBER_STATS_DIFF diff;

	id = member->id;

	if (member->att_exp > m->party.skel[id].per_att)
		member->att_exp = m->party.skel[id].per_att;
	if (member->def_exp > m->party.skel[id].per_def)
		member->def_exp = m->party.skel[id].per_def;
	if (member->speca_exp > m->party.skel[id].per_speca)
		member->speca_exp = m->party.skel[id].per_speca;
	if (member->specd_exp > m->party.skel[id].per_specd)
		member->specd_exp = m->party.skel[id].per_specd;
	if (member->speed_exp > m->party.skel[id].per_speed)
		member->speed_exp = m->party.skel[id].per_speed;
	
	p = M_PI_2 * member->att_exp;
	p /= m->party.skel[id].per_att;
	diff.att = m->party.skel[id].amp_att * sinf(p);
	diff.att += m->party.skel[id].base_att;

	p = M_PI_2 * member->def_exp;
	p /= m->party.skel[id].per_def;
	diff.def = m->party.skel[id].amp_def * sinf(p);
	diff.def += m->party.skel[id].base_def;

	p = M_PI_2 * member->speca_exp;
	p /= m->party.skel[id].per_speca;
	diff.speca = m->party.skel[id].amp_speca * sinf(p);
	diff.speca += m->party.skel[id].base_speca;

	p = M_PI_2 * member->specd_exp;
	p /= m->party.skel[id].per_specd;
	diff.specd = m->party.skel[id].amp_specd * sinf(p);
	diff.specd += m->party.skel[id].base_specd;

	p = M_PI_2 * member->speed_exp;
	p /= m->party.skel[id].per_speed;
	diff.speed = m->party.skel[id].amp_speed * sinf(p);
	diff.speed += m->party.skel[id].base_speed;

	p = M_PI_2 * (member->def_exp + member->specd_exp + member->speed_exp) / 3;
	p /= m->party.skel[id].per_HP;
	diff.hp = m->party.skel[id].amp_HP * sinf(p);
	diff.hp += m->party.skel[id].base_HP;

	p = M_PI_2 * (member->speca_exp + member->specd_exp + member->speed_exp) / 3;
	p /= m->party.skel[id].per_MP;
	diff.mp = m->party.skel[id].amp_MP * sinf(p);
	diff.mp += m->party.skel[id].base_MP;

	if (member->needs_init == 0)
		partyMemberLearnAbilities(m, member);
	
	if (itemrep != -1) {
		item = itemrep;
		if (partyMemberCanEquipItem(m, member, item) == -1) {
			fprintf(stderr, "Tried to test equip effects with an item that can't be equipped!\n");
			return diff;
		}
	} else
		item = -1;
	
	att = def = speca = specd = speed = 0;

	for (i = 0; i < m->system.inventory_size; i++) {
		test = i;
		if (member->inventory[i].equipped != -1)
			continue;
		if (item != -1 && repnottried == 0) {
			if (partyMemberEquipSlotGetFree(m, member, m->item.item[member->inventory[item].id].item_class) == -1) {
				repnottried = 1;
				test = member->inventory[item].id;
			}
		} else
			test = member->inventory[i].id;
		
		att += m->item.item[test].attboost * diff.att;
		def += m->item.item[test].defboost * diff.def;
		speca += m->item.item[test].specaboost * diff.speca;
		specd += m->item.item[test].specdboost * diff.specd;
		speed += m->item.item[test].speedboost * diff.speed;
	}

	if (repnottried == 0 && item != -1) {
		test = item;
		att += m->item.item[test].attboost * diff.att;
		def += m->item.item[test].defboost * diff.def;
		speca += m->item.item[test].specaboost * diff.speca;
		specd += m->item.item[test].specdboost * diff.specd;
		speed += m->item.item[test].speedboost * diff.speed;
	}

	diff.att += att, diff.def += def, diff.speca += speca, diff.specd += specd, diff.speed += speed;

	return diff;
}


void partyMemberRecalculateStats(void *handle, PARTY_ENTRY *member) {
	MAIN *m = handle;
	MEMBER_STATS_DIFF diff;
	diff = partyMemberCalcStats(m, member, -1);

	member->hp_max = diff.hp;
	member->mp_max = diff.mp;
	member->att = diff.att;
	member->def = diff.def;
	member->speca = diff.speca;
	member->specd = diff.specd;
	member->speed = diff.speed;

	return;
}


void partyRecalculateStats(void *handle, PARTY_ENTRY *party) {
	MAIN *m = handle;
	int i;

	for (i = 0; i < m->system.party_cap; i++)
		if (party[i].id != -1)
			partyMemberRecalculateStats(m, &party[i]);
	
	return;
}


int partyAddMember(void *handle, PARTY_ENTRY *party, int id, int exp) {
	MAIN *m = handle;
	int i, j;

	for (i = 0; i < m->system.party_cap; i++)
		if (party[i].id == -1)
			break;
	if (i == m->system.party_cap)
		return -1;
	for (j = 0; j < m->system.inventory_size; j++)
		party[i].magic[j] = -1;
	
	party[i].id = id;	
	party[i].att_exp = exp;
	party[i].def_exp = exp;
	party[i].speca_exp = exp;
	party[i].specd_exp = exp;
	party[i].speed_exp = exp;

	party[i].needs_init = 1;
	partyMemberRecalculateStats(m, &party[i]);
	party[i].needs_init = 0;
	partyMemberRecalculateStats(m, &party[i]);
	party[i].hp = party[i].hp_max;
	party[i].mp = party[i].mp_max;

	return i;
}


int partyMemberCanEquipItem(void *handle, PARTY_ENTRY *member, int item) {
	MAIN *m = handle;
	int i, type;

	type = m->item.item[member->inventory[item].id].item_class;

	for (i = 0; i < m->system.equip_slots; i++) 
		if (m->party.skel[member->id].slot[i] == type)
			return 0;

	return -1;
}


MEMBER_STATS_DIFF partyMemberDiffWhenEquip(void *handle, PARTY_ENTRY *member, int item) {
	MAIN *m = handle;
	MEMBER_STATS_DIFF diffa, diffb;

	diffb.att = diffb.def = diffb.speca = diffb.specd = diffb.speed = diffb.hp = diffb.mp = 0;
	diffa = diffb;

	diffb = partyMemberCalcStats(m, member, -1);
	diffa = partyMemberCalcStats(m, member, item);
	diffa.att -= diffb.att, diffa.def -= diffb.def, diffa.speca -= diffb.speca, diffa.specd -= diffb.specd, diffa.speed -= diffb.speed;

	return diffa;
}


void partyToggleEquip(void *handle, PARTY_ENTRY *member, int item_slot) {
	MAIN *m = handle;

	if (member->inventory[item_slot].equipped == 1)
		member->inventory[item_slot].equipped = 0;
	else {
		if (partyMemberEquipSlotGetFree(handle, member, m->item.item[member->inventory[item_slot].id].item_class) == -1)
			if (partyMemberReclaimEquipSlot(m, member, m->item.item[member->inventory[item_slot].id].item_class) < 0)
				return;
		member->inventory[item_slot].equipped = 1;
	}

	partyMemberRecalculateStats(m, member);

	return;
}


int partyMoveItemsWithinParty(void *handle, PARTY_ENTRY *party, int src, int dst, int itemslot, int amount) {
	MAIN *m = handle;
	int toss;

	toss = itemAddToInventory(m, party[dst].inventory, party[src].inventory[itemslot].id, amount);
	itemTossFromInventory(m, party[src].inventory, party[src].inventory[itemslot].id, itemslot, toss);

	partyRecalculateStats(m, party);

	return amount - toss;
}

