cmake_minimum_required ( VERSION 2.8.5 )
project ( mongoose )

set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/3rdparty/mongoose/" )

if ( BUILD_WINDOWS )
  set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DHAVE_STDINT=1" )
endif ( BUILD_WINDOWS )

set ( MONGOOSE_INCLUDES "${MOAI_ROOT}/3rdparty/mongoose/"  )

set ( MONGOOSE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/mongoose.c )
set ( MONGOOSE_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/mongoose.h )

add_library ( mongoose STATIC ${MONGOOSE_SOURCES} ${MONGOOSE_HEADERS} )
target_include_directories ( mongoose PUBLIC "$<BUILD_INTERFACE:${MONGOOSE_INCLUDES}>" )
set_target_properties ( mongoose PROPERTIES FOLDER Third-Party )
 install(TARGETS mongoose EXPORT libmoai ARCHIVE DESTINATION lib)