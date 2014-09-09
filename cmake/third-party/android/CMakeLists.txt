#Android NDK has some uncompiled libs that some third party libraries need
project( cpufeatures )

add_library( cpufeatures STATIC "${ANDROID_NDK}/sources/android/cpufeatures/cpu-features.c" )
target_include_directories (cpufeatures PUBLIC "${ANDROID_NDK}/sources/android/cpufeatures" )
