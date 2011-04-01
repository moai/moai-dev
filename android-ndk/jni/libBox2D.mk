#================================================================#
# libBox2D
#================================================================#
include $(CLEAR_VARS)

LOCAL_MODULE := libBox2D
LOCAL_LDLIBS := -llog
# LOCAL_CFLAGS := -std=c99

#----------------------------------------------------------------#
# header search paths
#----------------------------------------------------------------#
LOCAL_C_INCLUDES := $(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D \
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D \
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Collision/Shapes \
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Collision \
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Common \
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics \
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/Contacts \
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/Joints \

#----------------------------------------------------------------#
# included files
#----------------------------------------------------------------#
LOCAL_SRC_FILES := $(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Collision/Shapes/b2CircleShape.cpp \
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Collision/Shapes/b2PolygonShape.cpp \
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Collision/b2BroadPhase.cpp \
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Collision/b2CollideCircle.cpp \
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Collision/b2CollidePolygon.cpp\
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Collision/b2Collision.cpp\
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Collision/b2Distance.cpp\
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Collision/b2DynamicTree.cpp\
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Collision/b2TimeOfImpact.cpp\
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Common/b2BlockAllocator.cpp\
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Common/b2Math.cpp\
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Common/b2Settings.cpp\
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Common/b2StackAllocator.cpp\
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/Contacts/b2CircleContact.cpp\
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/Contacts/b2Contact.cpp\
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/Contacts/b2ContactSolver.cpp\
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.cpp\
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/Contacts/b2PolygonContact.cpp\
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/Contacts/b2TOISolver.cpp\
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/Joints/b2DistanceJoint.cpp\
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/Joints/b2FrictionJoint.cpp\
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/Joints/b2GearJoint.cpp\
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/Joints/b2Joint.cpp\
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/Joints/b2LineJoint.cpp\
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/Joints/b2MouseJoint.cpp\
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/Joints/b2PrismaticJoint.cpp\
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/Joints/b2PulleyJoint.cpp\
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/Joints/b2RevoluteJoint.cpp\
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/Joints/b2WeldJoint.cpp\
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/b2Body.cpp\
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/b2ContactManager.cpp\
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/b2Fixture.cpp\
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/b2Island.cpp\
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/b2World.cpp\
$(SRC_PATH)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/b2WorldCallbacks.cpp

include $(BUILD_STATIC_LIBRARY)