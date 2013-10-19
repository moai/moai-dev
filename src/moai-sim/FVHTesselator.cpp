// Expose the libtess2 'tesselator' library to MOAI.
// (c) www.fivevolthigh.com, 2013

#include "pch.h"

#include <moai-sim/FVHTesselator.h>

//----------------------------------------------------------------//
// We use a simple pool here. This is lifted straight from the libtess2 example code.
struct LibTessMemPool
{
	u32 mCap;
	u32 mSize;
	unsigned char* mBuf;

	LibTessMemPool ( u32 cap = 1024*512 )
		: mCap ( cap )
		, mSize ( 0 )
		, mBuf ( new unsigned char[cap] )
	{
		// Make sure libtess2 gets zeroed memory.
		memset( this->mBuf, 0, cap );
	}
	
	~LibTessMemPool ()
	{
		if ( mBuf ) 
		{
			delete [] this->mBuf;
			this->mBuf = 0;
		}
	}

	void* alloc ( u32 size )
	{
		if ( this->mSize + size < this->mCap )
		{
			unsigned char* ptr = this->mBuf + this->mSize;
			this->mSize += size;
			return ptr;
		}

		// Pool is empty
		return 0;
	}

	void free( void* ptr )
	{
		// empty
	}
};

static void* poolAlloc ( void* userData, u32 size )
{
	LibTessMemPool* pPool = reinterpret_cast < LibTessMemPool* >( userData );

	return pPool->alloc( size );
}

static void poolFree ( void* userData, void* ptr )
{
	LibTessMemPool* pPool = reinterpret_cast < LibTessMemPool* >( userData );

	return pPool->free ( ptr );
}

//----------------------------------------------------------------//
static void* heapAlloc ( void* userData, u32 size )
{
	void* ptr = malloc ( size );
	if ( ptr ) 
	{
		// Make sure libtess2 gets zeroed memory.
		memset( ptr, 0, size );
	}

	return ptr;
}

static void heapFree ( void* userData, void* ptr )
{
	if ( ptr )
	{
		free ( ptr );
	}
}

//----------------------------------------------------------------//
FVHTesselator::FVHTesselator ()
	: mTesselator ( 0 )
	, mMemPool ( 0 )
{
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
FVHTesselator::~FVHTesselator ()
{
	if ( this->mTesselator )
	{
		tessDeleteTess ( this->mTesselator );
		this->mTesselator = 0;
	}

	if ( this->mMemPool )
	{
		delete this->mMemPool;
		this->mMemPool = 0;
	}
}

//----------------------------------------------------------------//
void FVHTesselator::AddContour ( u32 coordsPerVert, const TESSreal* coords, u32 stride, u32 coordCount )
{
	assert ( this->mTesselator );

	tessAddContour ( this->mTesselator, coordsPerVert, coords, stride, coordCount / coordsPerVert );
}

//----------------------------------------------------------------//
u32 FVHTesselator::Tesselate ( u32 windingRule, u32 elementType, u32 polySize, u32 vertexSize, const TESSreal* normal )
{
	assert ( this->mTesselator );

	return tessTesselate ( this->mTesselator, windingRule, elementType, polySize, vertexSize, normal );
}

//----------------------------------------------------------------//
void FVHTesselator::SetMemoryUse ( MemoryMode mode, u32 poolBytes )
{
	assert ( !this->mTesselator );
	
	memset ( &mTessAlloc, 0, sizeof( TESSalloc ) );
	if ( mode == memHeap )
	{
		// Use simple heap allocation
		mTessAlloc.memalloc = heapAlloc;
		mTessAlloc.memfree = heapFree;		
	}
	else if ( mode == memPool )
	{
		// Use a pool of the specified size
		mMemPool = new LibTessMemPool( poolBytes );
		mTessAlloc.memalloc = poolAlloc;
		mTessAlloc.memfree = poolFree;
		
		mTessAlloc.userData = reinterpret_cast < void* >( mMemPool );
		mTessAlloc.extraVertices = 256; // realloc not provided, allow 256 extra vertices.
	}			
	
	this->mTesselator = tessNewTess( &mTessAlloc );			
}

//----------------------------------------------------------------//
u32 FVHTesselator::LoadVerts ( MOAILuaState& state, u32 idx, TESSreal* tessVerts, u32 max )
{
	u32 itr = state.PushTableItr ( idx );
	idx = 0;
	
	u32 total = 0;
	for ( ; state.TableItrNext( itr ) && ( total < max ); ++idx ) 
	{		
		tessVerts[ total ] = state.GetValue < double >( -1, 0 );
		++ total;
	}

	return total;
}

//----------------------------------------------------------------//
/**   @name addContour
@text Add a set of vertices to the tesselator.
 
@in         FVHTesselator self
@in         table verts Array containing vertex coordinate components ( t[1] = x0, t[2] = y0, t[3] = x1, t[4] = y1... )
*/
int FVHTesselator::_addContour ( lua_State* L )
{
	// This function is largely inspired by the MOAIBox2DBody::_addChain function.
	MOAI_LUA_SETUP ( FVHTesselator, "U" ) 

	assert ( self->mTesselator );

	// Take in a list of 2d vertexes and pack them into a libtess friendly buffer.
	u32 totalCoords = lua_objlen ( state, 2 );
	TESSreal* tessCoords = new TESSreal [ totalCoords ];
	totalCoords = self->LoadVerts ( state, 2, tessCoords, totalCoords );

	// Add 2D vertexes to the tesselator.
	self->AddContour ( 2 /* (x,y) coords per vertex */ , tessCoords, sizeof(TESSreal) * 2, totalCoords );
	
	// Tess takes a copy of the vertices, so we can release it immediately.
	delete [] tessCoords;
	tessCoords = 0;

	return 0;
}

//----------------------------------------------------------------//
/**   @name setMemoryUse
@text Control the way the tesselator uses memory during operations.
@in         FVHTesselator self
@in			mode	Memory mode	See FVHTesselator memory constants.
@in			poolSize	Number of bytes for memPool mode.
*/
int FVHTesselator::_setMemoryUse( lua_State* L )
{
	MOAI_LUA_SETUP ( FVHTesselator, "U" ) 

	assert ( !self->mTesselator );
	
	u32 poolBytes = 0;
	enum MemoryMode memMode = static_cast < enum MemoryMode >( state.GetValue < u32 >( 2, 0 ) );
	if ( memMode == memPool ) 
	{
		poolBytes = state.GetValue < u32 >( 3, 0 );
	}
	self->SetMemoryUse( memMode, poolBytes );
	
	return 0;
}

//----------------------------------------------------------------//
/**   @name tesselate
@text Add a set of vertices to the tesselator.
 
@in         FVHTesselator self
@in			mode	Tesselation mode	See FVHTesselator mode constants.
@in			winding	OpenGL winding	See FVHTesselator winding constants.
@in			number	maxPolyVerts	Max number of verts per output polygon.
@out		boolean	tesselation successful.
@out		table	elements	Table of elements, each element is a table of vertices.	
*/
int FVHTesselator::_tesselate( lua_State* L )
{
	MOAI_LUA_SETUP( FVHTesselator, "UNN" )

	assert( self->mTesselator );
	enum TessElementType tessMode = static_cast < enum TessElementType >( state.GetValue < u32 >( 2, 0 ) );
	enum TessWindingRule tessWinding = static_cast < enum TessWindingRule >( state.GetValue < u32 >( 3, 0 ) );
	u32 maxPolyVerts = state.GetValue < u32 >( 4, 0 );

	if ( tessTesselate( self->mTesselator, tessWinding, tessMode, maxPolyVerts, 2, 0 ) )
	{
		// Tesselation succeeded
		lua_pushboolean ( state, true );
		
		// Tesselator is ready to return information to caller 
		// We build a table of elements, where each element is a table of vertices
		const int nelems = tessGetElementCount ( self->mTesselator );
		const int* elems = tessGetElements ( self->mTesselator );
		const TESSreal* verts = tessGetVertices ( self->mTesselator );
		
		// Table of element tables
		lua_newtable ( L );
		
		for ( int i = 0; i < nelems; ++i )
		{				
			// Next element table
			lua_newtable ( L );
			
			// Next element offset
			const int* p = &elems[ i * maxPolyVerts ];
			// Vertices of next element
			int entry = 1;
			for ( int j = 0; j < maxPolyVerts && p[j] != TESS_UNDEF; ++j )
			{
				lua_pushnumber ( L, verts[p[j]*2] );
				lua_rawseti ( L,-2,entry ++ );
				
				lua_pushnumber ( L, verts[p[j]*2+1] );
				lua_rawseti ( L,-2,entry ++ );
			}

			// Set next entry in top table
			lua_rawseti ( L,-2, i+1 );
		}
	}	
	else
	{
		// Failed to tesselate
		return 0;
	} 
	
	return 2;
}

//----------------------------------------------------------------//
void FVHTesselator::RegisterLuaClass ( MOAILuaState& state )
{
	MOAILuaObject::RegisterLuaClass ( state );

    // Libtess modes
    state.SetField ( -1, "TESS_POLYGONS", TESS_POLYGONS );
	state.SetField ( -1, "TESS_CONNECTED_POLYGONS", TESS_CONNECTED_POLYGONS );
	state.SetField ( -1, "TESS_BOUNDARY_CONTOURS", TESS_BOUNDARY_CONTOURS );

	// See http://www.songho.ca/opengl/gl_tessellation.html#winding_rules
	state.SetField ( -1, "WINDING_ODD", TESS_WINDING_ODD );
	state.SetField ( -1, "WINDING_NONZERO", TESS_WINDING_NONZERO );
	state.SetField ( -1, "WINDING_POSITIVE", TESS_WINDING_POSITIVE );
	state.SetField ( -1, "WINDING_NEGATIVE", TESS_WINDING_NEGATIVE );
	state.SetField ( -1, "WINDING_ABS_GEQ_TWO", TESS_WINDING_ABS_GEQ_TWO );
	
	// MOAITesselator memory usage modes
	state.SetField ( -1, "MEM_HEAP", memHeap );
	state.SetField ( -1, "MEM_POOL", memPool );
}

//----------------------------------------------------------------//
void FVHTesselator::RegisterLuaFuncs ( MOAILuaState& state ) 
{
	MOAILuaObject::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "addContour",					_addContour },
		{ "setMemoryUse", 				_setMemoryUse },
		{ "tesselate",					_tesselate },

		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}
