  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMOAI_OS_HTML -DGL_ES -D_GNU_SOURCE -s LINKABLE=1 -Wno-warn-absolute-paths" )
  SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMOAI_OS_HTML -DGL_ES -D_GNU_SOURCE -s LINKABLE=1 -Wno-warn-absolute-paths"   )

  if ( CMAKE_HOST_WIN32 )
    #Workaround batchfiles (cmd.exe) not being able to handle long command lines

    SET(CMAKE_C_CREATE_STATIC_LIBRARY
      "python ${EMSCRIPTEN_ROOT_PATH}/emar cr <TARGET> <LINK_FLAGS> <OBJECTS> "
      "<CMAKE_RANLIB> <TARGET> ")

    SET(CMAKE_CXX_CREATE_STATIC_LIBRARY
      "python ${EMSCRIPTEN_ROOT_PATH}/emar cr <TARGET> <LINK_FLAGS> <OBJECTS> "
      "<CMAKE_RANLIB> <TARGET> ")
  endif ( CMAKE_HOST_WIN32 )