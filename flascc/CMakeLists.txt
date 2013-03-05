cmake_minimum_required ( VERSION 2.8.5 )
project ( moai )

#
# Multi-platform flags
#
set ( FLASCC_BUILD    true )

add_subdirectory(third-party)
add_subdirectory(zlcore)
add_subdirectory(uslscore)