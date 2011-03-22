#import "ChipmunkDemo.h"
#import "Accelerometer.h"

@implementation TheoJansen

static const NSString *group = @"group";

-(void)update
{
	space.gravity = cpvmult([Accelerometer getAcceleration], 500);
	[super update];
//	cpFloat coef = (2.0f + arrowDirection.y)/3.0f;
//	cpFloat rate = arrowDirection.x*10.0f*coef;
//	cpSimpleMotorSetRate(motor, rate);
//	motor->maxForce = (rate) ? 100000.0f : 0.0f;
//	
//	int steps = 3;
//	cpFloat dt = 1.0/60.0/(cpFloat)steps;
//	
//	for(int i=0; i<steps; i++){
//		cpSpaceStep(space, dt);
//	}
}

static cpFloat seg_radius = 3.0;

- (void)makeLegForSide:(cpFloat)side offset:(cpFloat)offset chassis:(ChipmunkBody *)chassis crank:(ChipmunkBody *)crank anchor:(cpVect)anchor
{
	cpVect a, b;
	ChipmunkShape *shape;
	
	cpFloat leg_mass = 1.0;

	// make leg
	a = cpvzero, b = cpv(0.0, side);
	ChipmunkBody *upper_leg = [space add:[ChipmunkBody bodyWithMass:leg_mass andMoment:cpMomentForSegment(leg_mass, a, b)]];
	upper_leg.pos = cpv(offset, 0.0);
	[space add:[ChipmunkSegmentShape segmentWithBody:upper_leg from:a to:b radius:seg_radius]];
	[space add:[ChipmunkPivotJoint pivotJointWithBodyA:chassis bodyB:upper_leg anchr1:cpv(offset, 0.0) anchr2:cpvzero]];
	
	// lower leg
	a = cpvzero, b = cpv(0.0, -1.0*side);
	ChipmunkBody *lower_leg = [space add:[ChipmunkBody bodyWithMass:leg_mass andMoment:cpMomentForSegment(leg_mass, a, b)]];
	lower_leg.pos = cpv(offset, -side);
	shape = [space add:[ChipmunkSegmentShape segmentWithBody:lower_leg from:a to:b radius:seg_radius]];
	shape.group = group;
	shape = [space add:[ChipmunkCircleShape circleWithBody:lower_leg radius:seg_radius*2.0 offset:b]];
	shape.group = group;
	shape.elasticity = 0.0; shape.friction = 1.0;
	[space add:[ChipmunkPinJoint pinJointWithBodyA:chassis bodyB:lower_leg anchr1:cpv(offset, 0.0) anchr2:cpvzero]];
	
	[space add:[ChipmunkGearJoint gearJointWithBodyA:upper_leg bodyB:lower_leg phase:0.0 ratio:1.0]];
	
	const cpFloat diag = sqrtf(side*side + offset*offset);
	
	ChipmunkPinJoint *pin;
	pin = [space add:[ChipmunkPinJoint pinJointWithBodyA:crank bodyB:upper_leg anchr1:anchor anchr2:cpv(0.0, side)]];
	pin.dist = diag;
	pin = [space add:[ChipmunkPinJoint pinJointWithBodyA:crank bodyB:lower_leg anchr1:anchor anchr2:cpvzero]];
	pin.dist = diag;
}

- (void)setupSpace
{
	space = [[ChipmunkSpace alloc] init];
	[space addBounds:CGRectMake(-320.0, -240.0, 640.0, 480.0) thickness:10.0 elasticity:0.0 friction:1.0 layers:NOT_GRABABLE_MASK group:nil collisionType:nil];
	
	ChipmunkShape *shape;
	cpVect a, b;
	
	
	cpFloat offset = 30.0f;

	// make chassis
	cpFloat chassis_mass = 2.0f;
	a = cpv(-offset, 0.0f), b = cpv(offset, 0.0f);
	ChipmunkBody *chassis = [space add:[ChipmunkBody bodyWithMass:chassis_mass andMoment:cpMomentForSegment(chassis_mass, a, b)]];
	shape = [space add:[ChipmunkSegmentShape segmentWithBody:chassis from:a to:b radius:seg_radius]];
	shape.group = group;
	
	// make crank
	cpFloat crank_mass = 1.0f;
	cpFloat crank_radius = 13.0f;
	ChipmunkBody *crank = [space add:[ChipmunkBody bodyWithMass:crank_mass andMoment:cpMomentForCircle(crank_mass, 0.0, crank_radius, cpvzero)]];
	shape = [space add:[ChipmunkCircleShape circleWithBody:crank radius:crank_radius offset:cpvzero]];
	shape.group = group;
	[space add:[ChipmunkPivotJoint pivotJointWithBodyA:chassis bodyB:crank anchr1:cpvzero anchr2:cpvzero]];
	
	cpFloat side = 30.0f;
	
	int num_legs = 2;
	for(int i=0; i<num_legs; i++){
		[self makeLegForSide:side offset: offset chassis:chassis crank:crank anchor:cpvmult(cpvforangle((cpFloat)(2*i+0)/(cpFloat)num_legs*M_PI), crank_radius)];
		[self makeLegForSide:side offset:-offset chassis:chassis crank:crank anchor:cpvmult(cpvforangle((cpFloat)(2*i+1)/(cpFloat)num_legs*M_PI), crank_radius)];
	}
}

@end
