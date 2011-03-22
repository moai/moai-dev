#import "ChipmunkDemo.h"
#import "Accelerometer.h"

@implementation Springies

- (void)update
{
	space.gravity = cpvmult([Accelerometer getAcceleration], 20000.0);
	[super update];
}

- (ChipmunkBody *)addBar:(cpVect)a to:(cpVect)b group:(int)group
{
	cpVect center = cpvmult(cpvadd(a, b), 1.0f/2.0f);
	cpFloat length = cpvlength(cpvsub(b, a));
	cpFloat mass = length/160.0f;
	
	ChipmunkBody *body = [space add:[ChipmunkBody bodyWithMass:mass andMoment:mass*length*length/12.0]];
	body.pos = center;
	
	[space add:[ChipmunkSegmentShape segmentWithBody:body from:cpvsub(a, center) to:cpvsub(b, center) radius:10.0]];
	
	return body;
}

- (void)setupSpace
{
	space = [[ChipmunkSpace alloc] init];
	[space resizeActiveHashWithDim:30.0 andCount:1000];
	[space resizeActiveHashWithDim:200.0 andCount:100];
	
	ChipmunkBody *staticBody = space.staticBody;
	
	ChipmunkBody *body1  = [self addBar:cpv(-240,  160) to:cpv(-160,   80) group:1];
	ChipmunkBody *body2  = [self addBar:cpv(-160,   80) to:cpv( -80,  160) group:1];
	ChipmunkBody *body3  = [self addBar:cpv(   0,  160) to:cpv(  80,    0) group:0];
	ChipmunkBody *body4  = [self addBar:cpv( 160,  160) to:cpv( 240,  160) group:0];
	ChipmunkBody *body5  = [self addBar:cpv(-240,    0) to:cpv(-160,  -80) group:2];
	ChipmunkBody *body6  = [self addBar:cpv(-160,  -80) to:cpv( -80,    0) group:2];
	ChipmunkBody *body7  = [self addBar:cpv( -80,    0) to:cpv(   0,    0) group:2];
	ChipmunkBody *body8  = [self addBar:cpv(   0,  -80) to:cpv(  80,  -80) group:0];
	ChipmunkBody *body9  = [self addBar:cpv( 240,   80) to:cpv( 160,    0) group:3];
	ChipmunkBody *body10 = [self addBar:cpv( 160,    0) to:cpv( 240,  -80) group:3];
	ChipmunkBody *body11 = [self addBar:cpv(-240,  -80) to:cpv(-160, -160) group:4];
	ChipmunkBody *body12 = [self addBar:cpv(-160, -160) to:cpv( -80, -160) group:0];
	ChipmunkBody *body13 = [self addBar:cpv(   0, -160) to:cpv(  80, -160) group:0];
	ChipmunkBody *body14 = [self addBar:cpv( 160, -160) to:cpv( 240, -160) group:0];
	
	[space add:[ChipmunkPivotJoint pivotJointWithBodyA: body1 bodyB: body2 anchr1:cpv( 40,-40) anchr2:cpv(-40,-40)]];
	[space add:[ChipmunkPivotJoint pivotJointWithBodyA: body5 bodyB: body6 anchr1:cpv( 40,-40) anchr2:cpv(-40,-40)]];
	[space add:[ChipmunkPivotJoint pivotJointWithBodyA: body6 bodyB: body7 anchr1:cpv( 40, 40) anchr2:cpv(-40,  0)]];
	[space add:[ChipmunkPivotJoint pivotJointWithBodyA: body9 bodyB:body10 anchr1:cpv(-40,-40) anchr2:cpv(-40, 40)]];
	[space add:[ChipmunkPivotJoint pivotJointWithBodyA:body11 bodyB:body12 anchr1:cpv( 40,-40) anchr2:cpv(-40,  0)]];
	
	cpFloat stiff = 100.0f;
	cpFloat damp = 0.5f;
	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA:staticBody bodyB: body1 anchr1:cpv(-320, 240) anchr2:cpv(-40, 40) restLength:0.0 stiffness:stiff damping:damp]];
	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA:staticBody bodyB: body1 anchr1:cpv(-320,  80) anchr2:cpv(-40, 40) restLength:0.0 stiffness:stiff damping:damp]];
	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA:staticBody bodyB: body1 anchr1:cpv(-160, 240) anchr2:cpv(-40, 40) restLength:0.0 stiffness:stiff damping:damp]];

	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA:staticBody bodyB: body2 anchr1:cpv(-160, 240) anchr2:cpv( 40, 40) restLength:0.0 stiffness:stiff damping:damp]];
	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA:staticBody bodyB: body2 anchr1:cpv(   0, 240) anchr2:cpv( 40, 40) restLength:0.0 stiffness:stiff damping:damp]];

	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA:staticBody bodyB: body3 anchr1:cpv(  80, 240) anchr2:cpv(-40, 80) restLength:0.0 stiffness:stiff damping:damp]];

	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA:staticBody bodyB: body4 anchr1:cpv(  80, 240) anchr2:cpv(-40,  0) restLength:0.0 stiffness:stiff damping:damp]];
	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA:staticBody bodyB: body4 anchr1:cpv( 320, 240) anchr2:cpv( 40,  0) restLength:0.0 stiffness:stiff damping:damp]];

	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA:staticBody bodyB: body5 anchr1:cpv(-320,  80) anchr2:cpv(-40, 40) restLength:0.0 stiffness:stiff damping:damp]];
	
	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA:staticBody bodyB: body9 anchr1:cpv( 320,  80) anchr2:cpv( 40, 40) restLength:0.0 stiffness:stiff damping:damp]];

	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA:staticBody bodyB:body10 anchr1:cpv( 320,   0) anchr2:cpv( 40,-40) restLength:0.0 stiffness:stiff damping:damp]];
	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA:staticBody bodyB:body10 anchr1:cpv( 320,-160) anchr2:cpv( 40,-40) restLength:0.0 stiffness:stiff damping:damp]];

	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA:staticBody bodyB:body11 anchr1:cpv(-320,-160) anchr2:cpv(-40, 40) restLength:0.0 stiffness:stiff damping:damp]];

	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA:staticBody bodyB:body12 anchr1:cpv(-240,-240) anchr2:cpv(-40,  0) restLength:0.0 stiffness:stiff damping:damp]];
	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA:staticBody bodyB:body12 anchr1:cpv(   0,-240) anchr2:cpv( 40,  0) restLength:0.0 stiffness:stiff damping:damp]];

	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA:staticBody bodyB:body13 anchr1:cpv(   0,-240) anchr2:cpv(-40,  0) restLength:0.0 stiffness:stiff damping:damp]];
	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA:staticBody bodyB:body13 anchr1:cpv(  80,-240) anchr2:cpv( 40,  0) restLength:0.0 stiffness:stiff damping:damp]];

	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA:staticBody bodyB:body14 anchr1:cpv(  80,-240) anchr2:cpv(-40,  0) restLength:0.0 stiffness:stiff damping:damp]];
	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA:staticBody bodyB:body14 anchr1:cpv( 240,-240) anchr2:cpv( 40,  0) restLength:0.0 stiffness:stiff damping:damp]];
	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA:staticBody bodyB:body14 anchr1:cpv( 320,-160) anchr2:cpv( 40,  0) restLength:0.0 stiffness:stiff damping:damp]];

	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA: body1 bodyB: body5 anchr1:cpv( 40,-40) anchr2:cpv(-40, 40) restLength:0.0 stiffness:stiff damping:damp]];
	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA: body1 bodyB: body6 anchr1:cpv( 40,-40) anchr2:cpv( 40, 40) restLength:0.0 stiffness:stiff damping:damp]];
	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA: body2 bodyB: body3 anchr1:cpv( 40, 40) anchr2:cpv(-40, 80) restLength:0.0 stiffness:stiff damping:damp]];
	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA: body3 bodyB: body4 anchr1:cpv(-40, 80) anchr2:cpv(-40,  0) restLength:0.0 stiffness:stiff damping:damp]];
	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA: body3 bodyB: body4 anchr1:cpv( 40,-80) anchr2:cpv(-40,  0) restLength:0.0 stiffness:stiff damping:damp]];
	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA: body3 bodyB: body7 anchr1:cpv( 40,-80) anchr2:cpv( 40,  0) restLength:0.0 stiffness:stiff damping:damp]];
	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA: body3 bodyB: body7 anchr1:cpv(-40, 80) anchr2:cpv(-40,  0) restLength:0.0 stiffness:stiff damping:damp]];
	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA: body3 bodyB: body8 anchr1:cpv( 40,-80) anchr2:cpv( 40,  0) restLength:0.0 stiffness:stiff damping:damp]];
	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA: body3 bodyB: body9 anchr1:cpv( 40,-80) anchr2:cpv(-40,-40) restLength:0.0 stiffness:stiff damping:damp]];
	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA: body4 bodyB: body9 anchr1:cpv( 40,  0) anchr2:cpv( 40, 40) restLength:0.0 stiffness:stiff damping:damp]];
	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA: body5 bodyB:body11 anchr1:cpv(-40, 40) anchr2:cpv(-40, 40) restLength:0.0 stiffness:stiff damping:damp]];
	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA: body5 bodyB:body11 anchr1:cpv( 40,-40) anchr2:cpv( 40,-40) restLength:0.0 stiffness:stiff damping:damp]];
	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA: body7 bodyB: body8 anchr1:cpv( 40,  0) anchr2:cpv(-40,  0) restLength:0.0 stiffness:stiff damping:damp]];
	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA: body8 bodyB:body12 anchr1:cpv(-40,  0) anchr2:cpv( 40,  0) restLength:0.0 stiffness:stiff damping:damp]];
	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA: body8 bodyB:body13 anchr1:cpv(-40,  0) anchr2:cpv(-40,  0) restLength:0.0 stiffness:stiff damping:damp]];
	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA: body8 bodyB:body13 anchr1:cpv( 40,  0) anchr2:cpv( 40,  0) restLength:0.0 stiffness:stiff damping:damp]];
	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA: body8 bodyB:body14 anchr1:cpv( 40,  0) anchr2:cpv(-40,  0) restLength:0.0 stiffness:stiff damping:damp]];
	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA:body10 bodyB:body14 anchr1:cpv( 40,-40) anchr2:cpv(-40,  0) restLength:0.0 stiffness:stiff damping:damp]];
	[space add:[ChipmunkDampedSpring dampedSpringWithBodyA:body10 bodyB:body14 anchr1:cpv( 40,-40) anchr2:cpv(-40,  0) restLength:0.0 stiffness:stiff damping:damp]];
}

@end
