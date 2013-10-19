// Expose a fixed version of srand() and rand() to MOAI, rather than
// relying on what the host libc supplies.
// (c) www.fivevolthigh.com, 2013

#ifndef	FVHRANDOM_H
#define	FVHRANDOM_H

#include <moai-core/MOAILua.h>

//================================================================//
// FVHRandom
//================================================================//
/**	@name	FVHRandom
	@text	Interface to host independent randomSeed and random functions.
*/
class FVHRandom :
	public virtual MOAILuaObject 
{
public:	
	typedef int int32_t;

	//----------------------------------------------------------------//
	struct fvh_random_data
	  {
		int32_t *fptr;		/* Front pointer.  */
		int32_t *rptr;		/* Rear pointer.  */
		int32_t *state;		/* Array of state values.  */
		int rand_type;		/* Type of random number generator.  */
		int rand_deg;		/* Degree of random number generator.  */
		int rand_sep;		/* Distance between front and rear.  */
		int32_t *end_ptr;		/* Pointer behind state table.  */
	  };
	
private:
	//----------------------------------------------------------------//
	struct fvh_random_data mState;
	
	//----------------------------------------------------------------//
	static int	_randomSeed		( lua_State* L );
	static int	_random			( lua_State* L );

	//----------------------------------------------------------------//
	long int Random(struct fvh_random_data* buf);
	void RandomSeed(struct fvh_random_data* buf, unsigned int seed);
	
public:

	DECL_LUA_FACTORY ( FVHRandom )

	//----------------------------------------------------------------//
			FVHRandom				();
			~FVHRandom				();
	
	void	RegisterLuaClass		( MOAILuaState& state );
	void	RegisterLuaFuncs		( MOAILuaState& state );
};

#endif // FVHRANDOM_H
