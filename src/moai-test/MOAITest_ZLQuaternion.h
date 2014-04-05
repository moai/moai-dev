// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEST_USQUATERNION_H
#define	MOAITEST_USQUATERNION_H

#include <moai-core/headers.h>
#include <moai-test/MOAITest.h>
#include <moai-test/MOAITestKeywords.h>
#include <moai-test/MOAITestMgr.h>

#include <zl-util/ZLQuaternion.h>

//================================================================//
// MOAITest_sample
//================================================================//
class MOAITest_USQuaternion :
	public MOAITest {
public:

	TEST_NAME ( "ZLQuaternion" )

	char messageBuffer[ 1024 ];

	//----------------------------------------------------------------//
	void Staging ( MOAITestMgr& testMgr ) {
		
		testMgr.SetFilter ( MOAI_TEST_MATH, 0 );
	}
	
	//----------------------------------------------------------------//
	void Test ( MOAITestMgr& testMgr ) {
		
		memset ( messageBuffer, 0, 1024 );

		//---------- 
		ZLQuaternion quat;
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

		testMgr.BeginTest ( "Quaternion Set" );

		quat.Set ( testX, testY, testZ );

		if ( ZLFloat::IsClose ( quat.mS, quatS, EPSILON ) &&
			 ZLFloat::IsClose ( quat.mV.mX, quatX, EPSILON ) &&
			 ZLFloat::IsClose ( quat.mV.mY, quatY, EPSILON ) &&
			 ZLFloat::IsClose ( quat.mV.mZ, quatZ, EPSILON )) {
			
			testMgr.Success ( "Passed" );
			testMgr.EndTest ( true );
		}
		else {

			sprintf ( messageBuffer, "Set      %f,%f,%f,%f", quat.mS, quat.mV.mX, quat.mV.mY, quat.mV.mZ );
			testMgr.Comment ( messageBuffer );

			sprintf ( messageBuffer, "Expected %f,%f,%f,%f", quatS, quatX, quatY, quatZ );
			testMgr.Comment ( messageBuffer );
			
			testMgr.Failure ( "Values differ", "Set values do not match expected" );
			testMgr.EndTest ( false );
		}


		//---------- 
		quat.mS	   = quatS;
		quat.mV.mX = quatX;
		quat.mV.mY = quatY;
		quat.mV.mZ = quatZ;

		testMgr.BeginTest ( "Quaternion Get" );

		quat.Get ( x, y, z );

		if ( ZLFloat::IsClose ( x, testX, EPSILON ) &&
			 ZLFloat::IsClose ( y, testY, EPSILON ) &&
			 ZLFloat::IsClose ( z, testZ, EPSILON )) {
			
			testMgr.Success ( "Passed" );
			testMgr.EndTest ( true );
		}
		else {
			sprintf ( messageBuffer, "Get      %f,%f,%f", x, y, z );
			testMgr.Comment ( messageBuffer );

			sprintf ( messageBuffer, "Expected %f,%f,%f", testX, testY, testZ );
			testMgr.Comment ( messageBuffer );

			testMgr.Failure ( "Values differ", "Get values do not match expected" );
			testMgr.EndTest ( false );
		}

		//---------- 
		float testS;

		testS = 0.5432889103326224f;
		testX = 0.6903496693854332f;
		testY = 0.4730377044370305f;
		testZ = 0.0670061494936035f;

		// 45 degrees
		ZLQuaternion quat2, result;
		quat2.Set ( 0.7325378163287418f, 0.46193976625564337f, 0.46193976625564337f, 0.1913417161825449f );
		quat.Set ( quatS, quatX, quatY, quatZ );

		testMgr.BeginTest ( "Quaternion Slerp" );

		result.Slerp ( quat, quat2, 0.45f );
		
		if ( ZLFloat::IsClose ( result.mS, testS, EPSILON ) &&
			 ZLFloat::IsClose ( result.mV.mX, testX, EPSILON ) &&
			 ZLFloat::IsClose ( result.mV.mY, testY, EPSILON ) &&
			 ZLFloat::IsClose ( result.mV.mZ, testZ, EPSILON )) {
			
			testMgr.Success ( "Passed" );
			testMgr.EndTest ( true );
		}
		else {

			sprintf ( messageBuffer, "Set      %f,%f,%f,%f", result.mS, result.mV.mX, result.mV.mY, result.mV.mZ );
			testMgr.Comment ( messageBuffer );

			sprintf ( messageBuffer, "Expected %f,%f,%f,%f", testS, testX, testY, testZ );
			testMgr.Comment ( messageBuffer );

			testMgr.Failure ( "Values differ", "Slerped values do not match expected" );
			testMgr.EndTest ( false );
		}
	}
};

#endif
