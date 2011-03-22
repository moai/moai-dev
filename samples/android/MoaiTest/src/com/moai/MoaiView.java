package com.moai;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.graphics.Canvas;
import android.opengl.GLSurfaceView;
import android.os.SystemClock;

public class MoaiView extends GLSurfaceView {

	int mWidth, mHeight;
	private MoaiRenderer _renderer;
	private MoaiThread _thread = null;
	private MoaiView forRen;
	boolean surfaceCreated = false;
	boolean startRun = false;
	boolean setDirectory = false;
	String luaThread = null;
	String appRoot = null;
	public MoaiView(Context context) {
		super(context);
		_renderer = new MoaiRenderer();
		setRenderer(_renderer);
		
		if(_thread == null)
			_thread = new MoaiThread();
		
		InitializeAku(this);
	}
	
	public MoaiView(Context context, int w, int h) {
		super(context);
		_renderer = new MoaiRenderer();
		setRenderer(_renderer);
		
		if(_thread == null)
			_thread = new MoaiThread();
		forRen = this;
		mWidth = w;
		mHeight = h;

	}
	
	/*public MoaiView(Context context, int w, int h, String input) {
		super(context);
		_renderer = new MoaiRenderer();
		setRenderer(_renderer);
		
		if(_thread == null)
			_thread = new MoaiThread();
		forRen = this;
		mWidth = w;
		mHeight = h;
		luaThread = input;
	}
	
	public MoaiView(Context context, int w, int h, String root, String input) {
		super(context);
		_renderer = new MoaiRenderer();
		setRenderer(_renderer);
		
		if(_thread == null)
			_thread = new MoaiThread();
		forRen = this;
		mWidth = w;
		mHeight = h;
		appRoot = root;
		luaThread = input;
	}*/
	

	//================================================================//
	// AKU callbacks
	//================================================================//

	//----------------------------------------------------------------//
	void AKUEnterFullscreenModeFunc () {
	}

	//----------------------------------------------------------------//
	void AKUExitFullscreenModeFunc () {
	}

	//----------------------------------------------------------------//
	void AKUHideLoadingScreenFunc () {
	}

	//----------------------------------------------------------------//
	void AKUOpenWindowFunc ( char[] title, int width, int height ) {
	}

	//----------------------------------------------------------------//
	void AKUShowLoadingScreenFunc () {
	}

	//----------------------------------------------------------------//
	void AKUShowSoftwareKeyboardFunc () {
	}

	//----------------------------------------------------------------//
	public void AKUStartGameLoopFunc () {
		
		if(!_thread.isAlive())
			_thread.start();
	}
	
	
	public void RunScript(String scriptName)
	{
		luaThread = scriptName;
		
		if(!surfaceCreated)
		{
			startRun = true;
			return;
		}
		Run(luaThread);
	}
	
	public void SetDirectory(String path)
	{
		appRoot = path;
		if(!surfaceCreated)
		{
			setDirectory = true;
			return;
		}
		if(appRoot != null)
			setWorkingDirectory(appRoot);
	}

	class MoaiRenderer implements GLSurfaceView.Renderer{

		@Override
		public void onDrawFrame(GL10 gl) {
			// TODO Auto-generated method stub
			
			//gl.glClear(0);
			if(_thread.isAlive())
				onDraw(mWidth,mHeight);
		}

		@Override
		public void onSurfaceChanged(GL10 gl, int width, int height) {
			// TODO Auto-generated method stub
			gl.glViewport(0, 0, width, height);
			mWidth = width;
			mHeight = height;
		}

		@Override
		public void onSurfaceCreated(GL10 gl, EGLConfig config) {
			// TODO Auto-generated method stub
			InitializeAku(forRen);
			surfaceCreated = true;
			if(setDirectory)
			{
				if(appRoot != null)
					setWorkingDirectory(appRoot);
			}
			
			if(startRun)
				Run(luaThread);
			

		}
		
		

	}
	
	class MoaiThread extends Thread
	{
		protected boolean _run = true;
		protected long curTime = 0;
		@Override
		public void run(){
			while(_run)
			{
				if(curTime == 0)
				{
					curTime = SystemClock.uptimeMillis();
					//	ms = SystemClock.uptimeMillis() - time;
					//time = SystemClock.uptimeMillis();
					continue;
				}
				if(SystemClock.uptimeMillis() - curTime < 32)
					yield();
				else
				{
					curTime = SystemClock.uptimeMillis();
					onUpdateAnim(); 
				}
					
			}
		}
	}
	
	protected static native void InitializeAku(MoaiView view);
	
	protected static native void Run(String fileName);
	
	protected static native void FinalizeAku();

	protected static native void DeinitializeAku();
	
	public static native void onDraw(int width, int height);

	public static native void onUpdateAnim();
	
	public static native void onUpdateHeading(int heading);
	
	public static native void onUpdateLocation(int longitude, int latitude, int altitude,
	float hAccuracy, float vAccuracy, float speed );
		
	public static native void handleTouches(int touch, boolean down, int locX, int locY, int tapCount);
	
	public static native void setWorkingDirectory(String path);

}
