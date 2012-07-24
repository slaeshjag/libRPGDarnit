#include "rpgdarnit.h"


void *partyLoadFaceTS(void *handle, const char *fname) {
	MAIN *m = handle;

	return  darnitRenderTilesheetLoad(m->darnit, fname, m->system.face_w, m->system.face_h, 0);
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
		
