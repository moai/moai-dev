// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USLexStream.h>
#include <uslscore/USParser.h>
#include <uslscore/USSyntaxNode.h>

//================================================================//
// USDfaToken
//================================================================//

//----------------------------------------------------------------//
USDfaToken::USDfaToken () :
	mSyntaxNode ( 0 ),
	mLine ( 0 ) {
}

//================================================================//
// USParser
//================================================================//

//----------------------------------------------------------------//
void USParser::Init ( USCgt& cgt, cc8* errorTerminal ) {
	
	this->mCGT = &cgt;
	
	if ( errorTerminal ) {
		USCgtSymbol* symbol = this->mCGT->FindTerminal ( errorTerminal );
		if ( symbol ) {
			this->mErrorSymbolID = symbol->mID;
			this->mHandleErrors = true;
		}
	}
}

//----------------------------------------------------------------//
USSyntaxNode* USParser::Parse ( USStream& stream ) {

	USLexStream scanner;
	scanner.SetStream ( &stream );
	
	return this->Parse ( &scanner, false );
}

//----------------------------------------------------------------//
USSyntaxNode* USParser::Parse ( USLexStream* scanner, bool trimReductions ) {

	this->mLine = 0;

	USSyntaxNode* syntaxTree = 0;

	USDfaToken startToken;
	startToken.mLALRStateID = this->mCGT->mLALRInitialStateID;
	startToken.mSymbol = this->mCGT->mSymbolTable [ this->mCGT->mLALRStartSymbol ];
	this->mTokenStack.push_back ( startToken );
	
	this->mCurrentLALRStateID = this->mCGT->mLALRInitialStateID;
	
	USDfaToken token;
	this->RetrieveToken ( &token, scanner );
	
	bool done = false;
	u32 commentLevel = 0;
	while ( !done ) {
	
		if ( commentLevel ) {
			switch ( token.mSymbol.mKind ) {
				case USCgtSymbol::COMMENTEND: {
					commentLevel = 0;
					break;
				}
				case USCgtSymbol::COMMENTSTART: {
					++commentLevel;
					break;
				}
			}
			this->RetrieveToken ( &token, scanner ); // Get the next token
			continue;
		}
	
		switch ( token.mSymbol.mKind ) {
		
			case USCgtSymbol::COMMENTSTART: {
				++commentLevel;
				this->RetrieveToken ( &token, scanner ); // Get the next token
				break;
			}
			
			case USCgtSymbol::ERROR: {
				// This is a lexical error; we won't bother recovering from it
				// Just report it and die
				return 0;
				break;
			}
			
			case USCgtSymbol::COMMENTLINE: {
				
				u8 nextChar;
				while ( !scanner->IsAtEnd ()) {
					nextChar = scanner->Read < u8 >( 0 );
					if ( nextChar == '\n' ) break;
				}

				this->RetrieveToken ( &token, scanner ); // Get the next token
				break;
			}
			
			case USCgtSymbol::END:
			case USCgtSymbol::NONTERMINAL:
			case USCgtSymbol::TERMINAL: {
			
				switch ( this->ParseToken ( &token, trimReductions )) {
				
					case USLalrAction::ACCEPT: {
						syntaxTree = this->mTokenStack.back ().mSyntaxNode;
						this->mTokenStack.pop_back ();
						assert ( this->mTokenStack.size () == 1 ); // Should only be start token left...
						done = true;
						break;
					}
					
					case USLalrAction::ERROR: {
						return 0;
						break;
					}
					
					case USLalrAction::REDUCE: {
						break;
					}
					
					case USLalrAction::GOTO:
					case USLalrAction::SHIFT: {
						this->RetrieveToken ( &token, scanner ); // Get the next token
						break;
					}
				}
				break;
			}
			
			case USCgtSymbol::WHITESPACE: {
				
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
u32 USParser::ParseToken ( USDfaToken* token, bool trimReductions ) {

	USLalrState& state = this->mCGT->mLALRStateTable [ this->mCurrentLALRStateID ];

	for ( u32 i = 0; i < state.mActions.Size (); ++i ) {
		USLalrAction& action = state.mActions [ i ];
		
		if ( action.mInputSymbolID == token->mSymbol.mID ) {
			switch ( action.mActionType ) {
				
				case USLalrAction::ACCEPT: {
					return USLalrAction::ACCEPT;
				}
				
				case USLalrAction::GOTO: {
					token->mLALRStateID = state.mID;
					this->mTokenStack.push_back ( *token );
					this->mCurrentLALRStateID = action.mTarget;
					
					//printf ( "REDUCE: %d\n", this->mCurrentLALRStateID );
					
					return USLalrAction::GOTO;
				}
				
				case USLalrAction::SHIFT: {
					
					if ( token->mSymbol.mKind == USCgtSymbol::TERMINAL ) {
						// Lookup the factory method for this terminal and use it to create the USSyntaxNode
						token->mSyntaxNode = new USSyntaxNode ();
						token->mSyntaxNode->mID = token->mSymbol.mID;
						token->mSyntaxNode->mLine = token->mLine;
						token->mSyntaxNode->mName = token->mSymbol.mName;
						token->mSyntaxNode->mTerminal = token->mData;
					}
					
					token->mLALRStateID = state.mID;
					this->mTokenStack.push_back ( *token );
					this->mCurrentLALRStateID = action.mTarget;
					
					//printf ( "kSHIFT: %d\n", this->mCurrentLALRStateID );
					
					return USLalrAction::SHIFT;
				}
				
				case USLalrAction::REDUCE: {
					USCgtRule& rule = this->mCGT->mRuleTable [ action.mTarget ];
					USCgtSymbol& ruleResult = this->mCGT->mSymbolTable [ rule.mRuleResult ];
					//assert ( rule.mRuleSymbols.Size ()); // allow for empty rule...
					
					// Check to see if we should trim reductions
					bool doTrim = false;
					if ( rule.mRuleSymbols.Size () == 1 ) {
						USCgtSymbol& handle = this->mCGT->mSymbolTable [ rule.mRuleSymbols [ 0 ]];
						doTrim = (( handle.mKind == USCgtSymbol::NONTERMINAL ) && trimReductions );
					}
					
					// Create the head
					USDfaToken head;
					head.mSymbol = ruleResult;
					
					// Create the syntax node and initialize it...
					u32 handleSize = rule.mRuleSymbols.Size ();
					
					u16 prevStateID = this->mTokenStack.back ().mLALRStateID;
					
					if ( doTrim ) {
						
						head.mSyntaxNode = this->mTokenStack.back ().mSyntaxNode;
						this->mTokenStack.pop_back ();
					}
					else {
						head.mSyntaxNode = new USSyntaxNode;
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
							
							USSyntaxNode* eof = new USSyntaxNode;
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
					return USLalrAction::REDUCE;
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
				USLalrState& state2 = this->mCGT->mLALRStateTable [ this->mCurrentLALRStateID ];
				
				this->mTokenStack.pop_back ();
				
				for ( u32 i = 0; i < state2.mActions.Size (); ++i ) {
					USLalrAction& action = state2.mActions [ i ];
					if ( action.mInputSymbolID == token->mSymbol.mID ) {
						return USLalrAction::REDUCE;
					}
				}
			}
			return USLalrAction::REDUCE;
		}
		else {
			// Make the error token
			token->mData = "error";
			token->mSymbol.mKind = USCgtSymbol::TERMINAL;
			token->mSymbol.mID = this->mErrorSymbolID;
			return USLalrAction::REDUCE;
		}
	}
	
	return USLalrAction::ERROR;
}

//----------------------------------------------------------------//
void USParser::RetrieveToken ( USDfaToken* token, USLexStream* scanner ) {
	
	assert ( token );
	
	if ( scanner->IsAtEnd ()) {
		token->mSymbol.mID = 0; // This is supposed to be the ID for EOF in the symbol table for any GOLD CGT
		token->mSymbol.mKind = USCgtSymbol::END;
		token->mData = "EOF";
		return;
	}
	
	u16 stateID = this->mCGT->mDFAInitialStateID;
	USDfaState* dfaState = &this->mCGT->mDFAStateTable [ stateID ];
	
	USDfaState* acceptState = 0;
	u32 acceptLength = 0;
	
	bool done = false;
	
	u32 startCursor = scanner->GetCursor ();
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
			
			USDfaStateEdge& edge = dfaState->mEdges [ i ];
			
			assert ( edge.mCharSetID < this->mCGT->mCharSetTable.Size ());
			USCgtCharSet& charSet = this->mCGT->mCharSetTable [ edge.mCharSetID ];
			
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
					token->mSymbol.mKind = USCgtSymbol::TERMINAL;
					token->mSymbol.mID = this->mErrorSymbolID;
				}
				else {
					acceptLength = 1;
					token->mSymbol.mKind = USCgtSymbol::ERROR;
				}
			}
			
			scanner->Seek ( startCursor, SEEK_SET );
			token->mLine = scanner->GetLine ();
			
			if ( acceptLength < ( bufferSize - 1 )) {

				scanner->ReadBytes ( buffer, acceptLength );
				buffer [ acceptLength ] = 0;
				token->mData = buffer;
			}
			else {
				
				USLeanArray < char > bigBuffer;
				bigBuffer.Init ( acceptLength + 1 );
				scanner->ReadBytes ( bigBuffer, acceptLength );
				bigBuffer [ acceptLength ] = 0;
				token->mData = bigBuffer;
			}
			return;
		}
	}
}

//----------------------------------------------------------------//
USParser::USParser () :
	mHandleErrors ( false ),
	mErrorSymbolID ( 0 ) {
}

//----------------------------------------------------------------//
USParser::~USParser () {
}