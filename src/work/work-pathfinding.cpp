#include <moaicore/moaicore.h>

//================================================================//
// MOAIPathNode
//================================================================//
class MOAIPathNode {
public:

	bool	mIsOpen;
};

//================================================================//
// MOAIPathState
//================================================================//
class MOAIPathState {
public:

	int				mAddr;
	MOAIPathState*	mParent;
	MOAIPathState*	mNext;
	
	float			mMoveCost;
	float			mHeuristic;
	
	MOAICellCoord	mCoord;
	
	GET ( float, Score, ( this->mMoveCost + this->mHeuristic ))
	
	//----------------------------------------------------------------//
	//bool operator < ( const MOAITextSprite& rhs ) const {
	//
	//	return this->mAddr < rhs.mAddr;
	//}
};

//================================================================//
// MOAIPathFinder
//================================================================//
class MOAIPathFinder {
public:

	MOAIPathState*		mOpen;
	MOAIPathState*		mClosed;
	MOAIPathState*		mPath;
	
	int					mTargetAddr;
	MOAICellCoord		mTargetCoord;
	
	MOAIGridSpace					mGridSpace;
	USLeanArray < MOAIPathNode >	mNodes;
	
	//----------------------------------------------------------------//
	void BuildPath ( MOAIPathState* step ) {
	
		this->mPath = step;
	}
	
	//----------------------------------------------------------------//
	void CloseStep ( MOAIPathState* stepToClose ) {
		
		MOAIPathState* cursor = this->mOpen;
		this->mOpen = 0;
		
		while ( cursor ) {
			MOAIPathState* step = cursor;
			cursor = cursor->mNext;
			
			if ( step == stepToClose ) {
				step->mNext = this->mClosed;
				this->mClosed = step;
			}
			else {
				step->mNext = this->mOpen;
				this->mOpen = step;
			}
		}
	}
	
	//----------------------------------------------------------------//
	float ComputeHeuristic ( MOAIPathState* step ) {
	
		// manhattan
		return ( float )(( this->mTargetCoord.mX - step->mCoord.mX ) + ( this->mTargetCoord.mY - step->mCoord.mY ));
	}
	
	//----------------------------------------------------------------//
	float ComputeMoveCost ( MOAIPathState* parent, MOAIPathState* step, bool diagonal ) {
	
		if ( parent ) {
			return diagonal ? 1.4f : 1.0f;
		}
		return 0.0f;
	}
	
	//----------------------------------------------------------------//
	MOAIPathState* FindBestStep () {

		MOAIPathState* step = this->mOpen;
		MOAIPathState* best = step;
		step = step->mNext;

		for ( ; step; step = step->mNext ) {
			if ( step->GetScore () < best->GetScore ()) {
				best = step;
			}
		}
		return best;
	}
	
	//----------------------------------------------------------------//
	bool IsCandidate ( int addr ) {
		
		if ( this->mNodes [ addr ].mIsOpen ) {
		
			for ( MOAIPathState* step = this->mOpen; step; step = step->mNext ) {
				if ( step->mAddr == addr ) return false;
			}
			
			for ( MOAIPathState* step = this->mClosed; step; step = step->mNext ) {
				if ( step->mAddr == addr ) return false;
			}
			
			return true;
		}
		return false;
	}
	
	//----------------------------------------------------------------//
	bool IsClosed ( int addr ) {
			
		for ( MOAIPathState* step = this->mClosed; step; step = step->mNext ) {
			if ( step->mAddr == addr ) return true;
		}
		return false;
	}
	
	//----------------------------------------------------------------//
	bool IsInPath ( int addr ) {
	
		for ( MOAIPathState* step = this->mPath; step; step = step->mParent ) {
			if ( step->mAddr == addr ) return true;
		}
		return false;
	}
	
	//----------------------------------------------------------------//
	bool IsOpen ( int addr ) {
			
		for ( MOAIPathState* step = this->mOpen; step; step = step->mNext ) {
			if ( step->mAddr == addr ) return true;
		}
		return false;
	}
	
	//----------------------------------------------------------------//
	void Print () {
	
		for ( int y = 0; y < this->mGridSpace.GetHeight (); ++y ) {
			for ( int x = 0; x < this->mGridSpace.GetWidth (); ++x ) {
			
				int addr = this->mGridSpace.GetCellAddr ( this->mGridSpace.GetCellCoord ( x, y ));
				
				if ( this->IsInPath ( addr )) {
					printf ( "##" );
				}
				else if ( this->IsOpen ( addr )) {
					printf ( "==" );
				}
				else if ( this->IsClosed ( addr )) {
					printf ( "--" );
				}
				else {
				
					MOAIPathNode& node = this->mNodes [ addr ];
				
					if ( node.mIsOpen ) {
						printf ( ". " );
					}
					else {
						printf ( "[]" );
					}
				}
			}
			printf ( "\n" );
		}
	}
	
	//----------------------------------------------------------------//
	void Push ( int xTile, int yTile, MOAIPathState* parent, bool diagonal ) {
	
		MOAICellCoord coord = this->mGridSpace.GetCellCoord ( xTile, yTile );
	
		if ( this->mGridSpace.IsValidCoord ( coord )) {
			
			int addr = this->mGridSpace.GetCellAddr ( coord );
			
			if ( this->IsCandidate ( addr )) {
				
				MOAIPathState* step = new MOAIPathState ();
				step->mAddr = addr;
				step->mParent = parent;
				step->mCoord = coord;
				
				step->mNext = this->mOpen;
				this->mOpen = step;
				
				step->mMoveCost = this->ComputeMoveCost ( parent, step, diagonal );
				step->mHeuristic = this->ComputeHeuristic ( step );
			}
		}
	}
	
	//----------------------------------------------------------------//
	void PushNeighbors ( MOAIPathState* step ) {
		
		int xTile = step->mCoord.mX;
		int yTile = step->mCoord.mY;
		
		this->Push ( xTile - 1, yTile, step, false );
		this->Push ( xTile + 1, yTile, step, false );
		this->Push ( xTile, yTile + 1, step, false );
		this->Push ( xTile, yTile - 1, step, false );
		
		this->Push ( xTile - 1, yTile - 1, step, true );
		this->Push ( xTile - 1, yTile + 1, step, true );
		this->Push ( xTile + 1, yTile - 1, step, true );
		this->Push ( xTile + 1, yTile + 1, step, true );
	}
	
	//----------------------------------------------------------------//
	void Test () {
		
		this->mOpen = 0;
		this->mClosed = 0;
		this->mPath = 0;
		
		this->mGridSpace.Init ( 8, 8, 32.0f, 32.0f );
		
		MOAIPathNode node;
		node.mIsOpen = true;
		
		this->mNodes.Init ( this->mGridSpace.GetTotalCells ());
		this->mNodes.Fill ( node );
		
		this->mNodes [ this->mGridSpace.GetCellAddr ( this->mGridSpace.GetCellCoord ( 3, 0 ))].mIsOpen = false;
		this->mNodes [ this->mGridSpace.GetCellAddr ( this->mGridSpace.GetCellCoord ( 3, 1 ))].mIsOpen = false;
		this->mNodes [ this->mGridSpace.GetCellAddr ( this->mGridSpace.GetCellCoord ( 3, 2 ))].mIsOpen = false;
		this->mNodes [ this->mGridSpace.GetCellAddr ( this->mGridSpace.GetCellCoord ( 3, 3 ))].mIsOpen = false;
		this->mNodes [ this->mGridSpace.GetCellAddr ( this->mGridSpace.GetCellCoord ( 3, 4 ))].mIsOpen = false;
		this->mNodes [ this->mGridSpace.GetCellAddr ( this->mGridSpace.GetCellCoord ( 3, 5 ))].mIsOpen = false;
		
		this->mNodes [ this->mGridSpace.GetCellAddr ( this->mGridSpace.GetCellCoord ( 5, 2 ))].mIsOpen = false;
		this->mNodes [ this->mGridSpace.GetCellAddr ( this->mGridSpace.GetCellCoord ( 5, 3 ))].mIsOpen = false;
		this->mNodes [ this->mGridSpace.GetCellAddr ( this->mGridSpace.GetCellCoord ( 5, 4 ))].mIsOpen = false;
		this->mNodes [ this->mGridSpace.GetCellAddr ( this->mGridSpace.GetCellCoord ( 5, 5 ))].mIsOpen = false;
		this->mNodes [ this->mGridSpace.GetCellAddr ( this->mGridSpace.GetCellCoord ( 5, 6 ))].mIsOpen = false;
		this->mNodes [ this->mGridSpace.GetCellAddr ( this->mGridSpace.GetCellCoord ( 5, 7 ))].mIsOpen = false;
		
		this->mTargetCoord = this->mGridSpace.GetCellCoord ( 7, 7 );
		this->mTargetAddr = this->mGridSpace.GetCellAddr ( this->mTargetCoord );
		
		this->Push ( 0, 0, 0, false );
		
		while ( this->mOpen ) {
			
			MOAIPathState* step = this->FindBestStep ();
			this->CloseStep ( step );
			
			if ( step->mAddr == this->mTargetAddr ) {
				this->BuildPath ( step );
				break;
			}
			this->PushNeighbors ( step );
		}
		this->Print ();
	}
};

//----------------------------------------------------------------//
int work_pathfinding ( int argc, char **argv ) {
	
	MOAIPathFinder pathFinder;
	pathFinder.Test ();
	
	return 0;
}
