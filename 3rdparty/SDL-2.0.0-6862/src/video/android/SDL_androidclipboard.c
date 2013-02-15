#include "SDL_config.h"

#if SDL_VIDEO_DRIVER_ANDROID

#include "SDL_androidvideo.h"

#include "../../core/android/SDL_android.h"

int
Android_SetClipboardText(_THIS, const char *text)
{
	return Android_JNI_SetClipboardText(text);
}

char *
Android_GetClipboardText(_THIS)
{
	return Android_JNI_GetClipboardText();
}

SDL_bool Android_HasClipboardText(_THIS)
{
	return Android_JNI_HasClipboardText();
}

#endif /* SDL_VIDEO_DRIVER_ANDROID */
