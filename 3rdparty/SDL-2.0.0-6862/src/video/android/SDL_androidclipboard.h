#include "SDL_config.h"

#ifndef _SDL_androidclipboard_h
#define _SDL_androidclipboard_h

extern int Android_SetClipboardText(_THIS, const char *text);
extern char *Android_GetClipboardText(_THIS);
extern SDL_bool Android_HasClipboardText(_THIS);

#endif /* _SDL_androidclipboard_h */
