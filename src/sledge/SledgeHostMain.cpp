#include "stdafx.h"
#include "SledgeHost.h"

/**
#ifdef WIN32
#include <shellapi.h> 
 
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
#ifdef _DEBUG
	printf("MOAI-OPEN DEBUG\n");
#endif
	SledgeHost* moaihost = new SledgeHost( __argc, __argv);
	if(moaihost->CheckStatus() == SLEDGE_NAMESPACE::SFS_OK)
		moaihost->RunGame();
	delete moaihost;

	return 0;
}

#else**/

int main (int argc, char** argv) {
#ifdef _DEBUG
	printf("MOAI-OPEN DEBUG\n");
#elif WIN32
	bool console = false;

	if (argc > 1) {
		if (strcasecmp(argv[argc - 1], "-console") == 0) {
			console = true;
			argc = argc - 1;
		}
	}

	if (!console)
		FreeConsole();
#endif

	SledgeHost* moaihost = new SledgeHost(argc, argv);
	if(moaihost->CheckStatus() == SLEDGE_NAMESPACE::SFS_OK)
		moaihost->RunGame();
	delete moaihost;

	return 0;
}
//#endif