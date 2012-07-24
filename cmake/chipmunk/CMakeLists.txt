cmake_minimum_required(VERSION 2.8.5)
project(chipmunk)

# Check to ensure that we are running as part of the
# root CMakeLists.txt file.
if(NOT DEFINED IS_ROOT)
  message(FATAL_ERROR "You must generate the CMake files as part of the root directory.")
endif(NOT DEFINED IS_ROOT)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
set(${PROJECT_NAME}_UnixInclude "${ROOT_DIR}/3rdparty/chipmunk-5.3.4/include/" PARENT_SCOPE)
include_directories("${ROOT_DIR}/3rdparty/chipmunk-5.3.4/include/chipmunk/")
set(CMAKE_CURRENT_SOURCE_DIR "${ROOT_DIR}/3rdparty/chipmunk-5.3.4/src/")

moai_manual(library
    ${CMAKE_CURRENT_SOURCE_DIR}/chipmunk.c
    ${CMAKE_CURRENT_SOURCE_DIR}/constraints/cpConstraint.c
    ${CMAKE_CURRENT_SOURCE_DIR}/constraints/cpDampedRotarySpring.c
    ${CMAKE_CURRENT_SOURCE_DIR}/constraints/cpDampedSpring.c
    ${CMAKE_CURRENT_SOURCE_DIR}/constraints/cpGearJoint.c
    ${CMAKE_CURRENT_SOURCE_DIR}/constraints/cpGrooveJoint.c
    ${CMAKE_CURRENT_SOURCE_DIR}/constraints/cpPinJoint.c
    ${CMAKE_CURRENT_SOURCE_DIR}/constraints/cpPivotJoint.c
    ${CMAKE_CURRENT_SOURCE_DIR}/constraints/cpRatchetJoint.c
    ${CMAKE_CURRENT_SOURCE_DIR}/constraints/cpRotaryLimitJoint.c
    ${CMAKE_CURRENT_SOURCE_DIR}/constraints/cpSimpleMotor.c
    ${CMAKE_CURRENT_SOURCE_DIR}/constraints/cpSlideJoint.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cpArbiter.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cpArray.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cpBB.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cpBody.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cpCollision.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cpHashSet.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cpPolyShape.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cpShape.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cpSpace.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cpSpaceComponent.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cpSpaceHash.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cpSpaceQuery.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cpSpaceStep.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cpVect.c
)