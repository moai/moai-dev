// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#import <moai-apple/NSError+MOAILib.h>
#import <moai-apple/NSString+MOAILib.h>
#import <moai-ios/MOAIAppIOS.h>
#import <moai-ios/MOAIWebViewIOS.h>
#import <moai-ios/MOAIWebViewController.h>

#define TOOLBAR_HEIGHT 44

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
		
	//lua_pushboolean ( state, self->mWebView.canGoBack );
	
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

	//lua_pushboolean ( state, self->mWebView.canGoForward );
	
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
	
	//[ self->mWebView goBack ];
	
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
	
	//[ self->mWebView goForward ];
	
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
	
	//[ self->mWebView reload ];
	
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
		
	//[ self->mWebView stopLoading ];
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	closeWebView
	@text	Closes the current UIWebView.

	@in		MOAIWebViewIOS self
	@out	nil
*/
int MOAIWebViewIOS::_closeWebView ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
	
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
		
	//lua_pushboolean( L, self->mWebView.allowsInlineMediaPlayback );
	
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
	
//	NSURLRequest* request = [ self->mWebView request ];
//	cc8* urlString = [[ request.URL absoluteString ] UTF8String ];	
//	lua_pushstring( state, urlString );
	
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
		
	//lua_pushboolean( L, self->mWebView.mediaPlaybackRequiresUserAction );
	
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
		
	//lua_pushboolean( L, self->mWebView.scalesPageToFit );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	hasToolBar
	@text	Sets whether or not the UIWebView has a tool bar. Default is true.
			
	@in		MOAIWebViewIOS self
	@in		bool hasToolBar
	@out	nil
*/
int MOAIWebViewIOS::_hasToolBar ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
			
	//self->mHasToolBar = lua_toboolean ( state, 2 );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	hideWebView
	@text	Sets the UIWebView to hidden.
			
	@in		MOAIWebViewIOS self
	@out	nil
*/
int MOAIWebViewIOS::_hideWebView ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
			
	//self->Hide( false );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	init
	@text	Kills current UIWebView (if one exists) and creates a
			new one with desired dimensions.
			
	@in		MOAIWebViewIOS self
	@out	bool scalesPageToFit
*/
int MOAIWebViewIOS::_init ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
	
	bool hidden = state.GetValue < bool >( 6, false );
	
	self->mWebViewController = [[ MOAIWebViewController alloc ] init :TOOLBAR_HEIGHT ];
	if ( !hidden ) {
		[ self->mWebViewController show:NO ];
	}
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
		
	//lua_pushboolean( L, self->mWebView.hidden );
	
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
		
	//lua_pushboolean( L, self->mWebView.loading );
	
	return 1;
}

//----------------------------------------------------------------//
// TODO
int MOAIWebViewIOS::_loadData ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
	
	return 0;
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
	
//	cc8* st		= state.GetValue < cc8* >( 2, "" );
//	NSString* nsHtmlString = [ NSString stringWithGuessedEncoding:st ];
//	
//	if ( nsHtmlString != nil ) {
//	
//		cc8* url	= state.GetValue < cc8* >( 3, nil );
//		if ( url != nil ) {
//			
//			NSString* nsUrlString = [ NSString stringWithUTF8String:url ];
//			NSURL* nsURL = [ NSURL URLWithString:nsUrlString ];
//			[ self->mWebView loadHTMLString:nsHtmlString baseURL:nsURL ];
//		}
//		else {
//			
//			[ self->mWebView loadHTMLString:nsHtmlString baseURL:nil ];
//		}
//	}
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
		
//	cc8* script = lua_tostring ( state, 2 );
//	NSString* result = [ self->mWebView stringByEvaluatingJavaScriptFromString:[ NSString stringWithUTF8String:script ]];
//	
//	lua_pushstring ( L, [result UTF8String] );
	
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
	
	//BOOL allowsInlinePlayback = lua_toboolean ( state, 2 );
	//self->mWebView.allowsInlineMediaPlayback = allowsInlinePlayback;
	
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
	
	//BOOL mediaPlaybackRequiresAction = lua_toboolean ( state, 2 );
	//self->mWebView.mediaPlaybackRequiresUserAction = mediaPlaybackRequiresAction;
	
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
	
	//BOOL scalesPages = lua_toboolean ( state, 2 );
	//self->mWebView.scalesPageToFit = scalesPages;
	
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
	
	[ self->mWebViewController show:NO ];
	
//	self->mAnimate = lua_toboolean ( state, 2 );
//	
//	UIWindow* window = [[ UIApplication sharedApplication ] keyWindow ];
//	UIViewController* rootVC = [ window rootViewController ];
//	
//	if ( self->mToolBar ) {
//		
//		[ rootVC.view addSubview:self->mToolBar ];
//	}
//	
//	[ rootVC.view addSubview:self->mWebView ];
//	
//	if ( self->mAnimate ) {
//		
//		CGSize screenSize = MOAIAppIOS::GetScreenBoundsFromCurrentOrientation ([[ UIScreen mainScreen ] bounds ]).size;
//		BOOL landscape = rootVC.interfaceOrientation == UIInterfaceOrientationLandscapeLeft ||
//						 rootVC.interfaceOrientation == UIInterfaceOrientationLandscapeRight;
//		CGFloat offset = landscape?screenSize.width:screenSize.height;
//		
//		if ( self->mToolBar ) {
//			
//			self->mToolBar.transform = CGAffineTransformConcat (self->mToolBar.transform, CGAffineTransformMakeTranslation(0, offset));
//			self->mToolBar.hidden = false;
//		}
//		
//		self->mWebView.transform = CGAffineTransformConcat (self->mWebView.transform, CGAffineTransformMakeTranslation(0, offset));
//		self->mWebView.hidden = false;
//		
//		[UIView animateWithDuration:0.25
//							  delay:0.1
//							options: UIViewAnimationCurveEaseOut
//						 animations:^{
//							 
//							 if ( self->mToolBar ) {
//								 
//								 self->mToolBar.transform = CGAffineTransformConcat (self->mToolBar.transform, CGAffineTransformMakeTranslation(0, -offset));
//							 }
//							 
//							 self->mWebView.transform = CGAffineTransformConcat (self->mWebView.transform, CGAffineTransformMakeTranslation(0, -offset));
//						 }
//						 completion:^(BOOL){
//						 }];
//	}
//	else {
//
//		self->mWebView.hidden = false;		
//		if ( self->mToolBar ) {
//		
//			self->mToolBar.hidden = false;
//		}
//	}
	
	return 0;
}

//================================================================//
// MOAIWebViewIOS
//================================================================//


//----------------------------------------------------------------//
void MOAIWebViewIOS::Close () {
	
	Hide ( true );
}

//----------------------------------------------------------------//
void MOAIWebViewIOS::Hide ( bool clean ) {

//	if ( mAnimate ) {
//		
//		CGSize screenSize = MOAIAppIOS::GetScreenBoundsFromCurrentOrientation ([[ UIScreen mainScreen ] bounds ]).size;
//		
//		UIWindow* window = [[ UIApplication sharedApplication ] keyWindow ];
//		UIViewController* rootVC = [ window rootViewController ];
//		
//		BOOL landscape = rootVC.interfaceOrientation == UIInterfaceOrientationLandscapeLeft ||
//		rootVC.interfaceOrientation == UIInterfaceOrientationLandscapeRight;
//		CGFloat offset = landscape?screenSize.width:screenSize.height;
//		
//		[UIView animateWithDuration:0.25
//							  delay:0.1
//							options: UIViewAnimationCurveEaseIn
//						 animations:^{
//							 
//							 if ( mHasToolBar ) {
//								 
//								 mToolBar.transform = CGAffineTransformConcat (mToolBar.transform, CGAffineTransformMakeTranslation(0, offset));
//							 }
//							 
//							 mWebView.transform = CGAffineTransformConcat (mWebView.transform, CGAffineTransformMakeTranslation(0, offset));
//						 }
//						 completion:^(BOOL finished){
//							UNUSED ( finished );
//							 
//							mToolBar.hidden = true;
//							mToolBar.transform = CGAffineTransformConcat (mToolBar.transform, CGAffineTransformMakeTranslation(0, -offset));
//							[ mToolBar removeFromSuperview ];
//
//							mWebView.hidden = true;
//							mWebView.delegate = nil;
//							mWebView.transform = CGAffineTransformConcat (mWebView.transform, CGAffineTransformMakeTranslation(0, -offset));
//							[ mWebView removeFromSuperview ];
//							 
//							if ( clean ) {
//
//								[ mWebView stopLoading ];
//								[ mWebView release ];
//								mWebView = nil;
//
//								[ mToolBar release ];
//								mToolBar = nil;
//							}
//
//							this->RaiseWebViewDidHideEvent ();
//						 }];
//	}
//	else {
//	
//		mToolBar.hidden = true;
//		[ mToolBar removeFromSuperview ];
//		
//		mWebView.hidden = true;
//		mWebView.delegate = nil;
//		[ mWebView removeFromSuperview ];
//		
//		if ( clean ) {
//			
//			[ mWebView stopLoading ];
//			[ mWebView release ];
//			mWebView = nil;
//			
//			[ mToolBar release ];
//			mToolBar = nil;
//		}
//		
//		this->RaiseWebViewDidHideEvent ();
//	}
}

//----------------------------------------------------------------//
MOAIWebViewIOS::MOAIWebViewIOS () :
	mWebViewController ( false ) {

	RTTI_SINGLE ( MOAIInstanceEventSource )
}

//----------------------------------------------------------------//
MOAIWebViewIOS::~MOAIWebViewIOS () {

	if ( this->mWebViewController ) {
		[ this->mWebViewController release ];
	}
}

//----------------------------------------------------------------//
void MOAIWebViewIOS::RaiseDidFailLoadWithErrorEvent ( NSError* error ) {
	
//	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
//	if ( this->PushListenerAndSelf ( DID_FAIL_LOAD_WITH_ERROR, state )) {
//		[ error toLua:state ];
//		state.DebugCall ( 2, 0 );
//	}
}

//----------------------------------------------------------------//
BOOL MOAIWebViewIOS::RaiseShouldStartLoadWithRequestEvent ( NSURLRequest* request, UIWebViewNavigationType navType ) {

	cc8* urlString = [[ request.URL absoluteString ] UTF8String ];
	int nav = ( int )navType;
	bool result = true;
	
//	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
//	if ( this->PushListenerAndSelf ( SHOULD_START_LOAD_WITH_REQUEST, state )) {
//		
//		lua_pushstring ( state, urlString );
//		lua_pushinteger ( state, nav );			
//		state.DebugCall ( 3, 1 );
//		result = lua_toboolean ( state, -1 );
//	}
	
	return result;
}

//----------------------------------------------------------------//
void MOAIWebViewIOS::RaiseWebViewDidFinishLoadEvent () {

//	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
//	if ( this->PushListenerAndSelf ( WEB_VIEW_DID_FINISH_LOAD, state )) {
//			state.DebugCall ( 1, 0 );
//	}
}

//----------------------------------------------------------------//
void MOAIWebViewIOS::RaiseWebViewDidStartLoadEvent () {

//	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
//	if ( this->PushListenerAndSelf ( WEB_VIEW_DID_START_LOAD, state )) {
//			state.DebugCall ( 1, 0 );
//	}
}

//----------------------------------------------------------------//
void MOAIWebViewIOS::RaiseWebViewDidHideEvent () {
	
//	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
//	if ( this->PushListenerAndSelf ( WEB_VIEW_DID_HIDE, state )) {
//		state.DebugCall ( 1, 0 );
//	}
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
		{ "closeWebView",					_closeWebView },
		{ "getAllowsInlineMediaPlayback",	_getAllowsInlineMediaPlayback },
		{ "getCurrentRequest",				_getCurrentRequest },	
		{ "getMediaPlaybackRequiresAction", _getMediaPlaybackRequiresAction },
		{ "getScalesPageToFit",				_getScalesPageToFit },
		{ "hasToolBar",						_hasToolBar },
		{ "hideWebView",					_hideWebView },
		{ "init",							_init },
		{ "initWebView",					_init }, // bach compat
		{ "isHidden",						_isHidden },
		{ "isLoading",						_isLoading },
		{ "loadData",						_loadData },
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
