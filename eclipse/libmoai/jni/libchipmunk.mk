#================================================================#
# libchipmunk
#================================================================#
include $(CLEAR_VARS)

LOCAL_MODULE := libchipmunk
LOCAL_LDLIBS := -llog
# LOCAL_CFLAGS	:= -std=c99 -ffast-math -O3 -DNDEBUG
LOCAL_CFLAGS := -std=c99

#----------------------------------------------------------------#
# header search paths
#----------------------------------------------------------------#
LOCAL_C_INCLUDES := $(SRC_PATH)/3rdparty/chipmunk-5.3.4/include/chipmunk \
$(SRC_PATH)/3rdparty/chipmunk-5.3.4/include/chipmunk/constraints

#----------------------------------------------------------------#
# included files
#----------------------------------------------------------------#
LOCAL_SRC_FILES := $(SRC_PATH)/3rdparty/chipmunk-5.3.4/src/chipmunk.c \
$(SRC_PATH)/3rdparty/chipmunk-5.3.4/src/cpArbiter.c\
$(SRC_PATH)/3rdparty/chipmunk-5.3.4/src/cpArray.c\
$(SRC_PATH)/3rdparty/chipmunk-5.3.4/src/cpBB.c\
$(SRC_PATH)/3rdparty/chipmunk-5.3.4/src/cpBody.c\
$(SRC_PATH)/3rdparty/chipmunk-5.3.4/src/cpCollision.c\
$(SRC_PATH)/3rdparty/chipmunk-5.3.4/src/cpHashSet.c\
$(SRC_PATH)/3rdparty/chipmunk-5.3.4/src/cpPolyShape.c\
$(SRC_PATH)/3rdparty/chipmunk-5.3.4/src/cpShape.c\
$(SRC_PATH)/3rdparty/chipmunk-5.3.4/src/cpSpace.c\
$(SRC_PATH)/3rdparty/chipmunk-5.3.4/src/cpSpaceComponent.c\
$(SRC_PATH)/3rdparty/chipmunk-5.3.4/src/cpSpaceHash.c\
$(SRC_PATH)/3rdparty/chipmunk-5.3.4/src/cpSpaceQuery.c\
$(SRC_PATH)/3rdparty/chipmunk-5.3.4/src/cpSpaceStep.c\
$(SRC_PATH)/3rdparty/chipmunk-5.3.4/src/cpVect.c\
$(SRC_PATH)/3rdparty/chipmunk-5.3.4/src/constraints/cpConstraint.c\
$(SRC_PATH)/3rdparty/chipmunk-5.3.4/src/constraints/cpDampedRotarySpring.c\
$(SRC_PATH)/3rdparty/chipmunk-5.3.4/src/constraints/cpDampedSpring.c\
$(SRC_PATH)/3rdparty/chipmunk-5.3.4/src/constraints/cpGearJoint.c\
$(SRC_PATH)/3rdparty/chipmunk-5.3.4/src/constraints/cpGrooveJoint.c\
$(SRC_PATH)/3rdparty/chipmunk-5.3.4/src/constraints/cpPinJoint.c\
$(SRC_PATH)/3rdparty/chipmunk-5.3.4/src/constraints/cpPivotJoint.c\
$(SRC_PATH)/3rdparty/chipmunk-5.3.4/src/constraints/cpRatchetJoint.c\
$(SRC_PATH)/3rdparty/chipmunk-5.3.4/src/constraints/cpRotaryLimitJoint.c\
$(SRC_PATH)/3rdparty/chipmunk-5.3.4/src/constraints/cpSimpleMotor.c\
$(SRC_PATH)/3rdparty/chipmunk-5.3.4/src/constraints/cpSlideJoint.c

include $(BUILD_STATIC_LIBRARY)