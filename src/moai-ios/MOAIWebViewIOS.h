// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIWEBVIEWIOS_H
#define MOAIWEBVIEWIOS_H

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <moai-core/headers.h>

@class MOAIWebViewController;

//================================================================//
// MOAIWebViewIOS
//================================================================//
/**	@lua	MOAIWebViewIOS
	@text	Wrapper for UIWebView interaction on iOS devices.

	@const	DID_FAIL_LOAD_WITH_ERROR		Event indicating a failed UIWebView load.
	@const	SHOULD_START_LOAD_WITH_REQUEST	Event indicating an attempt to load a UIWebView.
	@const	WEB_VIEW_DID_FINISH_LOAD		Event indicating a completed UIWebView load.
	@const	WEB_VIEW_DID_START_LOAD			Event indicating the start of a UIWebView load.

	@const	NAVIGATION_LINK_CLICKED			Indicates the the navigation was due to a link click.
	@const	NAVIGATION_FORM_SUBMIT			Indicates the the navigation was due to a form submit.
	@const	NAVIGATION_BACK_FORWARD			Indicates the the navigation was due to a back/forward operation.
	@const	NAVIGATION_RELOAD				Indicates the the navigation was due to a page reload.
	@const	NAVIGATION_FORM_RESUBMIT		Indicates the the navigation was due to a form resubmit.
	@const	NAVIGATION_OTHER				Indicates the the navigation was due to other reasons.
*/
class MOAIWebViewIOS : 
	public MOAIInstanceEventSource {
private:
	
	MOAIWebViewController*	mWebViewController;
	
	//----------------------------------------------------------------//
	static int	_canGoBack						( lua_State* L );	
	static int	_canGoForward					( lua_State* L );		
	static int	_clickBack						( lua_State* L );
	static int	_clickForward					( lua_State* L );
	static int	_clickRefresh					( lua_State* L );
	static int	_clickStop						( lua_State* L );
	static int	_getAllowsInlineMediaPlayback	( lua_State* L );
	static int	_getCurrentRequest				( lua_State* L );
	static int	_getMediaPlaybackRequiresAction	( lua_State* L );
	static int	_getScalesPageToFit				( lua_State* L );
	static int	_hide							( lua_State* L );
	static int	_isHidden						( lua_State* L );
	static int	_isLoading						( lua_State* L );
	static int	_loadHTML						( lua_State* L );
	static int	_loadRequest					( lua_State* L ); 
	static int	_openUrlExternally				( lua_State* L );
	static int	_runJavaScript					( lua_State* L );
	static int	_setAllowsInlineMediaPlayback	( lua_State* L );
	static int	_setMediaPlaybackRequiresAction	( lua_State* L );
	static int	_setScalesPageToFit				( lua_State* L );
	static int	_show							( lua_State* L ); 

	//----------------------------------------------------------------//
	void		MOAILuaObject_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void		MOAILuaObject_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

public:

	DECL_LUA_FACTORY ( MOAIWebViewIOS )
	
	enum {
		DID_FAIL_LOAD_WITH_ERROR,
		SHOULD_START_LOAD_WITH_REQUEST,
		WEB_VIEW_DID_FINISH_LOAD,
		WEB_VIEW_DID_START_LOAD,
		WEB_VIEW_DID_HIDE
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
	void		Close									();
	void		Hide									( bool clean );
				MOAIWebViewIOS							();
				~MOAIWebViewIOS							();
	void		RaiseDidFailLoadWithErrorEvent			( NSError* error );
	BOOL		RaiseShouldStartLoadWithRequestEvent	( NSURLRequest* request, UIWebViewNavigationType navType );
	void		RaiseWebViewDidFinishLoadEvent			();
	void		RaiseWebViewDidStartLoadEvent			();
	void		RaiseWebViewDidHideEvent				();
};

#endif
