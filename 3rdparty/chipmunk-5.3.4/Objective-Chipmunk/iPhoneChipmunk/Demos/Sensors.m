#import "ChipmunkDemo.h"

NSString *BALL_TYPE = @"BALL_TYPE";
NSString *BLOCKING_SENSOR_TYPE = @"BLOCKING_SENSOR_TYPE";
NSString *CATCH_SENSOR_TYPE = @"CATCH_SENSOR_TYPE";

@interface Emitter : NSObject {
	int queue;
	int blockingCount;
	cpVect position;
}

@property (readonly) BOOL ready;
@property (readonly) cpVect position;

@end


@implementation Emitter

@synthesize position;

- (id)initWithCount:(int)count andPos:(cpVect)pos
{
	if(self = [super init]){
		queue = count;
		position = pos;
	}
	
	return self;
}

- (BOOL)blockerBegin:(cpArbiter *)arb space:(ChipmunkSpace *)space
{
	blockingCount++;
	return FALSE;
}

- (void)blockerSeparate:(cpArbiter *)arb space:(ChipmunkSpace *)space
{
	blockingCount--;
}

- (BOOL)catcherBarBegin:(cpArbiter *)arb space:(ChipmunkSpace *)space
{
	CHIPMUNK_ARBITER_GET_SHAPES(arb, a, b);
	
	queue++;
	
	[space addPostStepRemoval:b];
	[space addPostStepRemoval:b.body];
	
	return 0;
}

- (BOOL)ready {return blockingCount == 0 && queue > 0;}
- (void)dequeue {queue--;}

@end


@implementation Sensors

static cpFloat frand_unit(){return 2.0f*((cpFloat)rand()/(cpFloat)RAND_MAX) - 1.0f;}

- (void)update
{
	if([emitter ready]){
		[emitter dequeue];
		
		ChipmunkBody *body = [space add:[ChipmunkBody bodyWithMass:1.0 andMoment:cpMomentForCircle(1.0, 0.0, 15.0, cpvzero)]];
		body.pos = [emitter position];
		body.vel = cpvmult(cpv(frand_unit(), frand_unit()), 100.0f);
		
		ChipmunkShape *shape = [space add:[ChipmunkCircleShape circleWithBody:body radius:15.0 offset:cpvzero]];
		shape.collisionType = BALL_TYPE;
	}
	
	[super update];
}
- (void)setupSpace
{
	space = [[ChipmunkSpace alloc] init];
	space.iterations = 10;
	space.gravity = cpv(0.0, -100.0);
	
	ChipmunkBody *staticBody = space.staticBody;
	
	emitter = [[Emitter alloc] initWithCount:5 andPos:cpv(0.0, 150.0)];
	
	ChipmunkShape *shape;
	
	shape = [space add:[ChipmunkCircleShape circleWithBody:staticBody radius:15.0 offset:[emitter position]]];
	shape.sensor = TRUE;
	shape.collisionType = BLOCKING_SENSOR_TYPE;
	shape.layers = NOT_GRABABLE_MASK;
	
	// Create our catch sensor to requeue the balls when they reach the bottom of the screen
	shape = [space add:[ChipmunkSegmentShape segmentWithBody:staticBody from:cpv(-2000, -200) to:cpv(2000, -200) radius:15.0]];
	shape.sensor = TRUE;
	shape.collisionType = CATCH_SENSOR_TYPE;
	shape.layers = NOT_GRABABLE_MASK;
	
	[space addCollisionHandler:emitter
		typeA:BLOCKING_SENSOR_TYPE typeB:BALL_TYPE
		begin:@selector(blockerBegin:space:)
		preSolve:NULL
		postSolve:NULL
		separate:@selector(blockerSeparate:space:)
	];
	
	[space addCollisionHandler:emitter
		typeA:CATCH_SENSOR_TYPE typeB:BALL_TYPE
		begin:@selector(catcherBarBegin:space:)
		preSolve:NULL
		postSolve:NULL
		separate:NULL
	];
}

- (void) dealloc
{
	[emitter release];
	
	[super dealloc];
}


@end
