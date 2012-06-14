// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#import <moaiext-iphone/MOAIWebViewIOS.h>
#import <moaiext-iphone/MOAIWebViewDelegate.h>
#import <moaiext-iphone/NSError+MOAILib.h>
#import <moaiext-iphone/NSString+MOAILib.h>

#define TOOL_BAR_HEIGHT 44

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	canGoBack
	@text	Checks if the UIWebView can go back.

	@in		MOAIWebViewIOSIOS self
	@out	nil
*/
int MOAIWebViewIOS::_canGoBack ( lua_State* L ) {
	
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
		
	lua_pushboolean ( state, self->mWebView.canGoBack );
	
	return 1;	
}

//----------------------------------------------------------------//
/**	@name	canGoForward
	@text	Checks if the UIWebView can go forward.

	@in		MOAIWebViewIOSIOS self
	@out	nil
*/
int MOAIWebViewIOS::_canGoForward ( lua_State* L ) {
	
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );

	lua_pushboolean ( state, self->mWebView.canGoForward );
	
	return 1;	
}

//----------------------------------------------------------------//
/**	@name	clickBack
	@text	Sends an event to the UIWebView that the user has clicked
			'back'.

	@in		MOAIWebViewIOS self
	@out	nil
*/
int MOAIWebViewIOS::_clickBack ( lua_State* L ) {
	
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
	
	[ self->mWebView goBack ];
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	clickForward
	@text	Sends an event to the UIWebView that the user has clicked
			'forward'.

	@in		MOAIWebViewIOS self
	@out	nil
*/
int MOAIWebViewIOS::_clickForward ( lua_State* L ) {
	
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
	
	[ self->mWebView goForward ];
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	clickRefresh
	@text	Sends an event to the UIWebView that the user has clicked
			'refresh'.

	@in		MOAIWebViewIOS self
	@out	nil
*/
int MOAIWebViewIOS::_clickRefresh ( lua_State* L ) {
	
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
	
	[ self->mWebView reload ];
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	clickStop
	@text	Sends an event to the UIWebView that the user has clicked
			'stop'.

	@in		MOAIWebViewIOS self
	@out	nil
*/
int MOAIWebViewIOS::_clickStop ( lua_State* L ) {
	
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
		
	[ self->mWebView stopLoading ];
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	closeWebView
	@text	Closes the current UIWebView.

	@in		MOAIWebViewIOS self
	@out	nil
*/
int MOAIWebViewIOS::_closeWebView ( lua_State* L ) {
	
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
		
	if ( self->mWebView ) {
		
		self->mWebView.delegate = nil;
		[ self->mWebView stopLoading ];
		[ self->mWebView removeFromSuperview ];
	}	
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getAllowsInlineMediaPlayback
	@text	Returns whether or not the UIWebView allows inline media
			playback.

	@in		MOAIWebViewIOS self
	@out	bool allowsInlineMediaPlayback
*/
int MOAIWebViewIOS::_getAllowsInlineMediaPlayback ( lua_State* L ) {
	
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
		
	lua_pushboolean( L, self->mWebView.allowsInlineMediaPlayback );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getCurrentRequest
	@text	Returns the currently active URL.
			
	@in		MOAIWebViewIOS self
	@out	string currentURL
*/
int MOAIWebViewIOS::_getCurrentRequest ( lua_State* L ) {
	
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
	
	NSURLRequest* request = [ self->mWebView request ];
	cc8* urlString = [[ request.URL absoluteString ] UTF8String ];	
	lua_pushstring( state, urlString );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getMediaPlaybackRequiresAction
	@text	Returns whether or not the UIWebView requires user input
			to start media playback or if it start automatically.
			
	@in		MOAIWebViewIOS self
	@out	bool mediaPlaybackRequiresUserAction
*/
int MOAIWebViewIOS::_getMediaPlaybackRequiresAction ( lua_State* L ) {
	
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
		
	lua_pushboolean( L, self->mWebView.mediaPlaybackRequiresUserAction );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getScalesPageToFit
	@text	Returns whether or not the UIWebView automatically scales
			pages to fit the view.
			
	@in		MOAIWebViewIOS self
	@out	bool scalesPageToFit
*/
int MOAIWebViewIOS::_getScalesPageToFit ( lua_State* L ) {
	
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
		
	lua_pushboolean( L, self->mWebView.scalesPageToFit );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	hasToolBar
	@text	Sets whether or not the UIWebView has a tool bar. Default is true.
			
	@in		MOAIWebViewIOS self
	@in		bool hasToolBar
	@out	nil
*/
int MOAIWebViewIOS::_hasToolBar ( lua_State* L ) {
	
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
			
	self->mHasToolBar = lua_toboolean ( state, 2 );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	hideWebView
	@text	Sets the UIWebView to hidden.
			
	@in		MOAIWebViewIOS self
	@out	nil
*/
int MOAIWebViewIOS::_hideWebView ( lua_State* L ) {
	
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
			
	self->mWebView.hidden = true;
	self->mToolBar.hidden = true;
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	initWebView
	@text	Kills current UIWebView (if one exists) and creates a
			new one with desired dimensions.
			
	@in		MOAIWebViewIOS self
	@out	bool scalesPageToFit
*/
int MOAIWebViewIOS::_initWebView ( lua_State* L ) {
	
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
	
	int left = lua_tointeger ( state, 2 );
	int top = lua_tointeger ( state, 3 );
	int width = lua_tointeger ( state, 4 );
	int height = lua_tointeger ( state, 5 );
	bool hidden = lua_toboolean ( state, 6 );
			
	if ( self->mWebView ) {
		
		self->mWebView.delegate = nil;
		[ self->mWebView stopLoading ];
		[ self->mWebView removeFromSuperview ];
	}	
	
	UIWindow* window = [[ UIApplication sharedApplication ] keyWindow ];
	UIViewController* rootVC = [ window rootViewController ];		
	
	if ( self->mHasToolBar ) {
				
		self->mWebView = [[[ UIWebView alloc ] initWithFrame:CGRectMake ( left, top + TOOL_BAR_HEIGHT , width, height - TOOL_BAR_HEIGHT )] autorelease ];					
		self->mWebView.transform = CGAffineTransformConcat ([ rootVC.view transform ], CGAffineTransformMakeRotation (( float )( -M_PI / 2 )));
				
		self->mToolBar.frame = CGRectMake( left, top, width, TOOL_BAR_HEIGHT );	
		if ( rootVC.interfaceOrientation == UIInterfaceOrientationLandscapeLeft ) {
			
			self->mToolBar.transform = CGAffineTransformConcat ([ rootVC.view transform ], CGAffineTransformMakeRotation(( float )( M_PI / 2 )));				
			self->mWebView.transform = CGAffineTransformConcat ([ rootVC.view transform ], CGAffineTransformMakeRotation (( float )( M_PI / 2 )));
		}
		else {
			
			self->mToolBar.transform = CGAffineTransformConcat ([ rootVC.view transform ], CGAffineTransformMakeRotation(( float )( -M_PI / 2 )));				
			self->mWebView.transform = CGAffineTransformConcat ([ rootVC.view transform ], CGAffineTransformMakeRotation (( float )( -M_PI / 2 )));
		}
		
		self->mToolBar.hidden = hidden;		
		[ rootVC.view addSubview:self->mToolBar ];	
	}
	else {
	
		self->mWebView = [[[ UIWebView alloc ] initWithFrame:CGRectMake ( left, top, width, height )] autorelease ];
		if ( rootVC.interfaceOrientation == UIInterfaceOrientationLandscapeLeft ) {					
			
			self->mWebView.transform = CGAffineTransformConcat ([ rootVC.view transform ], CGAffineTransformMakeRotation(( float )( -M_PI / 2 )));
		}
		else {
			
			self->mWebView.transform = CGAffineTransformConcat ([ rootVC.view transform ], CGAffineTransformMakeRotation(( float )( -M_PI / 2 )));
		}			
	}
		
	[ self->mWebView setDelegate:self->mWebViewDelegate ];
	[ self->mWebView setScalesPageToFit:YES ];
	[ self->mWebView setMultipleTouchEnabled:YES ];
	[ rootVC.view addSubview:self->mWebView ];

	if ( hidden ) {
		
		self->mWebView.hidden = true;
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@name	isHidden
	@text	Returns whether or not the UIWebView is hidden.
				
	@in		MOAIWebViewIOS self
	@out	bool isHidden
*/
int MOAIWebViewIOS::_isHidden ( lua_State* L ) {
	
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
		
	lua_pushboolean( L, self->mWebView.hidden );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	isLoading
	@text	Returns whether or not the UIWebView is loading a page.
				
	@in		MOAIWebViewIOS self
	@out	bool isLoading
*/
int MOAIWebViewIOS::_isLoading ( lua_State* L ) {
	
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
		
	lua_pushboolean( L, self->mWebView.loading );
	
	return 1;
}

//----------------------------------------------------------------//
// TODO
int MOAIWebViewIOS::_loadData ( lua_State* L ) {
	
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	loadHTML
	@text	Sets the UIWebView main page content and base URL.

	@in		MOAIWebViewIOS self
	@in		string string
	@in		string baseURL
	@out	nil
*/
int MOAIWebViewIOS::_loadHTML ( lua_State* L ) {	
	
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
	
	cc8* st = lua_tostring ( state, 2 );
	cc8* url = lua_tostring ( state, 3 );
	
	NSString* nsHtmlString = [[ NSString alloc ] initWithUTF8String:st ];
	
	if ( url != NULL ) {
		
		NSString* nsUrlString = [[ NSString alloc ] initWithUTF8String:url ];
		NSURL* nsURL = [[ NSURL alloc ] initWithString:nsUrlString ];
		[ self->mWebView loadHTMLString:nsHtmlString baseURL:nsURL ];
	}
	else {
		
		[ self->mWebView loadHTMLString:nsHtmlString baseURL:nil ];
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@name	loadRequest
	@text	Sets the UIWebView main page content and base URL.

	@in		MOAIWebViewIOS self
	@in		string request
	@out	nil
*/
int MOAIWebViewIOS::_loadRequest ( lua_State* L ) {
	
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
		
	cc8* urlStr = lua_tostring ( state, 2 );
	NSString* urlString = [[ NSString alloc ] initWithCString:urlStr encoding:[NSString defaultCStringEncoding] ];
	NSURL* url = [ NSURL URLWithString:urlString ];
	NSURLRequest* request = [NSURLRequest requestWithURL:url ];
	
	if ( self->mWebView.delegate == nil ) {
		
		[ self->mWebView setDelegate:self->mWebViewDelegate ];
	}
		
	[ self->mWebView loadRequest:request ];
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	openUrlInSafari
	@text	Opens the URL in Safari.

	@in		string url
	@out	nil
*/
int MOAIWebViewIOS::_openUrlInSafari ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	if ( !state.CheckParams ( 1, "S" )) {
		
		return 0;
	}
	
	cc8* urlStr = lua_tostring ( state, 1 );
	NSString* urlString = [[ NSString alloc ] initWithCString:urlStr encoding:[ NSString defaultCStringEncoding ]];
	NSURL* url = [ NSURL URLWithString:urlString ];
	
	[[ UIApplication sharedApplication ] openURL:url ];
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	runJavaScript
	@text	Runs the specified JavaScript stringin the UIWebView.
	
	@in		MOAIWebViewIOS self
	@in		string script
	@out	string result   Result of the JavaScript or nil if script failed
*/
int MOAIWebViewIOS::_runJavaScript ( lua_State* L ) {	
	
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
		
	cc8* script = lua_tostring ( state, 2 );
	NSString* result = [ self->mWebView stringByEvaluatingJavaScriptFromString:[[ NSString alloc ] initWithUTF8String:script ]];
	
	lua_pushstring ( L, [result UTF8String] );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	setAllowsInlineMediaPlayback
	@text	Sets the value of the allowsInlineMediaPlayback on the 
			UIWebView.
	
	@in		MOAIWebViewIOS self
	@in		bool allowsInlineMediaPlayback
	@out	nil
*/
int MOAIWebViewIOS::_setAllowsInlineMediaPlayback ( lua_State* L ) {
		
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
	
	BOOL allowsInlinePlayback = lua_toboolean ( state, 2 );
	self->mWebView.allowsInlineMediaPlayback = allowsInlinePlayback;
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setMediaPlaybackRequiresAction
	@text	Sets the value of the mediaPlaybackRequiresUserAction on the 
			UIWebView.
	
	@in		MOAIWebViewIOS self
	@in		bool mediaPlaybackRequiresAction
	@out	nil
*/
int MOAIWebViewIOS::_setMediaPlaybackRequiresAction ( lua_State* L ) {	
	
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
	
	BOOL mediaPlaybackRequiresAction = lua_toboolean ( state, 2 );
	self->mWebView.mediaPlaybackRequiresUserAction = mediaPlaybackRequiresAction;
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setScalesPageToFit
	@text	Sets the value of the scalesPageToFit on the UIWebView.
	
	@in		MOAIWebViewIOS self
	@in		bool scalesPageToFit
	@out	nil
*/
int MOAIWebViewIOS::_setScalesPageToFit ( lua_State* L ) {	
	
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
	
	BOOL scalesPages = lua_toboolean ( state, 2 );
	self->mWebView.scalesPageToFit = scalesPages;
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	show
	@text	Shows the UIWebView.
	
	@in		MOAIWebViewIOS self
	@out	nil
*/
int MOAIWebViewIOS::_show ( lua_State* L ) {	
	
	MOAI_LUA_SETUP ( MOAIWebViewIOS, "U" );
			
	self->mWebView.hidden = false;		
	if ( self->mHasToolBar ) {
		
		self->mToolBar.hidden = false;
	}
	
	return 0;
}

//================================================================//
// MOAIWebViewIOS
//================================================================//

//----------------------------------------------------------------//
MOAIWebViewIOS::MOAIWebViewIOS () :
	mWebView ( 0 ) {

	RTTI_SINGLE ( MOAIInstanceEventSource )
	
	mWebViewDelegate = [[ MOAIWebViewDelegate alloc ] retain ];
	mWebViewDelegate.mMOAIWebView = this;

	//create toolbar using new
	mToolBar = [ UIToolbar new ];
	mToolBar.barStyle = UIBarStyleDefault;
	mHasToolBar = true;
	
	UIBarButtonItem *done = [[ UIBarButtonItem alloc ] initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:mWebViewDelegate action:@selector ( doneButtonPressed: )];
	
	NSArray* items = [ NSArray arrayWithObjects: done, nil ];
	[ done release ];
	[ mToolBar setItems:items animated:NO ];
}

//----------------------------------------------------------------//
MOAIWebViewIOS::~MOAIWebViewIOS () {

	if ( mWebView ) {
		
		mWebView.delegate = nil;
		[ mWebView stopLoading ];
		[ mWebView removeFromSuperview ];
	}
	
	if ( mWebViewDelegate ) {
		
		[ mWebViewDelegate release ];
		mWebViewDelegate = 0;
	}
}

//----------------------------------------------------------------//
void MOAIWebViewIOS::Hide () {

	mWebView.hidden = true;
	mToolBar.hidden = true;
}

//----------------------------------------------------------------//
void MOAIWebViewIOS::RaiseDidFailLoadWithErrorEvent ( NSError* error ) {
	
	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	if ( this->PushListenerAndSelf ( DID_FAIL_LOAD_WITH_ERROR, state )) {
		[ error toLua:state ];
		state.DebugCall ( 2, 0 );
	}
}

//----------------------------------------------------------------//
BOOL MOAIWebViewIOS::RaiseShouldStartLoadWithRequestEvent ( NSURLRequest* request, UIWebViewNavigationType navType ) {

	cc8* urlString = [[ request.URL absoluteString ] UTF8String ];
	int nav = navType;
	bool result = true;
	
	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
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

	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	if ( this->PushListenerAndSelf ( WEB_VIEW_DID_FINISH_LOAD, state )) {

			state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIWebViewIOS::RaiseWebViewDidStartLoadEvent () {

	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	if ( this->PushListenerAndSelf ( WEB_VIEW_DID_START_LOAD, state )) {

			state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIWebViewIOS::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "DID_FAIL_LOAD_WITH_ERROR", 		( u32 )DID_FAIL_LOAD_WITH_ERROR );
	state.SetField ( -1, "SHOULD_START_LOAD_WITH_REQUEST",	( u32 )SHOULD_START_LOAD_WITH_REQUEST );
	state.SetField ( -1, "WEB_VIEW_DID_FINISH_LOAD", 		( u32 )WEB_VIEW_DID_FINISH_LOAD );
	state.SetField ( -1, "WEB_VIEW_DID_START_LOAD", 		( u32 )WEB_VIEW_DID_START_LOAD );
	
	state.SetField( -1, "NAVIGATION_LINK_CLICKED", 			( u32 )NAVIGATION_LINK_CLICKED );
	state.SetField( -1, "NAVIGATION_FORM_SUBMIT", 			( u32 )NAVIGATION_FORM_SUBMIT );
	state.SetField( -1, "NAVIGATION_BACK_FORWARD", 			( u32 )NAVIGATION_BACK_FORWARD );
	state.SetField( -1, "NAVIGATION_RELOAD", 				( u32 )NAVIGATION_RELOAD );
	state.SetField( -1, "NAVIGATION_FORM_RESUBMIT", 		( u32 )NAVIGATION_FORM_RESUBMIT );
	state.SetField( -1, "NAVIGATION_OTHER", 				( u32 )NAVIGATION_OTHER );
	
	luaL_Reg regTable [] = {
		{ "openUrlInSafari",	_openUrlInSafari },
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
		{ "initWebView",					_initWebView },
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

//----------------------------------------------------------------//
STLString MOAIWebViewIOS::ToString () {

	STLString repr;
	return repr;
}