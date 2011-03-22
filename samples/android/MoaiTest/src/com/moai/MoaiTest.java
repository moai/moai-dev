package com.moai;

import java.io.File;

import android.app.Activity;
import android.content.Context;
import android.content.res.Resources;
import android.os.Bundle;
import android.util.Log;
import android.view.Display;
import android.view.Window;
import android.view.WindowManager;

public class MoaiTest extends Activity {
	
	private File _appRoot;

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
       // setContentView(R.layout.main);
        Resources r = getResources();
		Display display = ((WindowManager)getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay(); 


        try {

		 	//TODO: Switch here for library and non library modes
        	System.load("/data/data/com.moai/lib/libMoai.so");
        	//System.load("/data/data/com.unboundfiction.colorgame/lib/libcamera.so");

	     } catch (Exception ex) { 
	          Log.e("GoGoKiddo", "failed to install native library: " + ex);
	     }

        requestWindowFeature(Window.FEATURE_NO_TITLE);
	    
	    getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN); 
	    
	    getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
	    
	    
	   
		MoaiView mainView = new MoaiView(this, display.getWidth(), display.getHeight());
		setContentView(mainView);
	    
	    
	    try {
			setAppRoot();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		
		mainView.SetDirectory(_appRoot.getAbsolutePath());
		mainView.RunScript("main.lua");
		
		
		
		
    }
    
    private void setAppRoot() throws Exception
	{
		if(_appRoot == null)
			_appRoot = new File(getExternalFilesDir(null), "lua");
		
		try 
		{
			_appRoot.mkdirs();		

	    	if(!_appRoot.canWrite())
	    		throw new Exception();	
		} 
		catch (SecurityException e) 
		{
			e.printStackTrace();
			// TODO: handle security exception
		}
	}
	private String getAssetLocalUri(String path) throws Exception
	{
		setAppRoot();
		
		String fileName	= path.split("/")[path.split("/").length-1];
		String localUri = _appRoot.getAbsolutePath()+"/"+fileName;
		
		return localUri;
	}
}