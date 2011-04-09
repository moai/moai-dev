/*================================================================//
// All contents Copyright 2007 by Patrick Meehan
//================================================================//
NOTES:
	This is a class that may be used to convert an abstract syntax
	tree generated from a GOLD CTG to the concrete syntax tree
	of a user-designed language.
//----------------------------------------------------------------//
CONTENTS:
//----------------------------------------------------------------//
USAGE:
//----------------------------------------------------------------//
REVISIONS:
//----------------------------------------------------------------*/

#ifndef	USSYNTAXTREESCANNER_H
#define	USSYNTAXTREESCANNER_H

class USSyntaxNode;

//================================================================//
// USSyntaxScanner
//================================================================//
class USSyntaxScanner {
protected:

	//----------------------------------------------------------------//
	virtual void	OnRule				( u32 ruleID, USSyntaxNode& ast ) = 0;
	virtual void	OnTerminal			( u32 terminalID, USSyntaxNode& ast ) = 0;

public:

	//----------------------------------------------------------------//
	void			ScanSyntax			( USSyntaxNode* ast );
					USSyntaxScanner		();
	virtual			~USSyntaxScanner	();
};

#endif
