
if (LIB_PATH) 
  include(${LIB_PATH}/cmake/libmoai.cmake)
  link_directories(${LIB_PATH}/lib)
else(LIB_PATH)
  add_subdirectory(../../libmoai libmoai)
endif (LIB_PATH)

