#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	include $(CLEAR_VARS)

	LOCAL_MODULE 		:= box2D
	LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)

	LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/box2d-2.1.2/Box2D/Box2D/Collision/Shapes/b2CircleShape.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/box2d-2.1.2/Box2D/Box2D/Collision/Shapes/b2PolygonShape.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/box2d-2.1.2/Box2D/Box2D/Collision/b2BroadPhase.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/box2d-2.1.2/Box2D/Box2D/Collision/b2CollideCircle.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/box2d-2.1.2/Box2D/Box2D/Collision/b2CollidePolygon.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/box2d-2.1.2/Box2D/Box2D/Collision/b2Collision.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/box2d-2.1.2/Box2D/Box2D/Collision/b2Distance.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/box2d-2.1.2/Box2D/Box2D/Collision/b2DynamicTree.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/box2d-2.1.2/Box2D/Box2D/Collision/b2TimeOfImpact.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/box2d-2.1.2/Box2D/Box2D/Common/b2BlockAllocator.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/box2d-2.1.2/Box2D/Box2D/Common/b2Math.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/box2d-2.1.2/Box2D/Box2D/Common/b2Settings.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/box2d-2.1.2/Box2D/Box2D/Common/b2StackAllocator.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/Contacts/b2CircleContact.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/Contacts/b2Contact.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/Contacts/b2ContactSolver.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/Contacts/b2PolygonContact.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/Contacts/b2TOISolver.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/Joints/b2DistanceJoint.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/Joints/b2FrictionJoint.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/Joints/b2GearJoint.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/Joints/b2Joint.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/Joints/b2LineJoint.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/Joints/b2MouseJoint.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/Joints/b2PrismaticJoint.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/Joints/b2PulleyJoint.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/Joints/b2RevoluteJoint.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/Joints/b2WeldJoint.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/b2Body.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/b2ContactManager.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/b2Fixture.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/b2Island.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/b2World.cpp
	LOCAL_SRC_FILES 	+= $(MY_MOAI_ROOT)/3rdparty/box2d-2.1.2/Box2D/Box2D/Dynamics/b2WorldCallbacks.cpp

	include $(BUILD_STATIC_LIBRARY)