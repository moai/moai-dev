#import "ChipmunkDemo.h"
#import "Accelerometer.h"

@interface OneWayPlatform : NSObject {
	cpVect n;
}
@end

@implementation OneWayPlatform

- (id)initWithNormal:(cpVect)normal
{
	if(self = [super init]){
		n = normal;
	}
	
	return self;
}

- (BOOL)preSolve:(cpArbiter *)arb space:(ChipmunkSpace *)space
{
	if(cpvdot(cpArbiterGetNormal(arb, 0), n) < 0){
		cpArbiterIgnore(arb);
		return 0;
	}
	
	return 1;
}

@end


@implementation OneWay

- (void)update
{
	space.gravity = cpvmult([Accelerometer getAcceleration], 100);
	[super update];
}

- (void)setupSpace
{
	space = [[ChipmunkSpace alloc] init];
	
	[space addBounds:CGRectMake(-320.0, -240.0, 640.0, 480.0) thickness:10.0 elasticity:1.0 friction:0.0 layers:NOT_GRABABLE_MASK group:nil collisionType:nil];
	
	ChipmunkBody *body;
	ChipmunkShape *shape;
	
	cpFloat radius = 15.0f;
	body = [space add:[ChipmunkBody bodyWithMass:10.0 andMoment:cpMomentForCircle(10.0, 0.0, radius, cpvzero)]];
	body.pos = cpv(0, -200);
	body.vel = cpv(0, 170);

	shape = [space add:[ChipmunkCircleShape circleWithBody:body radius:radius offset:cpvzero]];
	shape.elasticity = 0.0f; shape.friction = 0.9f;
	
	
	platform = [[OneWayPlatform alloc] initWithNormal:cpv(0.0, 1.0)];
	
	shape = [space add:[ChipmunkSegmentShape segmentWithBody:space.staticBody from:cpv(-160,-100) to:cpv(160,-100) radius:10.0]];
	shape.elasticity = 1.0f; shape.friction = 1.0f;
	shape.layers = NOT_GRABABLE_MASK;
	shape.collisionType = [OneWayPlatform class]; // Class objects work great as collision type objects
	shape.data = platform; // handler methods will be called on this object
	
	// Call the handler methods on the platform object connected to the first shape
	[space addShapeAHandler:[OneWayPlatform class]
		typeA:[OneWayPlatform class] typeB:nil
		begin:nil
		preSolve:@selector(preSolve:space:)
		postSolve:nil
		separate:nil
	];
}

- (void) dealloc
{
	[platform release];
	[super dealloc];
}


@end
