//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.moai;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import android.app.Activity;
import android.content.Context;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.util.Log;
import android.view.Display;
import android.view.Window;
import android.view.WindowManager;

//================================================================//
// MoaiTest
//================================================================//
public class MoaiTest extends Activity {
	
	private File mAppRoot;

    //----------------------------------------------------------------//
    @Override
    public void onCreate ( Bundle savedInstanceState ) {

    	super.onCreate ( savedInstanceState );
		Display display = (( WindowManager ) getSystemService ( Context.WINDOW_SERVICE )).getDefaultDisplay(); 

        try {
        	System.load ( "/data/data/com.moai/lib/libmoai.so" );
	    } 
        catch ( Exception ex ) {
        	Log.e ( "MoaiTest", "failed to install native library: " + ex );
	    }

        requestWindowFeature ( Window.FEATURE_NO_TITLE );
	    
	    getWindow().setFlags( WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN ); 
	    getWindow().addFlags( WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON );
	    
		MoaiView mainView = new MoaiView ( this, display.getWidth(), display.getHeight() );
		setContentView ( mainView );
	    
	    try {
			setAppRoot();
		} 
	    catch ( Exception e ) {
			e.printStackTrace();
		}

		mainView.SetDirectory ( mAppRoot.getAbsolutePath() );
		mainView.RunScript ( "main.lua" );
    }
    
    //----------------------------------------------------------------//
    private void setAppRoot () throws Exception	{
		
    	if ( mAppRoot == null ) {
			mAppRoot = new File ( getExternalFilesDir ( null ), "lua" );
    	}
		
		try {
			
			mAppRoot.mkdirs();
	    	
			if ( ! mAppRoot.canWrite() ) {
	    		throw new Exception();
	    	}
		} 
		catch ( SecurityException e ) {
			e.printStackTrace();
		}
		
		CopyAssets();
	}

    //----------------------------------------------------------------//
	private void CopyAssets() {
	    
		AssetManager assetManager = getAssets ();
	    String[] files = null;
	    
	    try {
	        files = assetManager.list ( "lua" );
	    } 
	    catch ( IOException e ) {
	    	Log.e ( "MoaiTest", e.getMessage() );
	    }
	    
	    for ( int i = 0; i < files.length; i++ ) {
	        
	    	InputStream in = null;
	        OutputStream out = null;
	        try {
	        	in = assetManager.open ( "lua/" + files[ i ] );
	        	out = new FileOutputStream ( mAppRoot.getAbsolutePath() + "/" + files[ i ] );
	        	copyFile ( in, out );
	        	in.close();
	        	in = null;
	        	out.flush();
	        	out.close();
	        	out = null;
	        } 
	        catch ( Exception e ) {
	        	Log.e ( "Message", e.getMessage() );
	        }       
	    }
	}

    //----------------------------------------------------------------//
	private void copyFile ( InputStream in, OutputStream out ) throws IOException {
	    
		byte[] buffer = new byte[ 1024 ];
	    
	    int read;
	    while (( read = in.read ( buffer )) != -1 ) {
	    	out.write ( buffer, 0, read );
	    }
	}
}