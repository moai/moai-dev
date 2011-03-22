#import "ChipmunkDemo.h"

@implementation Planet

static cpFloat frand_unit(){return 2.0f*((cpFloat)rand()/(cpFloat)RAND_MAX) - 1.0f;}
static cpFloat frand(void){return (cpFloat)rand()/(cpFloat)RAND_MAX;}

static void
planetGravityVelocityFunc(cpBody *body, cpVect gravity, cpFloat damping, cpFloat dt)
{
	cpVect p = body->p;
	cpVect g = cpvmult(p, -50000.0f/cpvdot(p, p));
	
	cpBodyUpdateVelocity(body, g, damping, dt);
}

static cpVect
rand_pos(cpFloat radius)
{
	cpVect v;
	do {
		v = cpv(frand()*(640 - 2*radius) - (320 - radius), frand()*(480 - 2*radius) - (240 - radius));
	} while(cpvlength(v) < 100.0f);
	
	return v;
}

- (void)addBox
{
	const cpFloat size = 20.0f;
	const cpFloat mass = 1.0f;
	
	cpFloat radius = cpvlength(cpv(size, size));
	
	ChipmunkBody *body = [space add:[ChipmunkBody bodyWithMass:mass andMoment:cpMomentForBox(mass, size, size)]];
	body.body->velocity_func = planetGravityVelocityFunc;
	body.pos = rand_pos(radius);
	body.vel = cpvmult(cpv(2*frand() - 1, 2*frand() - 1), 200);
	
	ChipmunkShape *shape = [space add:[ChipmunkPolyShape boxWithBody:body width:size height:size]];
	shape.elasticity = 0.0f; shape.friction = 0.7f;
}

- (void)setupSpace
{
	space = [[ChipmunkSpace alloc] init];
	
	ChipmunkBody *planetBody = [space add:space.staticBody];
	planetBody.angVel = 0.2f;
	
	for(int i=0; i<30; i++)
		[self addBox];
	
	ChipmunkShape *shape = [space add:[ChipmunkCircleShape circleWithBody:planetBody radius:70.0 offset:cpvzero]];
	shape.elasticity = 1.0; shape.friction = 1.0;
	shape.layers = NOT_GRABABLE_MASK;
}

@end
