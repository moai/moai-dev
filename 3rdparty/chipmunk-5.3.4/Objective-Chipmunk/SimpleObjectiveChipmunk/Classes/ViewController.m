#import "ViewController.h"

#import "SimpleSound.h"

// An object to use as a collision type for the screen border.
// Class objects and strings are easy and convenient to use as collision types.
static NSString *borderType = @"borderType";

@implementation ViewController

- (void)viewDidLoad {
	[super viewDidLoad];
	
	// Create and initialize the Chipmunk space.
	// Chipmunk spaces are containers for simulating physics.
	// You add a bunch of objects and joints to the space and the space runs the physics simulation on them all together.
	space = [[ChipmunkSpace alloc] init];
	
	// This method adds four static line segment shapes to the space.
	// Most 2D physics games end up putting all the gameplay in a box. This method just makes that easy.
	// We'll tag these segment shapes with the borderType object. You'll see what this is for next.
	[space addBounds:self.view.bounds thickness:10.0f elasticity:1.0f friction:1.0f layers:CP_ALL_LAYERS group:CP_NO_GROUP collisionType:borderType];
	
	// This adds a callback that happens whenever a shape tagged with the
	// [FallingButton class] object and borderType objects collide.
	// You can use any object you want as a collision type identifier.
	// I often find it convenient to use class objects to define collision types.
	// There are 4 different collision events that you can catch: begin, pre-solve, post-solve and separate.
	// See the documentation for a description of what they are all for.
	[space addCollisionHandler:self
		typeA:[FallingButton class] typeB:borderType
		begin:@selector(beginCollision:space:)
		preSolve:nil
		postSolve:@selector(postSolveCollision:space:)
		separate:@selector(separateCollision:space:)
	];
	
	
	// You have to actually put things in a Chipmunk space for it to do anything interesting.
	// I've created a game object controller class called FallingButton that makes a UIButton that is influenced by physics.
	fallingButton = [[FallingButton alloc] init];
	// Add the UIButton to the view hierarchy.
	[self.view addSubview:fallingButton.button];
	
	// Adding physics objects in Objective-Chipmunk is a snap thanks to the ChipmunkObject protocol.
	// No matter how many physics objects (collision shapes, joints, etc) the fallingButton has, it can be added in a single line!
	// See FallingButton.m to see how easy it is to implement the protocol.
	[space add:fallingButton];
}



- (bool)beginCollision:(cpArbiter*)arbiter space:(ChipmunkSpace*)space {
	// This macro gets the colliding shapes from the arbiter and defines variables for them.
	CHIPMUNK_ARBITER_GET_SHAPES(arbiter, buttonShape, border);
	
	// It expands to look something like this:
	// ChipmunkShape *buttonShape = GetShapeWithFirstCollisionType();
	// ChipmunkShape *border = GetShapeWithSecondCollisionType();
	
	// Lets log the data pointers just to make sure we are getting what we think we are.
	NSLog(@"First object in the collision is %@ second object is %@.", buttonShape.data, border.data);
	
	// When we created the collision shape for the FallingButton,
	// we set the data pointer to point at the FallingButton it was associated with.
	FallingButton *fb = buttonShape.data;
	
	// Increment the touchedShapes counter on the FallingButton object.
	// We'll decrement this in the separate callback.
	// If the counter is 0, then you know you aren't touching anything.
	// You can use this technique in platformer games to track if the player is in the air on not.
	fb.touchedShapes++;
	
	// Change the background color to gray so we know when the button is touching something.
	self.view.backgroundColor = [UIColor grayColor];
	
	// begin and pre-solve callbacks MUST return a boolean.
	// Returning false from a begin callback ignores a collision permanently.
	// Returning false from a pre-solve callback ignores the collision for just one frame.
	// See the documentation on collision handlers for more information.
	return TRUE; // We return true, so the collision is handled normally.
}

// The post-solve collision callback is called right after Chipmunk has finished calculating all of the
// collision responses. You can use it to find out how hard objects hit each other.
// There is also a pre-solve callback that allows you to reject collisions conditionally.
- (void)postSolveCollision:(cpArbiter*)arbiter space:(ChipmunkSpace*)space {
	// We only care about the first frame of the collision.
	// If the shapes have been colliding for more than one frame, return early.
	if(!cpArbiterIsFirstContact(arbiter)) return;
	
	// This method gets the impulse that was applied between the two objects to resolve
	// the collision. We'll use the length of the impulse vector to approximate the sound
	// volume to play for the collision.
	cpFloat impulse = cpvlength(cpArbiterTotalImpulse(arbiter));
	
	float volume = MIN(impulse/500.0f, 1.0f);
	if(volume > 0.05f){
		[SimpleSound playSoundWithVolume:volume];
	}
}

static CGFloat frand(){return (CGFloat)rand()/(CGFloat)RAND_MAX;}

// The separate callback is called whenever shapes stop touching.
- (void)separateCollision:(cpArbiter*)arbiter space:(ChipmunkSpace*)space {
	CHIPMUNK_ARBITER_GET_SHAPES(arbiter, buttonShape, border);
	
	// Decrement the counter on the FallingButton.
	FallingButton *fb = buttonShape.data;
	fb.touchedShapes--;
	
	// If touchedShapes is 0, then we know the falling button isn't touching anything anymore.
	if(fb.touchedShapes == 0){
		// Let's set the background color to a random color so you can see each time the shape touches something new.
		self.view.backgroundColor = [UIColor colorWithRed:frand() green:frand() blue:frand() alpha:1.0f];
	}
}

// When the view appears on the screen, start the animation timer and tilt callbacks.
- (void)viewDidAppear:(BOOL)animated {
	// Set up the display link to control the timing of the animation.
	displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(update)];
	displayLink.frameInterval = 1;
	[displayLink addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSDefaultRunLoopMode];
	
	// Set up an accelerometer delegate.
	UIAccelerometer *accel = [UIAccelerometer sharedAccelerometer];
	accel.updateInterval = 1.0f/30.0f;
	accel.delegate = self;
}

// This method is called each frame to update the scene.
// It is called from the display link every time the screen wants to redraw itself.
- (void)update {
	// Step (simulate) the space based on the time since the last update.
	cpFloat dt = displayLink.duration*displayLink.frameInterval;
	[space step:dt];
	
	// Update the button.
	// This sets the position and rotation of the button to match the rigid body.
	[fallingButton updatePosition];
}

- (void)accelerometer:(UIAccelerometer *)accelerometer didAccelerate:(UIAcceleration *)accel {
	// Setting the gravity based on the tilt of the device is easy.
	space.gravity = cpvmult(cpv(accel.x, -accel.y), 100.0f);
}

// The view disappeared. Stop the animation timers and tilt callbacks.
- (void)viewDidDisappear:(BOOL)animated {
	// Remove the timer.
	[displayLink invalidate];
	displayLink = nil;
	
	// Remove the accelerometer delegate.
	[UIAccelerometer sharedAccelerometer].delegate = nil;
}

- (void)dealloc {
	[fallingButton release];
	
	[super dealloc];
}

@end
