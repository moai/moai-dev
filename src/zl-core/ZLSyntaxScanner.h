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

#ifndef	ZLSYNTAXTREESCANNER_H
#define	ZLSYNTAXTREESCANNER_H

class ZLSyntaxNode;

//================================================================//
// ZLSyntaxScanner
//================================================================//
class ZLSyntaxScanner {
protected:

	//----------------------------------------------------------------//
	virtual void	OnRule				( u32 ruleID, ZLSyntaxNode& ast ) = 0;
	virtual void	OnTerminal			( u32 terminalID, ZLSyntaxNode& ast ) = 0;

public:

	//----------------------------------------------------------------//
	void			ScanSyntax			( ZLSyntaxNode* ast );
					ZLSyntaxScanner		();
	virtual			~ZLSyntaxScanner	();
};

#endif
