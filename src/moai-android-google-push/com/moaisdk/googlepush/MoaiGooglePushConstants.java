//----------------------------------------------------------------//
// Portions Copyright (c) 2010-2011 Zipline Games, Inc. 
// Adapted from In-App Billing sample code from Google, Inc.
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

//================================================================//
// MoaiGooglePushConstants
//================================================================//
public class MoaiGooglePushConstants {
	
	public enum RegistrationCode {
		
		RESULT_REGISTERED,
		RESULT_UNREGISTERED,
        RESULT_ERROR_SERVICE_NOT_AVAILABLE,
        RESULT_ERROR_ACCOUNT_MISSING,
        RESULT_ERROR_AUTHENTICATION_FAILED,
        RESULT_ERROR_TOO_MANY_REGISTRATIONS,
        RESULT_ERROR_INVALID_SENDER,
        RESULT_ERROR_PHONE_REGISTRATION_ERROR;
						
        public static RegistrationCode valueOf ( int index ) {
	
            RegistrationCode [] values = RegistrationCode.values ();
            if (( index < 0 ) || ( index >= values.length )) {
	
                return RESULT_ERROR_PHONE_REGISTRATION_ERROR;
            }

            return values [ index ];
        }
    }

    public static final String ACTION_REGISTER = "com.google.android.c2dm.intent.REGISTER";
    public static final String ACTION_UNREGISTER = "com.google.android.c2dm.intent.UNREGISTER";
	public static final String ACTION_REGISTRATION = "com.google.android.c2dm.intent.REGISTRATION";
	public static final String ACTION_RECEIVE = "com.google.android.c2dm.intent.RECEIVE";

    public static final String APP_ID = "app";
    public static final String SENDER_ALIAS = "sender";
	public static final String ERROR = "error";
	public static final String UNREGISTERED = "unregistered";
	public static final String REGISTRATION_ID = "registration_id";
}
