#import <Foundation/Foundation.h>

#import "ObjectiveChipmunk.h"

@interface FallingButton : NSObject <ChipmunkObject> {
	UIButton *button;
	
	ChipmunkBody *body;
	NSSet *chipmunkObjects;
	
	int touchShapes;
}

@property (readonly) UIButton *button;
@property (readonly) NSSet *chipmunkObjects;
@property int touchedShapes;

- (void)updatePosition;

@end
