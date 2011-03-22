// This class is a game object controller of sorts.
// It basically ties the physics and graphics objects together, and gives you a
// place to put your controlling logic.

#import "FallingButton.h"

#define SIZE 100.0f

@implementation FallingButton

@synthesize button;
@synthesize touchedShapes;

// This property fulfills the ChipmunkObject protocol.
// It is perhaps the most important thing this class does.
// By maintaining this list of objects, a ChipmunkSpace can easily add and remove complicated game objects.
// Using the C API, you would have to keep references to each Chipmunk object and add them one by one
// based on their type to the Chipmunk space.
@synthesize chipmunkObjects;

static cpFloat frand_unit(){return 2.0f*((cpFloat)rand()/(cpFloat)RAND_MAX) - 1.0f;}

- (void)buttonClicked {
	// Apply a random velcity change to the body when the button is clicked.
	cpVect v = cpvmult(cpv(frand_unit(), frand_unit()), 300.0f);
	body.vel = cpvadd(body.vel, v);
	
	body.angVel += 5.0f*frand_unit();
}

- (void)updatePosition {
	// ChipmunkBodies have a handy affineTransform property that makes working with Cocoa or Cocos2D a snap.
	// This is all you have to do to move a button along with the physics!
	button.transform = body.affineTransform;
	
	// Alternatively, you could build a transform yourself from the body's position and rotation:
	//	cpVect pos = body.pos;
	//	button.transform = CGAffineTransformRotate(CGAffineTransformMakeTranslation(pos.x, pos.y), body.angle);
}

- (id)init {
	if(self = [super init]){
		// Set up the UIButton.
		button = [UIButton buttonWithType:UIButtonTypeCustom];
		[button setTitle:@"Click Me!" forState:UIControlStateNormal];
		[button setTitleColor:[UIColor blueColor] forState:UIControlStateNormal];
		[button setBackgroundImage:[UIImage imageNamed:@"logo.png"] forState:UIControlStateNormal];
		button.bounds = CGRectMake(0, 0, SIZE, SIZE);
		
		[button addTarget:self action:@selector(buttonClicked) forControlEvents:UIControlEventTouchDown];
		
		// Set up Chipmunk objects.
		cpFloat mass = 1.0f;
		
		// The moment of inertia is like the rotational mass of an object.
		// Chipmunk provides a number of helper functions to help you estimate the moment of inertia.
		cpFloat moment = cpMomentForBox(mass, SIZE, SIZE);
		
		// A rigid body is the basic skeleton you attach joints and collision shapes too.
		// Rigid bodies hold the physical properties of an object such as the postion, rotation, and mass of an object.
		// You attach collision shapes to rigid bodies to define their shape and allow them to collide with other objects,
		// and you can attach joints between rigid bodies to connect them together.
		body = [[ChipmunkBody alloc] initWithMass:mass andMoment:moment];
		body.pos = cpv(200.0f, 200.0f);
		
		// Chipmunk supports a number of collision shape types. See the documentation for more information.
		// Because we are storing this into a local variable instead of an instance variable, we can use the autorelease constructor.
		// We'll let the chipmunkObjects NSSet hold onto the reference for us.
		ChipmunkShape *shape = [ChipmunkPolyShape boxWithBody:body width:SIZE height:SIZE];
		
		// The elasticity of a shape controls how bouncy it is.
		shape.elasticity = 0.3f;
		// The friction propertry should be self explanatory. Friction values go from 0 and up- they can be higher than 1f.
		shape.friction = 0.3f;
		
		// Set the collision type to a unique value (the class object works well)
		// This type is used as a key later when setting up callbacks.
		shape.collisionType = [FallingButton class];
		
		// Set data to point back to this object.
		// That way you can get a reference to this object from the shape when you are in a callback.
		shape.data = self;
		
		// Keep in mind that you can attach multiple collision shapes to each rigid body, and that each shape can have
		// unique properties. You can make the player's head have a different collision type for instance. This is useful
    // for brain damage.
		
		// Now we just need to initialize the instance variable for the chipmunkObjects property.
		// ChipmunkObjectFlatten() is an easy way to build this set. You can pass any object to it that
		// implements the ChipmunkObject protocol and not just primitive types like bodies and shapes.
		
		// Notice that we didn't even have to keep a reference to 'shape'. It was created using the autorelease convenience function.
		// This means that the chipmunkObjects NSSet will manage the memory for us. No need to worry about forgetting to call
		// release later when you're using Objective-Chipmunk!
		
		// Note the nil terminator at the end! (this is how it knows you are done listing objects)
		chipmunkObjects = [ChipmunkObjectFlatten(body, shape, nil) retain];
	}
	
	return self;
}

- (void) dealloc
{
	[button release];
	[body release];
	[chipmunkObjects release];
	
	[super dealloc];
}

@end
