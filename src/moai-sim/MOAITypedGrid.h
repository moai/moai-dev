// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITYPEDGRID_H
#define	MOAITYPEDGRID_H

#include <moai-sim/MOAIGridSpace.h>

//================================================================//
// MOAITypedGrid
//================================================================//
/**	@name	MOAITypedGrid
	@text	Grid data object. Grid cells are indexed starting and (1,1).
			Grid indices will wrap if out of range.
*/
template <class T> class MOAITypedGrid :
	public MOAIGridSpace {
private:

	ZLLeanArray <T> mTiles;

	//----------------------------------------------------------------//
	/* These implementations will only work for T which are
	 * numerics.
	 */
	static int		_fill			( lua_State* L ) {
		MOAI_LUA_SETUP ( MOAITypedGrid<T>, "UN" )

		u32 value	= state.GetValue < T >( 2, 1 );
		
		self->Fill ( value );
		
		return 0;
	}
	static int		_getTile		( lua_State* L ) {
		MOAI_LUA_SETUP ( MOAITypedGrid<T>, "UNN" )

		int xTile	= state.GetValue < int >( 2, 1 ) - 1;
		int yTile	= state.GetValue < int >( 3, 1 ) - 1;
		
		T tile = self->GetTile ( xTile, yTile );
		state.Push ( tile );
		return 1;
	}

	static int		_setRow			( lua_State* L ) {
		MOAI_LUA_SETUP ( MOAITypedGrid<T>, "UN" )

		u32 row = state.GetValue < u32 >( 2, 1 ) - 1;
		u32 total = lua_gettop ( state ) - 2;
		
		for ( u32 i = 0; i < total; ++i ) {
		
			T tile = state.GetValue < T >( 3 + i, 0 );
			self->SetTile ( i, row, tile );
		}
	
		return 0;
	}
	static int		_setTile		( lua_State* L ) {
		MOAI_LUA_SETUP ( MOAITypedGrid<T>, "UNNN" )

		int xTile	= state.GetValue < int >( 2, 1 ) - 1;
		int yTile	= state.GetValue < int >( 3, 1 ) - 1;
		T tile	= state.GetValue < T >( 4, 0 );
		
		self->SetTile ( xTile, yTile, tile );
		
		return 0;
	}

	static int		_streamTilesIn		( lua_State* L ) {
		MOAI_LUA_SETUP ( MOAITypedGrid<T>, "UU" )
		
		MOAIStream* stream = state.GetLuaObject < MOAIStream >( 2, true );
		if ( stream ) {
			state.Push ( self->StreamTilesIn ( stream->GetZLStream ()));
			return 1;
		}
		return 0;
	}
	static int		_streamTilesOut		( lua_State* L ) {
		MOAI_LUA_SETUP ( MOAITypedGrid<T>, "UU" )
		
		MOAIStream* stream = state.GetLuaObject < MOAIStream >( 2, true );
		if ( stream ) {
			state.Push ( self->StreamTilesOut ( stream->GetZLStream ()));
			return 1;
		}
		return 0;
	}

	//----------------------------------------------------------------//
	void			OnResize			() {
		this->mTiles.Init ( this->GetTotalCells ());
		this->mTiles.Fill ( 0 );
	}

public:
	
	//----------------------------------------------------------------//
	T			GetTile			( int xTile, int yTile ) {

		MOAICellCoord coord ( xTile, yTile );
		if ( this->IsValidCoord ( coord )) {
			u32 addr = this->GetCellAddr ( coord );
			if ( addr < this->mTiles.Size ()) {
				return this->mTiles [ addr ];
			}
		}
		return 0;
	}
				MOAITypedGrid		() {
		RTTI_SINGLE ( MOAIGridSpace )
	}
				~MOAITypedGrid		() {
	}
	void			Fill			( T value ) {

		this->mTiles.Fill ( value );
		return;
	}
	void			RegisterLuaClass	( MOAILuaState& state ) {
		MOAIGridSpace::RegisterLuaClass ( state );
	}
	void			RegisterLuaFuncs	( MOAILuaState& state ) {

		MOAIGridSpace::RegisterLuaFuncs ( state );

		luaL_Reg regTable [] = {
			{ "fill",				_fill },
			{ "getTile",			_getTile },
			{ "setRow",				_setRow },
			{ "setTile",			_setTile },
			{ "streamTilesIn",		_streamTilesIn },
			{ "streamTilesOut",		_streamTilesOut },
			{ NULL, NULL }
		};

		luaL_register ( state, 0, regTable );
	}
	void			RowFromString		( u32 rowID, cc8* str );
	STLString		RowToString			( u32 rowID );
	void			SerializeIn			( MOAILuaState& state, MOAIDeserializer& serializer ) {
		UNUSED ( serializer );

		this->MOAIGridSpace::SerializeIn ( state, serializer );
		this->mTiles.Init ( this->MOAIGridSpace::GetTotalCells ());

		state.GetField ( -1, "mData" );

		if ( state.IsType ( -1, LUA_TSTRING )) {
			
			void* tiles = this->mTiles;
			size_t tilesSize = this->mTiles.Size () * sizeof ( T );
			
			STLString base64 = lua_tostring ( state, -1 ); 
			base64.base_64_decode ( tiles, tilesSize );
			
			ZLLeanArray < u8 > unzip;
			ZLZip::Inflate ( this->mTiles, this->mTiles.Size () * sizeof ( u32 ), unzip );
			
			tiles = unzip.Data ();
			if ( unzip.Size () < tilesSize ) {
				tilesSize = unzip.Size ();
			}
			memcpy ( this->mTiles, tiles, tilesSize );
		}
		
		lua_pop ( state, 1 );
	}
	void			SerializeOut		( MOAILuaState& state, MOAISerializer& serializer ) {
		UNUSED ( serializer );

		this->MOAIGridSpace::SerializeOut ( state, serializer );

		ZLLeanArray < u8 > zip;
		ZLZip::Deflate ( this->mTiles, this->mTiles.Size () * sizeof ( T ), zip );

		STLString base64;
		base64.base_64_encode ( zip.Data (), zip.Size ());
		
		lua_pushstring ( state, base64.str ());
		lua_setfield ( state, -2, "mData" );
	}
	void			SetTile ( u32 addr, T tile ) {

		u32 size = this->mTiles.Size ();

		if ( size ) {
			addr = addr % this->mTiles.Size ();
			this->mTiles [ addr ] = tile;
		}
	}
	void SetTile ( int xTile, int yTile, T tile ) {

		MOAICellCoord coord ( xTile, yTile );
		if ( this->IsValidCoord ( coord )) {
		
			u32 addr = this->GetCellAddr ( coord );
			if ( addr < this->mTiles.Size ()) {
				this->mTiles [ addr ] = tile;
			}
		}
	}
	size_t StreamTilesIn ( ZLStream* stream ) {

		if ( !stream ) return 0;
		
		size_t size = this->mTiles.Size () * sizeof ( T );
		return stream->ReadBytes ( this->mTiles, size );
	}

	size_t StreamTilesOut ( ZLStream* stream ) {

		if ( !stream ) return 0;

		size_t size = this->mTiles.Size () * sizeof ( T );
		return stream->WriteBytes ( this->mTiles, size );
	}

};

#include "pch.h"

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	fill
	@text	Set all tiles to a single value

	@in		MOAIGrid self
	@in		number value
	@out	nil
*/

//----------------------------------------------------------------//
/**	@name	getTile
	@text	Returns the value of a given tile.

	@in		MOAIGrid self
	@in		number xTile
	@in		number yTile
	@out	number tile
*/
//----------------------------------------------------------------//
/**	@name	setRow
	@text	Initializes a grid row given a variable argument list of values.

	@in		MOAIGrid self
	@in		number row
	@in		...
	@out	nil
*/

//----------------------------------------------------------------//
/**	@name	setTile
	@text	Sets the value of a given tile

	@in		MOAIGrid self
	@in		number xTile
	@in		number yTile
	@in		number value
	@out	nil
*/
//----------------------------------------------------------------//
/**	@name	streamTilesIn
	@text	Reads tiles directly from a stream. Call this only after
			initializing the grid. Only the content of the tiles
			buffer is read.

	@in		MOAIGrid self
	@in		MOAIStream stream
	@out	number bytesRead
*/

//----------------------------------------------------------------//
/**	@name	streamTilesOut
	@text	Writes tiles directly to a stream. Only the content of
			the tiles buffer is written.

	@in		MOAIGrid self
	@in		MOAIStream stream
	@out	number bytesWritten
*/

//================================================================//
// MOAIGrid
//================================================================//

#endif
