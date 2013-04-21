//#include "SDLHost.h"
#include "SledgeHost.h"
#include <cstdio>//<stdio.h>



int main (int argc, char** argv) {
#ifdef _DEBUG
	printf("MOAI-OPEN DEBUG\n");
#endif
	//return SdlHost(argc, argv);

	SledgeHost* moaihost = new SledgeHost(argc, argv);

	delete moaihost;

	return 0;
}