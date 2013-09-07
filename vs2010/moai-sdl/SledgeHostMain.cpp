#include "stdafx.h"
#include "SledgeHost.h"



int main (int argc, char** argv) {
#ifdef _DEBUG
	printf("MOAI-OPEN DEBUG\n");
#endif

	SledgeHost* moaihost = new SledgeHost(argc, argv);
	if(moaihost->CheckStatus() == SLEDGE_NAMESPACE::SFS_OK)
		moaihost->RunGame();
	
	delete moaihost;

	return 0;
}