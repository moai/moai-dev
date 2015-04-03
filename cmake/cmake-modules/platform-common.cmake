# Virtual File System Force include
# this should force all of moai to have this include and then we remove it from flags in zl-vfs
# easier than adding it to each project!

if (LIB_PATH)
	if (NOT MSVC)
		set (VFS_INCLUDE "-I${LIB_PATH}/include -include ${LIB_PATH}/include/zl-vfs/zl_replace.h")
	else (NOT MSVC)
		set (VFS_INCLUDE "-I${LIB_PATH}/include /FI${LIB_PATH}/include/zl-vfs/zl_replace.h")
	endif (NOT MSVC)
else (LIB_PATH)
	if (NOT MSVC)
		set (VFS_INCLUDE "-I${MOAI_ROOT}/src -include ${MOAI_ROOT}/src/zl-vfs/zl_replace.h")
	else (NOT MSVC)
		set (VFS_INCLUDE "-I${MOAI_ROOT}/src /FI${MOAI_ROOT}/src/zl-vfs/zl_replace.h")
	endif (NOT MSVC)
endif (LIB_PATH)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${VFS_INCLUDE}")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${VFS_INCLUDE}")


#
# Set as debug
#
set ( DEBUG OFF )

#
# Build testing code
#
set ( TESTING OFF )


