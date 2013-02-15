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

#if SDL_VIDEO_DRIVER_WINDOWS

#include "SDL.h"
#include "SDL_windowsvideo.h"


/* Display a Windows message box */

typedef struct
{
    LPDLGTEMPLATE lpDialog;
    Uint8 *data;
    size_t size;
    size_t used;
} WIN_DialogData;


static INT_PTR MessageBoxDialogProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    switch ( iMessage ) {
    case WM_COMMAND:
        /* Return the ID of the button that was pushed */
		EndDialog(hDlg, LOWORD(wParam));
        return TRUE;

    default:
        break;
    }
    return FALSE;
}

static SDL_bool ExpandDialogSpace(WIN_DialogData *dialog, size_t space)
{
    size_t size = dialog->size;

    if (size == 0) {
        size = space;
    } else {
        while ((dialog->used + space) > size) {
            size *= 2;
        }
    }
    if (size > dialog->size) {
        void *data = SDL_realloc(dialog->data, size);
        if (!data) {
            SDL_OutOfMemory();
            return SDL_FALSE;
        }
        dialog->data = data;
        dialog->size = size;
        dialog->lpDialog = (LPDLGTEMPLATE)dialog->data;
    }
    return SDL_TRUE;
}
 
static SDL_bool AlignDialogData(WIN_DialogData *dialog, size_t size)
{
    size_t padding = (dialog->used % size);

    if (!ExpandDialogSpace(dialog, padding)) {
        return SDL_FALSE;
    }

    dialog->used += padding;

    return SDL_TRUE;
}

static SDL_bool AddDialogData(WIN_DialogData *dialog, const void *data, size_t size)
{
    if (!ExpandDialogSpace(dialog, size)) {
        return SDL_FALSE;
    }

    SDL_memcpy(dialog->data+dialog->used, data, size);
    dialog->used += size;

    return SDL_TRUE;
}

static SDL_bool AddDialogString(WIN_DialogData *dialog, const char *string)
{
    WCHAR *wstring;
    WCHAR *p;
    size_t count;
    SDL_bool status;

    if (!string) {
        string = "";
    }

    wstring = WIN_UTF8ToString(string);
    if (!wstring) {
        return SDL_FALSE;
    }

    /* Find out how many characters we have, including null terminator */
    count = 0;
    for (p = wstring; *p; ++p) {
        ++count;
    }
    ++count;

    status = AddDialogData(dialog, wstring, count*sizeof(WCHAR));
    SDL_free(wstring);
    return status;
}

static SDL_bool AddDialogControl(WIN_DialogData *dialog, WORD type, DWORD style, DWORD exStyle, int x, int y, int w, int h, int id, const char *caption)
{
    DLGITEMTEMPLATE item;
    WORD marker = 0xFFFF;
    WORD extraData = 0;

    SDL_zero(item);
    item.style = style;
    item.dwExtendedStyle = exStyle;
    item.x = x;
    item.y = y;
    item.cx = w;
    item.cy = h;
    item.id = id;

    if (!AlignDialogData(dialog, sizeof(DWORD))) {
        return SDL_FALSE;
    }
    if (!AddDialogData(dialog, &item, sizeof(item))) {
        return SDL_FALSE;
    }
    if (!AddDialogData(dialog, &marker, sizeof(marker))) {
        return SDL_FALSE;
    }
    if (!AddDialogData(dialog, &type, sizeof(type))) {
        return SDL_FALSE;
    }
    if (!AddDialogString(dialog, caption)) {
        return SDL_FALSE;
    }
    if (!AddDialogData(dialog, &extraData, sizeof(extraData))) {
        return SDL_FALSE;
    }
    ++dialog->lpDialog->cdit;

    return SDL_TRUE;
}

static SDL_bool AddDialogStatic(WIN_DialogData *dialog, int x, int y, int w, int h, const char *text)
{
    DWORD style = WS_VISIBLE | WS_CHILD | SS_LEFT | SS_NOPREFIX;
    return AddDialogControl(dialog, 0x0082, style, 0, x, y, w, h, -1, text);
}

static SDL_bool AddDialogButton(WIN_DialogData *dialog, int x, int y, int w, int h, const char *text, int id, SDL_bool isDefault)
{
    DWORD style = WS_VISIBLE | WS_CHILD;
    if (isDefault) {
        style |= BS_DEFPUSHBUTTON;
    } else {
        style |= BS_PUSHBUTTON;
    }
    return AddDialogControl(dialog, 0x0080, style, 0, x, y, w, h, id, text);
}

static void FreeDialogData(WIN_DialogData *dialog)
{
    if (dialog->data) {
        SDL_free(dialog->data);
    }
    SDL_free(dialog);
}

static WIN_DialogData *CreateDialogData(int w, int h, const char *caption)
{
    WIN_DialogData *dialog;
    DLGTEMPLATE dialogTemplate;

    SDL_zero(dialogTemplate);
    dialogTemplate.style = (WS_CAPTION | DS_CENTER);
    dialogTemplate.x = 0;
    dialogTemplate.y = 0;
    dialogTemplate.cx = w;
    dialogTemplate.cy = h;

    dialog = (WIN_DialogData *)SDL_calloc(1, sizeof(*dialog));
    if (!dialog) {
        return NULL;
    }

    if (!AddDialogData(dialog, &dialogTemplate, sizeof(dialogTemplate))) {
        FreeDialogData(dialog);
        return NULL;
    }

    /* There is no menu or special class */
    if (!AddDialogString(dialog, "") || !AddDialogString(dialog, "")) {
        FreeDialogData(dialog);
        return NULL;
    }

    if (!AddDialogString(dialog, caption)) {
        FreeDialogData(dialog);
        return NULL;
    }

    return dialog;
}

int
WIN_ShowMessageBox(const SDL_MessageBoxData *messageboxdata, int *buttonid)
{
    WIN_DialogData *dialog;
    int i, x, y, w, h, gap, which;
    const SDL_MessageBoxButtonData *buttons = messageboxdata->buttons;
 
    /* FIXME: Need a better algorithm for laying out the message box */

    dialog = CreateDialogData(570, 260, messageboxdata->title);
    if (!dialog) {
        return -1;
    }

    w = 100;
    h = 25;
    gap = 10;
    x = gap;
    y = 50;

    if (!AddDialogStatic(dialog, x, y, 550, 100, messageboxdata->message)) {
        FreeDialogData(dialog);
        return -1;
    }

    y += 110;

    for (i = 0; i < messageboxdata->numbuttons; ++i) {
        SDL_bool isDefault;

        if (buttons[i].flags & SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT) {
            isDefault = SDL_TRUE;
        } else {
            isDefault = SDL_FALSE;
        }
        if (!AddDialogButton(dialog, x, y, w, h, buttons[i].text, i, isDefault)) {
            FreeDialogData(dialog);
            return -1;
        }
        x += w + gap;
    }

    /* FIXME: If we have a parent window, get the Instance and HWND for them */
    which = DialogBoxIndirect(NULL, dialog->lpDialog, NULL, (DLGPROC)MessageBoxDialogProc);
    *buttonid = buttons[which].buttonid;

    FreeDialogData(dialog);
    return 0;
}

#endif /* SDL_VIDEO_DRIVER_WINDOWS */

/* vi: set ts=4 sw=4 expandtab: */
