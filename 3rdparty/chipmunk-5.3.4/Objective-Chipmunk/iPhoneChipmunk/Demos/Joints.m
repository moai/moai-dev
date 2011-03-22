#import "ChipmunkDemo.h"
#import "Accelerometer.h"

static const NSString *car = @"car";

@implementation Joints

static cpFloat frand(void){return (cpFloat)rand()/(cpFloat)RAND_MAX;}

- (ChipmunkBody *)addBall:(cpVect)pos offset:(cpVect)boxOffset
{
	cpFloat radius = 15.0f;
	cpFloat mass = 1.0f;
	
	ChipmunkBody *body = [space add:[ChipmunkBody bodyWithMass:mass andMoment:cpMomentForCircle(mass, 0.0, radius, cpvzero)]];
	body.pos = cpvadd(pos, boxOffset);
	
	ChipmunkShape *shape = [space add:[ChipmunkCircleShape circleWithBody:body radius:radius offset:cpvzero]];
	shape.elasticity = 0.0f; shape.friction = 0.7f;
	
	return body;
}

- (ChipmunkBody *)addLever:(cpVect)pos offset:(cpVect)boxOffset
{
	cpFloat mass = 1.0f;
	cpVect a = cpv(0,  15);
	cpVect b = cpv(0, -15);
	
	ChipmunkBody *body = [space add:[ChipmunkBody bodyWithMass:mass andMoment:cpMomentForSegment(mass, a, b)]];
	body.pos = cpvadd(pos, cpvadd(boxOffset, cpv(0, -15)));
	
	ChipmunkShape *shape = [space add:[ChipmunkSegmentShape segmentWithBody:body from:a to:b radius:10.0]];
	shape.elasticity = 0.0f; shape.friction = 0.7f;
	
	return body;
}

- (ChipmunkBody *)addBar:(cpVect)pos offset:(cpVect)boxOffset
{
	cpFloat mass = 2.0f;
	cpVect a = cpv(0,  30);
	cpVect b = cpv(0, -30);
	
	ChipmunkBody *body = [space add:[ChipmunkBody bodyWithMass:mass andMoment:cpMomentForSegment(mass, a, b)]];
	body.pos = cpvadd(pos, boxOffset);
	
	ChipmunkShape *shape = [space add:[ChipmunkSegmentShape segmentWithBody:body from:a to:b radius:10.0]];
	shape.elasticity = 0.0f; shape.friction = 0.7f;
	
	return body;
}

- (ChipmunkBody *)addWheel:(cpVect)pos offset:(cpVect)boxOffset
{
	cpFloat radius = 15.0f;
	cpFloat mass = 1.0f;
	ChipmunkBody *body = [space add:[ChipmunkBody bodyWithMass:mass andMoment:cpMomentForCircle(mass, 0.0, radius, cpvzero)]];
	body.pos = cpvadd(pos, boxOffset);
	
	ChipmunkShape *shape = [space add:[ChipmunkCircleShape circleWithBody:body radius:radius offset:cpvzero]];
	shape.elasticity = 0.0f; shape.friction = 0.7f;
	shape.group = car; // use a group to keep the car parts from colliding
	
	return body;
}

- (ChipmunkBody *)addChassis:(cpVect)pos offset:(cpVect)boxOffset
{
	const cpFloat width = 80.0;
	const cpFloat height = 30.0;
	
	cpFloat mass = 5.0f;
	ChipmunkBody *body = [space add:[ChipmunkBody bodyWithMass:mass andMoment:cpMomentForBox(mass, width, height)]];
	body.pos = cpvadd(pos, boxOffset);
	
	ChipmunkShape *shape = [space add:[ChipmunkPolyShape boxWithBody:body width:width height:height]];
	shape.elasticity = 0.0f; shape.friction = 0.7f;
	shape.group = car; // use a group to keep the car parts from colliding
	
	return body;
}

- (void)setupSpace
{
	space = [[ChipmunkSpace alloc] init];
	space.iterations = 10;
	space.gravity = cpv(0, -100);
	
	ChipmunkBody *staticBody = space.staticBody;
	ChipmunkShape *shape;
	
	shape = [space add:[ChipmunkStaticSegmentShape segmentWithBody:staticBody from:cpv(-320,240) to:cpv(320,240) radius:0.0]];
	shape.elasticity = 1.0f; shape.friction = 1.0f;
	shape.layers = NOT_GRABABLE_MASK;
	
	shape = [space add:[ChipmunkStaticSegmentShape segmentWithBody:staticBody from:cpv(-320,120) to:cpv(320,120) radius:0.0]];
	shape.elasticity = 1.0f; shape.friction = 1.0f;
	shape.layers = NOT_GRABABLE_MASK;
	
	shape = [space add:[ChipmunkStaticSegmentShape segmentWithBody:staticBody from:cpv(-320,0) to:cpv(320,0) radius:0.0]];
	shape.elasticity = 1.0f; shape.friction = 1.0f;
	shape.layers = NOT_GRABABLE_MASK;
	
	shape = [space add:[ChipmunkStaticSegmentShape segmentWithBody:staticBody from:cpv(-320,-120) to:cpv(320,-120) radius:0.0]];
	shape.elasticity = 1.0f; shape.friction = 1.0f;
	shape.layers = NOT_GRABABLE_MASK;
	
	shape = [space add:[ChipmunkStaticSegmentShape segmentWithBody:staticBody from:cpv(-320,-240) to:cpv(320,-240) radius:0.0]];
	shape.elasticity = 1.0f; shape.friction = 1.0f;
	shape.layers = NOT_GRABABLE_MASK;
	
	
	shape = [space add:[ChipmunkStaticSegmentShape segmentWithBody:staticBody from:cpv(-320,-240) to:cpv(-320,240) radius:0.0]];
	shape.elasticity = 1.0f; shape.friction = 1.0f;
	shape.layers = NOT_GRABABLE_MASK;
	
	shape = [space add:[ChipmunkStaticSegmentShape segmentWithBody:staticBody from:cpv(-160,-240) to:cpv(-160,240) radius:0.0]];
	shape.elasticity = 1.0f; shape.friction = 1.0f;
	shape.layers = NOT_GRABABLE_MASK;
	
	shape = [space add:[ChipmunkStaticSegmentShape segmentWithBody:staticBody from:cpv(0,-240) to:cpv(0,240) radius:0.0]];
	shape.elasticity = 1.0f; shape.friction = 1.0f;
	shape.layers = NOT_GRABABLE_MASK;
	
	shape = [space add:[ChipmunkStaticSegmentShape segmentWithBody:staticBody from:cpv(160,-240) to:cpv(160,240) radius:0.0]];
	shape.elasticity = 1.0f; shape.friction = 1.0f;
	shape.layers = NOT_GRABABLE_MASK;
	
	shape = [space add:[ChipmunkStaticSegmentShape segmentWithBody:staticBody from:cpv(320,-240) to:cpv(320,240) radius:0.0]];
	shape.elasticity = 1.0f; shape.friction = 1.0f;
	shape.layers = NOT_GRABABLE_MASK;
	
	
	cpVect boxOffset;
	ChipmunkBody *body1, *body2;
	
	cpVect posA = cpv( 50, 60);
	cpVect posB = cpv(110, 60);
	
	#define POS_A cpvadd(boxOffset, posA)
	#define POS_B cpvadd(boxOffset, posB)
	
	// Pin Joints - Link shapes with a solid bar or pin.
	// Keeps the anchor points the same distance apart from when the joint was created.
	boxOffset = cpv(-320, -240);
	body1 = [self addBall:posA offset:boxOffset];
	body2 = [self addBall:posB offset:boxOffset];
	[space add:[ChipmunkPinJoint pinJointWithBodyA:body1 bodyB:body2 anchr1:cpv(15.0, 0.0) anchr2:cpv(-15.0, 0.0)]];
	
//	// Slide Joints - Like pin joints but with a min/max distance.
//	// Can be used for a cheap approximation of a rope.
	boxOffset = cpv(-160, -240);
	body1 = [self addBall:posA offset:boxOffset];
	body2 = [self addBall:posB offset:boxOffset];
	[space add:[ChipmunkSlideJoint slideJointWithBodyA:body1 bodyB:body2 anchr1:cpv(15,0) anchr2:cpv(-15,0) min:20.0 max:40.0]];
	
	// Pivot Joints - Holds the two anchor points together. Like a swivel.
	boxOffset = cpv(0, -240);
	body1 = [self addBall:posA offset:boxOffset];
	body2 = [self addBall:posB offset:boxOffset];
	[space add:[ChipmunkPivotJoint pivotJointWithBodyA:body1 bodyB:body2 pivot:cpvadd(boxOffset, cpv(80,60))]];
	// cpPivotJointNew() takes it's anchor parameter in world coordinates. The anchors are calculated from that
	// cpPivotJointNew2() lets you specify the two anchor points explicitly
	
	// Groove Joints - Like a pivot joint, but one of the anchors is a line segment that the pivot can slide in
	boxOffset = cpv(160, -240);
	body1 = [self addBall:posA offset:boxOffset];
	body2 = [self addBall:posB offset:boxOffset];
	[space add:[ChipmunkGrooveJoint grooveJointWithBodyA:body1 bodyB:body2 groove_a:cpv(30,30) groove_b:cpv(30,-30) anchr2:cpv(-30,0)]];
	
	// Damped Springs
	boxOffset = cpv(-320, -120);
	body1 = [self addBall:posA offset:boxOffset];
	body2 = [self addBall:posB offset:boxOffset];
	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA:body1 bodyB:body2 anchr1:cpv(15,0) anchr2:cpv(-15,0) restLength:20.0 stiffness:5.0 damping:0.3]];
	
	// Damped Rotary Springs
	boxOffset = cpv(-160, -120);
	body1 = [self addBar:posA offset:boxOffset];
	body2 = [self addBar:posB offset:boxOffset];
	// Add some pin joints to hold the circles in place.
	[space add:[ChipmunkPivotJoint pivotJointWithBodyA:body1 bodyB:staticBody pivot:POS_A]];
	[space add:[ChipmunkPivotJoint pivotJointWithBodyA:body2 bodyB:staticBody pivot:POS_B]];
	[space add:[ChipmunkDampedRotarySpring dampedRotarySpringWithBodyA:body1 bodyB:body2 restAngle:0.0 stiffness:3000.0 damping:60.0]];
	
	// Rotary Limit Joint
	boxOffset = cpv(0, -120);
	body1 = [self addLever:posA offset:boxOffset];
	body2 = [self addLever:posB offset:boxOffset];
	// Add some pin joints to hold the circles in place.
	[space add:[ChipmunkPivotJoint pivotJointWithBodyA:body1 bodyB:staticBody pivot:POS_A]];
	[space add:[ChipmunkPivotJoint pivotJointWithBodyA:body2 bodyB:staticBody pivot:POS_B]];
	// Hold their rotation within 90 degrees of each other.
	[space add:[ChipmunkRotaryLimitJoint rotaryLimitJointWithBodyA:body1 bodyB:body2 min:-M_PI_2 max:M_PI_2]];
	
	// Ratchet Joint - A rotary ratchet, like a socket wrench
	boxOffset = cpv(160, -120);
	body1 = [self addLever:posA offset:boxOffset];
	body2 = [self addLever:posB offset:boxOffset];
	// Add some pin joints to hold the circles in place.
	[space add:[ChipmunkPivotJoint pivotJointWithBodyA:body1 bodyB:staticBody pivot:POS_A]];
	[space add:[ChipmunkPivotJoint pivotJointWithBodyA:body2 bodyB:staticBody pivot:POS_B]];
	// Ratchet every 90 degrees
	[space add:[ChipmunkRatchetJoint ratchetJointWithBodyA:body1 bodyB:body2 phase:0.0 ratchet:M_PI_2]];
	
	// Gear Joint - Maintain a specific angular velocity ratio
	boxOffset = cpv(-320, 0);
	body1 = [self addBar:posA offset:boxOffset];
	body2 = [self addBar:posB offset:boxOffset];
	// Add some pin joints to hold the circles in place.
	[space add:[ChipmunkPivotJoint pivotJointWithBodyA:body1 bodyB:staticBody pivot:POS_A]];
	[space add:[ChipmunkPivotJoint pivotJointWithBodyA:body2 bodyB:staticBody pivot:POS_B]];
	// Force one to sping 2x as fast as the other
	[space add:[ChipmunkGearJoint gearJointWithBodyA:body1 bodyB:body2 phase:0.0 ratio:2.0]];
	
	// Simple Motor - Maintain a specific angular relative velocity
	boxOffset = cpv(-160, 0);
	body1 = [self addBar:posA offset:boxOffset];
	body2 = [self addBar:posB offset:boxOffset];
	// Add some pin joints to hold the circles in place.
	[space add:[ChipmunkPivotJoint pivotJointWithBodyA:body1 bodyB:staticBody pivot:POS_A]];
	[space add:[ChipmunkPivotJoint pivotJointWithBodyA:body2 bodyB:staticBody pivot:POS_B]];
	// Make them spin at 1/2 revolution per second in relation to each other.
	[space add:[ChipmunkSimpleMotor simpleMotorWithBodyA:body1 bodyB:body2 rate:M_PI]];
	
	// Make a car with some nice soft suspension
	boxOffset = cpv(0, 0);
	ChipmunkBody *wheel1 = [self addWheel:posA offset:boxOffset];
	ChipmunkBody *wheel2 = [self addWheel:posB offset:boxOffset];
	ChipmunkBody *chassis = [self addChassis:cpv(80, 100) offset:boxOffset];
	
	[space add:[ChipmunkGrooveJoint grooveJointWithBodyA:chassis bodyB:wheel1 groove_a:cpv(-30, -10) groove_b:cpv(-30, -40) anchr2:cpvzero]];
	[space add:[ChipmunkGrooveJoint grooveJointWithBodyA:chassis bodyB:wheel2 groove_a:cpv( 30, -10) groove_b:cpv( 30, -40) anchr2:cpvzero]];
	
	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA:chassis bodyB:wheel1 anchr1:cpv(-30, 0) anchr2:cpvzero restLength:50.0 stiffness:20.0 damping:1.5]];
	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA:chassis bodyB:wheel2 anchr1:cpv( 30, 0) anchr2:cpvzero restLength:50.0 stiffness:20.0 damping:1.5]];
}

@end
