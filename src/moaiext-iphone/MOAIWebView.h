// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIWEBVIEW_H
#define MOAIWEBVIEW_H

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <moaicore/moaicore.h>

@class MoaiUiWebViewDelegate;

class MOAIWebView :
	public MOAIEventSource {
private:
	
	UIWebView*					mWebView;
	MoaiUiWebViewDelegate*		mWebViewDelegate;
	
	//----------------------------------------------------------------//
	static int			_canGoBack						( lua_State* L );	
	static int			_canGoForward					( lua_State* L );		
	static int			_clickBack						( lua_State* L );
	static int			_clickForward					( lua_State* L );
	static int			_clickRefresh					( lua_State* L );
	static int			_clickStop						( lua_State* L );
	static int			_closeWebView					( lua_State* L );
	static int			_getAllowsInlineMediaPlayback	( lua_State* L );
	static int			_getCurrentRequest				( lua_State* L );
	static int			_getMediaPlaybackRequiresAction	( lua_State* L );
	static int			_getScalesPageToFit				( lua_State* L );
	static int			_hideWebView					( lua_State* L );
	static int			_isHidden						( lua_State* L );
	static int			_initWebView					( lua_State* L );
	static int			_isLoading						( lua_State* L );
	static int			_loadData						( lua_State* L ); //TODO
	static int			_loadHTML						( lua_State* L );
	static int			_loadRequest					( lua_State* L ); 
	static int			_openUrlInSafari				( lua_State* L ); 
	static int			_runJavaScript					( lua_State* L );
	static int			_setAllowsInlineMediaPlayback	( lua_State* L );
	static int			_setMediaPlaybackRequiresAction	( lua_State* L );
	static int			_setScalesPageToFit				( lua_State* L );
	static int			_show							( lua_State* L ); 
	
public:
	DECL_LUA_FACTORY ( MOAIWebView )
	
	enum {
		DID_FAIL_LOAD_WITH_ERROR,
		SHOULD_START_LOAD_WITH_REQUEST,
		WEB_VIEW_DID_FINISH_LOAD,
		WEB_VIEW_DID_START_LOAD
	};
	
	enum {
		NAVIGATION_LINK_CLICKED,
		NAVIGATION_FORM_SUBMIT,
		NAVIGATION_BACK_FORWARD,
		NAVIGATION_RELOAD,
		NAVIGATION_FORM_RESUBMIT,
		NAVIGATION_OTHER
	};
	
	//----------------------------------------------------------------//
						MOAIWebView								();
						~MOAIWebView							();
	void				RaiseDidFailLoadWithErrorEvent			( NSError* error );
	BOOL				RaiseShouldStartLoadWithRequestEvent	( NSURLRequest* request, UIWebViewNavigationType navType );
	void				RaiseWebViewDidFinishLoadEvent			();
	void				RaiseWebViewDidStartLoadEvent			();
	void				RegisterLuaClass						( USLuaState& state );
	void				RegisterLuaFuncs						( USLuaState& state );
	STLString			ToString								();

};

#endif