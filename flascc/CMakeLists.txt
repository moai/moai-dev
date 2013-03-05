cmake_minimum_required ( VERSION 2.8.5 )
project ( moai )

#
# Multi-platform flags
#
set ( FLASCC_BUILD    true )

set ( FLASCC_SDK "/Users/nictuku/flascc/sdk")

add_subdirectory(third-party)
add_subdirectory(zlcore)
add_subdirectory(uslscore)
add_subdirectory(moai-core)
add_subdirectory(untz)
add_subdirectory(aku)