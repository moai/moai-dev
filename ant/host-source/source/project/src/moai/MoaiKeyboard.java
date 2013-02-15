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

// To support Android Keyboard input 
import android.view.ViewGroup.LayoutParams;
import android.view.inputmethod.InputMethodManager;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.util.DisplayMetrics;

// These are necessary for the mKeyInTextView hack
import android.widget.EditText;
import android.text.TextWatcher;
import android.text.Editable;

//================================================================//
// MoaiLog
//================================================================//
public class MoaiKeyboard {
	
	private static Activity sActivity = null;
	private static EditText mKeyInTextView = null;

	protected static native void AKUNotifyKeyEvent();

	public static boolean mKeyIsReady = false;

	private static String mKeyString = new String();

	private static Context sContext;
	private static InputMethodManager mInputMethodManager;

	private static LinearLayoutIMETrap mContainer;
	
	public static String getString() {
		return mKeyString;
	}
	
	public static void showKeyboard()
	{
		MoaiLog.i("MoaiKeyboard showSoftKeyboard ");	
		mInputMethodManager.showSoftInput(mKeyInTextView, 0);	
	}

	public static void hideKeyboard()	
	{	
		MoaiLog.i("MoaiKeyboard hideSoftKeyboard ");

		mKeyInTextView.setText("");	
		mInputMethodManager.hideSoftInputFromWindow(mKeyInTextView.getWindowToken(), 0);
	}
		
	public static void setText(String text)	
	{	
		mKeyInTextView.setText(text);	
		mKeyInTextView.setSelection(text.length());	
	}
	
	//----------------------------------------------------------------//
	public static void onCreate ( Activity activity ) {

		sActivity = activity;
		sContext = activity;
		
		// input manager is used to pop up the native Android keyboard as needed
		mInputMethodManager = ( InputMethodManager ) sContext.getSystemService ( Context.INPUT_METHOD_SERVICE );
		
		// Our main container holds the EGL view as well as our fake TextEdit view for keyboard entry ..
		// not necessary, nor does it work as focus is lost: //mKeyInTextView.setVisibility(View.INVISIBLE);
		mContainer = ( LinearLayoutIMETrap ) new LinearLayoutIMETrap ( sContext );
		mContainer.setLayoutParams ( new LinearLayout.LayoutParams ( LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT ));
		mContainer.setOrientation ( LinearLayout.VERTICAL );
		mContainer.setMainActivity ( activity );
		
		/* 	The mKeyInTextView will be set up to capture softkey input.  
			It is not displayed to the user, but instead captures keyboard 
			events for text field entry in the moai project layouts_ */

		mKeyInTextView = new EditText ( activity );

		mKeyInTextView.setText ( "" );
		mKeyInTextView.setMinLines ( 1 );
		mKeyInTextView.setMaxLines ( 1 );
		
		// Set up the main view for MOAI, but include the KeyInTextView for keyboard UI
		mKeyInTextView.addTextChangedListener ( new TextWatcher () {

			public void afterTextChanged ( Editable s ) {

				MoaiLog.i("MoaiActivity mKeyInTextView: afterTextChanged s = " + s);
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
	//================================================================//
	// MoaiKeyboard JNI callback methods
	//================================================================//
	public static void update () {
		if ( mKeyIsReady ) {
			AKUNotifyKeyEvent();
			mKeyIsReady = false;
		}
	}
	
	//----------------------------------------------------------------//
	public static void init ( String url ) {
		
	}

}