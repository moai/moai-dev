/*
 * Copyright (c) 2012 allaboutapps.at. All Rights Reserved.
 *
 * Contributed to the MOAI project by Jay Vaughan, allaboutapps.at
 *
 * This file implements the MOAIKeyboardAndroid functionality
 * for user-input from the soft keyboard on Android
 * 
 * http://getmoai.com
 *
 * This is an extension to the LinearLayout used by the Android MOAI host.
 * The purpose of this class is to allow the Input Manager events to be
 * intercepted so that the Android soft keyboard can properly be used
 * with the moaigui framework.  This class intercepts IME key events
 * so that the softinput BACK button can be intercepted as needed.
 * Doing so ensures that screen updates within moaigui can be properly
 * triggered.
 */

package com.ziplinegames.moai;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.ContentValues;
import android.content.Context;
import android.os.Bundle;
import android.os.IBinder;
import android.view.Display;
import android.view.KeyEvent;
import android.view.View;

import android.view.inputmethod.InputMethodManager;
import android.widget.LinearLayout;

import android.widget.EditText;
import android.text.TextWatcher;
import android.text.Editable;
import android.util.AttributeSet;


public class LinearLayoutIMETrap extends LinearLayout {
	Activity activityReference;

	public LinearLayoutIMETrap ( Context context ) {
		super ( context );
	}
	public LinearLayoutIMETrap ( Context context, AttributeSet attrs ) {
		super ( context, attrs );
	}
	public void setMainActivity ( Activity activityReference ) {
		this.activityReference = activityReference;
	}

	@Override
	public boolean dispatchKeyEventPreIme ( KeyEvent event ) {
		if ( activityReference != null ) {
			InputMethodManager imm = ( InputMethodManager ) activityReference.getSystemService ( Context.INPUT_METHOD_SERVICE );
			if ( imm.isActive () && event.getKeyCode () == KeyEvent.KEYCODE_BACK ) {
				MoaiLog.i ( "LinearLayoutIMETrap dispatchKeyEventPreIme, event: " + event );
				MoaiKeyboard.hideKeyboard (); // hide the keyboard if its visible ..
				if ( Moai.backButtonPressed ()) {
					return true;
				}
			}
		}

		return super.dispatchKeyEventPreIme ( event );
	}
}