cmake_minimum_required ( VERSION 2.8.5 )
project ( flascc-host )

include_directories ( 
  "../../src/"
  "../../3rdparty/GLS3D/install/usr/include/"
  "${FLASCC_SDK}/usr/include/SDL"
)

link_directories (
  "../../3rdparty/GLS3D/install/usr/lib/"
  "third-party"
)

set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__FLASCC__ -pthread -emit-swf" )

set ( CMAKE_CURRENT_SOURCE_DIR "../../src/hosts/flascc" )

add_executable ( flascc-host.swf
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_link_libraries ( flascc-host.swf aku moai-core uslscore zlcore third-party Flash++ AS3++ SDL GL vgl )