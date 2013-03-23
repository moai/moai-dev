cmake_minimum_required ( VERSION 2.8.5 )
project ( flascc-host )

set ( APP_SRC "../../../samples/input/input-callbacks/")

include_directories ( 
  "../../src/"
)

link_directories (
  "third-party"
)


set ( CMAKE_CURRENT_SOURCE_DIR "../../src/hosts/flascc" )
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GNU_SOURCE=1 -D__FLASCC__ -pthread -emit-swf -symbol-abc=Console.abc -swf-size=800x600 MoaiVFS.abc ${FLASCC}/usr/lib/AlcVFSZip.abc" )

# Generate VFS
add_custom_command (
  OUTPUT ${PROJECT_BINARY_DIR}/moaiRootFSBackingStore.as
  COMMAND ${FLASCC}/usr/bin/genfs ${APP_SRC} moai
)
add_custom_target ( moaiRootFSBackingStore.as DEPENDS "${PROJECT_BINARY_DIR}/moaiRootFSBackingStore.as" )

add_custom_command (
  OUTPUT ${PROJECT_BINARY_DIR}/MoaiVFS.abc
  COMMAND ${JAVA} -jar ${AS3COMPILER} -merge -md -import ${FLASCC}/usr/lib/builtin.abc -import ${FLASCC}/usr/lib/playerglobal.abc -import ${FLASCC}/usr/lib/BinaryData.abc -import ${FLASCC}/usr/lib/ISpecialFile.abc -import ${FLASCC}/usr/lib/IBackingStore.abc -import ${FLASCC}/usr/lib/IVFS.abc -import ${FLASCC}/usr/lib/InMemoryBackingStore.abc -import ${FLASCC}/usr/lib/PlayerKernel.abc ${FLASCC}/usr/share/LSOBackingStore.as moaiRootFSBackingStore.as -outdir . -out MoaiVFS
  DEPENDS ${PROJECT_BINARY_DIR}/moaiRootFSBackingStore.as
)
add_custom_target ( MoaiVFS.abc DEPENDS "${PROJECT_BINARY_DIR}/MoaiVFS.abc" )

# Compile Console.as into Console.abc
add_custom_command (
  OUTPUT ${PROJECT_BINARY_DIR}/Console.abc
  COMMAND ${JAVA} -jar ${AS3COMPILER} -merge -md -AS3 -strict -optimize -import ${FLASCC}/usr/lib/builtin.abc -import ${FLASCC}/usr/lib/playerglobal.abc -import ${GLS3D}/install/usr/lib/libGL.abc -import ${FLASCC}/usr/lib/ISpecialFile.abc -import ${FLASCC}/usr/lib/IBackingStore.abc -import ${FLASCC}/usr/lib/IVFS.abc -import ${FLASCC}/usr/lib/InMemoryBackingStore.abc -import ${FLASCC}/usr/lib/AlcVFSZip.abc -import ${FLASCC}/usr/lib/CModule.abc -import ${FLASCC}/usr/lib/C_Run.abc -import ${FLASCC}/usr/lib/BinaryData.abc -import ${FLASCC}/usr/lib/PlayerKernel.abc -import MoaiVFS.abc ../${CMAKE_CURRENT_SOURCE_DIR}/Console.as -outdir . -out Console
  DEPENDS MoaiVFS.abc
)
add_custom_target ( Console.abc DEPENDS "${PROJECT_BINARY_DIR}/Console.abc" )



add_executable ( flascc-host.swf 
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
add_dependencies ( flascc-host.swf Console.abc )






# target_link_libraries ( flascc-host.swf aku moai-core uslscore zlcore moai-untz untz Flash++ AS3++ SDL GL m vgl third-party )
 target_link_libraries ( flascc-host.swf aku moai-core zlcore uslscore Flash++ AS3++ m third-party )