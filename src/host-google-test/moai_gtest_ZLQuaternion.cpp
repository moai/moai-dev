//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#include <moai-sim/headers.h>
#include "moai_gtest.h"

//----------------------------------------------------------------//
TEST ( ZLQuaternion, UnitTests ) {
	
	ZLQuaternion quat, quat2, result;
	float x,y,z;
	float testX,testY,testZ;
	float quatS,quatX,quatY,quatZ;

	testX = 32.0f;
	testY = 45.0f;
	testZ = 121.0f;

	quatS = 0.34550971412299136f;
	quatX = 0.824895943273407f;
	quatY = 0.44556658287798756f;
	quatZ = -0.040498647386775055f;

	quat = ZLQuaternion ( testX, testY, testZ );

	ASSERT_TRUE (
		ZLFloat::IsClose ( quat.mS, quatS, EPSILON ) &&
		ZLFloat::IsClose ( quat.mV.mX, quatX, EPSILON ) &&
		ZLFloat::IsClose ( quat.mV.mY, quatY, EPSILON ) &&
		ZLFloat::IsClose ( quat.mV.mZ, quatZ, EPSILON )
	);

	quat.mS	   = quatS;
	quat.mV.mX = quatX;
	quat.mV.mY = quatY;
	quat.mV.mZ = quatZ;

	quat.Get ( x, y, z );

	ASSERT_TRUE (
		ZLFloat::IsClose ( x, testX, EPSILON ) &&
		ZLFloat::IsClose ( y, testY, EPSILON ) &&
		ZLFloat::IsClose ( z, testZ, EPSILON )
	);

	//---------- 
	float testS;

	testS = 0.5432889103326224f;
	testX = 0.6903496693854332f;
	testY = 0.4730377044370305f;
	testZ = 0.0670061494936035f;

	// 45 degrees
	quat2 = ZLQuaternion ( 0.7325378163287418f, 0.46193976625564337f, 0.46193976625564337f, 0.1913417161825449f );
	quat = ZLQuaternion ( quatS, quatX, quatY, quatZ );

	result.Slerp ( quat, quat2, 0.45f );
	
	ASSERT_TRUE (
		ZLFloat::IsClose ( result.mS, testS, EPSILON ) &&
		ZLFloat::IsClose ( result.mV.mX, testX, EPSILON ) &&
		ZLFloat::IsClose ( result.mV.mY, testY, EPSILON ) &&
		ZLFloat::IsClose ( result.mV.mZ, testZ, EPSILON )
	);
}
