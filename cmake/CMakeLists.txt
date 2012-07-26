cmake_minimum_required(VERSION 2.8.5)
project(moai-dev)

# Main version information.
set(VERSION_STAGE "linux")
execute_process(COMMAND git rev-parse HEAD OUTPUT_VARIABLE git_hash)
if("${git_hash}" STREQUAL "")
    add_definitions(-DVERSION_STAGE=${VERSION_STAGE})
else("${git_hash}" STREQUAL "")
    string(STRIP ${git_hash} git_hash)
    string(SUBSTRING ${git_hash} 0 9 git_hash)
    set(VERSION_GIT ${git_hash})
    add_definitions(-DVERSION_STAGE=${VERSION_STAGE} -DVERSION_GIT=${VERSION_GIT})
endif("${git_hash}" STREQUAL "")

# Set up the basics.
set(IS_ROOT 1)
get_filename_component(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../" ABSOLUTE)

# Find globally required packages.
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Show configuration.
message("-- Showing configuration")
if(DEFINED VERSION_GIT)
    message("  >> version is: '${VERSION_STAGE}; #${VERSION_GIT}'")
else(DEFINED VERSION_GIT)
    message("  >> version is: '${VERSION_STAGE}'")
endif(DEFINED VERSION_GIT)
message("  >> root folder is ${ROOT_DIR}")
if(NOT "${EXPERIMENTAL}" STREQUAL "")
message("  >> experimental features are enabled!")
set(EXPERIMENTAL true)
endif(NOT "${EXPERIMENTAL}" STREQUAL "")

# Define a function that automatically includes
# all of the .h and .c files in the project folder.
function(moai_project type)
    # Initialize.
    set(${PROJECT_NAME}_all "")

    # Determine base source path for this project.
    set(${PROJECT_NAME}_path "${ROOT_DIR}/src/${PROJECT_NAME}")

    # Add all source and header files first.
    file(GLOB ${PROJECT_NAME}_source_c ${${PROJECT_NAME}_path}/*.c)
    file(GLOB ${PROJECT_NAME}_source_cpp ${${PROJECT_NAME}_path}/*.cpp)
    file(GLOB ${PROJECT_NAME}_header_c ${${PROJECT_NAME}_path}/*.h)
    file(GLOB ${PROJECT_NAME}_header_cpp ${${PROJECT_NAME}_path}/*.hpp)

    # Output messages.
    if("${type}" MATCHES "^executable$")
        message("  >> ${PROJECT_NAME}: executable")
    elseif("${type}" MATCHES "^library$")
        message("  >> ${PROJECT_NAME}: library")
    else("${type}" MATCHES "^executable$")
        message(FATAL_ERROR "Unknown project type definition for
${PROJECT_NAME}")
    endif("${type}" MATCHES "^executable$")

    # Add any additional directories.
    if(NOT 1 EQUAL ${ARGC})
        math(EXPR ARGC_OFFSET "${ARGC} - 1")
        foreach(v RANGE 1 ${ARGC_OFFSET})
            set(i ${ARGV${v}})
            message("    .. including '${i}'")

            # Search for all C / C++ files in the directory.
            file(GLOB ${PROJECT_NAME}_${i}_source_c ${${PROJECT_NAME}_path}/${i}/*.c)
            file(GLOB ${PROJECT_NAME}_${i}_source_cpp ${${PROJECT_NAME}_path}/${i}/*.cpp)
            file(GLOB ${PROJECT_NAME}_${i}_header_c ${${PROJECT_NAME}_path}/${i}/*.h)
            file(GLOB ${PROJECT_NAME}_${i}_header_cpp ${${PROJECT_NAME}_path}/${i}/*.hpp)

            # Set groupings for IDEs.
            source_group("Source Files\\\\${i}" FILES ${${PROJECT_NAME}_${i}_source_c}
                                                      ${${PROJECT_NAME}_${i}_source_cpp})
            source_group("Header Files\\\\${i}" FILES ${${PROJECT_NAME}_${i}_header_c}
                                                      ${${PROJECT_NAME}_${i}_header_cpp})

            # Append to source code list.
            set(${PROJECT_NAME}_all ${${PROJECT_NAME}_all}
                                    ${${PROJECT_NAME}_${i}_source_c}
                                    ${${PROJECT_NAME}_${i}_source_cpp}
                                    ${${PROJECT_NAME}_${i}_header_c}
                                    ${${PROJECT_NAME}_${i}_header_cpp})
        endforeach(v RANGE 1 ${ARGC})
    endif(NOT 1 EQUAL ${ARGC})

    # Join it all together.
    set(${PROJECT_NAME}_all ${${PROJECT_NAME}_all}
                            ${${PROJECT_NAME}_source_c}
                            ${${PROJECT_NAME}_source_cpp}
                            ${${PROJECT_NAME}_header_c}
                            ${${PROJECT_NAME}_header_cpp})

    # Set groupings for IDEs.
    source_group("Source Files" FILES ${${PROJECT_NAME}_source_c}
                                      ${${PROJECT_NAME}_source_cpp})
    source_group("Header Files" FILES ${${PROJECT_NAME}_header_c}
                                      ${${PROJECT_NAME}_header_cpp})

    # Create either an executable or library.
    if("${type}" MATCHES "^executable$")
        add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_all})
    elseif("${type}" MATCHES "^library$")
        add_library(${PROJECT_NAME} ${${PROJECT_NAME}_all})
    else("${type}" MATCHES "^executable$")
        message(FATAL_ERROR "Unknown project type definition for ${PROJECT_NAME}")
    endif("${type}" MATCHES "^executable$")
    set_target_properties(${PROJECT_NAME} PROPERTIES include_path "${CMAKE_CURRENT_SOURCE_DIR}")
endfunction(moai_project type)

# Define a function that manually includes file paths.
function(moai_manual type)
    # Output messages.
    if("${type}" MATCHES "^executable$")
        message("  >> ${PROJECT_NAME}: executable (manual)")
    elseif("${type}" MATCHES "^library$")
        message("  >> ${PROJECT_NAME}: library (manual)")
    else("${type}" MATCHES "^executable$")
        message(FATAL_ERROR "Unknown project type definition for
${PROJECT_NAME}")
    endif("${type}" MATCHES "^executable$")

    # Create either an executable or library.
    if("${type}" MATCHES "^executable$")
        add_executable(${PROJECT_NAME} ${ARGN})
    elseif("${type}" MATCHES "^library$")
        add_library(${PROJECT_NAME} ${ARGN})
    else("${type}" MATCHES "^executable$")
        message(FATAL_ERROR "Unknown project type definition for ${PROJECT_NAME}")
    endif("${type}" MATCHES "^executable$")
endfunction(moai_manual type)

# Define a function which specifies a dependency on
# the project.
function(moai_needs_local dependency)
    get_target_property(${dependency}_include_path ${dependency} include_path)
    if("${${dependency}_include_path}" MATCHES "NOTFOUND")
        # This is potentially a prebuilt library (like libtcod
        # which varies).
        moai_needs_prebuilt(${dependency})
    else("${${dependency}_include_path}" MATCHES "NOTFOUND")
        # We've found this as an internal dependency.
        string(LENGTH ${ROOT_DIR} root_dir_length)
        math(EXPR root_dir_length "${root_dir_length} + 1")
        string(SUBSTRING "${${dependency}_include_path}" "${root_dir_length}" -1 pretty_include_path)
        message("    .. adding '${dependency}' include at ${pretty_include_path}")
        add_dependencies(${PROJECT_NAME} ${dependency})
        include_directories("${${dependency}_include_path}")
        target_link_libraries(${PROJECT_NAME} ${dependency})
    endif("${${dependency}_include_path}" MATCHES "NOTFOUND")
endfunction(moai_needs_local)

# Define a function which specifies a **pre-built**
# dependency on the project.
function(moai_needs dependency)
    set(should_include_headers true)

    # Check to see if we should exclude distributed headers on this platform.
    if(UNIX)
        if(NOT DEFINED ${dependency}_UnixInclude)
            set(should_include_headers false)
        endif(NOT DEFINED ${dependency}_UnixInclude)
    endif(UNIX)
    if(${should_include_headers})
        message("    .. adding '${dependency}' library at ${${dependency}_UnixInclude}")
        include_directories("${${dependency}_UnixInclude}")
    else(${should_include_headers})
        #message("    .. skipping inclusion of distributed '${dependency}' headers on this platform.")
    endif(${should_include_headers})

    # Add include and library dependencies.
    if(DEFINED ${dependency}_Unix)
        if(NOT ${should_include_headers})
            message("    .. adding '${dependency}' library")
        endif(NOT ${should_include_headers})
        find_library(${${dependency}_Unix} ${${dependency}_Unix})
        target_link_libraries(${PROJECT_NAME} ${${dependency}_Unix})
    else(DEFINED ${dependency}_Unix)
        if(NOT ${should_include_headers})
            message("    .. adding '${dependency}' library")
        endif(NOT ${should_include_headers})
        target_link_libraries(${PROJECT_NAME} ${dependency})
        #message(FATAL_ERROR "    .. dependency '${dependency}' could not be satisified on your platform")
    endif(DEFINED ${dependency}_Unix)
endfunction(moai_needs dependency)

# Add library folders.
add_subdirectory(box2d)
add_subdirectory(chipmunk)
add_subdirectory(expat)
add_subdirectory(freetype)
add_subdirectory(glew)
add_subdirectory(jansson)
add_subdirectory(zlib)
add_subdirectory(libcrypto)
add_subdirectory(libssl)
add_subdirectory(libcurl)
add_subdirectory(libcares)
add_subdirectory(libjpg)
add_subdirectory(libpng)
add_subdirectory(libogg)
add_subdirectory(libvorbis)
add_subdirectory(lua-lib)
add_subdirectory(luaext)
add_subdirectory(contrib)
add_subdirectory(tlsf)
add_subdirectory(zlcore)
add_subdirectory(ooid)
add_subdirectory(uslscore)
add_subdirectory(tinyxml)
add_subdirectory(moaicore)
add_subdirectory(aku)

# Add executable folders.
add_subdirectory(moai)
add_subdirectory(glewinfo)
add_subdirectory(visualinfo)
add_subdirectory(lua)
add_subdirectory(luac)