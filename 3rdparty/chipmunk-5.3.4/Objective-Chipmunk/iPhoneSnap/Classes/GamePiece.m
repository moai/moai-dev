#import "GamePiece.h"


@implementation GamePiece

@synthesize chipmunkObjects;
@synthesize body;
@synthesize points;

- (void)updateTransform
{
	self.transform = [body affineTransform];
}

static cpFloat cpfrand(void){return (cpFloat)rand()/(cpFloat)RAND_MAX;}

- (id)initWithSize:(GamePieceSize)size staticBody:(ChipmunkBody *)staticBody
{
	static NSString *images[] = {@"1.png",@"3.png",@"5.png"};
	static int pointAmounts[] = {1, 3, 5};
	
	if(self = [super initWithImage:[UIImage imageNamed:images[size]]]){
		points = pointAmounts[size];
		
		cpFloat radius = 0.975*(self.bounds.size.width/2.0);
		cpFloat mass = 1.0;
		body = [[ChipmunkBody alloc] initWithMass:1.0 andMoment:cpMomentForCircle(mass, 0.0, radius, cpvzero)];
		body.data = self;
		body.pos = cpv(cpfrand()*320.0, cpfrand()*320.0);
		
		ChipmunkShape *shape = [ChipmunkCircleShape circleWithBody:body radius:radius offset:cpvzero];
		shape.data = self;
		shape.elasticity = 0.0;
		shape.friction = FRICTION_AMOUNT;
		
		// Use Joints to fake top down friction
		ChipmunkConstraint *linear = [ChipmunkPivotJoint pivotJointWithBodyA:staticBody bodyB:body anchr1:cpvzero anchr2:cpvzero];
		linear.biasCoef = 0.0; // Disable joint correction.
		linear.maxForce = 50.0;
		
		ChipmunkConstraint *rotary = [ChipmunkGearJoint gearJointWithBodyA:staticBody bodyB:body phase:0.0 ratio:1.0];
		rotary.biasCoef = 0.0; // Disable joint correction.
		rotary.maxForce = 2000.0;
		
		chipmunkObjects = [ChipmunkObjectFlatten(body, shape, linear, rotary, nil) retain];
		
		self.center = CGPointZero;
		[self updateTransform];
	}
	
	return self;
}

+ (id)pieceWithSize:(GamePieceSize)size staticBody:(ChipmunkBody *)staticBody;
{
	return [[[self alloc] autorelease] initWithSize:size staticBody:staticBody];
}

- (void)dealloc {
	[body release];
	[chipmunkObjects release];
	
	[super dealloc];
}

@end
