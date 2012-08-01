#import <UIKit/UIKit.h>

#import "ObjectiveChipmunk.h"

#define FRICTION_AMOUNT 0.4

typedef enum GamePieceSize {
	GamePieceSmall,
	GamePieceMed,
	GamePieceLarge,
} GamePieceSize;

@interface GamePiece : UIImageView <ChipmunkObject> {
	int points;
	
	ChipmunkBody *body;
	NSSet *chipmunkObjects;
}

@property (readonly) NSSet *chipmunkObjects;
@property (readonly) ChipmunkBody *body;
@property (readonly) int points;

+ (id)pieceWithSize:(GamePieceSize)size staticBody:(ChipmunkBody *)staticBody;

@end
