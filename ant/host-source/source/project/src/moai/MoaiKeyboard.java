//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import android.app.Activity;
import android.content.Context;
import android.view.Gravity;
import android.view.Window;
import android.view.KeyEvent;
import android.view.inputmethod.EditorInfo;

// To support Android Keyboard input 
import android.view.ViewGroup.LayoutParams;
import android.view.inputmethod.InputMethodManager;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView.OnEditorActionListener;
import android.widget.TextView;

// These are necessary for the mKeyInTextView hack
import android.widget.EditText;Ï
import android.text.TextWatcher;
import android.text.Editable;

//================================================================//
// MoaiLog
//================================================================//
public class MoaiKeyboard {
	
	private static Activity sActivity = null;
	private static EditText mKeyInTextView = null;

	protected static native void AKUNotifyKeyEvent ();
	protected static native void AKUNotifyTextDone ();

	public static boolean mKeyIsReady = false;
	public static boolean mTextIsReady = false;

	private static String mKeyString = new String ();

	private static Context sContext;
	private static InputMethodManager mInputMethodManager;

	private static LinearLayoutIMETrap mContainer;
		
	//----------------------------------------------------------------//
	public static void onCreate ( Activity activity ) {

		sActivity = activity;
		sContext = activity;
		
		// input manager is used to pop up the native Android keyboard as needed
		mInputMethodManager = ( InputMethodManager ) sContext.getSystemService ( Context.INPUT_METHOD_SERVICE );
		
		// Our main container holds the EGL view as well as our fake TextEdit view for keyboard entry ..
		mContainer = ( LinearLayoutIMETrap ) new LinearLayoutIMETrap ( sContext );
		mContainer.setLayoutParams ( new LinearLayout.LayoutParams ( LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT ));
		mContainer.setOrientation ( LinearLayout.VERTICAL );
		mContainer.setMainActivity ( activity );

		mKeyInTextView = new EditText ( activity );

		mKeyInTextView.setText ( "" );
		mKeyInTextView.setMinLines ( 1 );
		mKeyInTextView.setMaxLines ( 1 );
		
		mKeyInTextView.addTextChangedListener ( new TextWatcher () {

			public void afterTextChanged ( Editable s ) {

   				mKeyString = s.toString ();
			}

			public void beforeTextChanged ( CharSequence s, int start, int count, int after ) {

				mKeyString = s.toString ();
			}

			public void onTextChanged ( CharSequence s, int start, int before, int count ) {

				mKeyString = s.toString ();
				mKeyIsReady = true;
			}
		});
		
		mKeyInTextView.setOnEditorActionListener(new OnEditorActionListener() {
		    @Override
		    public boolean onEditorAction(TextView v, int actionId, KeyEvent event) {
		        if (actionId == EditorInfo.IME_ACTION_DONE) {
					mTextIsReady = true;
		        }
		        return false;
		    }
		});

		// Create the fake EditText, and push it outside the margins so that its not visible.
		LinearLayout.LayoutParams paramsKeyInTextView = new LinearLayout.LayoutParams ( LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT );

		// re-set the Margins so that the field is hidden.
		paramsKeyInTextView.setMargins ( 0, 64, 0, 0 );
		mKeyInTextView.setLayoutParams ( paramsKeyInTextView );
		
	}

	public static LinearLayoutIMETrap getContainer () {
		return mContainer;
	}
	
	public static EditText getEditText() {
		return mKeyInTextView;
	}
	
	public static void update () {
		if ( mKeyIsReady ) {
			AKUNotifyKeyEvent();
			mKeyIsReady = false;
		}
		if ( mTextIsReady ) {
			AKUNotifyTextDone();
			mTextIsReady = false;
		}
	}
	
	//================================================================//
	// MoaiKeyboard JNI callback methods
	//================================================================//
	public static String getString () {
		return mKeyString;
	}
	
	public static void showKeyboard () {	
		mInputMethodManager.showSoftInput ( mKeyInTextView, 0 );	
	}

	public static void hideKeyboard () {	
		mKeyInTextView.setText ( "" );
		mInputMethodManager.hideSoftInputFromWindow ( mKeyInTextView.getWindowToken (), 0 );
	}
		
	public static void setText ( String text )	{	
		mKeyInTextView.setText ( text );	
		mKeyInTextView.setSelection ( text.length ());	
	}
}