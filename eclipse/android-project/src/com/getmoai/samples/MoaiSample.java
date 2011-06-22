//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.getmoai.samples;

import java.io.BufferedInputStream;   
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream; 

import android.app.Activity; 
import android.content.Context;
import android.os.Bundle;
import android.util.Log;
import android.view.Display;
import android.view.Window;
import android.view.WindowManager;
 
//================================================================//
// MoaiSample
//================================================================//
public class MoaiSample extends Activity {
	
	private File		mAppRoot;
	private MoaiView	mView;
    
    //----------------------------------------------------------------//
    @Override
    public void onCreate ( Bundle savedInstanceState ) {

    	super.onCreate ( savedInstanceState );
		Display display = (( WindowManager ) getSystemService ( Context.WINDOW_SERVICE )).getDefaultDisplay(); 
 
        try {
        	System.load ( "/data/data/com.getmoai.samples/lib/libmoai.so" );
	    } 
        catch ( Exception ex ) {
        	Log.e ( "MoaiSample", "failed to install native library: " + ex );
	    } 
          
        requestWindowFeature ( Window.FEATURE_NO_TITLE );
	    
	    getWindow().setFlags( WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN ); 
	    getWindow().addFlags( WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON );
	    
	    mView = new MoaiView ( this, display.getWidth(), display.getHeight());
		setContentView ( mView );
	    
	    try {
	    	unpackAssets ();
		} 
	    catch ( Exception e ) {
			e.printStackTrace();
		} 

	    mView.SetDirectory ( mAppRoot.getAbsolutePath ());
    }
    
    //----------------------------------------------------------------//
//    @Override
//    public void onStop () {
//    	
//    	if ( mView != null ) {
//    		mView.cleanup ();
//    		mView = null;
//    	}
//    }
     
    //----------------------------------------------------------------//
	private void unpackAssets () {
		
		mAppRoot = new File ( getExternalFilesDir ( null ), "lua" );
		
		InputStream is = getResources ().openRawResource ( R.raw.lua );
		File extractTo = new File ( mAppRoot.getAbsolutePath() + "/" );
		ZipInputStream zis;
		
		try {
			
			zis = new ZipInputStream ( new BufferedInputStream ( is ));
			
			ZipEntry ze; 
			byte [] buffer = new byte [ 4096 ];
			
			while (( ze = zis.getNextEntry ()) != null ) {
				
				File file = new File ( extractTo, ze.getName ());
				
				if ( ze.isDirectory () && !file.exists ()) {
					file.mkdirs();
					
					while ( zis.read ( buffer ) != -1 );
				}
				else {
					if ( !file.getParentFile().exists()) {
						file.getParentFile().mkdirs();
					}
					
					BufferedOutputStream out = new BufferedOutputStream ( new FileOutputStream ( file ));
					
					int count;
					while (( count = zis.read ( buffer )) != -1) {
						out.write ( buffer, 0, count ); 
					}
					out.close();
				}
			}
			zis.close();
		}
		catch ( Exception e ) {
		}
		finally {
		}
	}
}