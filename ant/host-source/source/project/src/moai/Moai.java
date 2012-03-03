//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import android.app.Activity;
import android.content.Intent;

import java.lang.reflect.Method;

//================================================================//
// Moai
//================================================================//
public class Moai {

	private static Class sTapjoyClass = null;
	private static Class sFacebookClass = null;
	private static Class sGooglePushClass = null;
	private static Class sGoogleBillingClass = null;

	//----------------------------------------------------------------//
	static {
	
		sTapjoyClass = findClass ( "com.ziplinegames.moai.MoaiTapjoy" );
		sFacebookClass = findClass ( "com.ziplinegames.moai.MoaiFacebook" );
		sGooglePushClass = findClass ( "com.ziplinegames.moai.MoaiGooglePush" );
		sGoogleBillingClass = findClass ( "com.ziplinegames.moai.MoaiGoogleBilling" );
	}
	
	//----------------------------------------------------------------//
	public static void onActivityResult ( int requestCode, int resultCode, Intent data ) {
	
	}

	//----------------------------------------------------------------//
	public static void onCreate ( Activity activity ) {

		executeMethod ( sTapjoyClass, null, "onCreate", new Class [] { Activity.class }, new Object [] { activity });
		executeMethod ( sFacebookClass, null, "onCreate", new Class [] { Activity.class }, new Object [] { activity });
		executeMethod ( sGooglePushClass, null, "onCreate", new Class [] { Activity.class }, new Object [] { activity });
		executeMethod ( sGoogleBillingClass, null, "onCreate", new Class [] { Activity.class }, new Object [] { activity });
	}
	
	//----------------------------------------------------------------//
	public static void onDestroy () {
	
		executeMethod ( sTapjoyClass, null, "onDestroy", new Class [] { }, new Object [] { });
		executeMethod ( sFacebookClass, null, "onDestroy", new Class [] { }, new Object [] { });
		executeMethod ( sGooglePushClass, null, "onDestroy", new Class [] { }, new Object [] { });
		executeMethod ( sGoogleBillingClass, null, "onDestroy", new Class [] { }, new Object [] { });
	}

	//----------------------------------------------------------------//
	public static void onPause () {
	
		executeMethod ( sTapjoyClass, null, "onPause", new Class [] { }, new Object [] { });
		executeMethod ( sFacebookClass, null, "onPause", new Class [] { }, new Object [] { });
		executeMethod ( sGooglePushClass, null, "onPause", new Class [] { }, new Object [] { });
		executeMethod ( sGoogleBillingClass, null, "onPause", new Class [] { }, new Object [] { });
	}

	//----------------------------------------------------------------//
	public static void onResume () {
	
		executeMethod ( sTapjoyClass, null, "onResume", new Class [] { }, new Object [] { });
		executeMethod ( sFacebookClass, null, "onResume", new Class [] { }, new Object [] { });
		executeMethod ( sGooglePushClass, null, "onResume", new Class [] { }, new Object [] { });
		executeMethod ( sGoogleBillingClass, null, "onResume", new Class [] { }, new Object [] { });
	}

	//----------------------------------------------------------------//
	public static void onStart () {
	
		executeMethod ( sTapjoyClass, null, "onStart", new Class [] { }, new Object [] { });
		executeMethod ( sFacebookClass, null, "onStart", new Class [] { }, new Object [] { });
		executeMethod ( sGooglePushClass, null, "onStart", new Class [] { }, new Object [] { });
		executeMethod ( sGoogleBillingClass, null, "onStart", new Class [] { }, new Object [] { });
	}

	//----------------------------------------------------------------//
	public static void onStop () {
	
		executeMethod ( sTapjoyClass, null, "onStop", new Class [] { }, new Object [] { });
		executeMethod ( sFacebookClass, null, "onStop", new Class [] { }, new Object [] { });
		executeMethod ( sGooglePushClass, null, "onStop", new Class [] { }, new Object [] { });
		executeMethod ( sGoogleBillingClass, null, "onStop", new Class [] { }, new Object [] { });
	}
	
	//================================================================//
	// Private methods
	//================================================================//
	
	//----------------------------------------------------------------//
	private static Class findClass ( String className ) {
		
		Class theClass = null;
		try {

			theClass = Class.forName ( className );
		} catch ( Throwable e ) {

		}
		
		return theClass;
	}
	
	//----------------------------------------------------------------//
	private static Object executeMethod ( Class theClass, Object theInstance, String methodName, Class [] parameterTypes, Object [] parameterValues ) {
		
		Object result = null;
		if ( theClass != null ) {
			
			try {

				Method theMethod = theClass.getMethod ( methodName, parameterTypes );

				result = theMethod.invoke ( theInstance, parameterValues );
			} catch ( Throwable e ) {

			}			
		}
		
		return result;
	}
}