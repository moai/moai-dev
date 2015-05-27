// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGfxBuffer.h>
#include <moai-sim/MOAIMeshBuilder.h>

//================================================================//
// VertexComparator
//================================================================//

//----------------------------------------------------------------//
class VertexComparator {
public:

	MOAIMeshBuilder* mMeshBuilder;
	
	//----------------------------------------------------------------//
	VertexComparator () : mMeshBuilder ( 0 ) {}
	VertexComparator ( MOAIMeshBuilder* meshBuilder ) : mMeshBuilder ( meshBuilder ) {}
	
	//----------------------------------------------------------------//
	bool operator () ( const void* v0, const void* v1 ) const {
	
		less < u32 > foo;
	
		assert ( this->mMeshBuilder && this->mMeshBuilder->mVertexFormat );
		return ( this->mMeshBuilder->mVertexFormat->Compare ( v0, v1, 0.00001f, 0.00001f ) == -1 );
	}
};

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMeshBuilder::_build ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMeshBuilder, "U" )
	
	float snap = state.GetValue < float >( 2, 0.0f );
	
	self->PrepareRawVertices ();
	self->SnapCoords ( snap, snap, snap );
	self->Build ();
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMeshBuilder::_getMesh ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMeshBuilder, "U" )

	MOAIGfxBuffer* vtxBuffer	= state.GetLuaObject < MOAIGfxBuffer >( 2, false );
	MOAIGfxBuffer* idxBuffer	= state.GetLuaObject < MOAIGfxBuffer >( 3, false );
	
	u32 totalElements = 0;
	
	if ( vtxBuffer && idxBuffer ) {
		u32 idxSizeInBytes = state.GetValue < u32 >( 4, 4 );
		totalElements = self->GetMesh ( vtxBuffer, idxBuffer, idxSizeInBytes );
	}

	state.Push ( totalElements );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMeshBuilder::_setVertexFormat ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMeshBuilder, "U" )
	
	self->mVertexFormat.Set ( *self, MOAIVertexFormat::AffirmVertexFormat ( state, 2 ));
	return 0;
}

//----------------------------------------------------------------//
int MOAIMeshBuilder::_writeIndex ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMeshBuilder, "U" )
	
	u32 defaultIndex = self->mVertexFormat ? ( self->GetLength () / self->mVertexFormat->GetVertexSize ()) : 0;
	
	u32 index = state.GetValue < u32 >( 2, defaultIndex + 1 ) - 1;
	self->mIndices.Write < u32 >( index );
	return 0;
}

//================================================================//
// MOAIMeshBuilder
//================================================================//

//----------------------------------------------------------------//
void MOAIMeshBuilder::Build () {

	typedef STLMap < const void*, u32, VertexComparator > VertexMap;
	typedef VertexMap::iterator VertexMapIt;
	
	VertexMap vertexMap ( VertexMap::key_compare ( this ));

	this->mIndexMap.Init ( this->mTotalVertices );
	
	for ( size_t i = 0; i < this->mTotalVertices; ++i ) {
	
		const void* vertex = this->GetVertexBuffer ( this->mVertices, i );
		
		u32 index = 0;
		
		if ( vertexMap.contains ( vertex )) {
			index = vertexMap [ vertex ];
		}
		else {
			index = vertexMap.size ();
			vertexMap [ vertex ] =  index;
		}
		this->mIndexMap [ i ] = index;
		
		printf ( "INDEX: %d -> %d\n", i, index );
	}
	
	size_t vertexSize = this->mVertexFormat->GetVertexSize ();
	size_t totalVertices = vertexMap.size ();
	void* vertices = malloc ( vertexSize * totalVertices );
	
	for ( VertexMapIt vertexMapIt = vertexMap.begin (); vertexMapIt != vertexMap.end (); ++vertexMapIt ) {
	
		const void* source = vertexMapIt->first;
		void* dest = this->GetVertexBuffer ( vertices, vertexMapIt->second );
		
		printf ( "copy: %d\n", vertexMapIt->second );
		
		memcpy ( dest, source, vertexSize );
	}
	
	this->ClearVertices ();
	
	this->mVertices = vertices;
	this->mTotalVertices = totalVertices;
}

//----------------------------------------------------------------//
void MOAIMeshBuilder::ClearVertices () {

	if ( this->mVertices ) {
		free ( this->mVertices );
	}
	this->mVertices = 0;
	this->mTotalVertices = 0;
}

//----------------------------------------------------------------//
u32 MOAIMeshBuilder::GetMesh ( MOAIGfxBuffer* vtxBuffer, MOAIGfxBuffer* idxBuffer, u32 idxSizeInBytes ) {

	assert ( this->mVertexFormat );

	u32 vertexSize = this->mVertexFormat->GetVertexSize ();

	if ( vtxBuffer ) {
		
		size_t vtxBufferSize = this->mTotalVertices * vertexSize;
		
		vtxBuffer->Clear ();
		vtxBuffer->SetTarget ( ZGL_BUFFER_TARGET_ARRAY );
		vtxBuffer->Reserve ( vtxBufferSize );
		vtxBuffer->WriteBytes ( this->mVertices, vtxBufferSize );
	}

	size_t totalIndices = 0;

	if ( idxBuffer ) {
		
		idxBuffer->Clear ();
		idxBuffer->SetTarget ( ZGL_BUFFER_TARGET_ELEMENT_ARRAY );
		
		size_t idxStreamLength = this->mIndices.GetLength ();
		
		if ( idxStreamLength ) {
			
			totalIndices = idxStreamLength >> 2; // index stream is always 4 bytes
		
			size_t idxBufferSize = totalIndices * idxSizeInBytes;
			idxBuffer->Reserve ( idxBufferSize );
			
			this->mIndices.Seek ( 0, SEEK_SET );
			for ( u32 i = 0; i < totalIndices; ++i ) {
				
				u32 index = this->mIndexMap [ this->mIndices.Read < u32 >( 0 )];
			
				if ( idxSizeInBytes == 4 ) {
					idxBuffer->Write < u32 >( index );
				}
				else {
					idxBuffer->Write < u16 >( index );
				}
			}
		}
		else {
		
			totalIndices = this->mTotalInputVertices;
			
			size_t idxBufferSize = totalIndices * idxSizeInBytes;
			idxBuffer->Reserve ( idxBufferSize );
		
			for ( u32 i = 0; i < this->mTotalInputVertices; ++i ) {
			
				u32 index = this->mIndexMap [ i ];
				
				if ( idxSizeInBytes == 4 ) {
					idxBuffer->Write < u32 >( index );
				}
				else {
					idxBuffer->Write < u16 >( index );
				}
			}
		}
	}
	return totalIndices;
}

//----------------------------------------------------------------//
void* MOAIMeshBuilder::GetVertexBuffer ( void* vertices, size_t i ) {

	assert ( this->mVertexFormat );
	return ( void* )(( size_t )vertices + ( i * this->mVertexFormat->GetVertexSize ()));
}

//----------------------------------------------------------------//
MOAIMeshBuilder::MOAIMeshBuilder () :
	mVertices ( 0 ),
	mTotalVertices ( 0 ),
	mTotalInputVertices ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIMemStream )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIMeshBuilder::~MOAIMeshBuilder () {

	this->mVertexFormat.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIMeshBuilder::PrepareRawVertices () {

	assert ( this->mVertexFormat );
	
	this->ClearVertices ();
	this->mTotalInputVertices = 0;
	
	size_t rawSize = this->GetLength ();
	
	if ( rawSize ) {
	
		this->mTotalVertices = rawSize / this->mVertexFormat->GetVertexSize ();
		this->mTotalInputVertices = this->mTotalVertices;
	
		this->mVertices = malloc ( rawSize );
		this->Seek ( 0, SEEK_SET );
		this->ReadBytes ( this->mVertices, rawSize );
	}
	this->MOAIMemStream::Clear ();
}

//----------------------------------------------------------------//
void MOAIMeshBuilder::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIMemStream::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIMeshBuilder::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIMemStream::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "build",				_build },
		{ "getMesh",			_getMesh },
		{ "setVertexFormat",	_setVertexFormat },
		{ "writeIndex",			_writeIndex },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIMeshBuilder::SnapCoords ( float xSnap, float ySnap, float zSnap ) {

	assert ( this->mVertexFormat );
	
	for ( u32 i = 0; i < this->mTotalVertices; ++i ) {
		void* vertex = this->GetVertexBuffer ( this->mVertices, i );
		this->mVertexFormat->SnapCoords ( vertex, xSnap, ySnap, zSnap );
	}
}
