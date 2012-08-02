/**
	Chipmunk spaces are simulation containers. You add a bunch of physics objects to a space (rigid bodies, collision shapes, and joints) and step the entire space forward through time as a whole.
*/ 
@interface ChipmunkSpace : NSObject {
@private
	cpSpace _space;
	ChipmunkBody *_staticBody;
	
	NSMutableSet *_children;
	NSMutableArray *_handlers;
	
	// TODO should probably add a weak data reference like the other objects have
}

/**
	The iteration count is how many solver passes the space should use when solving collisions and joints (default is 10).
	Fewer iterations mean less CPU usage, but lower quality (mushy looking) physics.
*/
@property int iterations;

/// Deprecated in 5.3.0, should not be needed. @deprecated since 5.3.0
@property int elasticIterations;

/// Global gravity value to use for all rigid bodies in this space (default value is @c cpvzero).
@property cpVect gravity;

/**
	Global viscous damping value to use for all rigid bodies in this space (default value is 1.0 which disables damping).
	This value is the fraction of velocity a body should have after 1 second.
	A value of 0.9 would mean that each second, a body would have 80% of the velocity it had the previous second.
*/
@property cpFloat damping;

/// If a body is moving slower than this speed, it is considered idle. The default value is 0, which signals that the space should guess a good value based on the current gravity.
@property cpFloat idleSpeedThreshold;

/**
	Elapsed time before a group of idle bodies is put to sleep (defaults to infinity which disables sleeping).
	If an entire group of touching or jointed bodies has been idle for at least this long, the space will put all of the bodies into a sleeping state where they consume very little CPU.
*/
@property cpFloat sleepTimeThreshold;

/// Returns a pointer to the underlying cpSpace C struct
@property (readonly) cpSpace *space;

/**
	The space's designated static body.
	Collision shapes added to the body will automatically be marked as static shapes, and rigid bodies that come to rest while touching or jointed to this body will fall asleep.
*/
@property (readonly) ChipmunkBody *staticBody;

/**
	A macro that defines and initializes shape variables for you in a collision callback.
	They are initialized in the order that they were defined in the collision handler associated with the arbiter.
	If you defined the handler as:
	
	@code
		[space addCollisionHandler:target typeA:foo typeB:bar ...]
	@endcode
	
	You you will find that @code a->collision_type == 1 @endcode and @code b->collision_type == 2 @endcode.
*/
#define CHIPMUNK_ARBITER_GET_SHAPES(arb, a, b) ChipmunkShape *a, *b; { \
	cpShape *shapeA, *shapeB; \
	cpArbiterGetShapes(arb, &shapeA, &shapeB); \
	a = shapeA->data; b = shapeB->data; \
}

/**
  Set the default collision handler.
  The default handler is used for all collisions when a specific collision handler cannot be found.
  
  The expected method selectors are as follows:
	@code
- (bool)begin:(cpArbiter *)arbiter space:(ChipmunkSpace*)space
- (bool)preSolve:(cpArbiter *)arbiter space:(ChipmunkSpace*)space
- (void)postSolve:(cpArbiter *)arbiter space:(ChipmunkSpace*)space
- (void)separate:(cpArbiter *)arbiter space:(ChipmunkSpace*)space
	@endcode
*/
- (void)setDefaultCollisionHandler:(id)target
	begin:(SEL)begin
	preSolve:(SEL)preSolve
	postSolve:(SEL)postSolve
	separate:(SEL)separate;

/**
  Set a collision handler to handle specific collision types.
  The methods are called only when shapes with the specified collisionTypes collide.
  
  @c typeA and @c typeB should be the same object references set to ChipmunkShape.collisionType. They can be any uniquely identifying object.
	Class and global NSString objects work well as collision types as they are easy to get a reference to and do not require you to allocate any objects.
  
  The expected method selectors are as follows:
	@code
- (bool)begin:(cpArbiter *)arbiter space:(ChipmunkSpace*)space
- (bool)preSolve:(cpArbiter *)arbiter space:(ChipmunkSpace*)space
- (void)postSolve:(cpArbiter *)arbiter space:(ChipmunkSpace*)space
- (void)separate:(cpArbiter *)arbiter space:(ChipmunkSpace*)space
	@endcode
*/
- (void)addCollisionHandler:(id)target
	typeA:(id)a typeB:(id)b
	begin:(SEL)begin
	preSolve:(SEL)preSolve
	postSolve:(SEL)postSolve
	separate:(SEL)separate;


// TODO these might be a really bad idea as they will not call subclass methods.
// The following two methods use the [shape data] object as the collision handler.
// In order to cache the method lookups, it needs to know the class of the object.
// This means that [shape data] objects _must_ be a direct instance of klass and never a subclass.
// See OneWay.m for an example.

/// Deprecated in 5.3.0 @deprecated since 5.3.0
- (void)addShapeAHandler:(Class)klass
	typeA:(id)a typeB:(id)b
	begin:(SEL)begin
	preSolve:(SEL)preSolve
	postSolve:(SEL)postSolve
	separate:(SEL)separate
 __attribute__((deprecated));

/// Deprecated in 5.3.0 @deprecated since 5.3.0
- (void)addShapeBHandler:(Class)klass
	typeA:(id)a typeB:(id)b
	begin:(SEL)begin
	preSolve:(SEL)preSolve
	postSolve:(SEL)postSolve
	separate:(SEL)separate
 __attribute__((deprecated));

/// Remove a collision handler.
- (void)removeCollisionHandlerForTypeA:(id)a andB:(id)b;

/**
  Add an object to the space.
  This can be any object that implements the ChipmunkObject protocol.
	This includes all the basic types such as ChipmunkBody, ChipmunkShape and ChipmunkConstraint as well as any composite game objects you may define that implement the protocol.
	@warning This method may not be called from a collision handler callback. See ChipmunkSpace.addPostStepCallback:selector:context: for information on how to do that.
*/
- (id)add:(NSObject<ChipmunkObject> *)obj;

/**
  Add a collection of ChipmunkBaseObject objects to the space.
  The objects must implement the ChipmunkBaseObject protocol. These include ChipmunkBody, ChipmunkShape and ChipmunkConstraint objects.
	@warning This method may not be called from a collision handler callback. See ChipmunkSpace.addPostStepCallback:selector:context: for information on how to do that.
*/
- (void)addBaseObjects:(id <NSFastEnumeration>)objects;

/**
  Remove an object from the space.
  This can be any object that implements the ChipmunkObject protocol.
	This includes all the basic types such as ChipmunkBody, ChipmunkShape and ChipmunkConstraint as well as any composite game objects you may define that implement the protocol.
	@warning This method may not be called from a collision handler callback. See ChipmunkSpace.addPostStepCallback:selector:context: for information on how to do that.
*/
- (id)remove:(NSObject<ChipmunkObject> *)obj;

/**
  Remove a collection of ChipmunkBaseObject objects from the space.
  The objects must implement the ChipmunkBaseObject protocol. These include ChipmunkBody, ChipmunkShape and ChipmunkConstraint objects.
	@warning This method may not be called from a collision handler callback. See ChipmunkSpace.addPostStepCallback:selector:context: for information on how to do that.
*/
- (void)removeBaseObjects:(id <NSFastEnumeration>)objects;

// Deprecated due to a bug in the implementation that cannot be fixed without breaking backwards compatibility.
// Use [ChipmunkSpace addPostStepCallback:selector:key:] instead.
- (void)addPostStepCallback:(id)target selector:(SEL)selector context:(id)object __attribute__((deprecated));

/**
  Define a callback to be run just before [ChipmunkSpace step:] finishes.
  The main reason you want to define post-step callbacks is to get around the restriction that you cannot call the add/remove methods from a collision handler callback.
	Post-step callbacks run right before the next (or current) call to ChipmunkSpace.step: returns when it is safe to add and remove objects.
	You can only schedule one post-step callback per context value, this prevents you from accidentally removing an object twice. Registering a second callback for the same object will replace the first.
  
  The method signature of the method should be:
  @code
- (void)postStepCallback:(id)key</code></pre>
	@endcode
	
  This makes it easy to call a removal method on your game controller to remove a game object that died or was destroyed as the result of a collision:
  @code
[space addPostStepCallback:gameController selector:@selector(remove:) key:gameObject];
	@endcode
	
	@attention Not to be confused with post-solve collision handler callbacks.
	@warning @c target and @c object cannot be retained by the ChipmunkSpace. If you need to release either after registering the callback, use autorelease to ensure that they won't be deallocated until after [ChipmunkSpace step:] returns.
	@see ChipmunkSpace.addPostStepRemoval:
*/
- (void)addPostStepCallback:(id)target selector:(SEL)selector key:(id)key;

/// Remove the Chipmunk Object from the space at the end of the step:
- (void)addPostStepRemoval:(id <ChipmunkObject>)obj;

/// Returns a NSArray of all shapes that overlap the given point. The point is treated as having the given group and layers. 
- (NSArray *)pointQueryAll:(cpVect)point layers:(cpLayers)layers group:(id)group;

// TODO?
//- (void)pointQuery:(cpVect)point layers:(cpLayers)layers group:(id)group target:(id)target selector:(SEL)selector;

/// Returns the first shape that overlaps the given point. The point is treated as having the given group and layers. 
- (ChipmunkShape *)pointQueryFirst:(cpVect)point layers:(cpLayers)layers group:(id)group;

/// Return a NSArray of ChipmunkSegmentQueryInfo objects for all the shapes that overlap the segment. The objects are unsorted.
- (NSArray *)segmentQueryAllFrom:(cpVect)start to:(cpVect)end layers:(cpLayers)layers group:(id)group;

// TODO?
//- (void)segmentQueryFrom:(cpVect)start to:(cpVect)end layers:(cpLayers)layers group:(id)group target:(id)target selector:(SEL)selector;

/// Returns the first shape that overlaps the given segment. The segment is treated as having the given group and layers. 
- (ChipmunkSegmentQueryInfo *)segmentQueryFirstFrom:(cpVect)start to:(cpVect)end layers:(cpLayers)layers group:(id)group;

/// Returns a NSArray of all shapes whose bounding boxes overlap the given bounding box. The box is treated as having the given group and layers. 
- (NSArray *)bbQueryAll:(cpBB)bb layers:(cpLayers)layers group:(id)group;

// TODO?
//- (void)bbQuery:(cpBB)bb layers:(cpLayers)layers group:(id)group target:(id)target selector:(SEL)selector;

/// Returns a NSArray of ChipmunkShapeQueryInfo objects for all the shapes that overlap @c shape.
- (NSArray *)shapeQueryAll:(ChipmunkShape *)shape;

/// Returns true if the shape overlaps anything in the space.
- (BOOL)shapeTest:shape;


/// Perform a shape query for shape and call cpBodyActivate() for everythnig it touches.
- (void)activateShapesTouchingShape:(ChipmunkShape *)shape;

/// Retune the performance of the static shapes hash. See the C API documentation for a detailed description.
- (void)resizeStaticHashWithDim:(cpFloat)dim andCount:(int)count;

/// Retune the performance of the active shapes hash. See the C API documentation for a detailed description.
- (void)resizeActiveHashWithDim:(cpFloat)dim andCount:(int)count;

/// Update all the static shapes.
- (void)rehashStatic;

/// Update the collision info for a single shape.
/// Can be used to update individual static shapes that were moved or active shapes that were moved that you want to query against.
- (void)rehashShape:(ChipmunkShape *)shape;

/// Step time forward. While variable timesteps may be used, a constant timestep will allow you to reduce CPU usage by using fewer iterations.
- (void)step:(cpFloat)dt;

@end
