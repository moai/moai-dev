#import "BoardView.h"

#import <QuartzCore/QuartzCore.h>

#import "AppDelegate.h"
#import "GamePiece.h"

#define TIMESTEP 1.0/60.0

@implementation BoardView

static cpFloat kineticEnergy(cpBody *body){return body->m*cpvlengthsq(body->v) + body->i*body->w*body->w;}

- (void)update:(CADisplayLink *)sender;
{
	// Drawing using CoreAnimation is simple but slow, so the framerate is set to 30fps
	// We want the space to be stepped at 60Hz, so we can step it twice per frame.
	// This works great for a simple game like this, though action games will want to do
	// something more elegant.
	[space step:TIMESTEP];
	[space step:TIMESTEP];
	
	meter.progress = 1.0 - MIN(MAX((barBody.pos.y - lastBarHeight)/40.0, 0.0), 1.0);
	
	cpFloat energy = barBody.mass*cpvlengthsq(barBody.vel);
	for(GamePiece *piece in pieces){
		// This isn't really performance critical code, but you can still access and use
		// the underlying C types if you need them to better optimize parts of your code.
		energy += kineticEnergy(piece.body.body);
	}
	
	if(energy < 1.0){
		if(++idleTicks > 10){
			// Nothing has been moving much for a few steps
			// stop simulating and end the turn
			[AppDelegate sharedDelegate].simulationPaused = TRUE;
			lastBarHeight = barBody.pos.y;
		}
	} else {
		idleTicks = 0;
	}
	
	[pieces makeObjectsPerformSelector:@selector(updateTransform)];
	barView.transform = [barBody affineTransform];
}

- (void)addPiece:(GamePiece *)piece
{
	[pieces addObject:piece];
	[space add:piece];
	[self addSubview:piece];
}

- (void)removeSubView:(NSString *)animationID finished:(NSNumber *)finished context:(UIView *)view
{
	[view removeFromSuperview];
}

- (void)removePiece:(GamePiece *)piece
{
	if(piece == nil) return;
	
	idleTicks = 0;
	[AppDelegate sharedDelegate].thisScore += piece.points;
	
	[pieces removeObject:piece];
	[space remove:piece];
//	[piece removeFromSuperview];
	
	[UIView beginAnimations:nil context:piece];
		[UIView setAnimationDelegate:self];
		[UIView setAnimationDidStopSelector:@selector(removeSubView:finished:context:)];
		piece.alpha = 0.0;
	[UIView commitAnimations];
	
	[AppDelegate sharedDelegate].simulationPaused = FALSE;
	
	if([pieces count] <= 2){
		[[AppDelegate sharedDelegate] gameOver];
	}
}

- (void)startNewGame;
{
	for(GamePiece *piece in [NSSet setWithSet:pieces]){
		[piece removeFromSuperview];
	}
	
	[pieces release];
	[space release];
	
	pieces = [[NSMutableSet alloc] init];
	space = [[ChipmunkSpace alloc] init];
	staticBody = [ChipmunkBody staticBody];
	
	lastBarHeight = INFINITY;
	
	CGRect bounds = self.bounds;
	bounds.size.height += 320.0;
	bounds.origin.y -= 320.0;
	[space addBounds:bounds thickness:100.0 elasticity:0.0 friction:FRICTION_AMOUNT layers:CP_ALL_LAYERS group:[BoardView class] collisionType:nil];
	
	// Set up the bar
	{
		CGSize barSize = barView.bounds.size;
		cpFloat offset = barSize.width/2.0;
		cpFloat radius = barSize.height/2.0;
		
		[barBody release];
		barBody = [[ChipmunkBody alloc] initWithMass:10.0 andMoment:INFINITY];
		[space add:barBody];
		barBody.pos = cpv(160.0, -radius);
		barBody.force = cpv(0.0 , 6000.0);
		
		ChipmunkShape *barShape = [space add:[ChipmunkSegmentShape segmentWithBody:barBody from:cpv(-offset, 0.0) to:cpv(offset, 0.0) radius:radius]];
		barShape.friction = FRICTION_AMOUNT;
		barShape.group = [BoardView class];
		
		cpFloat bottom = self.bounds.size.height - barSize.height/2.0;
		[space add:[ChipmunkGrooveJoint grooveJointWithBodyA:staticBody bodyB:barBody groove_a:cpv(160.0, -1000.0) groove_b:cpv(160.0, bottom) anchr2:cpvzero]];
		
		barView.center = CGPointZero;
		barView.transform = [barBody affineTransform];
	}
	
	// Add the pieces
	for(int i=0; i<12; i++)
		[self addPiece:[GamePiece pieceWithSize:GamePieceSmall staticBody:staticBody]];
	for(int i=0; i<9; i++)
		[self addPiece:[GamePiece pieceWithSize:GamePieceMed staticBody:staticBody]];
	for(int i=0; i<6; i++)
		[self addPiece:[GamePiece pieceWithSize:GamePieceLarge staticBody:staticBody]];
	
	// Let everything settle into place
	for(int i=0; i<200; i++)
		[space step:TIMESTEP];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	// Can only remove pieces when the sim is not running
	if(![AppDelegate sharedDelegate].simulationPaused) return;
	
	cpVect point = [[[event touchesForView:self] anyObject] locationInView:self];
	[self removePiece:[space pointQueryFirst:point layers:CP_ALL_LAYERS group:[BoardView class]].data];
}

- (void)dealloc
{
	[space release];
	[pieces release];
	
	[barView release];
	[barBody release];
	
	[super dealloc];
}

@end
