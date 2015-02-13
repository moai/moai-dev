// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#import <moai-apple/NSError+MOAILib.h>
#import <moai-apple/NSString+MOAILib.h>
#import <moai-ios/MOAIAppIOS.h>
#import <moai-ios/MOAIWebViewIOS.h>
#import <moai-ios/MOAIWebViewController.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	canGoBack
	@text	Checks if the UIWebView can go back.

	@in		MOAIWebViewIOSIOS self
	@out	nil
*/
int MOAIWebViewIOS::_canGoBack ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
		
	lua_pushboolean ( state, self->mWebViewController.webView.canGoBack );
	return 1;	
}

//----------------------------------------------------------------//
/**	@lua	canGoForward
	@text	Checks if the UIWebView can go forward.

	@in		MOAIWebViewIOSIOS self
	@out	nil
*/
int MOAIWebViewIOS::_canGoForward ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );

	lua_pushboolean ( state, self->mWebViewController.webView.canGoForward );
	return 1;	
}

//----------------------------------------------------------------//
/**	@lua	clickBack
	@text	Sends an event to the UIWebView that the user has clicked
			'back'.

	@in		MOAIWebViewIOS self
	@out	nil
*/
int MOAIWebViewIOS::_clickBack ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
	
	[ self->mWebViewController.webView goBack ];
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	clickForward
	@text	Sends an event to the UIWebView that the user has clicked
			'forward'.

	@in		MOAIWebViewIOS self
	@out	nil
*/
int MOAIWebViewIOS::_clickForward ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
	
	[ self->mWebViewController.webView goForward ];
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	clickRefresh
	@text	Sends an event to the UIWebView that the user has clicked
			'refresh'.

	@in		MOAIWebViewIOS self
	@out	nil
*/
int MOAIWebViewIOS::_clickRefresh ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
	
	[ self->mWebViewController.webView reload ];
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	clickStop
	@text	Sends an event to the UIWebView that the user has clicked
			'stop'.

	@in		MOAIWebViewIOS self
	@out	nil
*/
int MOAIWebViewIOS::_clickStop ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
		
	[ self->mWebViewController.webView stopLoading ];
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getAllowsInlineMediaPlayback
	@text	Returns whether or not the UIWebView allows inline media
			playback.

	@in		MOAIWebViewIOS self
	@out	bool allowsInlineMediaPlayback
*/
int MOAIWebViewIOS::_getAllowsInlineMediaPlayback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
		
	lua_pushboolean( L, self->mWebViewController.webView.allowsInlineMediaPlayback );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getCurrentRequest
	@text	Returns the currently active URL.
			
	@in		MOAIWebViewIOS self
	@out	string currentURL
*/
int MOAIWebViewIOS::_getCurrentRequest ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
	
	NSURLRequest* request = [ self->mWebViewController.webView request ];
	cc8* urlString = [[ request.URL absoluteString ] UTF8String ];
	lua_pushstring( state, urlString );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getMediaPlaybackRequiresAction
	@text	Returns whether or not the UIWebView requires user input
			to start media playback or if it start automatically.
			
	@in		MOAIWebViewIOS self
	@out	bool mediaPlaybackRequiresUserAction
*/
int MOAIWebViewIOS::_getMediaPlaybackRequiresAction ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
		
	lua_pushboolean( L, self->mWebViewController.webView.mediaPlaybackRequiresUserAction );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getScalesPageToFit
	@text	Returns whether or not the UIWebView automatically scales
			pages to fit the view.
			
	@in		MOAIWebViewIOS self
	@out	bool scalesPageToFit
*/
int MOAIWebViewIOS::_getScalesPageToFit ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
		
	lua_pushboolean( L, self->mWebViewController.webView.scalesPageToFit );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	hide
	@text	Sets the UIWebView to hidden.
			
	@in		MOAIWebViewIOS self
	@out	nil
*/
int MOAIWebViewIOS::_hide ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
			
	BOOL animated = state.GetValue < bool >( 2, false );
	[ self->mWebViewController hide:animated ];
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	isHidden
	@text	Returns whether or not the UIWebView is hidden.
				
	@in		MOAIWebViewIOS self
	@out	bool isHidden
*/
int MOAIWebViewIOS::_isHidden ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
		
	lua_pushboolean( L, self->mWebViewController.webView.hidden );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	isLoading
	@text	Returns whether or not the UIWebView is loading a page.
				
	@in		MOAIWebViewIOS self
	@out	bool isLoading
*/
int MOAIWebViewIOS::_isLoading ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
		
	lua_pushboolean( L, self->mWebViewController.webView.loading );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	loadHTML
	@text	Sets the UIWebView main page content and base URL.

	@in		MOAIWebViewIOS self
	@in		string string
	@in		string baseURL
	@out	nil
*/
int MOAIWebViewIOS::_loadHTML ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
	
	if ( !( self->mWebViewController && self->mWebViewController.webView )) return 0;
	
	cc8* st		= state.GetValue < cc8* >( 2, "" );
	NSString* nsHtmlString = [ NSString stringWithGuessedEncoding:st ];
	
	if ( nsHtmlString != nil ) {
	
		cc8* url	= state.GetValue < cc8* >( 3, nil );
		if ( url != nil ) {
			
			NSString* nsUrlString = [ NSString stringWithUTF8String:url ];
			NSURL* nsURL = [ NSURL URLWithString:nsUrlString ];
			[ self->mWebViewController.webView loadHTMLString:nsHtmlString baseURL:nsURL ];
		}
		else {
			
			[ self->mWebViewController.webView loadHTMLString:nsHtmlString baseURL:nil ];
		}
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	loadRequest
	@text	Sets the UIWebView main page content and base URL.

	@in		MOAIWebViewIOS self
	@in		string request
	@out	nil
*/
int MOAIWebViewIOS::_loadRequest ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "US" );
		
	cc8* urlStr = lua_tostring ( state, 2 );
	NSURL* url = [ NSURL URLWithString:[ NSString stringWithUTF8String:urlStr ]];
	NSURLRequest* request = [NSURLRequest requestWithURL:url ];
		
	[ self->mWebViewController.webView loadRequest:request ];
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	openUrlInSafari
	@text	Opens the URL in Safari.

	@in		string url
	@out	boolean true if successful
*/
int MOAIWebViewIOS::_openUrlExternally ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* url = state.GetValue < cc8* >( 1, "" );
		
	BOOL result = NO;
	if ( url && url [ 0 ] != '\0' ) {
		result = [[ UIApplication sharedApplication ] openURL:[ NSURL URLWithString:[ NSString stringWithFormat: @"%s", url ]]];
	}
	lua_pushboolean ( state, result );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	runJavaScript
	@text	Runs the specified JavaScript stringin the UIWebView.
	
	@in		MOAIWebViewIOS self
	@in		string script
	@out	string result   Result of the JavaScript or nil if script failed
*/
int MOAIWebViewIOS::_runJavaScript ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
		
	cc8* script = lua_tostring ( state, 2 );
	NSString* result = [ self->mWebViewController.webView stringByEvaluatingJavaScriptFromString:[ NSString stringWithUTF8String:script ]];
	lua_pushstring ( L, [result UTF8String] );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	setAllowsInlineMediaPlayback
	@text	Sets the value of the allowsInlineMediaPlayback on the 
			UIWebView.
	
	@in		MOAIWebViewIOS self
	@in		bool allowsInlineMediaPlayback
	@out	nil
*/
int MOAIWebViewIOS::_setAllowsInlineMediaPlayback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
	
	BOOL allowsInlinePlayback = lua_toboolean ( state, 2 );
	self->mWebViewController.webView.allowsInlineMediaPlayback = allowsInlinePlayback;
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setMediaPlaybackRequiresAction
	@text	Sets the value of the mediaPlaybackRequiresUserAction on the 
			UIWebView.
	
	@in		MOAIWebViewIOS self
	@in		bool mediaPlaybackRequiresAction
	@out	nil
*/
int MOAIWebViewIOS::_setMediaPlaybackRequiresAction ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
	
	BOOL mediaPlaybackRequiresAction = lua_toboolean ( state, 2 );
	self->mWebViewController.webView.mediaPlaybackRequiresUserAction = mediaPlaybackRequiresAction;
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setScalesPageToFit
	@text	Sets the value of the scalesPageToFit on the UIWebView.
	
	@in		MOAIWebViewIOS self
	@in		bool scalesPageToFit
	@out	nil
*/
int MOAIWebViewIOS::_setScalesPageToFit ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
	
	BOOL scalesPages = lua_toboolean ( state, 2 );
	self->mWebViewController.webView.scalesPageToFit = scalesPages;
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	show
	@text	Shows the UIWebView.
	
	@in		MOAIWebViewIOS self
	@out	nil
*/
int MOAIWebViewIOS::_show ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
	
	BOOL animated = state.GetValue < bool >( 2, false );
	[ self->mWebViewController show:animated ];
	return 0;
}

//================================================================//
// MOAIWebViewIOS
//================================================================//

//----------------------------------------------------------------//
MOAIWebViewIOS::MOAIWebViewIOS () :
	mWebViewController ( false ) {

	RTTI_SINGLE ( MOAIInstanceEventSource )
	
	this->mWebViewController = [[ MOAIWebViewController alloc ] init ];
	[ this->mWebViewController setMoaiWebView:this ];
}

//----------------------------------------------------------------//
MOAIWebViewIOS::~MOAIWebViewIOS () {

	if ( this->mWebViewController ) {
		[ this->mWebViewController setMoaiWebView:NULL ];
		[ this->mWebViewController release ];
	}
}

//----------------------------------------------------------------//
void MOAIWebViewIOS::RaiseDidFailLoadWithErrorEvent ( NSError* error ) {
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	if ( this->PushListenerAndSelf ( DID_FAIL_LOAD_WITH_ERROR, state )) {
		[ error toLua:state ];
		state.DebugCall ( 2, 0 );
	}
}

//----------------------------------------------------------------//
BOOL MOAIWebViewIOS::RaiseShouldStartLoadWithRequestEvent ( NSURLRequest* request, UIWebViewNavigationType navType ) {

	cc8* urlString = [[ request.URL absoluteString ] UTF8String ];
	int nav = ( int )navType;
	bool result = true;
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	if ( this->PushListenerAndSelf ( SHOULD_START_LOAD_WITH_REQUEST, state )) {
		
		lua_pushstring ( state, urlString );
		lua_pushinteger ( state, nav );			
		state.DebugCall ( 3, 1 );
		result = lua_toboolean ( state, -1 );
	}
	
	return result;
}

//----------------------------------------------------------------//
void MOAIWebViewIOS::RaiseWebViewDidFinishLoadEvent () {

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	if ( this->PushListenerAndSelf ( WEB_VIEW_DID_FINISH_LOAD, state )) {
			state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIWebViewIOS::RaiseWebViewDidStartLoadEvent () {

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	if ( this->PushListenerAndSelf ( WEB_VIEW_DID_START_LOAD, state )) {
			state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIWebViewIOS::RaiseWebViewDidHideEvent () {
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	if ( this->PushListenerAndSelf ( WEB_VIEW_DID_HIDE, state )) {
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIWebViewIOS::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "DID_FAIL_LOAD_WITH_ERROR", 		( u32 )DID_FAIL_LOAD_WITH_ERROR );
	state.SetField ( -1, "SHOULD_START_LOAD_WITH_REQUEST",	( u32 )SHOULD_START_LOAD_WITH_REQUEST );
	state.SetField ( -1, "WEB_VIEW_DID_FINISH_LOAD", 		( u32 )WEB_VIEW_DID_FINISH_LOAD );
	state.SetField ( -1, "WEB_VIEW_DID_START_LOAD", 		( u32 )WEB_VIEW_DID_START_LOAD );
	state.SetField ( -1, "WEB_VIEW_DID_HIDE",				( u32 )WEB_VIEW_DID_HIDE );
	
	state.SetField( -1, "NAVIGATION_LINK_CLICKED", 			( u32 )NAVIGATION_LINK_CLICKED );
	state.SetField( -1, "NAVIGATION_FORM_SUBMIT", 			( u32 )NAVIGATION_FORM_SUBMIT );
	state.SetField( -1, "NAVIGATION_BACK_FORWARD", 			( u32 )NAVIGATION_BACK_FORWARD );
	state.SetField( -1, "NAVIGATION_RELOAD", 				( u32 )NAVIGATION_RELOAD );
	state.SetField( -1, "NAVIGATION_FORM_RESUBMIT", 		( u32 )NAVIGATION_FORM_RESUBMIT );
	state.SetField( -1, "NAVIGATION_OTHER", 				( u32 )NAVIGATION_OTHER );
	
	luaL_Reg regTable [] = {
		{ "openUrlExternally",	_openUrlExternally },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIWebViewIOS::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIInstanceEventSource::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "canGoBack",						_canGoBack },
		{ "canGoForward",					_canGoForward },
		{ "clickBack",						_clickBack },
		{ "clickForward",					_clickForward },
		{ "clickRefresh",					_clickRefresh },
		{ "clickStop",						_clickStop },
		{ "getAllowsInlineMediaPlayback",	_getAllowsInlineMediaPlayback },
		{ "getCurrentRequest",				_getCurrentRequest },	
		{ "getMediaPlaybackRequiresAction", _getMediaPlaybackRequiresAction },
		{ "getScalesPageToFit",				_getScalesPageToFit },
		{ "isHidden",						_isHidden },
		{ "isLoading",						_isLoading },
		{ "loadHTML",						_loadHTML },
		{ "loadRequest",					_loadRequest },
		{ "runJavaScript",					_runJavaScript },
		{ "setAllowsInlineMediaPLayback",   _setAllowsInlineMediaPlayback },
		{ "setMediaPlaybackRequiresAction", _setMediaPlaybackRequiresAction },
		{ "setScalesPageToFit",				_setScalesPageToFit },
		{ "show",							_show },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}
