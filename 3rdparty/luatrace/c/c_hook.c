#include "lauxlib.h"

#ifdef __MACH__
#include <mach/mach.h>
#include <mach/mach_time.h>
typedef uint64_t hook_time_t;
#define CLOCK_FUNCTION mach_absolute_time
#elif __linux__
#include <time.h>
typedef long hook_time_t;
#define CLOCK_FUNCTION lclock
#ifdef CLOCK_MONOTONIC_RAW
#define LINUX_CLOCK CLOCK_MONOTONIC_RAW
#else
#define LINUX_CLOCK CLOCK_MONOTONIC
#endif
#elif _WIN32
#include <windows.h>
typedef long long hook_time_t;
#define CLOCK_FUNCTION wclock
#else
#include <time.h>
typedef clock_t hook_time_t;
#define CLOCK_FUNCTION clock
#endif


/*============================================================================*/

static unsigned long microseconds_numerator;
static lua_Number microseconds_denominator;


#ifdef __MACH__
static void get_microseconds_info(void)
{
  mach_timebase_info_data_t timebase_info;
  mach_timebase_info(&timebase_info);
  microseconds_numerator = timebase_info.numer;
  microseconds_denominator = (lua_Number)(timebase_info.denom * 1000);
}
#elif __linux__
static void get_microseconds_info(void)
{
  microseconds_numerator = 1;
  microseconds_denominator = 1000;
}

static hook_time_t lclock()
{
  struct timespec tp;
  clock_gettime(LINUX_CLOCK, &tp);
  return tp.tv_sec * 1000000000L + tp.tv_nsec;
}
#elif _WIN32
static void get_microseconds_info(void)
{
  LARGE_INTEGER frequency;
  long long f;
  QueryPerformanceFrequency(&frequency);
  f = frequency.QuadPart;
  if (f < 1000000)
  {
    microseconds_numerator = 1000000 / f;
    microseconds_denominator = 1;
  }
  else
  {
    microseconds_numerator = 1;
    microseconds_denominator = f / 1000000;
  }
}

static hook_time_t wclock()
{
  LARGE_INTEGER t;
  QueryPerformanceCounter(&t);
  return t.QuadPart;
}
#else
static void get_microseconds_info(void)
{
  if (CLOCKS_PER_SEC < 1000000)
  {
    microseconds_numerator = 1000000 / CLOCKS_PER_SEC;
    microseconds_denominator = 1;
  }
  else
  {
    microseconds_numerator = 1;
    microseconds_denominator = CLOCKS_PER_SEC / 1000000;
  }
}
#endif


static lua_Number convert_to_fp_microseconds(hook_time_t t)
{
  return ((lua_Number)(microseconds_numerator * t)) / microseconds_denominator;
}


/*============================================================================*/

static const char *const hooknames[] = {"call", "return", "line", "count", "tail return"};
static int hook_index = -1;
static hook_time_t time_out, elapsed;


void hook(lua_State *L, lua_Debug *ar)
{
  int event;
  hook_time_t time_in = CLOCK_FUNCTION();
  elapsed += time_in - time_out;

  lua_rawgeti(L, LUA_REGISTRYINDEX, hook_index);

  event = ar->event;
  lua_pushstring(L, hooknames[event]);

  if (event == LUA_HOOKLINE)
    lua_pushinteger(L, ar->currentline);
  else
    lua_pushnil(L);

  lua_pushnumber(L, convert_to_fp_microseconds(elapsed));
  lua_call(L, 3, 0);

  elapsed = 0;
  time_out = CLOCK_FUNCTION();
}


static int set_hook(lua_State *L)
{
  if (lua_isnoneornil(L, 1))
  {
    if (hook_index >= 0)
    {
      luaL_unref(L, LUA_REGISTRYINDEX, hook_index);
      hook_index = -1;
    }
    lua_sethook(L, 0, 0, 0);
  }
  else
  {
    get_microseconds_info();

    luaL_checktype(L, 1, LUA_TFUNCTION);
    hook_index = luaL_ref(L, LUA_REGISTRYINDEX);
    lua_sethook(L, hook, LUA_MASKCALL | LUA_MASKRET | LUA_MASKLINE, 0);
    elapsed = 0;
    time_out = CLOCK_FUNCTION();
  }
  return 0;
}

/*============================================================================*/


static luaL_Reg hook_functions[] =
{
  {"set_hook",  set_hook},
  {NULL, NULL}
};


LUALIB_API int luaopen_luatrace_c_hook(lua_State *L)
{
  /* Register the module functions */
  luaL_register(L, "c_hook", hook_functions);
  return 1;
}


/*============================================================================*/

