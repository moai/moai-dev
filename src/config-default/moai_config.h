#ifndef MOAI_CONFIG_H
#define MOAI_CONFIG_H

	//----------------------------------------------------------------//
	// libraries

	#ifndef MOAI_WITH_ARES
		#define MOAI_WITH_ARES 0
	#endif
	
	#ifndef MOAI_WITH_AUDIOSAMPLER
		#define MOAI_HOST_USE_AUDIOSAMPLER 0
	#endif
	
	#ifndef MOAI_WITH_BOX2D
		#define MOAI_WITH_BOX2D 1
	#endif

	#ifndef MOAI_WITH_CHIPMUNK
		#define MOAI_WITH_CHIPMUNK 1
	#endif
	
	#ifndef MOAI_WITH_EXPAT
		#define MOAI_WITH_EXPAT 1
	#endif
	
	#ifndef MOAI_WITH_FMOD_DESIGNER
		#define MOAI_WITH_FMOD_DESIGNER 0
	#endif
	
	#ifndef MOAI_WITH_FMOD_EX
		#define MOAI_WITH_FMOD_EX 0
	#endif
	
	#ifndef MOAI_WITH_FOLDER_WATCHER
		#define MOAI_WITH_FOLDER_WATCHER 0
	#endif
	
	#ifndef MOAI_WITH_FREETYPE
		#define MOAI_WITH_FREETYPE 1
	#endif

	#ifndef MOAI_WITH_GPB
		#define MOAI_WITH_GPB 1
	#endif
	
	#ifndef MOAI_WITH_HARNESS
		#define MOAI_WITH_HARNESS 0
	#endif
	
	#ifndef MOAI_WITH_HTTP_CLIENT
		#define MOAI_WITH_HTTP_CLIENT 1
	#endif
	
	#ifndef MOAI_WITH_JANSSON
		#define MOAI_WITH_JANSSON 1
	#endif
	
	#ifndef MOAI_WITH_LIBCRYPTO
		#define MOAI_WITH_LIBCRYPTO 1
	#endif
	
	#ifndef MOAI_WITH_LIBCURL
		#define MOAI_WITH_LIBCURL 1
	#endif
	
	#ifndef MOAI_WITH_LIBJPG
		#define MOAI_WITH_LIBJPG 1
	#endif
	
	#ifndef MOAI_WITH_LIBPNG
		#define MOAI_WITH_LIBPNG 1
	#endif
	
	#ifndef MOAI_WITH_LUAEXT
		#define MOAI_WITH_LUAEXT 1
	#endif
	
	#ifndef MOAI_WITH_MONGOOSE
		#define MOAI_WITH_MONGOOSE 1
	#endif
	
	#ifndef MOAI_WITH_OGG
		#define MOAI_WITH_OGG 1
	#endif

	#ifndef MOAI_WITH_OPENSSL
		#define MOAI_WITH_OPENSSL 1
	#endif
	
	#ifndef MOAI_WITH_TINYXML
		#define MOAI_WITH_TINYXML 1
	#endif
	
	#ifndef MOAI_WITH_SFMT
		#define MOAI_WITH_SFMT 1
	#endif
	
	#ifndef MOAI_WITH_SQLITE
		#define MOAI_WITH_SQLITE 1
	#endif
	
	#ifndef MOAI_WITH_TLSF
		#define MOAI_WITH_TLSF 1
	#endif
	
	#ifndef MOAI_WITH_UNTZ
		#define MOAI_WITH_UNTZ 1
	#endif
	
	#ifndef MOAI_WITH_VORBIS
		#define MOAI_WITH_VORBIS 1
	#endif

	//----------------------------------------------------------------//
	// graphics subsystem
	
	#ifndef MOAI_GFX_DUMMY
		#define MOAI_GFX_DUMMY 0
	#endif

	#ifndef MOAI_GFX_FLASCC
		#define MOAI_GFX_FLASCC 0
	#endif

	#ifndef MOAI_GFX_OPENGL
		#define MOAI_GFX_OPENGL 1
	#endif
	
#endif
