//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import android.util.Log;

//================================================================//
// MoaiLog
//================================================================//
public class MoaiLog {

	//----------------------------------------------------------------//
	public static void e ( String message, Exception e ) {
		
	    Log.e ( "MoaiLog", message, e );
	}

	//----------------------------------------------------------------//
	public static void e ( String message ) {
		
	    Log.e ( "MoaiLog", message );
	}
	
	//----------------------------------------------------------------//
	public static void w ( String message ) {
		
	    Log.w ( "MoaiLog", message );
	}

	//----------------------------------------------------------------//
	public static void d ( String message ) {
		
	    Log.d ( "MoaiLog", message );
	}

	//----------------------------------------------------------------//
	public static void i ( String message ) {
		
	    Log.i ( "MoaiLog", message );
	}
}