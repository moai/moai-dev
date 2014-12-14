 set ( CMAKE_OSX_SYSROOT iphoneos )
  set ( CMAKE_OSX_ARCHITECTURES "armv7;i386" )
  set ( CMAKE_XCODE_EFFECTIVE_PLATFORMS "-iphoneos;-iphonesimulator" )
  set ( XCODE_ATTRIBUTE_SDKROOT iphoneos )

  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99  -DHAVE_MEMMOVE " )
  #default in ios7 is libc++ but some of our ad support libs are libstdc++ so force the lot to build as libstdc++
  #set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++" )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++" )