//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package @PACKAGE@;
 
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Handler;
import android.os.Looper;
import android.view.MotionEvent;

// Moai
import com.ziplinegames.moai.*;

//================================================================//
// MoaiView
//================================================================//
public class MoaiView extends GLSurfaceView {

	private boolean		mForceLandscape;
	private Handler		mHandler;
	private int 		mHeight;
	private Runnable	mUpdateRunnable;
	private int 		mWidth;
	
	private static final Object	sAkuLock = new Object ();
	private static final long	AKU_UPDATE_FREQUENCY = 1000 / 60; // 60 Hz, in milliseconds

    //----------------------------------------------------------------//
	public MoaiView ( Context context, int width, int height, int glesVersion ) {

		super ( context );
				
		// If the screen is locked when the view is created, the orientation 
		// may initially be portrait because the lock screen is portrait
		// orientation, so we need to compensate and force landscape. This, 
		// of course, assumes that you're locking your application to landscape
		// mode in the manifest. If your application is portrait or supports
		// multiple orientations, you'll have to edit the manfifest, set this
		// setting to false, and may have to make other adjustments to properly
		// handle orientation changes.
		mForceLandscape = true;
		
		setScreenDimensions ( width, height );
		Moai.setScreenSize ( mWidth, mHeight );
		
		if ( glesVersion >= 0x20000 ) {
			
			// NOTE: Must be set before the renderer is set.
			setEGLContextClientVersion ( 2 );
		}
		
		// Create a handler that we can use to post to the main thread and a pseudo-
		// periodic runnable that will handle calling Moai.update on the main thread.
		mHandler = new Handler ( Looper.getMainLooper ());
		mUpdateRunnable = new Runnable () {

			public void run () {
			
				synchronized ( sAkuLock ) {
				
					Moai.update ();
				}

				mHandler.postDelayed ( mUpdateRunnable , AKU_UPDATE_FREQUENCY );
			}
		};

        setRenderer ( new MoaiRenderer ());
		onPause (); // Pause rendering until restarted by the activity lifecycle.		
	}		
	
	//================================================================//
	// Public methods
	//================================================================//
	
	//----------------------------------------------------------------//
	public void pause ( boolean paused ) {
	
		if ( paused ) {

			mHandler.removeCallbacks ( mUpdateRunnable );
			
			Moai.pause ( true );
			setRenderMode ( GLSurfaceView.RENDERMODE_WHEN_DIRTY );
			onPause ();			
		}
		else {

			onResume ();
			setRenderMode ( GLSurfaceView.RENDERMODE_CONTINUOUSLY );
			Moai.pause ( false );

			mHandler.postDelayed ( mUpdateRunnable , AKU_UPDATE_FREQUENCY );
		}
	}

	//================================================================//
	// MotionEvent methods
	//================================================================//

    //----------------------------------------------------------------//
	@Override
	public boolean onTouchEvent ( MotionEvent event ) {

		boolean isDown = ( event.getAction () == MotionEvent.ACTION_DOWN );
		isDown |= ( event.getAction() == MotionEvent.ACTION_MOVE );
			
		final int pointerCount = event.getPointerCount ();
		for ( int pointerIndex = 0; pointerIndex < pointerCount; ++pointerIndex ) {
				
			int pointerId = event.getPointerId ( pointerIndex );
								
			Moai.enqueueTouchEvent (
				Moai.InputDevice.INPUT_DEVICE.ordinal (),
				Moai.InputSensor.SENSOR_TOUCH.ordinal (),
				pointerId, 
				isDown, 
				Math.round ( event.getX ( pointerIndex )), 
				Math.round ( event.getY ( pointerIndex )), 
				1
			);
		}			
		
		return true;
	}
	
	//================================================================//
	// Private methods
	//================================================================//
	
	//----------------------------------------------------------------//
	public void setScreenDimensions ( int width, int height ) {
	
		if ( mForceLandscape && ( height > width )) {
			
			mWidth = height;
			mHeight = width;
		}
		else {
			
			mWidth = width;
			mHeight = height;
		}
	}
	
	//================================================================//
	// MoaiRenderer
	//================================================================//
	
	private class MoaiRenderer implements GLSurfaceView.Renderer {

		private boolean mRunScriptsExecuted = false;

	    //----------------------------------------------------------------//
		@Override
		public void onDrawFrame ( GL10 gl ) {

			synchronized ( sAkuLock ) {
				
				Moai.render ();
			}
		}

	    //----------------------------------------------------------------//
		@Override
		public void onSurfaceChanged ( GL10 gl, int width, int height ) {

			MoaiLog.i ( "MoaiRenderer onSurfaceChanged: surface CHANGED" );

			setScreenDimensions ( width, height );
			Moai.setViewSize ( mWidth, mHeight );
		}
        
	    //----------------------------------------------------------------//
		@Override
		public void onSurfaceCreated ( GL10 gl, EGLConfig config ) {

			MoaiLog.i ( "MoaiRenderer onSurfaceCreated: surface CREATED" );

			synchronized ( sAkuLock ) {
				
				Moai.detectGraphicsContext ();
			}
			
			if ( !mRunScriptsExecuted ) {

				mRunScriptsExecuted = true;

				mHandler.postAtFrontOfQueue ( new Runnable () {

					public void run () {
				
						MoaiLog.i ( "MoaiRenderer onSurfaceCreated: Running game scripts" );
				
						@RUN_COMMAND@

						synchronized ( sAkuLock ) {

							Moai.startSession ( false );

							Moai.setApplicationState ( Moai.ApplicationState.APPLICATION_RUNNING );
						}
					}
				});
			} else {
				
				mHandler.post ( new Runnable () {

					public void run () {
				
						synchronized ( sAkuLock ) {

							Moai.startSession ( true );

							Moai.setApplicationState ( Moai.ApplicationState.APPLICATION_RUNNING );
						}
					}
				});
			}
		}	
		
	    //----------------------------------------------------------------//
		private void runScripts ( String [] filenames ) {

			for ( String file : filenames ) {
				
				synchronized ( sAkuLock ) {
					
					MoaiLog.i ( "MoaiRenderer runScripts: Running " + file + " script" );
					
					Moai.runScript ( file );
				}
			}
		}	
	}
}