#import "ChipmunkDemo.h"
#import "Accelerometer.h"

@implementation PyramidStack

- (void)update
{
	space.gravity = cpvmult([Accelerometer getAcceleration], 200);
	[super update];
}

- (void)setupSpace
{
	space = [[ChipmunkSpace alloc] init];
	[space resizeStaticHashWithDim:40.0 andCount:1000];
	[space resizeActiveHashWithDim:40.0 andCount:1000];
	space.gravity = cpv(0, -100);
	
	ChipmunkBody *body;
	ChipmunkShape *shape;
	
	[space addBounds:CGRectMake(-320.0, -240.0, 640.0, 1000.0) thickness:20.0 elasticity:1.0 friction:1.0 layers:NOT_GRABABLE_MASK group:nil collisionType:nil];
	
	cpFloat boxSize = 40.0;
	
	// Add lots of boxes.
	for(int i=0; i<10; i++){
		for(int j=0; j<=i; j++){
			body = [space add:[ChipmunkBody bodyWithMass:1.0 andMoment:cpMomentForBox(1.0, boxSize, boxSize)]];
			body.pos = cpv(j*42 - i*21, 300 - i*42);
			shape = [space add:[ChipmunkPolyShape boxWithBody:body width:boxSize height:boxSize]];
			shape.elasticity = 0.0; shape.friction = 0.8;
		}
	}
	
	// Add a ball to make things more interesting
	cpFloat radius = 20.0;
	body = [space add:[ChipmunkBody bodyWithMass:10.0 andMoment:cpMomentForCircle(10.0, 0.0, radius, cpvzero)]];
	body.pos = cpv(0, -240 + radius);

	shape = [space add:[ChipmunkCircleShape circleWithBody:body radius:radius offset:cpvzero]];
	shape.elasticity = 0.0; shape.friction = 0.9;
}

@end