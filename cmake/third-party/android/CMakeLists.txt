#Android NDK has some uncompiled libs that some third party libraries need
project( cpufeatures )

add_library( cpufeatures STATIC "${ANDROID_NDK}/sources/android/cpufeatures/cpu-features.c" )


target_include_directories (cpufeatures PUBLIC 
 	$<BUILD_INTERFACE:${ANDROID_NDK}/sources/android/cpufeatures>
 	$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/cpufeatures> 
)

install(TARGETS cpufeatures EXPORT libmoai ARCHIVE DESTINATION lib)
file(GLOB CPU_FEATURES_HEADERS "${ANDROID_NDK}/sources/android/cpufeatures/*.h")
install(FILES ${CPU_FEATURES_HEADERS} DESTINATION include/cpufeatures)