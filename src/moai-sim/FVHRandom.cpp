// Expose a fixed version of srand() and rand() to MOAI, rather than
// relying on what the host libc supplies.
// (c) www.fivevolthigh.com, 2013

#include "pch.h"

#include "FVHRandom.h"

/* Lifted entirely from the eglibc-2.15/stlib code and simplified 
   for use as a common random generator across fivevolthigh.com code. */

#define	FVH_RAND_MAX	2147483647

/* x**31 + x**3 + 1.  */
#define	FVH_TYPE_3		3
#define	FVH_BREAK_3		128
#define	FVH_DEG_3		31
#define	FVH_SEP_3		3

/* Initially, everything is set up as if from:
	initstate(1, randtbl, 128);
   Note that this initialization takes advantage of the fact that srandom
   advances the front and rear pointers 10*rand_deg times, and hence the
   rear pointer which starts at 0 will also end up at zero; thus the zeroth
   element of the state information, which contains info about the current
   position of the rear pointer is just
	(MAX_TYPES * (rptr - state)) + TYPE_3 == TYPE_3.  */
static int32_t fvh_randtbl[FVH_DEG_3 + 1] =
  {
    FVH_TYPE_3,

    -1726662223, 379960547, 1735697613, 1040273694, 1313901226,
    1627687941, -179304937, -2073333483, 1780058412, -1989503057,
    -615974602, 344556628, 939512070, -1249116260, 1507946756,
    -812545463, 154635395, 1388815473, -1926676823, 525320961,
    -1009028674, 968117788, -123449607, 1284210865, 435012392,
    -2017506339, -911064859, -370259173, 1132637927, 1398500161,
    -205601318,
  };
   
static struct FVHRandom::fvh_random_data fvh_unsafe_state =
  {
/* FPTR and RPTR are two pointers into the state info, a front and a rear
   pointer.  These two pointers are always rand_sep places aparts, as they
   cycle through the state information.  (Yes, this does mean we could get
   away with just one pointer, but the code for random is more efficient
   this way).  The pointers are left positioned as they would be from the call:
	initstate(1, randtbl, 128);
   (The position of the rear pointer, rptr, is really 0 (as explained above
   in the initialization of randtbl) because the state table pointer is set
   to point to randtbl[1] (as explained below).)  */

    &fvh_randtbl[FVH_SEP_3 + 1],
    &fvh_randtbl[1],

/* The following things are the pointer to the state information table,
   the type of the current generator, the degree of the current polynomial
   being used, and the separation between the two pointers.
   Note that for efficiency of random, we remember the first location of
   the state information, not the zeroth.  Hence it is valid to access
   state[-1], which is used to store the type of the R.N.G.
   Also, we remember the last location, since this is more efficient than
   indexing every time to find the address of the last element to see if
   the front and rear pointers have wrapped.  */

    &fvh_randtbl[1],

    FVH_TYPE_3,
    FVH_DEG_3,
    FVH_SEP_3,

    &fvh_randtbl[sizeof (fvh_randtbl) / sizeof (fvh_randtbl[0])]
};  

//----------------------------------------------------------------//
FVHRandom::FVHRandom ()
{
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
	
	memcpy(&this->mState, &fvh_unsafe_state, sizeof(struct FVHRandom::fvh_random_data));
}

//----------------------------------------------------------------//
FVHRandom::~FVHRandom ()
{
}

//----------------------------------------------------------------//
long int FVHRandom::Random(struct FVHRandom::fvh_random_data* buf)
{
  long int result;

  int32_t *state;

  state = buf->state;

  int32_t *fptr = buf->fptr;
  int32_t *rptr = buf->rptr;
  int32_t *end_ptr = buf->end_ptr;
  int32_t val;

  val = *fptr += *rptr;
  /* Chucking least random bit.  */
  result = (val >> 1) & 0x7fffffff;
  ++fptr;
  if (fptr >= end_ptr)
  {
	  fptr = state;
	  ++rptr;
  }
  else
  {
	  ++rptr;
	  if (rptr >= end_ptr)
	    rptr = state;
  }
  buf->fptr = fptr;
  buf->rptr = rptr;
  
  return result;
}

//----------------------------------------------------------------//
void FVHRandom::RandomSeed(struct FVHRandom::fvh_random_data* buf, unsigned int seed)
{
  int32_t *state;
  long int i;
  int32_t word;
  int32_t *dst;
  int kc;

  state = buf->state;
  /* We must make sure the seed is not 0.  Take arbitrarily 1 in this case.  */
  if (seed == 0)
    seed = 1;
  state[0] = seed;

  dst = state;
  word = seed;
  kc = buf->rand_deg;
  for (i = 1; i < kc; ++i)
    {
      /* This does:
	   state[i] = (16807 * state[i - 1]) % 2147483647;
	 but avoids overflowing 31 bits.  */
      long int hi = word / 127773;
      long int lo = word % 127773;
      word = 16807 * lo - 2836 * hi;
      if (word < 0)
	word += 2147483647;
      *++dst = word;
    }

  buf->fptr = &state[buf->rand_sep];
  buf->rptr = &state[0];
  kc *= 10;
  while (--kc >= 0)
    {
      this->Random(buf);
    }
}

//----------------------------------------------------------------//
int	FVHRandom::_randomSeed ( lua_State* L )
{
	MOAI_LUA_SETUP(FVHRandom, "U")

	u32 seed = state.GetValue<u32>(2, 0);

	self->RandomSeed(&(self->mState), seed);

	return 0;
}

//----------------------------------------------------------------//
int	FVHRandom::_random ( lua_State* L )
{
	MOAI_LUA_SETUP(FVHRandom, "U")

	// Lifted from Lua 5.1.3 lmathlib.c
	/* the `%' avoids the (rare) case of r==1, and is needed also because on
	   some systems (SunOS!) `rand()' may return a value larger than RAND_MAX */
	lua_Number r = (lua_Number)(self->Random(&(self->mState)) % FVH_RAND_MAX) / (lua_Number)RAND_MAX;
	switch (lua_gettop(L)) {  /* check number of arguments */
		case 0 + 1: {  /* no arguments */
			lua_pushnumber(L, r);  /* Number between 0 and 1 */
			break;
		}
		case 1 + 1: {  /* only upper limit */
			int u = luaL_checkint(L, 2);
			luaL_argcheck(L, 1<=u, 1, "interval is empty");
			lua_pushnumber(L, floor(r*u)+1);  /* int between 1 and `u' */
			break;
		}
		case 2 + 1: {  /* lower and upper limits */
			int l = luaL_checkint(L, 2);
			int u = luaL_checkint(L, 3);
			luaL_argcheck(L, l<=u, 2, "interval is empty");
			lua_pushnumber(L, floor(r*(u-l+1))+l);  /* int between `l' and `u' */
			break;
		}
		default: return luaL_error(L, "wrong number of arguments");
	  }

	  return 1;
}

//----------------------------------------------------------------//
void FVHRandom::RegisterLuaClass ( MOAILuaState& state )
{
	MOAILuaObject::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void FVHRandom::RegisterLuaFuncs ( MOAILuaState& state ) 
{
	MOAILuaObject::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "randomSeed",					_randomSeed },
		{ "random", 					_random },

		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}
