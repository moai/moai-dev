//#include "SDLHost.h"
#include "SDLHost2.h"
#include <stdio.h>

int main (int argc, char** argv) {
#ifdef _DEBUG
	printf("MOAI-OPEN DEBUG\n");
#endif
	//return SdlHost(argc, argv);

	SdlHost2* moaihost = new SdlHost2(argc, argv);

	return 0;
}