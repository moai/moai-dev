
#include <moai-plugins/host.h>

#ifdef MOAI_WITH_PLUGINS
	#include <host_plugins.h>
#endif


//================================================================//
// aku-plugins
//================================================================//

//----------------------------------------------------------------//
void AKUPluginsAppFinalize () {
  #ifdef MOAI_WITH_PLUGINS
  	HostPluginsAppFinalize();
  #endif
}

//----------------------------------------------------------------//
void AKUPluginsAppInitialize () {
  #ifdef MOAI_WITH_PLUGINS
	 HostPluginsAppInitialize();
  #endif
}

//----------------------------------------------------------------//
void AKUPluginsContextInitialize () {
  #ifdef MOAI_WITH_PLUGINS
  	 HostPluginsContextInitialize();
  #endif
}

void AKUPluginsUpdate () {
  #ifdef MOAI_WITH_PLUGINS
  	 HostPluginsUpdate();
  #endif
}
