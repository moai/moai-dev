//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package @PACKAGE@;

import java.io.BufferedInputStream;   
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream; 

import @PACKAGE@.R;

import android.app.Activity; 
import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.Log;
import android.view.Display;
import android.view.Window;
import android.view.WindowManager;
 
//================================================================//
// MoaiActivity
//================================================================//
public class MoaiActivity extends Activity implements SensorEventListener {
	
	private File		mAppRoot; 
	private MoaiView	mView;

	private SensorManager mSensorManager;
	private Sensor mAccelerometer;
    
    //----------------------------------------------------------------//
    @Override
    public void onCreate ( Bundle savedInstanceState ) {

       	System.load ( "/data/data/@PACKAGE@/lib/libmoai.so" ); 

    	super.onCreate ( savedInstanceState );
		Display display = (( WindowManager ) getSystemService ( Context.WINDOW_SERVICE )).getDefaultDisplay(); 
 
		mSensorManager = ( SensorManager ) getSystemService ( Context.SENSOR_SERVICE );
		mAccelerometer = mSensorManager.getDefaultSensor ( Sensor.TYPE_ACCELEROMETER );

        requestWindowFeature ( Window.FEATURE_NO_TITLE );
	    
	    getWindow().setFlags( WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN ); 
	    getWindow().addFlags( WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON );
	    
	    mView = new MoaiView ( this, display.getWidth(), display.getHeight());
	    if ( detectOpenGLES20 ()) mView.setEGLContextClientVersion( 2 );
        mView.setRenderer ( mView.new MoaiRenderer ());
        mView.setRenderMode ( GLSurfaceView.RENDERMODE_WHEN_DIRTY );
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
    private boolean detectOpenGLES20 () {
        
		ActivityManager am = ( ActivityManager ) getSystemService ( Context.ACTIVITY_SERVICE );
        ConfigurationInfo info = am.getDeviceConfigurationInfo ();
        boolean retVal = ( info.reqGlEsVersion >= 0x20000 );
		
		if ( retVal ) {
			Log.e ( "MoaiLog", "OpenGL 2.0" );
		}
		else {
			Log.e ( "MoaiLog", "OpenGL 1.0" );
		}
		
		return retVal;
    }

	//----------------------------------------------------------------//
	protected void onResume () {
		
		super.onResume ();
		mSensorManager.registerListener ( this, mAccelerometer, SensorManager.SENSOR_DELAY_NORMAL );
	}

	//----------------------------------------------------------------//
	protected void onPause () {
		
		super.onPause();
		mSensorManager.unregisterListener ( this );
	}

	//----------------------------------------------------------------//
	public void onAccuracyChanged ( Sensor sensor, int accuracy ) {
	}

	//----------------------------------------------------------------//
	public void onSensorChanged ( SensorEvent event ) {
		
		mView.UpdateAccelerometer ( 
			event.values [ 0 ], 
			event.values [ 1 ], 
			event.values [ 2 ]
		);
	}


//    @Override
//    protected void onResume() {
//        // Ideally a game should implement onResume() and onPause()
//        // to take appropriate action when the activity looses focus
//        super.onResume();
//        mGLSurfaceView.onResume();
//    }
//
//    @Override
//    protected void onPause() {
//        // Ideally a game should implement onResume() and onPause()
//        // to take appropriate action when the activity looses focus
//        super.onPause();
//        mGLSurfaceView.onPause();
//    }
    
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
		
		mAppRoot = new File ( getExternalFilesDir ( null ), "" );
		
		InputStream is = getResources ().openRawResource ( R.raw.bundle );
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
			Log.e( "MoaiActivity", "Unable to read or write to SD card");
		}
		finally {
		}
	}
}