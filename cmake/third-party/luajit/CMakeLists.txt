cmake_minimum_required(VERSION 2.8)
#
# LuaJIT configuration file.
#
# A copy of LuaJIT is maintained within Tarantool
# source. It's located in third_party/luajit.
#
# Instead of this copy, Tarantool can be compiled
# with a system-wide LuaJIT, or LuaJIT at a given
# prefix. This is used when compiling Tarantool
# as part of a distribution, e.g. Debian.
#
# To explicitly request use of the bundled LuaJIT,
# add -DENABLE_BUNDLED_LUAJIT=True to CMake
# configuration flags.
# To explicitly request use of LuaJIT at a given


    set (LUAJIT_ROOT "${MOAI_ROOT}/3rdparty/LuaJIT-2.0.1")
    set (LUAJIT_INCLUDE "${LUAJIT_ROOT}/src")

    set (luajit_buildoptions BUILDMODE=static)
    set (luajit_copt "")
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set (luajit_buildoptions ${luajit_buildoptions} CCDEBUG=${CC_DEBUG_OPT})
        set (luajit_copt ${luajit_copt} -O1)
        set (luajit_buildoptions ${luajit_buildoptions} XCFLAGS="-DLUA_USE_APICHECK -DLUA_USE_ASSERT")
    else ()
        set (luajit_copt ${luajit_copt} -O2)
    endif()
    #set (luajit_copt ${luajit_copt} -I${PROJECT_SOURCE_DIR}/libobjc)


    #remove our forced include from target_cc commands
    string(REPLACE "${VFS_INCLUDE}" " " VFS_C_FLAGS ${CMAKE_C_FLAGS} )

    set (luajit_target_cc "${CMAKE_C_COMPILER} ${VFS_C_FLAGS}")
    set (luajit_conly_flags "${VFS_INCLUDE}")

    # Use external unwind on all platforms.
    #set (luajit_target_cc "${luajit_target_cc} -DLUAJIT_UNWIND_EXTERNAL=1")

    if (BUILD_WINDOWS)
        set(GCC_FORCE_32BIT True)
    endif(BUILD_WINDOWS)

    set(luajit_host_ldflags "")
    set(luajit_target_ldflags "${CMAKE_SHARED_LINKER_FLAGS}")

    




    set(CFLAGS "")
    
    if (GCC_FORCE_32BIT)
        set (CFLAGS "-m32")
        set (luajit_target_ldflags "${luajit_target_ldflags} -m32")
    endif(GCC_FORCE_32BIT)


    if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL ${CMAKE_HOST_SYSTEM_PROCESSOR})
        # Regular mode - use CMake compiler for building host utils.
        set (luajit_host_cc ${CMAKE_C_COMPILER})
        set (luajit_host_ldflags ${luajit_target_ldflags} )
    else()
        # Crosscompile mode - use a host CC compiler for building host utils.
        # Since CMake does not support cross compilation properly
        # we have to use system CC here.
        
          set (luajit_host_cc "gcc")
    endif()
    

    message( "Size of void P =  ${CMAKE_SIZEOF_VOID_P} and ${CMAKE_HOST_SYSTEM_PROCESSOR}")
    if (${CMAKE_HOST_SYSTEM_PROCESSOR} MATCHES "64" AND
            ${CMAKE_SIZEOF_VOID_P} EQUAL 4 OR GCC_FORCE_32BIT)
        # The host compiler must have same pointer size as the target compiler.

        set (luajit_host_cc "${luajit_host_cc} -m32")
        set (luajit_host_ldflags "${luajit_host_ldflags} -m32")
      
    endif()

    if (BUILD_IOS)
        set (luajit_buildoptions ${luajit_buildoptions}
             TARGET_SYS=iOS)
        #our sysroot
        EXEC_PROGRAM( xcrun ARGS --show-sdk-path
                      OUTPUT_VARIABLE HOST_SDK_ROOT)

        set ( luajit_host_cc ${HOST_SDK_ROOT}/../../)
        message("Host SDK ROOT ${HOST_SDK_ROOT}")
        set (luajit_host_cc "gcc  -arch i386  -I${HOST_SDK_ROOT}/usr/include ")
        set (luajit_host_ldflags "${luajit_host_ldflags}")
    endif()
    
    if (BUILD_ANDROID) 

        set (luajit_buildoptions ${luajit_buildoptions}
             TARGET_SYS=Linux)
        message( "compiler ${CMAKE_C_COMPILER}")
        set(CROSS "")
        string( REGEX REPLACE "(.*-)gcc(\\.exe)*" "\\1"  CROSS ${CMAKE_C_COMPILER})
        set (luajit_buildoptions ${luajit_buildoptions} CROSS=${CROSS})
        set (luajit_target_ldflags  "--sysroot=${ANDROID_SYSROOT}")
    endif()

    separate_arguments(CFLAGS)
    separate_arguments(luajit_copt)
    separate_arguments(luajit_ldflags)
    separate_arguments(luajit_host_cc)
    separate_arguments(luajit_target_cc)
    separate_arguments(luajit_host_ldflags)
    separate_arguments(luajit_target_ldflags)
    separate_arguments(luajit_conly_flags)
    set (luajit_buildoptions ${luajit_buildoptions}
        CFLAGS="${CFLAGS}"
        CXXFLAGS=""
        HOST_CC="${luajit_host_cc}"
        STATIC_CC="${luajit_target_cc}"
        DYNAMIC_CC="${luajit_target_cc}"
        CCOPT="${luajit_copt}"
        TARGET_CONLY_FLAGS="${luajit_conly_flags}"
        TARGET_LDFLAGS="${luajit_target_ldflags}"
        HOST_LDFLAGS="${luajit_host_ldflags}"
   )
    #set (luajit_buildoptions ${luajit_buildoptions} ) #Q=''
    
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/luajit
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/luajit"
        COMMAND ${CMAKE_COMMAND} -E copy_directory  ${LUAJIT_ROOT} "${CMAKE_CURRENT_BINARY_DIR}/luajit"
    )

    if (MSVC)
	      add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/luajit/src/lua51.lib
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/luajit/src
            COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/msvcbuild-moai.bat" static ${VFS_INCLUDE}
            DEPENDS  ${CMAKE_CURRENT_BINARY_DIR}/luajit 
        ) 

        add_custom_target(liblua-static
            DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/luajit/src/lua51.lib
        )
        set( LUA_LIB ${CMAKE_CURRENT_BINARY_DIR}/luajit/src/lua51.lib PARENT_SCOPE )
    elseif(BUILD_IOS)

        add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/luajit/src/libluajit.a
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/luajit/src
            COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/luajit_ios.sh libluajit.a '${CMAKE_CURRENT_BINARY_DIR}/luajit' '${CMAKE_CURRENT_BINARY_DIR}/luajit/src' '${VFS_INCLUDE}'
            DEPENDS  ${CMAKE_CURRENT_BINARY_DIR}/luajit 
        ) 

        add_custom_target(liblua-static
            DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/luajit/src/libluajit.a
        )
        set( LUA_LIB ${CMAKE_CURRENT_BINARY_DIR}/luajit/src/libluajit.a PARENT_SCOPE )

    else()
        add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/luajit/src/libluajit.a
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/luajit/src
            COMMAND $(MAKE) VERBOSE=1 clean ${luajit_buildoptions}
            COMMAND $(MAKE) VERBOSE=1 ${luajit_buildoptions} libluajit.a
            DEPENDS  ${CMAKE_CURRENT_BINARY_DIR}/luajit 
        ) 

        add_custom_target(liblua-static
            DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/luajit/src/libluajit.a
        )
        set( LUA_LIB ${CMAKE_CURRENT_BINARY_DIR}/luajit/src/libluajit.a PARENT_SCOPE )
    endif()
    
   set_target_properties( liblua-static PROPERTIES FOLDER Third-Party )

   # add_library(liblua-static STATIC IMPORTED)
   # set_property(TARGET liblua-static PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/luajit/src/libluajit.a)
   # add_dependencies(liblua-static libluajit)

   set( LUA_HEADERS ${LUAJIT_INCLUDE}  PARENT_SCOPE)
    