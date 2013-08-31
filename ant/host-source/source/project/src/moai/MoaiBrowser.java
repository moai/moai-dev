//----------------------------------------------------------------//
// Copyright (c) 2010-2013 Zipline Games, Inc.
// All Rights Reserved.
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.net.Uri.Builder;
import android.os.Bundle;

//================================================================//
// MoaiBrowser
//================================================================//
public class MoaiBrowser {
	private static Activity sActivity = null;

    //----------------------------------------------------------------//
	public static void onCreate ( Activity activity ) {
		sActivity = activity;
	}

	//----------------------------------------------------------------//
	public static void openURL ( String url ) {
		sActivity.startActivity ( new Intent ( Intent.ACTION_VIEW, Uri.parse ( url )));
	}

    //----------------------------------------------------------------//
	public static void openURLWithParams ( String url, Bundle parameters ) {
        Uri.Builder builder = Uri.parse ( url ).buildUpon ( );
        for ( String key : parameters ) {
            builder.appendQueryParameter( key, parameters.getString( key ) );
        }
        Uri uri = builder.build ( );
		sActivity.startActivity ( new Intent ( Intent.ACTION_VIEW, uri ) );
	}
}
