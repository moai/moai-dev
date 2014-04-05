// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLCgt.h>
#include <zl-util/ZLFileStream.h>

//----------------------------------------------------------------//
ZLCgtSymbol* ZLCgt::FindNonterminal ( cc8* symbolName ) {

	for ( u32 i = 0; i < this->mSymbolTable.Size (); ++i ) {
		ZLCgtSymbol& symbol = this->mSymbolTable [ i ];
		if ( symbol.mKind != ZLCgtSymbol::CGT_NONTERMINAL ) continue;
		if ( symbol.mName == symbolName ) {
			return &symbol;
		} 
	}
	return 0;
}

//----------------------------------------------------------------//
ZLCgtSymbol* ZLCgt::FindSymbol ( cc8* symbolName ) {

	for ( u32 i = 0; i < this->mSymbolTable.Size (); ++i ) {
		ZLCgtSymbol& symbol = this->mSymbolTable [ i ];
		if ( symbol.mName == symbolName ) {
			return &symbol;
		} 
	}
	return 0;
}

//----------------------------------------------------------------//
ZLCgtSymbol* ZLCgt::FindTerminal ( cc8* symbolName ) {

	for ( u32 i = 0; i < this->mSymbolTable.Size (); ++i ) {
		ZLCgtSymbol& symbol = this->mSymbolTable [ i ];
		if ( symbol.mKind != ZLCgtSymbol::CGT_TERMINAL ) continue;
		if ( symbol.mName == symbolName ) {
			return &symbol;
		} 
	}
	return 0;
}

//----------------------------------------------------------------//
void ZLCgt::Load ( cc8* filename ) {

	ZLFileStream stream;
	stream.OpenRead ( filename );

	this->mHeader = this->ReadUnicodeAsASCII ( stream );
	
	size_t length = stream.GetLength ();
	while ( stream.GetCursor () < length ) {
		
		char recordType = stream.Read < u8 >( 0 );
		UNUSED ( recordType );
		assert ( recordType == 'M' );
		
		u16 totalEntries = stream.Read < u16 >( 0 );
		
		u8 contentType = this->ReadByteEntry ( stream );
		switch ( contentType ) {
			
			// Parameters
			case 'P': {
				
				this->mName = this->ReadStringEntry ( stream );
				this->mVersion = this->ReadStringEntry ( stream );
				this->mAuthor = this->ReadStringEntry ( stream );
				this->mAbout = this->ReadStringEntry ( stream );
				this->mCaseSensitive = this->ReadBoolEntry ( stream );
				this->mLALRStartSymbol = this->ReadIntEntry ( stream );
				
				break;
			}
			
			// Table Counts
			case 'T': {
				
				this->mSymbolTable.Init ( this->ReadIntEntry ( stream ));
				this->mCharSetTable.Init ( this->ReadIntEntry ( stream ));
				this->mRuleTable.Init ( this->ReadIntEntry ( stream ));
				this->mDFAStateTable.Init ( this->ReadIntEntry ( stream ));
				this->mLALRStateTable.Init ( this->ReadIntEntry ( stream ));
				
				break;
			}
			
			// Initial States
			case 'I': {
				
				this->mDFAInitialStateID = this->ReadIntEntry ( stream );
				this->mLALRInitialStateID = this->ReadIntEntry ( stream );
				
				break;
			}
			
			// Symbol Table Entry
			case 'S': {
				
				u16 index = this->ReadIntEntry ( stream );
				assert ( index < this->mSymbolTable.Size ());
				ZLCgtSymbol& symbol = this->mSymbolTable [ index ];
				
				symbol.mID = index;
				symbol.mName = this->ReadStringEntry ( stream );
				symbol.mKind = this->ReadIntEntry ( stream );
				
				break;
			}
			
			// Character Set Table Entry
			case 'C': {
			
				u16 index = this->ReadIntEntry ( stream );
				assert ( index < this->mCharSetTable.Size ());
				
				this->mCharSetTable [ index ].mCharacters = this->ReadStringEntry ( stream );
				
				break;
			}
			
			// Rule Table Entry
			case 'R': {
			
				u16 index = this->ReadIntEntry ( stream );
				assert ( index < this->mRuleTable.Size ());
				ZLCgtRule& rule = this->mRuleTable [ index ];
				
				rule.mRuleResult = this->ReadIntEntry ( stream );
				this->ReadEmptyEntry ( stream );
				
				// 4 is the # of entries read so far...
				u32 ruleSize = totalEntries - 4;
				rule.mRuleSymbols.Init ( ruleSize );
				for ( u32 i = 0; i < ruleSize; ++i ) {
					rule.mRuleSymbols [ i ] = this->ReadIntEntry ( stream );
				}
				break;
			}
			
			// DFA State Entry
			case 'D': {
			
				u16 index = this->ReadIntEntry ( stream );
				assert ( index < this->mDFAStateTable.Size ());
				ZLDfaState& dfaState = this->mDFAStateTable [ index ];
				
				dfaState.mID = index;
				dfaState.mAcceptState = this->ReadBoolEntry ( stream );
				dfaState.mAcceptSymbolID = this->ReadIntEntry ( stream );
				this->ReadEmptyEntry ( stream );
				
				// 5 is the # of entries read so far...
				u32 totalEdges = totalEntries - 5;
				assert (( totalEdges % 3 ) == 0 ); // Make sure it's a sensible number...
				totalEdges = ( u32 )( totalEdges / 3 ); // '3' is the size of each edge...
				
				dfaState.mEdges.Init ( totalEdges );
				
				for ( u32 i = 0; i < totalEdges; ++i ) {
					dfaState.mEdges [ i ].mCharSetID = this->ReadIntEntry ( stream );
					dfaState.mEdges [ i ].mTargetStateID = this->ReadIntEntry ( stream );
					this->ReadEmptyEntry ( stream );
				}
				break;
			}
			
			// LALR State Entry
			case 'L': {
			
				u16 index = this->ReadIntEntry ( stream );
				assert ( index < this->mLALRStateTable.Size ());
				ZLLalrState& lalrState = this->mLALRStateTable [ index ];
				
				lalrState.mID = index;
				this->ReadEmptyEntry ( stream );
				
				// 3 is the # of entries read so far...
				u32 totalActions = totalEntries - 3;
				assert (( totalActions % 4 ) == 0 ); // Make sure it's a sensible number...
				totalActions = ( u32 )( totalActions / 4 ); // '4' is the size of each edge...
				
				lalrState.mActions.Init ( totalActions );
				
				for ( u32 i = 0; i < totalActions; ++i ) {
					ZLLalrAction& action = lalrState.mActions [ i ];
					action.mInputSymbolID = this->ReadIntEntry ( stream );
					action.mActionType = this->ReadIntEntry ( stream );
					action.mTarget = this->ReadIntEntry ( stream );
					this->ReadEmptyEntry ( stream );
				}
				break;
			}
			
			// Unknown
			default: assert ( false );
		}
	}
}

//----------------------------------------------------------------//
bool ZLCgt::ReadBoolEntry ( ZLStream& stream ) {

	char entryType = stream.Read < char >( 0 );
	UNUSED ( entryType );
	assert ( entryType == 'B' );

	u8 result = stream.Read < char >( 0 );
	return ( result != 0 );
}

//----------------------------------------------------------------//
u8 ZLCgt::ReadByteEntry( ZLStream& stream ) {

	char entryType = stream.Read < char >( 0 );
	UNUSED ( entryType );
	assert ( entryType == 'b' );

	u8 result = stream.Read < u8 >( 0 );
	return result;
}

//----------------------------------------------------------------//
void ZLCgt::ReadEmptyEntry ( ZLStream& stream ) {

	char entryType = stream.Read < char >( 0 );
	UNUSED ( entryType );
	assert ( entryType == 'E' );
}

//----------------------------------------------------------------//
u16 ZLCgt::ReadIntEntry ( ZLStream& stream ) {

	char entryType = stream.Read < char >( 0 );
	UNUSED ( entryType );
	assert ( entryType == 'I' );

	u16 result = stream.Read < u16 >( 0 );
	return result;
}

//----------------------------------------------------------------//
STLString ZLCgt::ReadStringEntry ( ZLStream& stream ) {

	char entryType = stream.Read < char >( 0 );
	UNUSED ( entryType );
	assert ( entryType == 'S' );
	
	return ReadUnicodeAsASCII ( stream );
}

//----------------------------------------------------------------//
STLString ZLCgt::ReadUnicodeAsASCII ( ZLStream& stream ) {

	// TODO: change this to convert to UTF8!
	// Also: GOLD now has native UTF8 support. Look into it.

	STLString result = "";

	static const u32 BUFFER_SIZE = 1024;
	char buffer [ BUFFER_SIZE ];
	
	u16 utf = 1;
	while ( utf ) {
		for ( u32 i = 0; ( i < BUFFER_SIZE ) && utf; ++i ) {
			
			utf = stream.Read < u16 >( 0 );
			buffer [ i ] = ( char )( utf & 0x00ff );
		}
		result.append ( buffer );
	}
	return result;
}
