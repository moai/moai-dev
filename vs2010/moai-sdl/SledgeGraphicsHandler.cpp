#include <uslscore/uslscore.h>
#include <moaicore-config.h>
#include "SledgeGraphicsHandler.h"


SDL_Window* SledgeGraphicsHandler::m_window = NULL;

SledgeGraphicsHandler::SledgeGraphicsHandler(){
	RTTI_BEGIN
		RTTI_EXTEND(MOAILuaObject)
	RTTI_END
}

SledgeGraphicsHandler::~SledgeGraphicsHandler()
{

}

int SledgeGraphicsHandler::_getSupportedResolutions( lua_State* L )
{
	MOAI_LUA_SETUP ( SledgeGraphicsHandler, "U" )

	// Get the index of the display we're currently on.
	int displayIdx = SDL_GetWindowDisplayIndex(m_window);
	// Get the number of modes this display can do.
	int num_displayModes = SDL_GetNumDisplayModes(displayIdx);

	vector<SDL_DisplayMode> modesfound;

	modesfound.clear();

	SDL_DisplayMode current_mode;

	int foo = SDL_GetWindowDisplayMode(
		m_window,
		&current_mode
	);
	
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

	lua_setglobal(L,"resolutionCurrent");
	//printf("resolutions: %d\n", num_displayModes);


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

		lua_settable(L, -3);
		++i;
	}
	lua_setglobal(L,"availableResolutions");
	return 0;
}

/** expects parameters in order: width, height, bpp, fullscreen
 */
int SledgeGraphicsHandler::_setResolution( lua_State* L )
{
	MOAI_LUA_SETUP ( SledgeGraphicsHandler, "UNNNNN" );


	int width = state.GetValue<int>(2, 0);
	int height = state.GetValue<int>(3, 0);
	int refresh = state.GetValue<int>(4, 0);
	int bpp = state.GetValue<int>(5, 0);
	//bool fullscreen = state.GetValue<int>(6, 0) == 1;

	SDL_bool bFullscreen = SDL_FALSE;
	if(state.GetValue<int>(6, 0) == 1) bFullscreen = SDL_TRUE;

	if(bFullscreen == SDL_FALSE)
	{
		SDL_SetWindowFullscreen(m_window, bFullscreen);
		SDL_SetWindowSize(m_window, width, height);
	} else {
		SDL_DisplayMode fullscreenMode;
		fullscreenMode.w = width;
		fullscreenMode.h = height;
		fullscreenMode.refresh_rate = refresh;
		fullscreenMode.format = SDL_PIXELFORMAT_RGBA8888;

		SDL_SetWindowDisplayMode(m_window, &fullscreenMode);
		SDL_SetWindowFullscreen(m_window, bFullscreen);
	}

	AKUSetScreenSize (width, height);
	AKUSetViewSize (width, height);

	

	return 0;
}

void SledgeGraphicsHandler::RegisterLuaClass(MOAILuaState& state) {
	luaL_Reg regTable[] = {
		{ "getCurrentMode", _getSupportedResolutions },
		{ "setMode", _setResolution },
		{ NULL, NULL }
	};
	luaL_register(state, 0, regTable);
}

void SledgeGraphicsHandler::RegisterLuaFuncs(MOAILuaState& state) {
	luaL_Reg regTable[] = {
		{ NULL, NULL}
	};
	luaL_register(state, 0, regTable);
}


void SledgeGraphicsHandler::SetWindow(SDL_Window* p_window)
{
	SledgeGraphicsHandler::m_window = p_window;
}