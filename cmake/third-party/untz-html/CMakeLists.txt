cmake_minimum_required ( VERSION 2.8.5 )
project ( untz )

if ( MOAI_UNTZ )

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_UNTZ=1" PARENT_SCOPE )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_UNTZ=1" PARENT_SCOPE )
    set ( UNTZ_INCLUDES "${MOAI_ROOT}/3rdparty/untz/include/;${MOAI_ROOT}/3rdparty/untz/src/;${MOAI_ROOT}/3rdparty/untz/src/native/sdl/" PARENT_SCOPE )
	include_directories (
		${UNTZ_INCLUDES}	
	)
	set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/untz/webuntz/" )

	set( UNTZ_SOURCES 
  		${CMAKE_CURRENT_SOURCE_DIR}/UntzSound.cpp
  		${CMAKE_CURRENT_SOURCE_DIR}/UntzSystem.cpp 
		PARENT_SCOPE
	)      
	


else ( MOAI_UNTZ )

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_WITH_UNTZ=0" PARENT_SCOPE )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_WITH_UNTZ=0" PARENT_SCOPE )

endif ( MOAI_UNTZ )
