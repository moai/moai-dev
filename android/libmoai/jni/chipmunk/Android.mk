#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	include $(CLEAR_VARS)

	LOCAL_MODULE 		:= chipmunk
	LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)
	LOCAL_CFLAGS		:= -std=c99
	
	LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/chipmunk-5.3.4/src/chipmunk.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/chipmunk-5.3.4/src/cpArbiter.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/chipmunk-5.3.4/src/cpArray.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/chipmunk-5.3.4/src/cpBB.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/chipmunk-5.3.4/src/cpBody.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/chipmunk-5.3.4/src/cpCollision.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/chipmunk-5.3.4/src/cpHashSet.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/chipmunk-5.3.4/src/cpPolyShape.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/chipmunk-5.3.4/src/cpShape.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/chipmunk-5.3.4/src/cpSpace.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/chipmunk-5.3.4/src/cpSpaceComponent.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/chipmunk-5.3.4/src/cpSpaceHash.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/chipmunk-5.3.4/src/cpSpaceQuery.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/chipmunk-5.3.4/src/cpSpaceStep.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/chipmunk-5.3.4/src/cpVect.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/chipmunk-5.3.4/src/constraints/cpConstraint.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/chipmunk-5.3.4/src/constraints/cpDampedRotarySpring.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/chipmunk-5.3.4/src/constraints/cpDampedSpring.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/chipmunk-5.3.4/src/constraints/cpGearJoint.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/chipmunk-5.3.4/src/constraints/cpGrooveJoint.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/chipmunk-5.3.4/src/constraints/cpPinJoint.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/chipmunk-5.3.4/src/constraints/cpPivotJoint.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/chipmunk-5.3.4/src/constraints/cpRatchetJoint.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/chipmunk-5.3.4/src/constraints/cpRotaryLimitJoint.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/chipmunk-5.3.4/src/constraints/cpSimpleMotor.c
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/chipmunk-5.3.4/src/constraints/cpSlideJoint.c

	include $(BUILD_STATIC_LIBRARY)