option(BOX2D_INSTALL "Install Box2D libs, includes, and CMake scripts" OFF)
option(BOX2D_BUILD_EXAMPLES "Build Box2D examples" OFF)
add_subdirectory(box2d-2.1.2/Box2D)

option(BUILD_DEMOS "Build the demo applications" OFF)
option(INSTALL_STATIC "Install the static library" OFF)
add_subdirectory(chipmunk-5.3.4)

add_subdirectory(contrib)
add_subdirectory(expat-2.0.1)
add_subdirectory(glew-1.5.6)
add_subdirectory(sqlite-3.6.16)
add_subdirectory(tinyxml)
