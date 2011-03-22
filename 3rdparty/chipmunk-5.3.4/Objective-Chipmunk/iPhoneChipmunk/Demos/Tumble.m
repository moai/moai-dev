#import "ChipmunkDemo.h"
#import "Accelerometer.h"

@implementation Tumble

-(void)update
{
	space.gravity = cpvmult([Accelerometer getAcceleration], 600);
	cpBodyUpdatePosition(box.body, timeStep);
	
	[super update];
}

- (void)setupSpace
{
	space = [[ChipmunkSpace alloc] init];
	[space resizeActiveHashWithDim:30.0 andCount:1000];
	[space resizeActiveHashWithDim:200.0 andCount:100];
	space.gravity = cpv(0.0, -600.0);
	
	ChipmunkBody *body;
	ChipmunkShape *shape;
	
	box = [ChipmunkBody bodyWithMass:INFINITY andMoment:INFINITY];
	box.angVel = 0.4;
	
	// Set up the static box.
	cpVect a = cpv(-200, -200);
	cpVect b = cpv(-200,  200);
	cpVect c = cpv( 200,  200);
	cpVect d = cpv( 200, -200);
	
	shape = [space add:[ChipmunkSegmentShape segmentWithBody:box from:a to:b radius:0.0]];
	shape.elasticity = 1.0; shape.friction = 1.0;
	shape.layers = NOT_GRABABLE_MASK;

	shape = [space add:[ChipmunkSegmentShape segmentWithBody:box from:b to:c radius:0.0]];
	shape.elasticity = 1.0; shape.friction = 1.0;
	shape.layers = NOT_GRABABLE_MASK;

	shape = [space add:[ChipmunkSegmentShape segmentWithBody:box from:c to:d radius:0.0]];
	shape.elasticity = 1.0; shape.friction = 1.0;
	shape.layers = NOT_GRABABLE_MASK;

	shape = [space add:[ChipmunkSegmentShape segmentWithBody:box from:d to:a radius:0.0]];
	shape.elasticity = 1.0; shape.friction = 1.0;
	shape.layers = NOT_GRABABLE_MASK;
	
	// Add the bricks.
	for(int i=0; i<3; i++){
		for(int j=0; j<7; j++){
			cpFloat width = 60.0;
			cpFloat height = 30.0;
			
			body = [space add:[ChipmunkBody bodyWithMass:1.0 andMoment:cpMomentForBox(1.0, width, height)]];
			body.pos = cpv(i*61 - 150, j*31 - 150);
			shape = [space add:[ChipmunkPolyShape boxWithBody:body width:width height:height]];
			shape.elasticity = 0.0; shape.friction = 1.0;
		}
	}
}

@end
