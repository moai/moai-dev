#include "stdafx.h"
#include <config-default/moai_config.h>
#include "SledgeGraphicsHandler.h"
#ifdef  __APPLE__
#include <AppKit/NSScreen.h>
#endif

SDL_Window* SledgeGraphicsHandler::m_window = NULL;

SledgeGraphicsHandler::SledgeGraphicsHandler(){
	RTTI_BEGIN
		RTTI_EXTEND(MOAILuaObject)
	RTTI_END
}

SledgeGraphicsHandler::~SledgeGraphicsHandler()
{
	m_window = NULL;
}

/**
 * Registers this class's static methods with MOAI.
 *
 * @author	Jetha Chan
 * @date	14/09/2013
 *
 * @param [in,out]	state	MOAI's current Lua state.
 */
void SledgeGraphicsHandler::RegisterLuaClass(MOAILuaState& state) {
	luaL_Reg regTable[] = {
		{ "getCurrentMode", DoGetResolutionInfo },
		{ "setMode", DoSetResolution },
		{ NULL, NULL }
	};
	luaL_register(state, 0, regTable);
}

/**
 * Registers this class's member methods with MOAI.
 *
 * @author	Jetha Chan
 * @date	14/09/2013
 *
 * @param [in,out]	state	MOAI's current Lua state.
 */
void SledgeGraphicsHandler::RegisterLuaFuncs(MOAILuaState& state) {
	luaL_Reg regTable[] = {
		{ NULL, NULL}
	};
	luaL_register(state, 0, regTable);
}

int SledgeGraphicsHandler::DoGetScreenList( lua_State* L )
{
	MOAI_LUA_SETUP (SledgeGraphicsHandler, "U")
	
	// @todo  get the number of screens we have?
	
	
}

/**
 * Get resolution information from SDL and make it available to MOAI in the form
 * of resolution objects - Lua tables roughly analogous to SDL_DisplayMode
 * structures, taking the following format:
 * 
 *		w:			width
 *		h:			height
 *		bpp:		bit depth
 *		refresh:	refresh rate
 *		format:		a SDL_PixelFormatEnum value (unused)
 *
 * @author	Jetha Chan
 * @date	14/09/2013
 *
 * @return	three objects in the following order: current resolution, desktop
 *			resolution, and a list of all resolutions supported by the screen.
 */
int SledgeGraphicsHandler::DoGetResolutionInfo( lua_State* L )
{
	MOAI_LUA_SETUP ( SledgeGraphicsHandler, "U" )
		
	// Get the index of the display we're currently on.
	int displayIdx = SDL_GetWindowDisplayIndex(m_window);
	// Get the number of modes this display can do.
	int num_displayModes = SDL_GetNumDisplayModes(displayIdx);

	vector<SDL_DisplayMode> modesfound;
	vector<bool> doublesfound;
	
	modesfound.clear();
	doublesfound.clear();

	SDL_DisplayMode current_mode;
	SDL_DisplayMode desktop_mode;
	
	bool bIsRetina = false;
#ifdef  __APPLE__
	bIsRetina = ([[NSScreen mainScreen] backingScaleFactor] > 1.0);		
	//if(bIsRetina) printf("I AM RETINAAAAA\n");
#endif
	

	int foo = SDL_GetWindowDisplayMode(
		m_window,
		&current_mode
	);

	int nib = SDL_GetDesktopDisplayMode(displayIdx, &desktop_mode);

	// Push the current resolution information into Lua.
	lua_newtable(L);
	lua_pushstring(L, "w");
	lua_pushnumber(L, current_mode.w);
	lua_settable(L, -3);

	lua_pushstring(L, "h");
	lua_pushnumber(L, current_mode.h);
	lua_settable(L,-3);

	lua_pushstring(L, "bpp");
	lua_pushnumber(L, SDL_BITSPERPIXEL(current_mode.format));
	lua_settable(L,-3);
	lua_pushstring(L, "refresh");
	lua_pushnumber(L, current_mode.refresh_rate);
	lua_settable(L,-3);

	// @todo	Determine whether fullscreen or not.
	lua_pushstring(L, "format");
	lua_pushnumber(L, current_mode.format);
	//lua_pushboolean(L, 0);
	lua_settable(L,-3);

#ifdef RETURNGLOBALS
	lua_setglobal(L,"resolutionCurrent");
#endif

	// Push the desktop resolution information into Lua.
	lua_newtable(L);
	lua_pushstring(L, "w");
	lua_pushnumber(L, desktop_mode.w);
	lua_settable(L, -3);

	lua_pushstring(L, "h");
	lua_pushnumber(L, desktop_mode.h);
	lua_settable(L,-3);

	lua_pushstring(L, "bpp");
	lua_pushnumber(L, SDL_BITSPERPIXEL(desktop_mode.format));
	lua_settable(L,-3);
	lua_pushstring(L, "refresh");
	lua_pushnumber(L, desktop_mode.refresh_rate);
	lua_settable(L,-3);

	// @todo	Determine whether fullscreen or not.
	lua_pushstring(L, "format");
	lua_pushnumber(L, desktop_mode.format);
	//lua_pushboolean(L, 0);
	lua_settable(L,-3);

#ifdef RETURNGLOBALS
	lua_setglobal(L,"resolutionDesktop");
#endif

	// Get a list of unique resolutions.
	int i = 0;
	int j = 0;
	for (i = 0; i < num_displayModes; i++)
	{
		SDL_DisplayMode thismode;
		int bar = SDL_GetDisplayMode(displayIdx, i, &thismode);
		if(bar == 0){ //&& thismode.format == current_mode.format){

			// compare to last item in modesfound
			if(modesfound.size() != 0)
			{
				SDL_DisplayMode last = modesfound.back();

				if(
					last.w == thismode.w &&
					last.h == thismode.h &&
					last.refresh_rate == thismode.refresh_rate &&
					last.format == thismode.format
				)
				{
					// ignore thismode
				} else {
					modesfound.push_back(thismode);
				}
			} else {
				modesfound.push_back(thismode);
			}
		}
	}
	
	// 2013/04/25: detect doubled resolutions
	if(bIsRetina)
	{
		for (
			std::vector<SDL_DisplayMode>::iterator it = modesfound.begin();
			it != modesfound.end();
			++it
		)
		{
			bool bIsDouble = it->w > desktop_mode.w && it->h > desktop_mode.h;
			doublesfound.push_back(bIsDouble);
		}
	}
	
	// Tell Lua about our unique resolutions...
	i = 1;
	lua_newtable(L);
	for(
		std::vector<SDL_DisplayMode>::iterator it = modesfound.begin();
		it != modesfound.end();
		++it
	)
	{
		SDL_DisplayMode filteredmode = *it;
		lua_pushnumber(L, i);
		lua_newtable(L);
			lua_pushstring(L, "w");
			lua_pushnumber(L, filteredmode.w);
			lua_settable(L, -3);

			lua_pushstring(L, "h");
			lua_pushnumber(L, filteredmode.h);
			lua_settable(L, -3);

			lua_pushstring(L, "bpp");
			lua_pushnumber(L, SDL_BITSPERPIXEL(filteredmode.format));
			lua_settable(L, -3);

			lua_pushstring(L, "refresh");
			lua_pushnumber(L, filteredmode.refresh_rate);
			lua_settable(L, -3);

			lua_pushstring(L, "format");
			lua_pushnumber(L, filteredmode.format);
		lua_settable(L, -3);
		if(bIsRetina)
		{
			lua_pushstring(L, "retina");
			lua_pushboolean(L, doublesfound[i-1]);
			lua_settable(L, -3);
		}

		lua_settable(L, -3);
		++i;
		}

#ifdef RETURNGLOBALS
		lua_setglobal(L,"availableResolutions");
#endif
	return 3;
}

/**
 * Attempts to set the game to a specified resolution.
 * 
 * @author	Jetha Chan
 * @date	14/09/2013
 *
 * @param	width		Screen width.
 * @param	height		Screen height.
 * @param	refresh		Screen refresh.
 * @param	bpp			Screen bit depth.
 * @param	fullscreen	Whether to run in fullscreen mode or not.
 *
 * @return	nil
 */
int SledgeGraphicsHandler::DoSetResolution( lua_State* L )
{
	MOAI_LUA_SETUP ( SledgeGraphicsHandler, "UNNNNN" );

	int width = state.GetValue<int>(2, 0);
	int height = state.GetValue<int>(3, 0);
	int refresh = state.GetValue<int>(4, 0);
	int bpp = state.GetValue<int>(5, 0);

	SDL_bool bFullscreen = SDL_FALSE;
	if(state.GetValue<int>(6, 0) == 1)
	{
		bFullscreen = SDL_TRUE;
	}
	printf("setRes call: [%dx%d @ %dHz, %dbpp][%d]\n", width, height, refresh, bpp, bFullscreen);

	if(bFullscreen == SDL_FALSE)
	{
		SDL_SetWindowFullscreen(m_window, bFullscreen);
		SDL_SetWindowSize(m_window, width, height);
		
		SDL_ShowCursor(1);
	} else {
		SDL_DisplayMode fullscreenMode;
		fullscreenMode.w = width;
		fullscreenMode.h = height;
		fullscreenMode.refresh_rate = refresh;
		fullscreenMode.format = SDL_PIXELFORMAT_RGBA8888;
		
		SDL_SetWindowDisplayMode(m_window, &fullscreenMode);
		SDL_SetWindowFullscreen(m_window, !bFullscreen);
		SDL_SetWindowFullscreen(m_window, bFullscreen);
		
		SDL_ShowCursor(0);
	}
	AKUSetScreenSize (width, height);
	AKUSetViewSize (width, height);

	return 0;
}

/**
 * Sets window handle.
 *
 * @author	Jetha Chan
 * @date	14/09/2013
 *
 * @param [in,out]	p_window	If non-null, the window.
 */
void SledgeGraphicsHandler::SetWindowHandle(SDL_Window* p_window)
{
	SledgeGraphicsHandler::m_window = p_window;
}

/**
 * Gets screen environment information.
 *
 * @author	Jetha Chan
 * @date	14/09/2013
 *
 * @return	The screen environment information.
 */
ScreenEnvInfo SledgeGraphicsHandler::GetScreenEnvInfo(void)
{
	ScreenEnvInfo sei;

	// Get screen count.
	sei.screenCount = SDL_GetNumVideoDisplays();

	// Get the index of the display we're currently on.
	int displayIdx = SDL_GetWindowDisplayIndex(m_window);
	// Get the number of modes this display can do.
	int num_displayModes = SDL_GetNumDisplayModes(displayIdx);

	SDL_DisplayMode current_mode;
	SDL_DisplayMode desktop_mode;

	sei.retina = false;
#ifdef  __APPLE__
	sei.retina = ([[NSScreen mainScreen] backingScaleFactor] > 1.0);		
	//if(bIsRetina) printf("I AM RETINAAAAA\n");
#endif

	int foo = SDL_GetWindowDisplayMode(
		m_window,
		&current_mode
		);

	int nib = SDL_GetDesktopDisplayMode(displayIdx, &desktop_mode);

	sei.screenDim[0] = desktop_mode.w;
	sei.screenDim[1] = desktop_mode.h;

	return sei;
}

