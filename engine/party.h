#ifndef __PARTY_H__
#define	__PARTY_H__


typedef struct {
	unsigned int		mareq;
	unsigned int		mdreq;
	unsigned int		agilityreq;
	int			ability;
} PARTY_MEMBER_AT;


typedef struct {
	int			att;
	int			def;
	int			speca;
	int			specd;
	int			speed;
	int			hp;
	int			mp;
	int			error;
} MEMBER_STATS_DIFF;


typedef struct {
	char			name[16];
	int			ai;
	int			base_HP;
	int			base_MP;
	int			base_att;
	int			base_def;
	int			base_speca;
	int			base_specd;
	int			base_speed;

	int			per_HP;
	int			per_MP;
	int			per_att;
	int			per_def;
	int			per_speca;
	int			per_specd;
	int			per_speed;

	int			amp_HP;
	int			amp_MP;
	int			amp_att;
	int			amp_def;
	int			amp_speca;
	int			amp_specd;
	int			amp_speed;

	int			resist[8];
	char			anim_db[32];
	PARTY_MEMBER_AT		*at;
	int			ats;
} PARTY_SKEL;


typedef struct {
	int			id;
	int			ai;
	int			hp;
	int			hp_max;
	int			att;
	int			def;
	int			speca;
	int			specd;
	int			speed;
	int			mp;
	int			mp_max;
	int			att_exp;
	int			def_exp;
	int			speca_exp;
	int			specd_exp;
	int			speed_exp;
	ITEM_ENTRY		*inventory;
	int			*magic;
	unsigned int		status_flags;
	int			resist[8];
	int			needs_init;
} PARTY_ENTRY;


typedef struct {
	PARTY_ENTRY		*member;
	int			members;
	int			coins;
	PARTY_SKEL		*skel;
	int			skels;
	void			*party_face_ts;
} PARTY;


int partyInit(void *handle, const char *fname);


#endif
