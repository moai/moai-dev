// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEST_USQUATERNION_H
#define	MOAITEST_USQUATERNION_H

#include <moaicore/moaicore.h>
#include <moaiext-test/MOAITest.h>
#include <moaiext-test/MOAITestKeywords.h>
#include <moaiext-test/MOAITestMgr.h>

#include <uslscore/USQuaternion.h>

//================================================================//
// MOAITest_sample
//================================================================//
class MOAITest_USQuaternion :
	public MOAITest {
public:

	TEST_NAME ( "USQuaternion" )

	char messageBuffer[ 1024 ];
	//----------------------------------------------------------------//
	void Staging ( MOAITestMgr& testMgr ) {
		
		testMgr.SetFilter ( MOAI_TEST_MATH, 0 );
	}
	
	//----------------------------------------------------------------//
	void Test ( MOAITestMgr& testMgr ) {
		
		memset ( messageBuffer, 0, 1024 );

		//---------- 
		USQuaternion quat;
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

		if ( USFloat::IsClose ( quat.mS, quatS, EPSILON ) &&
			 USFloat::IsClose ( quat.mV.mX, quatX, EPSILON ) &&
			 USFloat::IsClose ( quat.mV.mY, quatY, EPSILON ) &&
			 USFloat::IsClose ( quat.mV.mZ, quatZ, EPSILON )) {
			
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

		if ( USFloat::IsClose ( x, testX, EPSILON ) &&
			 USFloat::IsClose ( y, testY, EPSILON ) &&
			 USFloat::IsClose ( z, testZ, EPSILON )) {
			
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
		USQuaternion quat2, result;
		quat2.Set ( 0.7325378163287418f, 0.46193976625564337f, 0.46193976625564337f, 0.1913417161825449f );
		quat.Set ( quatS, quatX, quatY, quatZ );

		testMgr.BeginTest ( "Quaternion Slerp" );

		result.Slerp ( quat, quat2, 0.45f );
		
		if ( USFloat::IsClose ( result.mS, testS, EPSILON ) &&
			 USFloat::IsClose ( result.mV.mX, testX, EPSILON ) &&
			 USFloat::IsClose ( result.mV.mY, testY, EPSILON ) &&
			 USFloat::IsClose ( result.mV.mZ, testZ, EPSILON )) {
			
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
