  # set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -V4.6.3,gcc_ntoarmv7le -w1 -D__QNX__")
  # set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__QNX__ -V4.6.3,gcc_ntoarmv7le_cpp")
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__QNX__ -Wc,-std=gnu99 -DNDEBUG -DHAVE_MEMMOVE ")
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__QNX__" )

  set ( CMAKE_RANLIB $ENV{QNX_HOST}/usr/bin/ntoarmv7-ranlib )
  set ( CMAKE_AR $ENV{QNX_HOST}/usr/bin/ntoarmv7-ar )
  set ( BLACKBERRY_INCLUDES "$ENV{QNX_TARGET}/usr/include;$ENV{QNX_TARGET}/usr/include/freetype2" )