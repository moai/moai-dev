#import "ChipmunkDemo.h"
#import "Accelerometer.h"

@implementation PyramidTopple

- (void)addDomino:(cpVect)pos angle:(cpFloat)angle
{
	const cpFloat u = 0.8;
	const cpFloat width = 6.0;
	const cpFloat height = 40.0;
	
	ChipmunkBody *body = [space add:[ChipmunkBody bodyWithMass:1.0 andMoment:cpMomentForBox(1.0, width, height)]];
	body.pos = pos; body.angle = angle;
	
	ChipmunkShape *shape = [space add:[ChipmunkPolyShape boxWithBody:body width:width height:height]];
	shape.elasticity = 0.0; shape.friction = u;
}

- (void)setupSpace
{
	space = [[ChipmunkSpace alloc] init];
	space.iterations = 30;
	[space resizeActiveHashWithDim:40.0 andCount:3000];
	[space resizeStaticHashWithDim:40.0 andCount:1000];
	space.gravity = cpv(0.0, -300.0);
	
	[space addBounds:CGRectMake(-320.0, -240.0, 640.0, 480.0) thickness:10.0 elasticity:1.0 friction:0.0 layers:NOT_GRABABLE_MASK group:nil collisionType:nil];
	
	int n = 5;
	for(int i=1; i<=n; i++){
		cpVect offset = cpv(-i*60/2.0f, (n - i)*52);
		
		for(int j=0; j<i; j++){
			[self addDomino:cpvadd(cpv(j*60, -220), offset) angle:0.0];
			[self addDomino:cpvadd(cpv(j*60, -197), offset) angle:M_PI/2.0];

			if(j == (i - 1)) continue;
			[self addDomino:cpvadd(cpv(j*60 + 30, -191), offset) angle:M_PI/2.0];
		}

		[self addDomino:cpvadd(cpv(-17, -174), offset) angle:0.0];
		[self addDomino:cpvadd(cpv((i - 1)*60 + 17, -174), offset) angle:0.0];
	}
}

@end
