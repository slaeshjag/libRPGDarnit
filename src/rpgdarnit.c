#include "rpgdarnit.h"

int main(int argc, char **argv) {
	MAIN *m;

	if ((m = malloc(sizeof(MAIN))) == NULL) {
		fprintf(stderr, "Error: Unable to allocate memory for the main struct!\n");
		return -1;
	}

	m->darnit = darnitInit("RPG Darnit!");

	for (;;) {
		darnitLoop(m->darnit);
	}

	return 0;
}
