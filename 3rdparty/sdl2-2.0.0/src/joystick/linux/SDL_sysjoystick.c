/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2013 Sam Lantinga <slouken@libsdl.org>

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

#ifdef SDL_JOYSTICK_LINUX

#ifndef SDL_INPUT_LINUXEV
#error SDL now requires a Linux 2.4+ kernel with /dev/input/event support.
#endif

/* This is the system specific header for the SDL joystick API */

#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <limits.h>             /* For the definition of PATH_MAX */
#include <linux/joystick.h>

#include "SDL_assert.h"
#include "SDL_joystick.h"
#include "SDL_endian.h"
#include "../SDL_sysjoystick.h"
#include "../SDL_joystick_c.h"
#include "SDL_sysjoystick_c.h"

/* !!! FIXME: move this somewhere else. */
#if !SDL_EVENTS_DISABLED
#include "../../events/SDL_events_c.h"
#endif

/* This isn't defined in older Linux kernel headers */
#ifndef SYN_DROPPED
#define SYN_DROPPED 3
#endif

/*
 * !!! FIXME: move all the udev stuff to src/core/linux, so I can reuse it
 * !!! FIXME:  for audio hardware disconnects.
 */
#ifdef HAVE_LIBUDEV_H
#define SDL_USE_LIBUDEV 1
#include "SDL_loadso.h"
#include <libudev.h>
#include <sys/time.h>
#include <sys/types.h>

/* we never link directly to libudev. */
/* !!! FIXME: can we generalize this? ALSA, etc, do the same things. */
static const char *udev_library = "libudev.so.0";
static void *udev_handle = NULL;

/* !!! FIXME: this is kinda ugly. */
static SDL_bool
load_udev_sym(const char *fn, void **addr)
{
    *addr = SDL_LoadFunction(udev_handle, fn);
    if (*addr == NULL) {
        /* Don't call SDL_SetError(): SDL_LoadFunction already did. */
        return SDL_FALSE;
    }

    return SDL_TRUE;
}

/* libudev entry points... */
static const char *(*UDEV_udev_device_get_action)(struct udev_device *) = NULL;
static const char *(*UDEV_udev_device_get_devnode)(struct udev_device *) = NULL;
static const char *(*UDEV_udev_device_get_property_value)(struct udev_device *, const char *) = NULL;
static struct udev_device *(*UDEV_udev_device_new_from_syspath)(struct udev *, const char *) = NULL;
static void (*UDEV_udev_device_unref)(struct udev_device *) = NULL;
static int (*UDEV_udev_enumerate_add_match_property)(struct udev_enumerate *, const char *, const char *) = NULL;
static int (*UDEV_udev_enumerate_add_match_subsystem)(struct udev_enumerate *, const char *) = NULL;
static struct udev_list_entry *(*UDEV_udev_enumerate_get_list_entry)(struct udev_enumerate *) = NULL;
static struct udev_enumerate *(*UDEV_udev_enumerate_new)(struct udev *) = NULL;
static int (*UDEV_udev_enumerate_scan_devices)(struct udev_enumerate *) = NULL;
static void (*UDEV_udev_enumerate_unref)(struct udev_enumerate *) = NULL;
static const char *(*UDEV_udev_list_entry_get_name)(struct udev_list_entry *) = NULL;
static struct udev_list_entry *(*UDEV_udev_list_entry_get_next)(struct udev_list_entry *) = NULL;
static int (*UDEV_udev_monitor_enable_receiving)(struct udev_monitor *) = NULL;
static int (*UDEV_udev_monitor_filter_add_match_subsystem_devtype)(struct udev_monitor *, const char *, const char *) = NULL;
static int (*UDEV_udev_monitor_get_fd)(struct udev_monitor *) = NULL;
static struct udev_monitor *(*UDEV_udev_monitor_new_from_netlink)(struct udev *, const char *) = NULL;
static struct udev_device *(*UDEV_udev_monitor_receive_device)(struct udev_monitor *) = NULL;
static void (*UDEV_udev_monitor_unref)(struct udev_monitor *) = NULL;
static struct udev *(*UDEV_udev_new)(void) = NULL;
static void (*UDEV_udev_unref)(struct udev *) = NULL;

static int
load_udev_syms(void)
{
    /* cast funcs to char* first, to please GCC's strict aliasing rules. */
    #define SDL_UDEV_SYM(x) \
        if (!load_udev_sym(#x, (void **) (char *) &UDEV_##x)) return -1

    SDL_UDEV_SYM(udev_device_get_action);
    SDL_UDEV_SYM(udev_device_get_devnode);
    SDL_UDEV_SYM(udev_device_get_property_value);
    SDL_UDEV_SYM(udev_device_new_from_syspath);
    SDL_UDEV_SYM(udev_device_unref);
    SDL_UDEV_SYM(udev_enumerate_add_match_property);
    SDL_UDEV_SYM(udev_enumerate_add_match_subsystem);
    SDL_UDEV_SYM(udev_enumerate_get_list_entry);
    SDL_UDEV_SYM(udev_enumerate_new);
    SDL_UDEV_SYM(udev_enumerate_scan_devices);
    SDL_UDEV_SYM(udev_enumerate_unref);
    SDL_UDEV_SYM(udev_list_entry_get_name);
    SDL_UDEV_SYM(udev_list_entry_get_next);
    SDL_UDEV_SYM(udev_monitor_enable_receiving);
    SDL_UDEV_SYM(udev_monitor_filter_add_match_subsystem_devtype);
    SDL_UDEV_SYM(udev_monitor_get_fd);
    SDL_UDEV_SYM(udev_monitor_new_from_netlink);
    SDL_UDEV_SYM(udev_monitor_receive_device);
    SDL_UDEV_SYM(udev_monitor_unref);
    SDL_UDEV_SYM(udev_new);
    SDL_UDEV_SYM(udev_unref);

    #undef SDL_UDEV_SYM

    return 0;
}

static void
UnloadUDEVLibrary(void)
{
    if (udev_handle != NULL) {
        SDL_UnloadObject(udev_handle);
        udev_handle = NULL;
    }
}

static int
LoadUDEVLibrary(void)
{
    int retval = 0;
    if (udev_handle == NULL) {
        udev_handle = SDL_LoadObject(udev_library);
        if (udev_handle == NULL) {
            retval = -1;
            /* Don't call SDL_SetError(): SDL_LoadObject already did. */
        } else {
            retval = load_udev_syms();
            if (retval < 0) {
                UnloadUDEVLibrary();
            }
        }
    }

    return retval;
}

static struct udev *udev = NULL;
static struct udev_monitor *udev_mon = NULL;
#endif


/* A linked list of available joysticks */
typedef struct SDL_joylist_item
{
    int device_instance;
    char *path;   /* "/dev/input/event2" or whatever */
    char *name;   /* "SideWinder 3D Pro" or whatever */
    SDL_JoystickGUID guid;
    dev_t devnum;
    struct joystick_hwdata *hwdata;
    struct SDL_joylist_item *next;
} SDL_joylist_item;

static SDL_joylist_item *SDL_joylist = NULL;
static SDL_joylist_item *SDL_joylist_tail = NULL;
static int numjoysticks = 0;
static int instance_counter = 0;

#define test_bit(nr, addr) \
    (((1UL << ((nr) % (sizeof(long) * 8))) & ((addr)[(nr) / (sizeof(long) * 8)])) != 0)
#define NBITS(x) ((((x)-1)/(sizeof(long) * 8))+1)

static int
IsJoystick(int fd, char *namebuf, const size_t namebuflen, SDL_JoystickGUID *guid)
{
    unsigned long evbit[NBITS(EV_MAX)] = { 0 };
    unsigned long keybit[NBITS(KEY_MAX)] = { 0 };
    unsigned long absbit[NBITS(ABS_MAX)] = { 0 };
    struct input_id inpid;
    Uint16 *guid16 = (Uint16 *) ((char *) &guid->data);

    if ((ioctl(fd, EVIOCGBIT(0, sizeof(evbit)), evbit) < 0) ||
        (ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(keybit)), keybit) < 0) ||
        (ioctl(fd, EVIOCGBIT(EV_ABS, sizeof(absbit)), absbit) < 0)) {
        return (0);
    }

    if (!(test_bit(EV_KEY, evbit) && test_bit(EV_ABS, evbit) &&
          test_bit(ABS_X, absbit) && test_bit(ABS_Y, absbit))) {
        return 0;
    }

    if (ioctl(fd, EVIOCGNAME(namebuflen), namebuf) < 0) {
        return 0;
    }

    if (ioctl(fd, EVIOCGID, &inpid) < 0) {
        return 0;
    }

#ifdef DEBUG_JOYSTICK
    printf("Joystick: %s, bustype = %d, vendor = 0x%x, product = 0x%x, version = %d\n", namebuf, inpid.bustype, inpid.vendor, inpid.product, inpid.version);
#endif

    SDL_memset(guid->data, 0, sizeof(guid->data));

    /* We only need 16 bits for each of these; space them out to fill 128. */
    /* Byteswap so devices get same GUID on little/big endian platforms. */
    *(guid16++) = SDL_SwapLE16(inpid.bustype);
    *(guid16++) = 0;

    if (inpid.vendor && inpid.product && inpid.version) {
        *(guid16++) = SDL_SwapLE16(inpid.vendor);
        *(guid16++) = 0;
        *(guid16++) = SDL_SwapLE16(inpid.product);
        *(guid16++) = 0;
        *(guid16++) = SDL_SwapLE16(inpid.version);
        *(guid16++) = 0;
    } else {
        SDL_strlcpy((char*)guid16, namebuf, sizeof(guid->data) - 4);
    }

    return 1;
}


/* !!! FIXME: I would love to dump this code and use libudev instead. */
static int
MaybeAddDevice(const char *path)
{
    struct stat sb;
    int fd = -1;
    int isstick = 0;
    char namebuf[128];
    SDL_JoystickGUID guid;
    SDL_joylist_item *item;

    if (path == NULL) {
        return -1;
    }

    if (stat(path, &sb) == -1) {
        return -1;
    }

    /* Check to make sure it's not already in list. */
    for (item = SDL_joylist; item != NULL; item = item->next) {
        if (sb.st_rdev == item->devnum) {
            return -1;  /* already have this one */
        }
    }

    fd = open(path, O_RDONLY, 0);
    if (fd < 0) {
        return -1;
    }

#ifdef DEBUG_INPUT_EVENTS
    printf("Checking %s\n", path);
#endif

    isstick = IsJoystick(fd, namebuf, sizeof (namebuf), &guid);
    close(fd);
    if (!isstick) {
        return -1;
    }

    item = (SDL_joylist_item *) SDL_malloc(sizeof (SDL_joylist_item));
    if (item == NULL) {
        return -1;
    }

    SDL_zerop(item);
    item->devnum = sb.st_rdev;
    item->path = SDL_strdup(path);
    item->name = SDL_strdup(namebuf);
    item->guid = guid;

    if ( (item->path == NULL) || (item->name == NULL) ) {
         SDL_free(item->path);
         SDL_free(item->name);
         SDL_free(item);
         return -1;
    }

    item->device_instance = instance_counter++;
    if (SDL_joylist_tail == NULL) {
        SDL_joylist = SDL_joylist_tail = item;
    } else {
        SDL_joylist_tail->next = item;
        SDL_joylist_tail = item;
    }

    return numjoysticks++;
}

#if SDL_USE_LIBUDEV
/* !!! FIXME: I would love to dump this code and use libudev instead. */
static int
MaybeRemoveDevice(const char *path)
{
    SDL_joylist_item *item;
    SDL_joylist_item *prev = NULL;

    if (path == NULL) {
        return -1;
    }

    for (item = SDL_joylist; item != NULL; item = item->next) {
        /* found it, remove it. */
        if (SDL_strcmp(path, item->path) == 0) {
            const int retval = item->device_instance;
            if (item->hwdata) {
                item->hwdata->item = NULL;
            }
            if (prev != NULL) {
                prev->next = item->next;
            } else {
                SDL_assert(SDL_joylist == item);
                SDL_joylist = item->next;
            }
            if (item == SDL_joylist_tail) {
                SDL_joylist_tail = prev;
            }
            SDL_free(item->path);
            SDL_free(item->name);
            SDL_free(item);
            numjoysticks--;
            return retval;
        }
        prev = item;
    }

    return -1;
}
#endif

static int
JoystickInitWithoutUdev(void)
{
    int i;
    char path[PATH_MAX];

    /* !!! FIXME: only finds sticks if they're called /dev/input/event[0..31] */
    /* !!! FIXME:  we could at least readdir() through /dev/input...? */
    /* !!! FIXME:  (or delete this and rely on libudev?) */
    for (i = 0; i < 32; i++) {
        SDL_snprintf(path, SDL_arraysize(path), "/dev/input/event%d", i);
        MaybeAddDevice(path);
    }

    return numjoysticks;
}


#if SDL_USE_LIBUDEV
static int
JoystickInitWithUdev(void)
{
    struct udev_enumerate *enumerate = NULL;
    struct udev_list_entry *devs = NULL;
    struct udev_list_entry *item = NULL;

    SDL_assert(udev == NULL);
    udev = UDEV_udev_new();
    if (udev == NULL) {
        return SDL_SetError("udev_new() failed");
    }

    udev_mon = UDEV_udev_monitor_new_from_netlink(udev, "udev");
    if (udev_mon != NULL) {  /* okay if it's NULL, we just lose hotplugging. */
        UDEV_udev_monitor_filter_add_match_subsystem_devtype(udev_mon,
                                                             "input", NULL);
        UDEV_udev_monitor_enable_receiving(udev_mon);
    }

    enumerate = UDEV_udev_enumerate_new(udev);
    if (enumerate == NULL) {
        return SDL_SetError("udev_enumerate_new() failed");
    }

    UDEV_udev_enumerate_add_match_subsystem(enumerate, "input");
    UDEV_udev_enumerate_add_match_property(enumerate, "ID_INPUT_JOYSTICK", "1");
    UDEV_udev_enumerate_scan_devices(enumerate);
    devs = UDEV_udev_enumerate_get_list_entry(enumerate);
    for (item = devs; item; item = UDEV_udev_list_entry_get_next(item)) {
        const char *path = UDEV_udev_list_entry_get_name(item);
        struct udev_device *dev = UDEV_udev_device_new_from_syspath(udev, path);
        MaybeAddDevice(UDEV_udev_device_get_devnode(dev));
        UDEV_udev_device_unref(dev);
    }

    UDEV_udev_enumerate_unref(enumerate);

    return numjoysticks;
}
#endif

int
SDL_SYS_JoystickInit(void)
{
    /* First see if the user specified one or more joysticks to use */
    if (SDL_getenv("SDL_JOYSTICK_DEVICE") != NULL) {
        char *envcopy, *envpath, *delim;
        envcopy = SDL_strdup(SDL_getenv("SDL_JOYSTICK_DEVICE"));
        envpath = envcopy;
        while (envpath != NULL) {
            delim = SDL_strchr(envpath, ':');
            if (delim != NULL) {
                *delim++ = '\0';
            }
            MaybeAddDevice(envpath);
            envpath = delim;
        }
        SDL_free(envcopy);
    }

#if SDL_USE_LIBUDEV
    if (LoadUDEVLibrary() == 0) {   /* okay if this fails, FOR NOW. */
        return JoystickInitWithUdev();
    }
#endif

    return JoystickInitWithoutUdev();
}

int SDL_SYS_NumJoysticks()
{
    return numjoysticks;
}

static SDL_bool
HotplugUpdateAvailable(void)
{
#if SDL_USE_LIBUDEV
    if (udev_mon != NULL) {
        const int fd = UDEV_udev_monitor_get_fd(udev_mon);
        fd_set fds;
        struct timeval tv;

        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        if ((select(fd+1, &fds, NULL, NULL, &tv) > 0) && (FD_ISSET(fd, &fds))) {
            return SDL_TRUE;
        }
    }
#endif

    return SDL_FALSE;
}

void SDL_SYS_JoystickDetect()
{
#if SDL_USE_LIBUDEV
    struct udev_device *dev = NULL;
    const char *devnode = NULL;
    const char *action = NULL;
    const char *val = NULL;

    while (HotplugUpdateAvailable()) {
        dev = UDEV_udev_monitor_receive_device(udev_mon);
        if (dev == NULL) {
            break;
        }
        val = UDEV_udev_device_get_property_value(dev, "ID_INPUT_JOYSTICK");
        if ((!val) || (SDL_strcmp(val, "1") != 0)) {
            continue;
        }

        action = UDEV_udev_device_get_action(dev);
        devnode = UDEV_udev_device_get_devnode(dev);

        if (SDL_strcmp(action, "add") == 0) {
            const int device_index = MaybeAddDevice(devnode);
            if (device_index != -1) {
                /* !!! FIXME: Move this to an SDL_PrivateJoyDeviceAdded() function? */
                #if !SDL_EVENTS_DISABLED
                SDL_Event event;
                event.type = SDL_JOYDEVICEADDED;

                if (SDL_GetEventState(event.type) == SDL_ENABLE) {
                    event.jdevice.which = device_index;
                    if ( (SDL_EventOK == NULL) ||
                         (*SDL_EventOK) (SDL_EventOKParam, &event) ) {
                        SDL_PushEvent(&event);
                    }
                }
                #endif /* !SDL_EVENTS_DISABLED */
            }
        } else if (SDL_strcmp(action, "remove") == 0) {
            const int inst = MaybeRemoveDevice(devnode);
            if (inst != -1) {
                /* !!! FIXME: Move this to an SDL_PrivateJoyDeviceRemoved() function? */
                #if !SDL_EVENTS_DISABLED
                SDL_Event event;
                event.type = SDL_JOYDEVICEREMOVED;

                if (SDL_GetEventState(event.type) == SDL_ENABLE) {
                    event.jdevice.which = inst;
                    if ( (SDL_EventOK == NULL) ||
                         (*SDL_EventOK) (SDL_EventOKParam, &event) ) {
                        SDL_PushEvent(&event);
                    }
                }
                #endif /* !SDL_EVENTS_DISABLED */
            }
        }
        UDEV_udev_device_unref(dev);
    }
#endif
}

SDL_bool SDL_SYS_JoystickNeedsPolling()
{
    /*
     * This results in a select() call, so technically we're polling to
     *  decide if we should poll, but I think this function is here because
     *  Windows has to do an enormous amount of work to detect new sticks,
     *  whereas libudev just needs to see if there's more data available on
     *  a socket...so this should be acceptable, I hope.
     */
    return HotplugUpdateAvailable();
}

static SDL_joylist_item *
JoystickByDevIndex(int device_index)
{
    SDL_joylist_item *item = SDL_joylist;

    if ((device_index < 0) || (device_index >= numjoysticks)) {
        return NULL;
    }

    while (device_index > 0) {
        SDL_assert(item != NULL);
        device_index--;
        item = item->next;
    }

    return item;
}

/* Function to get the device-dependent name of a joystick */
const char *
SDL_SYS_JoystickNameForDeviceIndex(int device_index)
{
    return JoystickByDevIndex(device_index)->name;
}

/* Function to perform the mapping from device index to the instance id for this index */
SDL_JoystickID SDL_SYS_GetInstanceIdOfDeviceIndex(int device_index)
{
    return JoystickByDevIndex(device_index)->device_instance;
}

static int
allocate_hatdata(SDL_Joystick * joystick)
{
    int i;

    joystick->hwdata->hats =
        (struct hwdata_hat *) SDL_malloc(joystick->nhats *
                                         sizeof(struct hwdata_hat));
    if (joystick->hwdata->hats == NULL) {
        return (-1);
    }
    for (i = 0; i < joystick->nhats; ++i) {
        joystick->hwdata->hats[i].axis[0] = 1;
        joystick->hwdata->hats[i].axis[1] = 1;
    }
    return (0);
}

static int
allocate_balldata(SDL_Joystick * joystick)
{
    int i;

    joystick->hwdata->balls =
        (struct hwdata_ball *) SDL_malloc(joystick->nballs *
                                          sizeof(struct hwdata_ball));
    if (joystick->hwdata->balls == NULL) {
        return (-1);
    }
    for (i = 0; i < joystick->nballs; ++i) {
        joystick->hwdata->balls[i].axis[0] = 0;
        joystick->hwdata->balls[i].axis[1] = 0;
    }
    return (0);
}

static void
ConfigJoystick(SDL_Joystick * joystick, int fd)
{
    int i, t;
    unsigned long keybit[NBITS(KEY_MAX)] = { 0 };
    unsigned long absbit[NBITS(ABS_MAX)] = { 0 };
    unsigned long relbit[NBITS(REL_MAX)] = { 0 };

    /* See if this device uses the new unified event API */
    if ((ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(keybit)), keybit) >= 0) &&
        (ioctl(fd, EVIOCGBIT(EV_ABS, sizeof(absbit)), absbit) >= 0) &&
        (ioctl(fd, EVIOCGBIT(EV_REL, sizeof(relbit)), relbit) >= 0)) {

        /* Get the number of buttons, axes, and other thingamajigs */
        for (i = BTN_JOYSTICK; i < KEY_MAX; ++i) {
            if (test_bit(i, keybit)) {
#ifdef DEBUG_INPUT_EVENTS
                printf("Joystick has button: 0x%x\n", i);
#endif
                joystick->hwdata->key_map[i - BTN_MISC] = joystick->nbuttons;
                ++joystick->nbuttons;
            }
        }
        for (i = BTN_MISC; i < BTN_JOYSTICK; ++i) {
            if (test_bit(i, keybit)) {
#ifdef DEBUG_INPUT_EVENTS
                printf("Joystick has button: 0x%x\n", i);
#endif
                joystick->hwdata->key_map[i - BTN_MISC] = joystick->nbuttons;
                ++joystick->nbuttons;
            }
        }
        for (i = 0; i < ABS_MISC; ++i) {
            /* Skip hats */
            if (i == ABS_HAT0X) {
                i = ABS_HAT3Y;
                continue;
            }
            if (test_bit(i, absbit)) {
                struct input_absinfo absinfo;

                if (ioctl(fd, EVIOCGABS(i), &absinfo) < 0)
                    continue;
#ifdef DEBUG_INPUT_EVENTS
                printf("Joystick has absolute axis: %x\n", i);
                printf("Values = { %d, %d, %d, %d, %d }\n",
                       absinfo.value, absinfo.minimum, absinfo.maximum,
                       absinfo.fuzz, absinfo.flat);
#endif /* DEBUG_INPUT_EVENTS */
                joystick->hwdata->abs_map[i] = joystick->naxes;
                if (absinfo.minimum == absinfo.maximum) {
                    joystick->hwdata->abs_correct[i].used = 0;
                } else {
                    joystick->hwdata->abs_correct[i].used = 1;
                    joystick->hwdata->abs_correct[i].coef[0] =
                        (absinfo.maximum + absinfo.minimum) - 2 * absinfo.flat;
                    joystick->hwdata->abs_correct[i].coef[1] =
                        (absinfo.maximum + absinfo.minimum) + 2 * absinfo.flat;
                    t = ((absinfo.maximum - absinfo.minimum) - 4 * absinfo.flat);
                    if (t != 0) {
                        joystick->hwdata->abs_correct[i].coef[2] =
                            (1 << 28) / t;
                    } else {
                        joystick->hwdata->abs_correct[i].coef[2] = 0;
                    }
                }
                ++joystick->naxes;
            }
        }
        for (i = ABS_HAT0X; i <= ABS_HAT3Y; i += 2) {
            if (test_bit(i, absbit) || test_bit(i + 1, absbit)) {
#ifdef DEBUG_INPUT_EVENTS
                printf("Joystick has hat %d\n", (i - ABS_HAT0X) / 2);
#endif
                ++joystick->nhats;
            }
        }
        if (test_bit(REL_X, relbit) || test_bit(REL_Y, relbit)) {
            ++joystick->nballs;
        }

        /* Allocate data to keep track of these thingamajigs */
        if (joystick->nhats > 0) {
            if (allocate_hatdata(joystick) < 0) {
                joystick->nhats = 0;
            }
        }
        if (joystick->nballs > 0) {
            if (allocate_balldata(joystick) < 0) {
                joystick->nballs = 0;
            }
        }
    }
}


/* Function to open a joystick for use.
   The joystick to open is specified by the index field of the joystick.
   This should fill the nbuttons and naxes fields of the joystick structure.
   It returns 0, or -1 if there is an error.
 */
int
SDL_SYS_JoystickOpen(SDL_Joystick * joystick, int device_index)
{
    SDL_joylist_item *item = JoystickByDevIndex(device_index);
    char *fname = NULL;
    int fd = -1;

    if (item == NULL) {
        return SDL_SetError("No such device");
    }

    fname = item->path;
    fd = open(fname, O_RDONLY, 0);
    if (fd < 0) {
        return SDL_SetError("Unable to open %s", fname);
    }

    joystick->instance_id = item->device_instance;
    joystick->hwdata = (struct joystick_hwdata *)
        SDL_malloc(sizeof(*joystick->hwdata));
    if (joystick->hwdata == NULL) {
        close(fd);
        return SDL_OutOfMemory();
    }
    SDL_memset(joystick->hwdata, 0, sizeof(*joystick->hwdata));
    joystick->hwdata->item = item;
    joystick->hwdata->guid = item->guid;
    joystick->hwdata->fd = fd;
    joystick->hwdata->fname = SDL_strdup(item->path);
    if (joystick->hwdata->fname == NULL) {
        SDL_free(joystick->hwdata);
        joystick->hwdata = NULL;
        close(fd);
        return SDL_OutOfMemory();
    }

    SDL_assert(item->hwdata == NULL);
    item->hwdata = joystick->hwdata;

    /* Set the joystick to non-blocking read mode */
    fcntl(fd, F_SETFL, O_NONBLOCK);

    /* Get the number of buttons and axes on the joystick */
    ConfigJoystick(joystick, fd);

    /* mark joystick as fresh and ready */
    joystick->hwdata->fresh = 1;

    return (0);
}

/* Function to determine is this joystick is attached to the system right now */
SDL_bool SDL_SYS_JoystickAttached(SDL_Joystick *joystick)
{
    return !joystick->closed && (joystick->hwdata->item != NULL);
}

static __inline__ void
HandleHat(SDL_Joystick * stick, Uint8 hat, int axis, int value)
{
    struct hwdata_hat *the_hat;
    const Uint8 position_map[3][3] = {
        {SDL_HAT_LEFTUP, SDL_HAT_UP, SDL_HAT_RIGHTUP},
        {SDL_HAT_LEFT, SDL_HAT_CENTERED, SDL_HAT_RIGHT},
        {SDL_HAT_LEFTDOWN, SDL_HAT_DOWN, SDL_HAT_RIGHTDOWN}
    };

    the_hat = &stick->hwdata->hats[hat];
    if (value < 0) {
        value = 0;
    } else if (value == 0) {
        value = 1;
    } else if (value > 0) {
        value = 2;
    }
    if (value != the_hat->axis[axis]) {
        the_hat->axis[axis] = value;
        SDL_PrivateJoystickHat(stick, hat,
                               position_map[the_hat->
                                            axis[1]][the_hat->axis[0]]);
    }
}

static __inline__ void
HandleBall(SDL_Joystick * stick, Uint8 ball, int axis, int value)
{
    stick->hwdata->balls[ball].axis[axis] += value;
}


static __inline__ int
AxisCorrect(SDL_Joystick * joystick, int which, int value)
{
    struct axis_correct *correct;

    correct = &joystick->hwdata->abs_correct[which];
    if (correct->used) {
        value *= 2;
        if (value > correct->coef[0]) {
            if (value < correct->coef[1]) {
                return 0;
            }
            value -= correct->coef[1];
        } else {
            value -= correct->coef[0];
        }
        value *= correct->coef[2];
        value >>= 13;
    }

    /* Clamp and return */
    if (value < -32768)
        return -32768;
    if (value > 32767)
        return 32767;

    return value;
}

static __inline__ void
PollAllValues(SDL_Joystick * joystick)
{
    struct input_absinfo absinfo;
    int a, b = 0;

    /* Poll all axis */
    for (a = ABS_X; b < ABS_MAX; a++) {
        switch (a) {
        case ABS_HAT0X:
        case ABS_HAT0Y:
        case ABS_HAT1X:
        case ABS_HAT1Y:
        case ABS_HAT2X:
        case ABS_HAT2Y:
        case ABS_HAT3X:
        case ABS_HAT3Y:
            /* ingore hats */
            break;
        default:
            if (joystick->hwdata->abs_correct[b].used) {
                if (ioctl(joystick->hwdata->fd, EVIOCGABS(a), &absinfo) >= 0) {
                    absinfo.value = AxisCorrect(joystick, b, absinfo.value);

#ifdef DEBUG_INPUT_EVENTS
                    printf("Joystick : Re-read Axis %d (%d) val= %d\n",
                        joystick->hwdata->abs_map[b], a, absinfo.value);
#endif
                    SDL_PrivateJoystickAxis(joystick,
                            joystick->hwdata->abs_map[b],
                            absinfo.value);
                }
            }
            b++;
        }
    }
}

static __inline__ void
HandleInputEvents(SDL_Joystick * joystick)
{
    struct input_event events[32];
    int i, len;
    int code;

    if (joystick->hwdata->fresh) {
        PollAllValues(joystick);
        joystick->hwdata->fresh = 0;
    }

    while ((len = read(joystick->hwdata->fd, events, (sizeof events))) > 0) {
        len /= sizeof(events[0]);
        for (i = 0; i < len; ++i) {
            code = events[i].code;
            switch (events[i].type) {
            case EV_KEY:
                if (code >= BTN_MISC) {
                    code -= BTN_MISC;
                    SDL_PrivateJoystickButton(joystick,
                                              joystick->hwdata->key_map[code],
                                              events[i].value);
                }
                break;
            case EV_ABS:
                if (code >= ABS_MISC) {
                    break;
                }

                switch (code) {
                case ABS_HAT0X:
                case ABS_HAT0Y:
                case ABS_HAT1X:
                case ABS_HAT1Y:
                case ABS_HAT2X:
                case ABS_HAT2Y:
                case ABS_HAT3X:
                case ABS_HAT3Y:
                    code -= ABS_HAT0X;
                    HandleHat(joystick, code / 2, code % 2, events[i].value);
                    break;
                default:
                    events[i].value =
                        AxisCorrect(joystick, code, events[i].value);
                    SDL_PrivateJoystickAxis(joystick,
                                            joystick->hwdata->abs_map[code],
                                            events[i].value);
                    break;
                }
                break;
            case EV_REL:
                switch (code) {
                case REL_X:
                case REL_Y:
                    code -= REL_X;
                    HandleBall(joystick, code / 2, code % 2, events[i].value);
                    break;
                default:
                    break;
                }
                break;
            case EV_SYN:
                switch (code) {
                case SYN_DROPPED :
#ifdef DEBUG_INPUT_EVENTS
                    printf("Event SYN_DROPPED detected\n");
#endif
                    PollAllValues(joystick);
                    break;
                default:
                    break;
                }
            default:
                break;
            }
        }
    }
}

void
SDL_SYS_JoystickUpdate(SDL_Joystick * joystick)
{
    int i;

    HandleInputEvents(joystick);

    /* Deliver ball motion updates */
    for (i = 0; i < joystick->nballs; ++i) {
        int xrel, yrel;

        xrel = joystick->hwdata->balls[i].axis[0];
        yrel = joystick->hwdata->balls[i].axis[1];
        if (xrel || yrel) {
            joystick->hwdata->balls[i].axis[0] = 0;
            joystick->hwdata->balls[i].axis[1] = 0;
            SDL_PrivateJoystickBall(joystick, (Uint8) i, xrel, yrel);
        }
    }
}

/* Function to close a joystick after use */
void
SDL_SYS_JoystickClose(SDL_Joystick * joystick)
{
    if (joystick->hwdata) {
        close(joystick->hwdata->fd);
        if (joystick->hwdata->item) {
            joystick->hwdata->item->hwdata = NULL;
        }
        SDL_free(joystick->hwdata->hats);
        SDL_free(joystick->hwdata->balls);
        SDL_free(joystick->hwdata->fname);
        SDL_free(joystick->hwdata);
        joystick->hwdata = NULL;
    }
    joystick->closed = 1;
}

/* Function to perform any system-specific joystick related cleanup */
void
SDL_SYS_JoystickQuit(void)
{
    SDL_joylist_item *item = NULL;
    SDL_joylist_item *next = NULL;

    for (item = SDL_joylist; item; item = next) {
        next = item->next;
        SDL_free(item->path);
        SDL_free(item->name);
        SDL_free(item);
    }

    SDL_joylist = SDL_joylist_tail = NULL;

    numjoysticks = 0;
    instance_counter = 0;

#if SDL_USE_LIBUDEV
    if (udev_mon != NULL) {
        UDEV_udev_monitor_unref(udev_mon);
        udev_mon = NULL;
    }
    if (udev != NULL) {
        UDEV_udev_unref(udev);
        udev = NULL;
    }
    UnloadUDEVLibrary();
#endif
}

SDL_JoystickGUID SDL_SYS_JoystickGetDeviceGUID( int device_index )
{
    return JoystickByDevIndex(device_index)->guid;
}

SDL_JoystickGUID SDL_SYS_JoystickGetGUID(SDL_Joystick * joystick)
{
    return joystick->hwdata->guid;
}

#endif /* SDL_JOYSTICK_LINUX */

/* vi: set ts=4 sw=4 expandtab: */
