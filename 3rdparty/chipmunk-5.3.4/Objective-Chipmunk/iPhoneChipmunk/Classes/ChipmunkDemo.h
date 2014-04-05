#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

#import "ObjectiveChipmunk.h"

#define GRABABLE_MASK_BIT (1<<31)
#define NOT_GRABABLE_MASK (~GRABABLE_MASK_BIT)

@class CADisplayLink;

@interface ChipmunkDemo : UIView {
@private
	cpVect mousePoint;
	cpVect mousePoint_last;
	ChipmunkBody *mouseBody;
	ChipmunkConstraint *mouseJoint;
	
	EAGLContext *context;
	GLuint viewRenderbuffer, viewFramebuffer;
	CADisplayLink *timer;
	
	int refTicks;
	double refTime;

@protected
	double timeStep;
	ChipmunkSpace *space;
}

@property BOOL paused;

- (void)setupSpace;
- (void)update;

@end

@interface Sensors : ChipmunkDemo {
	id emitter;
}
@end

@interface Planet : ChipmunkDemo; @end

@interface PyramidStack : ChipmunkDemo; @end

@interface Plink : ChipmunkDemo {
	NSMutableArray *pentagons;
}
@end

@interface Bounce : ChipmunkDemo; @end

@interface Joints : ChipmunkDemo; @end

@interface PyramidTopple : ChipmunkDemo; @end

@interface Springies : ChipmunkDemo; @end

@interface TheoJansen : ChipmunkDemo; @end

@interface Tumble : ChipmunkDemo {
	ChipmunkBody *box;
}
@end

@interface OneWay : ChipmunkDemo {
	id platform;
}
@end
