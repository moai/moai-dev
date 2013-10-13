// Expose the libtess2 'tesselator' library to MOAI. 
// (c) www.fivevolthigh.com, 2013

// Libtess2 is a game and tools oriented version of GLU tesselator.
// See https://code.google.com/p/libtess2/ 

#ifndef	FVHTESSELATOR_H
#define	FVHTESSELATOR_H

#include <libtess2/Include/tesselator.h>
#include <moai-core/MOAILua.h>

struct LibTessMemPool;

//================================================================//
// FVHTesselator
//================================================================//
/**	@name	FVHTesselator
	@text	Interface to the GLUT tesselation library.
	
    @const TESS_POLYGONS
	@const TESS_CONNECTED_POLYGONS
	@const TESS_BOUNDARY_CONTOURS

	@const WINDING_ODD
	@const WINDING_NONZERO
	@const WINDING_POSITIVE
	@const WINDING_NEGATIVE
	@const WINDING_ABS_GEQ_TWO
*/
class FVHTesselator :
	public virtual MOAILuaObject 
{
private:

	TESStesselator*	mTesselator;
	TESSalloc		mTessAlloc;
	
	// We use a simple fixed memory pool. Borrowed from the libtess2 example code.
	LibTessMemPool* mMemPool;
	
	enum MemoryMode {
		memHeap,		// Allocate memory as required from heap
		memPool,		// Use a fixed memory pool
	};

	//----------------------------------------------------------------//
	static int	_addContour		( lua_State* L );
	static int	_tesselate		( lua_State* L );
	static int	_setMemoryUse	( lua_State* L );

	//----------------------------------------------------------------//
	void		AddContour		( u32 coordsPerVert, const TESSreal* coords, u32 stride, u32 coordCount );
	u32			Tesselate		( u32 windingRule, u32 elementType, u32 polySize, u32 vertexSize, const TESSreal* normal );
	void		SetMemoryUse	( MemoryMode mode, u32 poolBytes );
	
public:

	DECL_LUA_FACTORY ( FVHTesselator )

	//----------------------------------------------------------------//
			FVHTesselator			();
			~FVHTesselator			();
	
	u32		LoadVerts				( MOAILuaState& state, u32 idx, TESSreal* tessVerts, u32 max );

	void	RegisterLuaClass		( MOAILuaState& state );
	void	RegisterLuaFuncs		( MOAILuaState& state );
};

#endif // FVHTESSELATOR_H
