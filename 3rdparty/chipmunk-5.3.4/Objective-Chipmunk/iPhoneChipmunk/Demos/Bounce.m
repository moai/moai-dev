#import "ChipmunkDemo.h"
#import "Accelerometer.h"

@implementation Bounce

static cpFloat frand(void){return (cpFloat)rand()/(cpFloat)RAND_MAX;}

- (void)setupSpace
{
	space = [[ChipmunkSpace alloc] init];
	
	[space addBounds:CGRectMake(-320.0, -240.0, 640.0, 480.0) thickness:10.0 elasticity:1.0 friction:0.0 layers:NOT_GRABABLE_MASK group:nil collisionType:nil];
	
	for(int i=0; i<50; i++){
		const cpFloat size = 20.0f;
		const cpFloat mass = 1.0f;
		
		ChipmunkBody *body = [space add:[ChipmunkBody bodyWithMass:mass andMoment:cpMomentForBox(mass, size, size)]];
		body.pos = cpv(frand()*(640 - 2*size) - (320 - size), frand()*(480 - 2*size) - (240 - size));
		body.vel = cpvmult(cpv(2*frand() - 1, 2*frand() - 1), 200);
		
		ChipmunkShape *shape = [space add:[ChipmunkPolyShape boxWithBody:body width:size height:size]];
		shape.elasticity = 1.0f; shape.friction = 0.0f;
	}
	
	ChipmunkBody *body = [space add:[ChipmunkBody bodyWithMass:100.0 andMoment:10000.0]];

	ChipmunkShape *shape = [space add:[ChipmunkSegmentShape segmentWithBody:body from:cpv(-75.0, 0.0) to:cpv(75.0, 0.0) radius:5.0]];
	shape.elasticity = 1.0; shape.friction = 0.0;
	
	[space add:[ChipmunkPivotJoint pivotJointWithBodyA:body bodyB:[ChipmunkBody staticBody] pivot:cpvzero]];
}

@end