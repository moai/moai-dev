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


    set (LUAJIT_ROOT "${MOAI_ROOT}/3rdparty/luajit-2.0.2")
    set (LUAJIT_INCLUDE "${CURRENT_BINARY_DIRECTORY}/luajit/src")

    set (luajit_buildoptions BUILDMODE=static)
    set (luajit_copt "")
    if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        set (luajit_buildoptions ${luajit_buildoptions} CCDEBUG=${CC_DEBUG_OPT})
        set (luajit_copt ${luajit_copt} -O1)
        set (luajit_buildoptions ${luajit_buildoptions} XCFLAGS='-DLUA_USE_APICHECK -DLUA_USE_ASSERT')
    else ()
        set (luajit_copt ${luajit_copt} -O2)
    endif()
    #set (luajit_copt ${luajit_copt} -I${PROJECT_SOURCE_DIR}/libobjc)
    set (luajit_target_cc "${CMAKE_C_COMPILER} ${CMAKE_C_FLAGS}")
    # Use external unwind on all platforms.
    #set (luajit_target_cc "${luajit_target_cc} -DLUAJIT_UNWIND_EXTERNAL=1")

    if (MINGW)
        set(MAKE_COMMAND "mingw32-make.exe")
    else()
        set(MAKE_COMMAND "make")
    endif()

    if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL ${CMAKE_HOST_SYSTEM_PROCESSOR})
        # Regular mode - use CMake compiler for building host utils.
        set (luajit_host_cc ${CMAKE_C_COMPILER})
    else()
        # Crosscompile mode - use a host CC compiler for building host utils.
        # Since CMake does not support cross compilation properly
        # we have to use system CC here.
        set (luajit_host_cc "cc")
    endif()
    if (${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "x86_64" AND
            ${CMAKE_SIZEOF_VOID_P} EQUAL 4)
        # The host compiler must have same pointer size as the target compiler.
        set (luajit_host_cc "${luajit_host_cc} -m32")
    endif()
    set(luajit_ldflags "${CMAKE_SHARED_LINKER_FLAGS}")
    separate_arguments(luajit_copt)
    separate_arguments(luajit_ldflags)
    separate_arguments(luajit_host_cc)
    separate_arguments(luajit_target_cc)
    set (luajit_buildoptions ${luajit_buildoptions}
        CFLAGS=""
        CXXFLAGS=""
        HOST_CC="${luajit_host_cc}"
        TARGET_CC="${luajit_target_cc}"
        CCOPT="${luajit_copt}")
    set (luajit_buildoptions ${luajit_buildoptions} Q='' LDFLAGS="${luajit_ldflags}")
    
    add_custom_command(OUTPUT ${PROJECT_BINARY_DIR}/luajit
        COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/luajit"
    )
    add_custom_command(OUTPUT ${PROJECT_BINARY_DIR}/luajit/src/libluajit.a
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/luajit
        COMMAND ${CMAKE_COMMAND} -E copy_directory  ${LUAJIT_ROOT}/* .
        COMMAND ${MAKE_COMMAND}  clean  ${luajit_buildoptions}
        COMMAND ${MAKE_COMMAND} -C src ${luajit_buildoptions}
        DEPENDS ${PROJECT_BINARY_DIR}/CMakeCache.txt ${PROJECT_BINARY_DIR}/luajit
    )
    
    add_custom_target(libluajit
        DEPENDS ${PROJECT_BINARY_DIR}/luajit/src/libluajit.a
    )

