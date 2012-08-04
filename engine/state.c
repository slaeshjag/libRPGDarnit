#include "rpgdarnit.h"


int stateSet(void *handle) {
	MAIN *m = handle;

	/* Frigör resurserna från det gamla läget, om vi inte behöver dom längre */
	switch (m->var.state) {
		case STATE_INVENTORY:
			inventoryUnset(m);
			break;
		default:
			break;
	}

	/* Initiera det nya läget */
	switch (m->var.newstate) {
		case STATE_OVERWORLD:
			/* Inget behövs vad jag kan komma på */
			break;
		case STATE_INVENTORY:
			inventorySet(m);
			break;
		case STATE_TELEPORTING:
			m->var.state = m->var.newstate = STATE_OVERWORLD;
			mapDoTeleport(m, m->var.teleporter, 1);
			break;
		default:
			/* På sikt passar felrapportering här, kanske */
			break;
	}

	m->var.state = m->var.newstate;

	return 0;
}


