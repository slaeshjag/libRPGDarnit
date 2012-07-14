#include <stdio.h>
#include <stdlib.h>

#include <darnit/darnit.h>

int main(int argc, char **argv) {
	void *lib;
	int (*rpgdarnit)(), ret;

	if ((lib = darnitDynlibOpen("bin/rpgdarnit.core")) == NULL) {
		fprintf(stderr, "Unable to load bin/rpgdarnit.core\n");
		return -1;
	}

	if ((rpgdarnit = darnitDynlibGet(lib, "rpg_main")) == NULL) {
		fprintf(stderr, "Unable to locate function rpg_main in bin/rpgdarnit.core\n");
		darnitDynlibClose(lib);
		return;
	}

	ret = (rpgdarnit)();

	darnitDynlibClose(lib);

	return ret;
}
