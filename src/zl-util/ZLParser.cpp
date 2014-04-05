// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLLexStream.h>
#include <zl-util/ZLParser.h>
#include <zl-util/ZLSyntaxNode.h>

//================================================================//
// ZLDfaToken
//================================================================//

//----------------------------------------------------------------//
ZLDfaToken::ZLDfaToken () :
	mSyntaxNode ( 0 ),
	mLine ( 0 ) {
}

//================================================================//
// ZLParser
//================================================================//

//----------------------------------------------------------------//
void ZLParser::Init ( ZLCgt& cgt, cc8* errorTerminal ) {
	
	this->mCGT = &cgt;
	
	if ( errorTerminal ) {
		ZLCgtSymbol* symbol = this->mCGT->FindTerminal ( errorTerminal );
		if ( symbol ) {
			this->mErrorSymbolID = symbol->mID;
			this->mHandleErrors = true;
		}
	}
}

//----------------------------------------------------------------//
ZLSyntaxNode* ZLParser::Parse ( ZLStream& stream ) {

	ZLLexStream scanner;
	scanner.SetStream ( &stream );
	
	return this->Parse ( &scanner, false );
}

//----------------------------------------------------------------//
ZLSyntaxNode* ZLParser::Parse ( ZLLexStream* scanner, bool trimReductions ) {

	this->mLine = 0;

	ZLSyntaxNode* syntaxTree = 0;

	ZLDfaToken startToken;
	startToken.mLALRStateID = this->mCGT->mLALRInitialStateID;
	startToken.mSymbol = this->mCGT->mSymbolTable [ this->mCGT->mLALRStartSymbol ];
	this->mTokenStack.push_back ( startToken );
	
	this->mCurrentLALRStateID = this->mCGT->mLALRInitialStateID;
	
	ZLDfaToken token;
	this->RetrieveToken ( &token, scanner );
	
	bool done = false;
	u32 commentLevel = 0;
	while ( !done ) {
	
		if ( commentLevel ) {
			switch ( token.mSymbol.mKind ) {
				case ZLCgtSymbol::CGT_COMMENTEND: {
					commentLevel = 0;
					break;
				}
				case ZLCgtSymbol::CGT_COMMENTSTART: {
					++commentLevel;
					break;
				}
			}
			this->RetrieveToken ( &token, scanner ); // Get the next token
			continue;
		}
	
		switch ( token.mSymbol.mKind ) {
		
			case ZLCgtSymbol::CGT_COMMENTSTART: {
				++commentLevel;
				this->RetrieveToken ( &token, scanner ); // Get the next token
				break;
			}
			
			case ZLCgtSymbol::CGT_ERROR: {
				// This is a lexical error; we won't bother recovering from it
				// Just report it and die
				return 0;
				break;
			}
			
			case ZLCgtSymbol::CGT_COMMENTLINE: {
				
				u8 nextChar;
				while ( !scanner->IsAtEnd ()) {
					nextChar = scanner->Read < u8 >( 0 );
					if ( nextChar == '\n' ) break;
				}

				this->RetrieveToken ( &token, scanner ); // Get the next token
				break;
			}
			
			case ZLCgtSymbol::CGT_END:
			case ZLCgtSymbol::CGT_NONTERMINAL:
			case ZLCgtSymbol::CGT_TERMINAL: {
			
				switch ( this->ParseToken ( &token, trimReductions )) {
				
					case ZLLalrAction::LALR_ACCEPT: {
						syntaxTree = this->mTokenStack.back ().mSyntaxNode;
						this->mTokenStack.pop_back ();
						assert ( this->mTokenStack.size () == 1 ); // Should only be start token left...
						done = true;
						break;
					}
					
					case ZLLalrAction::LALR_ERROR: {
						return 0;
						break;
					}
					
					case ZLLalrAction::LALR_REDUCE: {
						break;
					}
					
					case ZLLalrAction::LALR_GOTO:
					case ZLLalrAction::LALR_SHIFT: {
						this->RetrieveToken ( &token, scanner ); // Get the next token
						break;
					}
				}
				break;
			}
			
			case ZLCgtSymbol::CGT_WHITESPACE: {
				
				this->RetrieveToken ( &token, scanner ); // Get the next token
				break;
			}
		}
	}
	this->mTokenStack.clear ();
	return syntaxTree;
}

//----------------------------------------------------------------//
/*
	Error handling...
	Search back through the stack until a token with a transition to error occurs
	End search is top of stack is reached
	Shift the error onto the stack
	Continue
*/

//----------------------------------------------------------------//
u32 ZLParser::ParseToken ( ZLDfaToken* token, bool trimReductions ) {

	ZLLalrState& state = this->mCGT->mLALRStateTable [ this->mCurrentLALRStateID ];

	for ( u32 i = 0; i < state.mActions.Size (); ++i ) {
		ZLLalrAction& action = state.mActions [ i ];
		
		if ( action.mInputSymbolID == token->mSymbol.mID ) {
			switch ( action.mActionType ) {
				
				case ZLLalrAction::LALR_ACCEPT: {
					return ZLLalrAction::LALR_ACCEPT;
				}
				
				case ZLLalrAction::LALR_GOTO: {
					token->mLALRStateID = state.mID;
					this->mTokenStack.push_back ( *token );
					this->mCurrentLALRStateID = action.mTarget;
					
					//printf ( "REDUCE: %d\n", this->mCurrentLALRStateID );
					
					return ZLLalrAction::LALR_GOTO;
				}
				
				case ZLLalrAction::LALR_SHIFT: {
					
					if ( token->mSymbol.mKind == ZLCgtSymbol::CGT_TERMINAL ) {
						// Lookup the factory method for this terminal and use it to create the ZLSyntaxNode
						token->mSyntaxNode = new ZLSyntaxNode ();
						token->mSyntaxNode->mID = token->mSymbol.mID;
						token->mSyntaxNode->mLine = token->mLine;
						token->mSyntaxNode->mName = token->mSymbol.mName;
						token->mSyntaxNode->mTerminal = token->mData;
					}
					
					token->mLALRStateID = state.mID;
					this->mTokenStack.push_back ( *token );
					this->mCurrentLALRStateID = action.mTarget;
					
					//printf ( "kSHIFT: %d\n", this->mCurrentLALRStateID );
					
					return ZLLalrAction::LALR_SHIFT;
				}
				
				case ZLLalrAction::LALR_REDUCE: {
					ZLCgtRule& rule = this->mCGT->mRuleTable [ action.mTarget ];
					ZLCgtSymbol& ruleResult = this->mCGT->mSymbolTable [ rule.mRuleResult ];
					//assert ( rule.mRuleSymbols.Size ()); // allow for empty rule...
					
					// Check to see if we should trim reductions
					bool doTrim = false;
					if ( rule.mRuleSymbols.Size () == 1 ) {
						ZLCgtSymbol& handle = this->mCGT->mSymbolTable [ rule.mRuleSymbols [ 0 ]];
						doTrim = (( handle.mKind == ZLCgtSymbol::CGT_NONTERMINAL ) && trimReductions );
					}
					
					// Create the head
					ZLDfaToken head;
					head.mSymbol = ruleResult;
					
					// Create the syntax node and initialize it...
					u32 handleSize = rule.mRuleSymbols.Size ();
					
					u16 prevStateID = this->mTokenStack.back ().mLALRStateID;
					
					if ( doTrim ) {
						
						head.mSyntaxNode = this->mTokenStack.back ().mSyntaxNode;
						this->mTokenStack.pop_back ();
					}
					else {
						head.mSyntaxNode = new ZLSyntaxNode;
						head.mSyntaxNode->mID = action.mTarget; 
						head.mSyntaxNode->mName = ruleResult.mName;
						
						// Pop the handle off the stack and add it to the syntax node
						if ( handleSize ) {
						
							head.mSyntaxNode->mChildren.Init ( handleSize );
							for ( u32 j = 0; j < rule.mRuleSymbols.Size (); ++j ) {
								head.mSyntaxNode->mChildren [ handleSize - ( j + 1 )] = this->mTokenStack.back ().mSyntaxNode;
								prevStateID = this->mTokenStack.back ().mLALRStateID;
								this->mTokenStack.pop_back ();
							}
						
							head.mSyntaxNode->mLine = head.mSyntaxNode->mChildren [ 0 ]->mLine;
						}
						else {
							
							ZLSyntaxNode* eof = new ZLSyntaxNode;
							eof->mID = 0;
							eof->mLine = token->mLine;

							eof->mName = token->mSymbol.mName;
							eof->mTerminal = token->mData;
							
							head.mSyntaxNode->mChildren.Init ( 1 );
							head.mSyntaxNode->mChildren [ 0 ] = eof;
							
							head.mSyntaxNode->mLine = eof->mLine;
						}
					}
					
					// Set the current state back to before the rule was matched...
					this->mCurrentLALRStateID = prevStateID;
					
					// Push the token on the stack and change the state...
					this->ParseToken ( &head, trimReductions );
					return ZLLalrAction::LALR_REDUCE;
				}
			}
		}
	}
	
	// At this stage, no action has been found that matches the token's ID.
	// So we should turn the token into an error and start popping symbols from
	// the stack until we either hit bottom or find a symbol that can
	// take action on an error.
	if ( this->mHandleErrors ) {
	
		if ( token->mSymbol.mID == this->mErrorSymbolID ) {
		
			while ( this->mTokenStack.size () > 1 ) {
		
				this->mCurrentLALRStateID = this->mTokenStack.back ().mLALRStateID;
				ZLLalrState& state2 = this->mCGT->mLALRStateTable [ this->mCurrentLALRStateID ];
				
				this->mTokenStack.pop_back ();
				
				for ( u32 i = 0; i < state2.mActions.Size (); ++i ) {
					ZLLalrAction& action = state2.mActions [ i ];
					if ( action.mInputSymbolID == token->mSymbol.mID ) {
						return ZLLalrAction::LALR_REDUCE;
					}
				}
			}
			return ZLLalrAction::LALR_REDUCE;
		}
		else {
			// Make the error token
			token->mData = "error";
			token->mSymbol.mKind = ZLCgtSymbol::CGT_TERMINAL;
			token->mSymbol.mID = this->mErrorSymbolID;
			return ZLLalrAction::LALR_REDUCE;
		}
	}
	
	return ZLLalrAction::LALR_ERROR;
}

//----------------------------------------------------------------//
void ZLParser::RetrieveToken ( ZLDfaToken* token, ZLLexStream* scanner ) {
	
	assert ( token );
	
	if ( scanner->IsAtEnd ()) {
		token->mSymbol.mID = 0; // This is supposed to be the ID for EOF in the symbol table for any GOLD CGT
		token->mSymbol.mKind = ZLCgtSymbol::CGT_END;
		token->mData = "EOF";
		return;
	}
	
	u16 stateID = this->mCGT->mDFAInitialStateID;
	ZLDfaState* dfaState = &this->mCGT->mDFAStateTable [ stateID ];
	
	ZLDfaState* acceptState = 0;
	size_t acceptLength = 0;
	
	bool done = false;
	
	size_t startCursor = scanner->GetCursor ();
	static const u32 bufferSize = 1024;
	char buffer [ bufferSize ];
	
	while ( !done ) {
		
		if ( dfaState->mAcceptState ) {
			acceptState = dfaState;
			acceptLength = scanner->GetCursor () - startCursor;
		}
		
		u8 lookahead = 0; // Handle EOF
		if ( !scanner->IsAtEnd ()) {
			lookahead = scanner->Read < u8 >( 0 );
		}
		
		bool transition = false;
		for( u32 i = 0; i < dfaState->mEdges.Size (); ++i ) {
			
			ZLDfaStateEdge& edge = dfaState->mEdges [ i ];
			
			assert ( edge.mCharSetID < this->mCGT->mCharSetTable.Size ());
			ZLCgtCharSet& charSet = this->mCGT->mCharSetTable [ edge.mCharSetID ];
			
			if ( charSet.mCharacters.find (( char )lookahead ) != string::npos ) {
				dfaState = &this->mCGT->mDFAStateTable [ edge.mTargetStateID ];
				transition = true;
				break;
			}
		}
		
		if ( !transition ) {
			if ( acceptState ) {
				token->mSymbol = this->mCGT->mSymbolTable [ acceptState->mAcceptSymbolID ];
			}
			else {
				if ( this->mHandleErrors ) {
					acceptLength = scanner->GetCursor () - startCursor;
					token->mSymbol.mKind = ZLCgtSymbol::CGT_TERMINAL;
					token->mSymbol.mID = this->mErrorSymbolID;
				}
				else {
					acceptLength = 1;
					token->mSymbol.mKind = ZLCgtSymbol::CGT_ERROR;
				}
			}
			
			scanner->Seek ( (long) startCursor, SEEK_SET );
			token->mLine = (u32) scanner->GetLine ();
			
			if ( acceptLength < ( bufferSize - 1 )) {

				scanner->ReadBytes ( buffer, acceptLength );
				buffer [ acceptLength ] = 0;
				token->mData = buffer;
			}
			else {
				
				ZLLeanArray < char > bigBuffer;
				bigBuffer.Init ( (u32) acceptLength + 1 );
				scanner->ReadBytes ( bigBuffer, acceptLength );
				bigBuffer [ acceptLength ] = 0;
				token->mData = bigBuffer;
			}
			return;
		}
	}
}

//----------------------------------------------------------------//
ZLParser::ZLParser () :
	mHandleErrors ( false ),
	mErrorSymbolID ( 0 ) {
}

//----------------------------------------------------------------//
ZLParser::~ZLParser () {
}