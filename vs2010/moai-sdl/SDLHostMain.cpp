#include "SDLHost.h"
#include <stdio.h>

int main (int argc, char** argv) {
#ifdef _DEBUG
	printf("MOAI-OPEN DEBUG\n");
#endif
	return SdlHost(argc, argv);
}