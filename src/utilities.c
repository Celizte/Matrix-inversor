#include "utilities.h"

#include <stdio.h>

void clear_screen() {
	printf("\e[H\e[J");
	fflush(stdout);
}
