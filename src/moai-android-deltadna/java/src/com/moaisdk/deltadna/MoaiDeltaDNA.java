//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc.
// All Rights Reserved.
// http://getmoai.com
//----------------------------------------------------------------//

package com.moaisdk.deltadna;

import com.deltadna.android.sdk.DDNA;
import com.deltadna.android.sdk.helpers.NotStartedException;

import android.app.Activity;

@SuppressWarnings ( "unused" )

//================================================================//
// MoaiDeltaDNA
//================================================================//
public class MoaiDeltaDNA {

    private static Activity         sActivity           = null;
    private static String           sEnvironmentKey     = "56919948607282167963952652014071";
    private static String           sCollectHostname    = "http://collect2674dltcr.deltadna.net/collect/api";
    private static String           sEngageHostname     = "http://engage2674dltcr.deltadna.net";

    public static void initialize ( String clientVersion, boolean debugMode ) {

        DDNA.inst ().settings ().setDebugMode ( debugMode );
        DDNA.inst ().setClientVersion ( clientVersion );

        // Start deltaDNA SDK
        // SDK generates its own userID when null is passed
        DDNA.inst ().startSDK ( sEnvironmentKey, sCollectHostname, sEngageHostname, null );
    }

    public static void onCreate ( Activity activity ) {

        sActivity = activity;

        // Initialise the deltaDNA SDK
        // NB This can also be done in your applications OnCreate method.
        DDNA.inst ().init ( activity.getApplication () );
    }

    public static void onDestroy () {

        try {
            DDNA.inst ().stopSDK ();
        } 
        catch ( NotStartedException e ) {
            e.printStackTrace();
        }
    }
}