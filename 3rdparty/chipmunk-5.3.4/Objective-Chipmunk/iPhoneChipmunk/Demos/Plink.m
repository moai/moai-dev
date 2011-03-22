#import "ChipmunkDemo.h"
#import "Accelerometer.h"

@implementation Plink

cpVect pentagon[5];

+ (void)initialize
{
	static BOOL done = FALSE;
	if(done) return;
	
	for(int i=0; i<5; i++){
		cpFloat angle = -2*M_PI*i/5.0;
		pentagon[i] = cpv(10*cos(angle), 10*sin(angle));
	}
	
	done = TRUE;
}

- (void)update
{
	space.gravity = cpvmult([Accelerometer getAcceleration], 100);
	
	if([pentagons count] < 150){
		ChipmunkBody *body = [space add:[ChipmunkBody bodyWithMass:1.0 andMoment:cpMomentForPoly(1.0, 5, pentagon, cpvzero)]];
		cpFloat x = rand()/(cpFloat)RAND_MAX*640 - 320;
		cpFloat y = rand()/(cpFloat)RAND_MAX*300 + 350;
		body.pos = cpv(x, y);
		[pentagons addObject:body];
		
		ChipmunkShape *shape = [space add:[ChipmunkPolyShape polyWithBody:body count:5 verts:pentagon offset:cpvzero]];
		shape.elasticity = 0.0; shape.friction = 0.4;
	}
	
	for(ChipmunkBody *body in pentagons){
		cpVect pos = body.pos;
		if(pos.y < -260 || fabsf(pos.x) > 340){
			body.pos = cpv(((cpFloat)rand()/(cpFloat)RAND_MAX)*640.0 - 320.0, 260);
		}
	}
	
	[super update];
}

- (void)setupSpace
{
	space = [[ChipmunkSpace alloc] init];
	space.iterations = 5;
	
	[space resizeStaticHashWithDim:40.0 andCount:1000];
	[space resizeStaticHashWithDim:30.0 andCount:3000];
	
	ChipmunkBody *staticBody = space.staticBody;
	
	// Vertexes for a triangle shape.
	cpVect verts[] = {
		cpv(-15,-15),
		cpv(  0, 10),
		cpv( 15,-15),
	};

	// Create the static triangles.
	for(int i=0; i<9; i++){
		for(int j=0; j<6; j++){
			cpFloat stagger = (j%2)*40;
			cpVect offset = cpv(i*80 - 320 + stagger, j*70 - 240);
			ChipmunkShape *shape = [space add:[ChipmunkPolyShape polyWithBody:staticBody count:3 verts:verts offset:offset]];
			shape.elasticity = 1.0; shape.friction = 1.0;
			shape.layers = NOT_GRABABLE_MASK;
		}
	}
	
	pentagons = [[NSMutableArray alloc] init];
}

- (void) dealloc
{
	[pentagons release];
	[super dealloc];
}


@end