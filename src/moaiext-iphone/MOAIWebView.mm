// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#import <moai-iphone/MOAIWebView.h>
#import <moai-iphone/MoaiUiWebView.h>
#import <moai-iphone/NSError+MOAILib.h>
#import <moai-iphone/NSString+MOAILib.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	canGoBack
	@text	Checks if the UIWebView can go back

	@in		MOAIWebView self
	@out	nil
*/
int MOAIWebView::_canGoBack ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebView, "U" );
		
	lua_pushboolean ( state, self->mWebView.canGoBack );
	return 1;	
}

//----------------------------------------------------------------//
/**	@name	canGoForward
	@text	Checks if the UIWebView can go forward

	@in		MOAIWebView self
	@out	nil
*/
int MOAIWebView::_canGoForward ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebView, "U" );

	lua_pushboolean ( state, self->mWebView.canGoForward );
	return 1;	
}

//----------------------------------------------------------------//
/**	@name	clickBack
	@text	Sends an event to the UIWebView that the user has clicked
			'back'.

	@in		MOAIWebView self
	@out	nil
*/
int MOAIWebView::_clickBack ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebView, "U" );
	
	[ self->mWebView goBack ];
	return 0;
}

//----------------------------------------------------------------//
/**	@name	clickForward
	@text	Sends an event to the UIWebView that the user has clicked
			'forward'.

	@in		MOAIWebView self
	@out	nil
*/
int MOAIWebView::_clickForward ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebView, "U" );
	
	[ self->mWebView goForward ];
	return 0;
}

//----------------------------------------------------------------//
/**	@name	clickRefresh
	@text	Sends an event to the UIWebView that the user has clicked
			'refresh'.

	@in		MOAIWebView self
	@out	nil
*/
int MOAIWebView::_clickRefresh ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebView, "U" );
	
	[ self->mWebView reload ];
	return 0;
}

//----------------------------------------------------------------//
/**	@name	clickStop
	@text	Sends an event to the UIWebView that the user has clicked
			'stop'.

	@in		MOAIWebView self
	@out	nil
*/
int MOAIWebView::_clickStop ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebView, "U" );
		
	[ self->mWebView stopLoading ];
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getAllowsInlineMediaPlayback
	@text	Returns whether or not the UIWebView allows inline media
			playback.

	@in		MOAIWebView self
	@out	bool allowsInlineMediaPlayback
*/
int MOAIWebView::_getAllowsInlineMediaPlayback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebView, "U" );
		
	lua_pushboolean( L, self->mWebView.allowsInlineMediaPlayback );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getCurrentRequest
	@text	Returns the currently active URL
			
	@in		MOAIWebView self
	@out	string currentURL
*/
int MOAIWebView::_getCurrentRequest ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebView, "U" );
	
	NSURLRequest* request = [ self->mWebView request ];
	cc8* urlString = [[ request.URL absoluteString ] UTF8String ];	
	lua_pushstring( state, urlString );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getMediaPlaybackRequiresAction
	@text	Returns whether or not the UIWebView requires user input
			to start media playback or if it start automatically.
			
	@in		MOAIWebView self
	@out	bool mediaPlaybackRequiresUserAction
*/
int MOAIWebView::_getMediaPlaybackRequiresAction ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebView, "U" );
		
	lua_pushboolean( L, self->mWebView.mediaPlaybackRequiresUserAction );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getScalesPageToFit
	@text	Returns whether or not the UIWebView automatically scales
			pages to fit the view.
			
	@in		MOAIWebView self
	@out	bool scalesPageToFit
*/
int MOAIWebView::_getScalesPageToFit ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebView, "U" );
		
	lua_pushboolean( L, self->mWebView.scalesPageToFit );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	hideWebView
	@text	Returns whether or not the UIWebView automatically scales
			pages to fit the view.
			
	@in		MOAIWebView self
	@out	bool scalesPageToFit
*/
int MOAIWebView::_hideWebView ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebView, "U" );
			
	self->mWebView.hidden = true;	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	isHidden
	@text	Returns whether or not the UIWebView is hidden.
				
	@in		MOAIWebView self
	@out	bool isHidden
*/
int MOAIWebView::_isHidden ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebView, "U" );
		
	lua_pushboolean( L, self->mWebView.hidden );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	isLoading
	@text	Returns whether or not the UIWebView is loading a page.
				
	@in		MOAIWebView self
	@out	bool isLoading
*/
int MOAIWebView::_isLoading ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebView, "U" );
		
	lua_pushboolean( L, self->mWebView.loading );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	loadData
	@text	Sets the main page contents, MIME type, content encoding, and base URL

	@in		MOAIWebView self
	@in		TODO:type data
	@in		string MIMEType
	@in		string encodingName
	@in		string baseURL
	@out	nil
*/
int MOAIWebView::_loadData ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebView, "U" );
	
	//TODO
	//MOAIAction* action = state.GetLuaObject < MOAIAction >( 2 );
	//if ( !action ) return 0;
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	loadHTML
	@text	Sets the main page content and base URL

	@in		MOAIWebView self
	@in		string string
	@in		string baseURL
	@out	nil
*/
int MOAIWebView::_loadHTML ( lua_State* L ) {	
	MOAI_LUA_SETUP ( MOAIWebView, "U" );
	
	cc8* st = lua_tostring ( state, 2 );
	cc8* url = lua_tostring ( state, 3 );
	
	NSString* nsHtmlString = [[ NSString alloc ] initWithUTF8String:st ];
	
	if ( url != NULL ) {
		NSString* nsUrlString = [[ NSString alloc ] initWithUTF8String:url ];
		NSURL* nsURL = [[ NSURL alloc ] initWithString:nsUrlString ];
		[self->mWebView loadHTMLString:nsHtmlString baseURL:nsURL ];
	}
	else {
		[self->mWebView loadHTMLString:nsHtmlString baseURL:nil ];
	}

	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	loadRequest
	@text	Sets the main page content and base URL

	@in		MOAIWebView self
	@in		TODO:type request
	@out	nil
*/
int MOAIWebView::_loadRequest ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIWebView, "U" );
		
	cc8* urlStr = lua_tostring ( state, 2 );
	NSString* urlString = [[ NSString alloc ] initWithCString:urlStr encoding:[NSString defaultCStringEncoding] ];
	NSURL* url = [ NSURL URLWithString:urlString ];
	NSURLRequest* request = [NSURLRequest requestWithURL:url ];
	
	if ( self->mWebView.delegate == nil )
		[ self->mWebView setDelegate:self->mWebViewDelegate ];
		
	[ self->mWebView loadRequest:request ];
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	runJavaScript
	@text	Runs the passed in JavaScript
	
	@in		MOAIWebView self
	@in		string script
	@out	string result   Result of the JavaScript or nil if script failed
*/
int MOAIWebView::_runJavaScript ( lua_State* L ) {	
	MOAI_LUA_SETUP ( MOAIWebView, "U" );
		
	cc8* script = lua_tostring ( state, 2 );
	NSString* result = [ self->mWebView stringByEvaluatingJavaScriptFromString:[[ NSString alloc ] initWithUTF8String:script ]];
	
	lua_pushstring ( L, [result UTF8String] );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	setAllowsInlineMediaPlayback
	@text	Sets the value of the allowsInlineMediaPlayback on the 
			UIWebView.
	
	@in		MOAIWebView self
	@in		bool allowsInlineMediaPlayback
	@out	nil
*/
int MOAIWebView::_setAllowsInlineMediaPlayback ( lua_State* L ) {	
	MOAI_LUA_SETUP ( MOAIWebView, "U" );
	
	BOOL allowsInlinePlayback = lua_toboolean ( state, 2 );
	self->mWebView.allowsInlineMediaPlayback = allowsInlinePlayback;
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setMediaPlaybackRequiresAction
	@text	Sets the value of the mediaPlaybackRequiresUserAction on the 
			UIWebView.
	
	@in		MOAIWebView self
	@in		bool mediaPlaybackRequiresAction
	@out	nil
*/
int MOAIWebView::_setMediaPlaybackRequiresAction ( lua_State* L ) {	
	MOAI_LUA_SETUP ( MOAIWebView, "U" );
	
	BOOL mediaPlaybackRequiresAction = lua_toboolean ( state, 2 );
	self->mWebView.mediaPlaybackRequiresUserAction = mediaPlaybackRequiresAction;
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setScalesPageToFit
	@text	Sets the value of the scalesPageToFit on the UIWebView.
	
	@in		MOAIWebView self
	@in		bool scalesPageToFit
	@out	nil
*/
int MOAIWebView::_setScalesPageToFit ( lua_State* L ) {	
	MOAI_LUA_SETUP ( MOAIWebView, "U" );
	
	BOOL scalesPages = lua_toboolean ( state, 2 );
	self->mWebView.scalesPageToFit = scalesPages;
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	showWebView
	@text	puts the web view onto the current view controller
	
	@in		MOAIWebView self
	@out	nil
*/
int MOAIWebView::_showWebView ( lua_State* L ) {	
	MOAI_LUA_SETUP ( MOAIWebView, "U" );
			
	self->mWebView.hidden = false;		
	return 0;
}

//================================================================//
// MOAIWebView
//================================================================//

//----------------------------------------------------------------//
MOAIWebView::MOAIWebView () :
	mWebView ( 0 ) {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAIEventSource )
	RTTI_END
	
	mWebViewDelegate = [ MoaiUiWebViewDelegate alloc ];
	
	UIWindow* window = [[UIApplication sharedApplication] keyWindow];
	mWebView = [[[ UIWebView alloc ] initWithFrame:CGRectMake( 0, 0, window.bounds.size.width, window.bounds.size.height )] autorelease ];
	[ mWebView setDelegate:mWebViewDelegate ];
	[ mWebView setScalesPageToFit:YES ];
	[ mWebView setMultipleTouchEnabled:YES ];
	[ window addSubview:mWebView ];
}

//----------------------------------------------------------------//
MOAIWebView::~MOAIWebView () {

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
void MOAIWebView::RaiseDidFailLoadWithErrorEvent ( NSError* error ) {
	
	USLuaStateHandle state = USLuaRuntime::Get ().State ();
		if ( this->PushListenerAndSelf ( DID_FAIL_LOAD_WITH_ERROR, state )) {
			[ error toLua:state ];
			state.DebugCall ( 2, 0 );
		}
}

//----------------------------------------------------------------//
BOOL MOAIWebView::RaiseShouldStartLoadWithRequestEvent ( NSURLRequest* request, UIWebViewNavigationType navType ) {

	cc8* urlString = [[ request.URL absoluteString ] UTF8String ];	
	int nav = navType;

	USLuaStateHandle state = USLuaRuntime::Get ().State ();
		if ( this->PushListenerAndSelf ( SHOULD_START_LOAD_WITH_REQUEST, state )) {
			lua_pushstring ( state, urlString );
			lua_pushinteger ( state, nav );			
			state.DebugCall ( 3, 1 );
		}
	
	BOOL result = lua_toboolean ( state, 1 );
	return result;
}

//----------------------------------------------------------------//
void MOAIWebView::RaiseWebViewDidFinishLoadEvent () {

	USLuaStateHandle state = USLuaRuntime::Get ().State ();
		if ( this->PushListenerAndSelf ( WEB_VIEW_DID_FINISH_LOAD, state )) {
			state.DebugCall ( 1, 0 );
		}
}

//----------------------------------------------------------------//
void MOAIWebView::RaiseWebViewDidStartLoadEvent () {

	USLuaStateHandle state = USLuaRuntime::Get ().State ();
		if ( this->PushListenerAndSelf ( WEB_VIEW_DID_START_LOAD, state )) {
			state.DebugCall ( 1, 0 );
		}
}

//----------------------------------------------------------------//
void MOAIWebView::RegisterLuaClass ( USLuaState& state ) {

	MOAIEventSource::RegisterLuaClass ( state );

	// Event IDs
	state.SetField ( -1, "DID_FAIL_LOAD_WITH_ERROR", ( u32 )DID_FAIL_LOAD_WITH_ERROR );
	state.SetField ( -1, "SHOULD_START_LOAD_WITH_REQUEST", ( u32 )SHOULD_START_LOAD_WITH_REQUEST );
	state.SetField ( -1, "WEB_VIEW_DID_FINISH_LOAD", ( u32 )WEB_VIEW_DID_FINISH_LOAD );
	state.SetField ( -1, "WEB_VIEW_DID_START_LOAD", ( u32 )WEB_VIEW_DID_START_LOAD );
	
	// Navigation types
	state.SetField( -1, "NAVIGATION_LINK_CLICKED", ( u32 )NAVIGATION_LINK_CLICKED );
	state.SetField( -1, "NAVIGATION_FORM_SUBMIT", ( u32 )NAVIGATION_FORM_SUBMIT );
	state.SetField( -1, "NAVIGATION_BACK_FORWARD", ( u32 )NAVIGATION_BACK_FORWARD );
	state.SetField( -1, "NAVIGATION_RELOAD", ( u32 )NAVIGATION_RELOAD );
	state.SetField( -1, "NAVIGATION_FORM_RESUBMIT", ( u32 )NAVIGATION_FORM_RESUBMIT );
	state.SetField( -1, "NAVIGATION_OTHER", ( u32 )NAVIGATION_OTHER );
}

//----------------------------------------------------------------//
void MOAIWebView::RegisterLuaFuncs ( USLuaState& state ) {

	MOAIEventSource::RegisterLuaFuncs ( state );

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
		{ "hideWebView",					_hideWebView },
		{ "isHidden",						_isHidden },
		{ "isLoading",						_isLoading },
		{ "loadData",						_loadData },
		{ "loadHTML",						_loadHTML },
		{ "loadRequest",					_loadRequest },
		{ "runJavaScript",					_runJavaScript },
		{ "setAllowsInlineMediaPLayback",   _setAllowsInlineMediaPlayback },
		{ "setMediaPlaybackRequiresAction", _setMediaPlaybackRequiresAction },
		{ "setScalesPageToFit",				_setScalesPageToFit },
		{ "showWebView",					_showWebView },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
STLString MOAIWebView::ToString () {

	STLString repr;
	return repr;
}