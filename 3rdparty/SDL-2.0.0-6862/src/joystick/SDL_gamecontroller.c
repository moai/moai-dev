/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2012 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/
#include "SDL_config.h"

/* This is the game controller API for Simple DirectMedia Layer */

#include "SDL_events.h"
#include "SDL_assert.h"
#include "SDL_sysjoystick.h"
#include "SDL_hints.h"

#if !SDL_EVENTS_DISABLED
#include "../events/SDL_events_c.h"
#endif
#define ABS(_x) ((_x) < 0 ? -(_x) : (_x))


/* a list of currently opened game controllers */
static SDL_GameController *SDL_gamecontrollers = NULL;

/* keep track of the hat and mask value that transforms this hat movement into a button/axis press */
struct _SDL_HatMapping
{
	int hat;
	Uint8 mask;
};

#define k_nMaxReverseEntries 20

// We are encoding the "HAT" as 0xhm. where h == hat ID and m == mask
// MAX 4 hats supported
#define k_nMaxHatEntries 0x3f + 1

/* our in memory mapping db between joystick objects and controller mappings*/
struct _SDL_ControllerMapping
{
	SDL_JoystickGUID guid;
	const char *name;

	// mapping of axis/button id to controller version
	int axes[SDL_CONTROLLER_AXIS_MAX];
	int buttonasaxis[SDL_CONTROLLER_AXIS_MAX];

	int buttons[SDL_CONTROLLER_BUTTON_MAX];
	int axesasbutton[SDL_CONTROLLER_BUTTON_MAX];
	struct _SDL_HatMapping hatasbutton[SDL_CONTROLLER_BUTTON_MAX];

	// reverse mapping, joystick indices to buttons
	SDL_CONTROLLER_AXIS raxes[k_nMaxReverseEntries];
	SDL_CONTROLLER_AXIS rbuttonasaxis[k_nMaxReverseEntries];

	SDL_CONTROLLER_BUTTON rbuttons[k_nMaxReverseEntries];
	SDL_CONTROLLER_BUTTON raxesasbutton[k_nMaxReverseEntries];
	SDL_CONTROLLER_BUTTON rhatasbutton[k_nMaxHatEntries];

};


/* our hard coded list of mapping support */
typedef struct _ControllerMapping_t
{
	SDL_JoystickGUID guid;
	char *name;
	const char *mapping;
	struct _ControllerMapping_t *next;
} ControllerMapping_t;


/* default mappings we support */
const char *s_ControllerMappings [] =
{
#ifdef SDL_JOYSTICK_DINPUT
	"xinput,X360 Controller,a:b10,b:b11,y:b13,x:b12,start:b4,guide:b14,back:b5,dpup:b0,dpleft:b2,dpdown:b1,dpright:b3,leftshoulder:b8,rightshoulder:b9,leftstick:b6,rightstick:b7,leftx:a0,lefty:a1,rightx:a2,righty:a3,lefttrigger:a4,righttrigger:a5",
	"341a3608000000000000504944564944,Afterglow PS3 Controller,a:b1,b:b2,y:b3,x:b0,start:b9,guide:b12,back:b8,dpup:h0.1,dpleft:h0.8,dpdown:h0.4,dpright:h0.2,leftshoulder:b4,rightshoulder:b5,leftstick:b10,rightstick:b11,leftx:a0,lefty:a1,rightx:a2,righty:a3,lefttrigger:b6,righttrigger:b7",
	"88880803000000000000504944564944,PS3,a:b2,b:b1,x:b0,y:b3,start:b11,back:b8,leftstick:b9,rightstick:b10,leftshoulder:b4,rightshoulder:b5,dpup:h0.1,dpleft:h0.4,dpdown:h0.8,dpright:h0.2,leftx:a0,lefty:a1,rightx:a3,righty:a4,lefttrigger:b6,righttrigger:b7,guide:b12",
	"25090500000000000000504944564944,PS3 DualShock,a:b2,b:b1,x:b0,y:b3,start:b8,guide:,back:b9,leftstick:b10,rightstick:b11,leftshoulder:b6,rightshoulder:b7,dpup:h0.1,dpleft:h0.4,dpdown:h0.8,dpright:h0.2,leftx:a0,lefty:a1,rightx:a2,righty:a3,lefttrigger:b4,righttrigger:b5",
#elif defined(__MACOSX__)
	"5e040000000000008e02000000000000,X360 Controller,a:b0,b:b1,y:b3,x:b2,start:b8,guide:b10,back:b9,dpup:b11,dpleft:b13,dpdown:b12,dpright:b14,leftshoulder:b4,rightshoulder:b5,leftstick:b6,rightstick:b7,leftx:a0,lefty:a1,rightx:a3,righty:a4,lefttrigger:a2,righttrigger:a5",
	"4c050000000000006802000000000000,PS3 Controller,a:b14,b:b13,x:b12,y:b15,start:b3,guide:b16,back:b0,leftstick:b1,rightstick:b2,leftshoulder:b10,rightshoulder:b11,dpup:b4,dpleft:b6,dpdown:b7,dpright:b5,leftx:a0,lefty:a1,rightx:a2,righty:a3,lefttrigger:b8,righttrigger:b9",
#elif defined(__LINUX__)
	
#endif
	NULL
};

static ControllerMapping_t *s_pSupportedControllers = NULL;
#ifdef SDL_JOYSTICK_DINPUT
static ControllerMapping_t *s_pXInputMapping = NULL;
#endif

/* The SDL game controller structure */
struct _SDL_GameController
{
	SDL_Joystick *joystick;	/* underlying joystick device */
	int ref_count;
	Uint8 hatState[4]; /* the current hat state for this controller */
	struct _SDL_ControllerMapping mapping; /* the mapping object for this controller */
	struct _SDL_GameController *next; /* pointer to next game controller we have allocated */
};


int SDL_PrivateGameControllerAxis(SDL_GameController * gamecontroller, SDL_CONTROLLER_AXIS axis, Sint16 value);
int	SDL_PrivateGameControllerButton(SDL_GameController * gamecontroller, SDL_CONTROLLER_BUTTON button, Uint8 state);

/*
 * Event filter to fire controller events from joystick ones
 */
int SDL_GameControllerEventWatcher(void *userdata, SDL_Event * event)
{
	switch( event->type )
	{
	case SDL_JOYAXISMOTION:
		{
			SDL_GameController *controllerlist;

			if ( event->jaxis.axis >= k_nMaxReverseEntries ) break;

			controllerlist = SDL_gamecontrollers;
			while ( controllerlist )
			{
				if ( controllerlist->joystick->instance_id == event->jaxis.which )
				{
					if ( controllerlist->mapping.raxes[event->jaxis.axis] >= 0 ) // simple axis to axis, send it through
					{
						SDL_PrivateGameControllerAxis( controllerlist, controllerlist->mapping.raxes[event->jaxis.axis], event->jaxis.value );
					}
					else if ( controllerlist->mapping.raxesasbutton[event->jaxis.axis] >= 0 ) // simlate an axis as a button
					{
						SDL_PrivateGameControllerButton( controllerlist, controllerlist->mapping.raxesasbutton[event->jaxis.axis], ABS(event->jaxis.value) > 32768/2 ? SDL_PRESSED : SDL_RELEASED );
					}
					break;
				}
				controllerlist = controllerlist->next;
			}
		}
		break;
	case SDL_JOYBUTTONDOWN:
	case SDL_JOYBUTTONUP:
		{
			SDL_GameController *controllerlist;

			if ( event->jbutton.button >= k_nMaxReverseEntries ) break;

			controllerlist = SDL_gamecontrollers;
			while ( controllerlist )
			{
				if ( controllerlist->joystick->instance_id == event->jbutton.which )
				{
					if ( controllerlist->mapping.rbuttons[event->jbutton.button] >= 0 ) // simple button as button
					{
						SDL_PrivateGameControllerButton( controllerlist, controllerlist->mapping.rbuttons[event->jbutton.button], event->jbutton.state );
					}
					else if ( controllerlist->mapping.rbuttonasaxis[event->jbutton.button] >= 0 ) // an button pretending to be an axis
					{
						SDL_PrivateGameControllerAxis( controllerlist, controllerlist->mapping.rbuttonasaxis[event->jbutton.button], event->jbutton.state > 0 ? 32768 : 0 );
					}
					break;
				}
				controllerlist = controllerlist->next;
			}
		}
		break;
	case SDL_JOYHATMOTION:
		{
			SDL_GameController *controllerlist;

			if ( event->jhat.hat >= 4 ) break;

			controllerlist = SDL_gamecontrollers;
			while ( controllerlist )
			{
				if ( controllerlist->joystick->instance_id == event->jhat.which )
				{
					Uint8 bSame = controllerlist->hatState[event->jhat.hat] & event->jhat.value;
					// Get list of removed bits (button release)
					Uint8 bChanged = controllerlist->hatState[event->jhat.hat] ^ bSame;
					// the hat idx in the high nibble
					int bHighHat = event->jhat.hat << 4;

					if ( bChanged & SDL_HAT_DOWN )
						SDL_PrivateGameControllerButton( controllerlist, controllerlist->mapping.rhatasbutton[bHighHat | SDL_HAT_DOWN], SDL_RELEASED );
					if ( bChanged & SDL_HAT_UP )
						SDL_PrivateGameControllerButton( controllerlist, controllerlist->mapping.rhatasbutton[bHighHat | SDL_HAT_UP], SDL_RELEASED );
					if ( bChanged & SDL_HAT_LEFT )
						SDL_PrivateGameControllerButton( controllerlist, controllerlist->mapping.rhatasbutton[bHighHat | SDL_HAT_LEFT], SDL_RELEASED );
					if ( bChanged & SDL_HAT_RIGHT )
						SDL_PrivateGameControllerButton( controllerlist, controllerlist->mapping.rhatasbutton[bHighHat | SDL_HAT_RIGHT], SDL_RELEASED );

					// Get list of added bits (button press)
					bChanged = event->jhat.value ^ bSame;

					if ( bChanged & SDL_HAT_DOWN )
						SDL_PrivateGameControllerButton( controllerlist, controllerlist->mapping.rhatasbutton[bHighHat | SDL_HAT_DOWN], SDL_PRESSED );
					if ( bChanged & SDL_HAT_UP )
						SDL_PrivateGameControllerButton( controllerlist, controllerlist->mapping.rhatasbutton[bHighHat | SDL_HAT_UP], SDL_PRESSED );
					if ( bChanged & SDL_HAT_LEFT )
						SDL_PrivateGameControllerButton( controllerlist, controllerlist->mapping.rhatasbutton[bHighHat | SDL_HAT_LEFT], SDL_PRESSED );
					if ( bChanged & SDL_HAT_RIGHT )
						SDL_PrivateGameControllerButton( controllerlist, controllerlist->mapping.rhatasbutton[bHighHat | SDL_HAT_RIGHT], SDL_PRESSED );

					// update our state cache
					controllerlist->hatState[event->jhat.hat] = event->jhat.value;

					break;
				}
				controllerlist = controllerlist->next;
			}
		}
		break;
	case SDL_JOYDEVICEADDED:
		{
			if ( SDL_IsGameController(event->jdevice.which ) )
			{
				SDL_Event deviceevent;
				deviceevent.type = SDL_CONTROLLERDEVICEADDED;
				deviceevent.cdevice.which = event->jdevice.which;
				SDL_PushEvent(&deviceevent);
			}
		}
		break;
	case SDL_JOYDEVICEREMOVED:
		{
			SDL_GameController *controllerlist = SDL_gamecontrollers;
			while ( controllerlist )
			{
				if ( controllerlist->joystick->instance_id == event->jdevice.which )
				{
					SDL_Event deviceevent;
					deviceevent.type = SDL_CONTROLLERDEVICEREMOVED;
					deviceevent.cdevice.which = event->jdevice.which;
					SDL_PushEvent(&deviceevent);
					break;
				}
				controllerlist = controllerlist->next;
			}
		}
		break;
	default:
		break;
	}

	return 1;
}

/*
 * Helper function to determine pre-caclulated offset to certain joystick mappings
 */
ControllerMapping_t *SDL_PrivateGetControllerMapping(int device_index)
{
#ifdef SDL_JOYSTICK_DINPUT
	if ( SDL_SYS_IsXInputDeviceIndex(device_index) && s_pXInputMapping )
	{
		return s_pXInputMapping;
	}
	else
#endif
	{
		SDL_JoystickGUID jGUID = SDL_JoystickGetDeviceGUID( device_index );
		ControllerMapping_t *pSupportedController = s_pSupportedControllers;
		while ( pSupportedController )
		{
			if ( !SDL_memcmp( &jGUID, &pSupportedController->guid, sizeof(jGUID) ) )
			{
				return pSupportedController;
			}
			pSupportedController = pSupportedController->next;
		}
	}

	return NULL;
}

/*
 * convert a string to its enum equivalent
 */
SDL_CONTROLLER_AXIS SDL_GameControllerGetAxisFromString( const char *pchString )
{
	if ( !pchString || !pchString[0] )
		return SDL_CONTROLLER_AXIS_INVALID;

	if ( !SDL_strcasecmp( pchString, "leftx" ) )
		return SDL_CONTROLLER_AXIS_LEFTX;
	else if ( !SDL_strcasecmp( pchString, "lefty" ) )
		return SDL_CONTROLLER_AXIS_LEFTY;
	else if ( !SDL_strcasecmp( pchString, "rightx" ) )
		return SDL_CONTROLLER_AXIS_RIGHTX;
	else if ( !SDL_strcasecmp( pchString, "righty" ) )
		return SDL_CONTROLLER_AXIS_RIGHTY;
	else if ( !SDL_strcasecmp( pchString, "lefttrigger" ) )
		return SDL_CONTROLLER_AXIS_TRIGGERLEFT;
	else if ( !SDL_strcasecmp( pchString, "righttrigger" ) )
		return SDL_CONTROLLER_AXIS_TRIGGERRIGHT;
	else
		return SDL_CONTROLLER_AXIS_INVALID;
}


/*
 * convert a string to its enum equivalent
 */
SDL_CONTROLLER_BUTTON SDL_GameControllerGetButtonFromString( const char *pchString )
{
	if ( !pchString || !pchString[0] )
		return SDL_CONTROLLER_BUTTON_INVALID;

	if ( !SDL_strcasecmp( pchString, "a" ) )
		return SDL_CONTROLLER_BUTTON_A;
	else if ( !SDL_strcasecmp( pchString, "b" ) )
		return SDL_CONTROLLER_BUTTON_B;
	else if ( !SDL_strcasecmp( pchString, "x" ) )
		return SDL_CONTROLLER_BUTTON_X;
	else if ( !SDL_strcasecmp( pchString, "y" ) )
		return SDL_CONTROLLER_BUTTON_Y;
	else if ( !SDL_strcasecmp( pchString, "start" ) )
		return SDL_CONTROLLER_BUTTON_START;
	else if ( !SDL_strcasecmp( pchString, "guide" ) )
		return SDL_CONTROLLER_BUTTON_GUIDE;
	else if ( !SDL_strcasecmp( pchString, "back" ) )
		return SDL_CONTROLLER_BUTTON_BACK;
	else if ( !SDL_strcasecmp( pchString, "dpup" ) )
		return SDL_CONTROLLER_BUTTON_DPAD_UP;
	else if ( !SDL_strcasecmp( pchString, "dpdown" ) )
		return SDL_CONTROLLER_BUTTON_DPAD_DOWN;
	else if ( !SDL_strcasecmp( pchString, "dpleft" ) )
		return SDL_CONTROLLER_BUTTON_DPAD_LEFT;
	else if ( !SDL_strcasecmp( pchString, "dpright" ) )
		return SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
	else if ( !SDL_strcasecmp( pchString, "leftshoulder" ) )
		return SDL_CONTROLLER_BUTTON_LEFTSHOULDER;
	else if ( !SDL_strcasecmp( pchString, "rightshoulder" ) )
		return SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;
	else if ( !SDL_strcasecmp( pchString, "leftstick" ) )
		return SDL_CONTROLLER_BUTTON_LEFTSTICK;
	else if ( !SDL_strcasecmp( pchString, "rightstick" ) )
		return SDL_CONTROLLER_BUTTON_RIGHTSTICK;
	else
		return SDL_CONTROLLER_BUTTON_INVALID;
}


/*
 * given a controller button name and a joystick name update our mapping structure with it
 */
void SDL_PrivateGameControllerParseButton( const char *szGameButton, const char *szJoystickButton, struct _SDL_ControllerMapping *pMapping )
{
	int iSDLButton = 0;
	SDL_CONTROLLER_BUTTON button;
	SDL_CONTROLLER_AXIS axis;
	button = SDL_GameControllerGetButtonFromString( szGameButton );
	axis = SDL_GameControllerGetAxisFromString( szGameButton );
	iSDLButton = SDL_atoi( &szJoystickButton[1] );

	if ( szJoystickButton[0] == 'a' )
	{
		if ( iSDLButton >= k_nMaxReverseEntries )
		{
			SDL_SetError("Axis index too large: %d", iSDLButton );
			return;
		}
		if ( axis != SDL_CONTROLLER_AXIS_INVALID )
		{
			pMapping->axes[ axis ] = iSDLButton;
			pMapping->raxes[ iSDLButton ] = axis;
		}
		else if ( button != SDL_CONTROLLER_BUTTON_INVALID )
		{
			pMapping->axesasbutton[ button ] = iSDLButton;
			pMapping->raxesasbutton[ iSDLButton ] = button;
		}
		else
		{
			SDL_assert( !"How did we get here?" );
		}

	}
	else if ( szJoystickButton[0] == 'b' )
	{
		if ( iSDLButton >= k_nMaxReverseEntries )
		{
			SDL_SetError("Button index too large: %d", iSDLButton );
			return;
		}
		if ( button != SDL_CONTROLLER_BUTTON_INVALID )
		{
			pMapping->buttons[ button ] = iSDLButton;
			pMapping->rbuttons[ iSDLButton ] = button;
		}
		else if ( axis != SDL_CONTROLLER_AXIS_INVALID )
		{
			pMapping->buttonasaxis[ axis ] = iSDLButton;
			pMapping->rbuttonasaxis[ iSDLButton ] = axis;
		}
		else
		{
			SDL_assert( !"How did we get here?" );
		}
	}
	else if ( szJoystickButton[0] == 'h' )
	{
		int hat = SDL_atoi( &szJoystickButton[1] );
		int mask = SDL_atoi( &szJoystickButton[3] );
		if (hat >= 4) {
			SDL_SetError("Hat index too large: %d", iSDLButton );
		}

		if ( button != SDL_CONTROLLER_BUTTON_INVALID )
		{
            int ridx;
			pMapping->hatasbutton[ button ].hat = hat;
			pMapping->hatasbutton[ button ].mask = mask;
			ridx = (hat << 4) | mask;
			pMapping->rhatasbutton[ ridx ] = button;
		}
		else if ( axis != SDL_CONTROLLER_AXIS_INVALID )
		{
			SDL_assert( !"Support has as axis" );
		}
		else
		{
			SDL_assert( !"How did we get here?" );
		}
	}

}


/*
 * given a controller mapping string update our mapping object
 */
static void
SDL_PrivateGameControllerParseControllerConfigString( struct _SDL_ControllerMapping *pMapping, const char *pchString )
{
	char szGameButton[20];
	char szJoystickButton[20];
	SDL_bool bGameButton = SDL_TRUE;
	int i = 0;
	const char *pchPos = pchString;

	SDL_memset( szGameButton, 0x0, sizeof(szGameButton) );
	SDL_memset( szJoystickButton, 0x0, sizeof(szJoystickButton) );

	while ( pchPos && *pchPos )
	{
		if ( *pchPos == ':' )
		{
			i = 0;
			bGameButton = SDL_FALSE;
		}
		else if ( *pchPos == ' ' )
		{

		}
		else if ( *pchPos == ',' )
		{
			i = 0;
			bGameButton = SDL_TRUE;
			SDL_PrivateGameControllerParseButton( szGameButton, szJoystickButton, pMapping );
			SDL_memset( szGameButton, 0x0, sizeof(szGameButton) );
			SDL_memset( szJoystickButton, 0x0, sizeof(szJoystickButton) );

		}
		else if ( bGameButton )
		{
			if ( i >=  sizeof(szGameButton))
			{
				SDL_SetError( "Button name too large: %s", szGameButton );
				return;
			}
			szGameButton[i] = *pchPos;
			i++;
		}
		else
		{
			if ( i >=  sizeof(szJoystickButton))
			{
				SDL_SetError( "Joystick button name too large: %s", szJoystickButton );
				return;
			}
			szJoystickButton[i] = *pchPos;
			i++;
		}
		pchPos++;
	}

	SDL_PrivateGameControllerParseButton( szGameButton, szJoystickButton, pMapping );

}

/*
 * Make a new button mapping struct
 */
void SDL_PrivateLoadButtonMapping( struct _SDL_ControllerMapping *pMapping, SDL_JoystickGUID guid, const char *pchName, const char *pchMapping )
{
	int j;

	pMapping->guid = guid;
	pMapping->name = pchName;

	// set all the button mappings to non defaults
	for ( j = 0; j < SDL_CONTROLLER_AXIS_MAX; j++ )
	{
		pMapping->axes[j] = -1;
		pMapping->buttonasaxis[j] = -1;
	}
	for ( j = 0; j < SDL_CONTROLLER_BUTTON_MAX; j++ )
	{
		pMapping->buttons[j] = -1;
		pMapping->axesasbutton[j] = -1;
		pMapping->hatasbutton[j].hat = -1;
	}

	for ( j = 0; j < k_nMaxReverseEntries; j++ )
	{
		pMapping->raxes[j] = SDL_CONTROLLER_AXIS_INVALID;
		pMapping->rbuttonasaxis[j] = SDL_CONTROLLER_AXIS_INVALID;
		pMapping->rbuttons[j] = SDL_CONTROLLER_BUTTON_INVALID;
		pMapping->raxesasbutton[j] = SDL_CONTROLLER_BUTTON_INVALID;
	}

	for (j = 0; j < k_nMaxHatEntries; j++)
	{
		pMapping->rhatasbutton[j] = SDL_CONTROLLER_BUTTON_INVALID;
	}

	SDL_PrivateGameControllerParseControllerConfigString( pMapping, pchMapping );
}


/*
 * grab the guid string from a mapping string
 */
char *SDL_PrivateGetControllerGUIDFromMappingString( const char *pMapping )
{
	const char *pFirstComma = SDL_strchr( pMapping, ',' );
	if ( pFirstComma )
	{
		char *pchGUID = SDL_malloc( pFirstComma - pMapping + 1 );
		if ( !pchGUID )
		{
			SDL_OutOfMemory();
			return NULL;
		}
		SDL_memcpy( pchGUID, pMapping, pFirstComma - pMapping );
		pchGUID[ pFirstComma - pMapping ] = 0;
		return pchGUID;
	}
	return NULL;
}


/*
 * grab the name string from a mapping string
 */
char *SDL_PrivateGetControllerNameFromMappingString( const char *pMapping )
{
	const char *pFirstComma, *pSecondComma;
    char *pchName;

    pFirstComma = SDL_strchr( pMapping, ',' );
    if ( !pFirstComma )
        return NULL;

	pSecondComma = SDL_strchr( pFirstComma + 1, ',' );
    if ( !pSecondComma )
        return NULL;

    pchName = SDL_malloc( pSecondComma - pFirstComma );
    if ( !pchName )
    {
        SDL_OutOfMemory();
        return NULL;
    }
    SDL_memcpy( pchName, pFirstComma + 1, pSecondComma - pFirstComma );
    pchName[ pSecondComma - pFirstComma - 1 ] = 0;
    return pchName;
}


/*
 * grab the button mapping string from a mapping string
 */
const char *SDL_PrivateGetControllerMappingFromMappingString( const char *pMapping )
{
	const char *pFirstComma, *pSecondComma;

    pFirstComma = SDL_strchr( pMapping, ',' );
    if ( !pFirstComma )
        return NULL;

	pSecondComma = SDL_strchr( pFirstComma + 1, ',' );
    if ( !pSecondComma )
        return NULL;

    return pSecondComma + 1; /* mapping is everything after the 3rd comma, no need to malloc it */
}


/*
 * Initialize the game controller system, mostly load our DB of controller config mappings
 */
int
SDL_GameControllerInit(void)
{
	int i = 0;
	const char *pMappingString = NULL;
	s_pSupportedControllers = NULL;
	pMappingString = s_ControllerMappings[i];
	while ( pMappingString )
	{
		ControllerMapping_t *pControllerMapping;
		char *pchGUID;
		char *pchName;
		const char *pchMapping;
		pControllerMapping = SDL_malloc( sizeof(*pControllerMapping) );
		if ( !pControllerMapping )
		{
			SDL_OutOfMemory();
			return -1;
		}

		pchGUID = SDL_PrivateGetControllerGUIDFromMappingString( pMappingString );
		pchName = SDL_PrivateGetControllerNameFromMappingString( pMappingString );
		pchMapping = SDL_PrivateGetControllerMappingFromMappingString( pMappingString );
		if ( pchGUID && pchName )
		{
#ifdef SDL_JOYSTICK_DINPUT
			if ( !SDL_strcasecmp( pchGUID, "xinput" ) )
			{
				s_pXInputMapping = pControllerMapping;
			}
#endif
			pControllerMapping->guid = SDL_JoystickGetGUIDFromString( pchGUID );
			pControllerMapping->name = pchName;
			pControllerMapping->mapping = pchMapping;
			pControllerMapping->next = s_pSupportedControllers;
			s_pSupportedControllers = pControllerMapping;

			SDL_free( pchGUID );
		}

		i++;
		pMappingString = s_ControllerMappings[i];
	}

	// load in any user supplied config
	{
		const char *hint = SDL_GetHint(SDL_HINT_GAMECONTROLLERCONFIG);
		if ( hint && hint[0] )
		{
			int nchHints = SDL_strlen( hint );
			char *pUserMappings = SDL_malloc( nchHints + 1 ); /* FIXME: memory leak, but we can't free it in this function because pchMapping below points into this memory */
			SDL_memcpy( pUserMappings, hint, nchHints + 1 );
			while ( pUserMappings )
			{
				char *pchGUID;
				char *pchName;
				const char *pchMapping;
				char *pchNewLine = NULL;
				ControllerMapping_t *pControllerMapping;

				pchNewLine = SDL_strchr( pUserMappings, '\n' );
				if ( pchNewLine )
					*pchNewLine = '\0';

				pControllerMapping = SDL_malloc( sizeof(*pControllerMapping) );
				if ( !pControllerMapping )
				{
					SDL_OutOfMemory();
					return -1;
				}

				pchGUID = SDL_PrivateGetControllerGUIDFromMappingString( pUserMappings );
				pchName = SDL_PrivateGetControllerNameFromMappingString( pUserMappings );
				pchMapping = SDL_PrivateGetControllerMappingFromMappingString( pUserMappings );

				if ( pchGUID && pchName )
				{
#ifdef SDL_JOYSTICK_DINPUT
					if ( !SDL_strcasecmp( pchGUID, "xinput" ) )
					{
						s_pXInputMapping = pControllerMapping;
					}
#endif

					pControllerMapping->guid = SDL_JoystickGetGUIDFromString( pchGUID );
					pControllerMapping->name = pchName;
					pControllerMapping->mapping = pchMapping;
					pControllerMapping->next = s_pSupportedControllers;
					s_pSupportedControllers = pControllerMapping;

					SDL_free( pchGUID );
				}

				if ( pchNewLine )
					pUserMappings = pchNewLine + 1;
				else
					pUserMappings = NULL;
			}
		}
	}

	/* watch for joy events and fire controller ones if needed */
	SDL_AddEventWatch( SDL_GameControllerEventWatcher, NULL );
	return (0);
}


/*
 * Get the implementation dependent name of a controller
 */
const char *
SDL_GameControllerNameForIndex(int device_index)
{
	ControllerMapping_t *pSupportedController =  SDL_PrivateGetControllerMapping(device_index);
	if ( pSupportedController )
	{
		return pSupportedController->name;
	}
    return NULL;
}


/*
 * Return 1 if the joystick at this device index is a supported controller
 */
int SDL_IsGameController(int device_index)
{
	ControllerMapping_t *pSupportedController =  SDL_PrivateGetControllerMapping(device_index);
	if ( pSupportedController )
	{
		return 1;
	}
	return 0;
}

/*
 * Open a controller for use - the index passed as an argument refers to
 * the N'th controller on the system.  This index is the value which will
 * identify this controller in future controller events.
 *
 * This function returns a controller identifier, or NULL if an error occurred.
 */
SDL_GameController *
SDL_GameControllerOpen(int device_index)
{
    SDL_GameController *gamecontroller;
	SDL_GameController *gamecontrollerlist;
	ControllerMapping_t *pSupportedController = NULL;

    if ((device_index < 0) || (device_index >= SDL_NumJoysticks())) {
        SDL_SetError("There are %d joysticks available", SDL_NumJoysticks());
        return (NULL);
    }

	gamecontrollerlist = SDL_gamecontrollers;
    // If the controller is already open, return it 
	while ( gamecontrollerlist )
	{
		if ( SDL_SYS_GetInstanceIdOfDeviceIndex(device_index) == gamecontrollerlist->joystick->instance_id ) {
				gamecontroller = gamecontrollerlist;
				++gamecontroller->ref_count;
				return (gamecontroller);
		}
		gamecontrollerlist = gamecontrollerlist->next;
    }

	// Find a Controller Mapping 
	pSupportedController =  SDL_PrivateGetControllerMapping(device_index);
	if ( !pSupportedController )
	{
		SDL_SetError("Couldn't find mapping for device (%d)", device_index );
		return (NULL);
	}

	// Create and initialize the joystick 
	gamecontroller = (SDL_GameController *) SDL_malloc((sizeof *gamecontroller));
	if (gamecontroller == NULL) {
		SDL_OutOfMemory();
		return NULL;
	}

    SDL_memset(gamecontroller, 0, (sizeof *gamecontroller));
    gamecontroller->joystick = SDL_JoystickOpen(device_index);
	if ( !gamecontroller->joystick ) {
        SDL_free(gamecontroller);
        return NULL;
    }

	SDL_PrivateLoadButtonMapping( &gamecontroller->mapping, pSupportedController->guid, pSupportedController->name, pSupportedController->mapping );

    // Add joystick to list 
    ++gamecontroller->ref_count;
	// Link the joystick in the list 
	gamecontroller->next = SDL_gamecontrollers;
	SDL_gamecontrollers = gamecontroller;

	SDL_SYS_JoystickUpdate( gamecontroller->joystick );

    return (gamecontroller);
}


/*
 * Get the current state of an axis control on a controller
 */
Sint16
SDL_GameControllerGetAxis(SDL_GameController * gamecontroller, SDL_CONTROLLER_AXIS axis)
{
	if ( !gamecontroller )
		return 0;

	if (gamecontroller->mapping.axes[axis] >= 0 )
	{
		return ( SDL_JoystickGetAxis( gamecontroller->joystick, gamecontroller->mapping.axes[axis]) );
	}
	else if (gamecontroller->mapping.buttonasaxis[axis] >= 0 )
	{
		Uint8 value;
		value = SDL_JoystickGetButton( gamecontroller->joystick, gamecontroller->mapping.buttonasaxis[axis] );
		if ( value > 0 )
			return 32767;
		return 0;
	}
	return 0;
}


/*
 * Get the current state of a button on a controller
 */
Uint8
SDL_GameControllerGetButton(SDL_GameController * gamecontroller, SDL_CONTROLLER_BUTTON button)
{
	if ( !gamecontroller )
		return 0;

	if ( gamecontroller->mapping.buttons[button] >= 0 )
	{
		return ( SDL_JoystickGetButton( gamecontroller->joystick, gamecontroller->mapping.buttons[button] ) );
	}
	else if ( gamecontroller->mapping.axesasbutton[button] >= 0 )
	{
		Sint16 value;
		value = SDL_JoystickGetAxis( gamecontroller->joystick, gamecontroller->mapping.axesasbutton[button] );
		if ( ABS(value) > 32768/2 )
			return 1;
		return 0;
	}
	else if ( gamecontroller->mapping.hatasbutton[button].hat >= 0 )
	{
		Uint8 value;
		value = SDL_JoystickGetHat( gamecontroller->joystick, gamecontroller->mapping.hatasbutton[button].hat );
		
		if ( value & gamecontroller->mapping.hatasbutton[button].mask )
			return 1;
		return 0;
	}

	return 0;
}

/*
 * Return if the joystick in question is currently attached to the system,
 *  \return 0 if not plugged in, 1 if still present.
 */
int
SDL_GameControllerGetAttached( SDL_GameController * gamecontroller )
{
	if ( !gamecontroller )
		return 0;

	return SDL_JoystickGetAttached(gamecontroller->joystick);
}


/*
 * Get the number of multi-dimensional axis controls on a joystick
 */
const char *
SDL_GameControllerName(SDL_GameController * gamecontroller)
{
	if ( !gamecontroller )
		return NULL;

    return (gamecontroller->mapping.name);
}


/*
 * Get the joystick for this controller
 */
SDL_Joystick *SDL_GameControllerGetJoystick(SDL_GameController * gamecontroller)
{
	if ( !gamecontroller )
		return NULL;

	return gamecontroller->joystick;
}

/**
 *  get the sdl joystick layer binding for this controller axi mapping
 */
SDL_GameControllerButtonBind SDL_GameControllerGetBindForAxis( SDL_GameController * gamecontroller, SDL_CONTROLLER_AXIS axis )
{
	SDL_GameControllerButtonBind bind;
	SDL_memset( &bind, 0x0, sizeof(bind) );

	if ( !gamecontroller || axis == SDL_CONTROLLER_AXIS_INVALID )
		return bind;

	if (gamecontroller->mapping.axes[axis] >= 0 )
	{
		bind.m_eBindType = SDL_CONTROLLER_BINDTYPE_AXIS;
		bind.button = gamecontroller->mapping.axes[axis];
	}
	else if (gamecontroller->mapping.buttonasaxis[axis] >= 0 )
	{
		bind.m_eBindType = SDL_CONTROLLER_BINDTYPE_BUTTON;
		bind.button = gamecontroller->mapping.buttonasaxis[axis];
	}

	return bind;
}


/**
 *  get the sdl joystick layer binding for this controller button mapping
 */
SDL_GameControllerButtonBind SDL_GameControllerGetBindForButton( SDL_GameController * gamecontroller, SDL_CONTROLLER_BUTTON button )
{
	SDL_GameControllerButtonBind bind;
	SDL_memset( &bind, 0x0, sizeof(bind) );

	if ( !gamecontroller || button == SDL_CONTROLLER_BUTTON_INVALID )
		return bind;

	if ( gamecontroller->mapping.buttons[button] >= 0 )
	{
		bind.m_eBindType = SDL_CONTROLLER_BINDTYPE_BUTTON;
		bind.button = gamecontroller->mapping.buttons[button];
	}
	else if ( gamecontroller->mapping.axesasbutton[button] >= 0 )
	{
		bind.m_eBindType = SDL_CONTROLLER_BINDTYPE_AXIS;
		bind.axis = gamecontroller->mapping.axesasbutton[button];
	}
	else if ( gamecontroller->mapping.hatasbutton[button].hat >= 0 )
	{
		bind.m_eBindType = SDL_CONTROLLER_BINDTYPE_HAT;
		bind.hat.hat = gamecontroller->mapping.hatasbutton[button].hat;
		bind.hat.hat_mask = gamecontroller->mapping.hatasbutton[button].mask;
	}

	return bind;
}


/*
 * Close a joystick previously opened with SDL_JoystickOpen()
 */
void
SDL_GameControllerClose(SDL_GameController * gamecontroller)
{
	SDL_GameController *gamecontrollerlist, *gamecontrollerlistprev;

	if ( !gamecontroller )
		return;

	// First decrement ref count 
    if (--gamecontroller->ref_count > 0) {
        return;
    }

	SDL_JoystickClose( gamecontroller->joystick );
	
	gamecontrollerlist = SDL_gamecontrollers;
	gamecontrollerlistprev = NULL;
	while ( gamecontrollerlist )
	{
		if (gamecontroller == gamecontrollerlist) 
		{
			if ( gamecontrollerlistprev )
			{
				// unlink this entry
				gamecontrollerlistprev->next = gamecontrollerlist->next;
			}
			else
			{
				SDL_gamecontrollers = gamecontroller->next;
			}

			break;
		}
		gamecontrollerlistprev = gamecontrollerlist;
		gamecontrollerlist = gamecontrollerlist->next;
	}
 
    SDL_free(gamecontroller);
}


/*
 * Quit the controller subsystem
 */
void
SDL_GameControllerQuit(void)
{
	ControllerMapping_t *pControllerMap;
	while ( SDL_gamecontrollers )
	{
		SDL_gamecontrollers->ref_count = 1;
        SDL_GameControllerClose(SDL_gamecontrollers);
 	}

	while ( s_pSupportedControllers )
	{
		pControllerMap = s_pSupportedControllers;
		s_pSupportedControllers = s_pSupportedControllers->next;
		SDL_free( pControllerMap->name );
		SDL_free( pControllerMap );
	}

	SDL_DelEventWatch( SDL_GameControllerEventWatcher, NULL );

}

/*
 * Event filter to transform joystick events into appropriate game controller ones
 */
int
SDL_PrivateGameControllerAxis(SDL_GameController * gamecontroller, SDL_CONTROLLER_AXIS axis, Sint16 value)
{
	int posted;

    /* translate the event, if desired */
    posted = 0;
#if !SDL_EVENTS_DISABLED
    if (SDL_GetEventState(SDL_CONTROLLERAXISMOTION) == SDL_ENABLE) {
        SDL_Event event;
        event.type = SDL_CONTROLLERAXISMOTION;
        event.caxis.which = gamecontroller->joystick->instance_id;
        event.caxis.axis = axis;
        event.caxis.value = value;
		posted = SDL_PushEvent(&event) == 1;
    }
#endif /* !SDL_EVENTS_DISABLED */
    return (posted);
}


/*
 * Event filter to transform joystick events into appropriate game controller ones
 */
int
SDL_PrivateGameControllerButton(SDL_GameController * gamecontroller, SDL_CONTROLLER_BUTTON button, Uint8 state)
{
    int posted;
#if !SDL_EVENTS_DISABLED
	SDL_Event event;

    if ( button == SDL_CONTROLLER_BUTTON_INVALID )
        return (0);

    switch (state) {
    case SDL_PRESSED:
        event.type = SDL_CONTROLLERBUTTONDOWN;
        break;
    case SDL_RELEASED:
        event.type = SDL_CONTROLLERBUTTONUP;
        break;
    default:
        /* Invalid state -- bail */
        return (0);
    }
#endif /* !SDL_EVENTS_DISABLED */

    /* translate the event, if desired */
    posted = 0;
#if !SDL_EVENTS_DISABLED
    if (SDL_GetEventState(event.type) == SDL_ENABLE) {
        event.cbutton.which = gamecontroller->joystick->instance_id;
        event.cbutton.button = button;
        event.cbutton.state = state;
		posted = SDL_PushEvent(&event) == 1;
    }
#endif /* !SDL_EVENTS_DISABLED */
    return (posted);
}

/*
 * Turn off controller events
 */
int
SDL_GameControllerEventState(int state)
{
#if SDL_EVENTS_DISABLED
    return SDL_IGNORE;
#else
    const Uint32 event_list[] = {
        SDL_CONTROLLERAXISMOTION, SDL_CONTROLLERBUTTONDOWN, SDL_CONTROLLERBUTTONUP,
        SDL_CONTROLLERDEVICEADDED, SDL_CONTROLLERDEVICEREMOVED,
    };
    unsigned int i;

    switch (state) {
    case SDL_QUERY:
        state = SDL_IGNORE;
        for (i = 0; i < SDL_arraysize(event_list); ++i) {
            state = SDL_EventState(event_list[i], SDL_QUERY);
            if (state == SDL_ENABLE) {
                break;
            }
        }
        break;
    default:
        for (i = 0; i < SDL_arraysize(event_list); ++i) {
            SDL_EventState(event_list[i], state);
        }
        break;
    }
    return (state);
#endif /* SDL_EVENTS_DISABLED */
}


/* vi: set ts=4 sw=4 expandtab: */
