// TODO headerdoc the moment functions

/**
	Rigid bodies are the basic unit of simulation in Chipmunk.
	They hold the physical properties of an object (mass, position, rotation, velocity, etc.). After creating a ChipmunkBody object, you can attach collision shapes (ChipmunkShape) and joints (ChipmunkConstraint) to it.
*/
@interface ChipmunkBody : NSObject <ChipmunkBaseObject> {
@private
	cpBody _body;
	id data;
}

/**
  Create an autoreleased rigid body with the given mass and moment.
  Guessing the moment of inertia is usually a bad idea. Use the moment estimation functions (cpMomentFor*()).
*/
+ (id)bodyWithMass:(cpFloat)mass andMoment:(cpFloat)moment;

/**
  Create an autoreleased rigid body with infinite mass and moment.
  This is most useful when you want to create a rogue body for controlling a moving platform or some other strictly controlled object.
*/
+ (id)staticBody;

/**
  Initialize a rigid body with the given mass and moment of inertia.
  Guessing the moment of inertia is usually a bad idea. Use the moment estimation functions (cpMomentFor*()).
*/
- (id)initWithMass:(cpFloat)mass andMoment:(cpFloat)moment;

/**
  Initialize a rigid body with infinite mass and moment.
  This is most useful when you want to create a rogue body for controlling a moving platform or some other strictly controlled object.
*/
- (id)initStaticBody;

/// Mass of the rigid body. Mass does not have to be expressed in any particular units, but relative masses should be consistent.
@property cpFloat mass;

/// Moment of inertia of the body. The mass tells you how hard it is to push an object, the MoI tells you how hard it is to spin the object. Don't try to guess the MoI, use the cpMomentFor*() functions to try and estimate it.
@property cpFloat moment;

/// The position of the rigid body's center of gravity.
@property cpVect pos;

/// The linear velocity of the rigid body.
@property cpVect vel;

/// The linear force applied to the rigid body. Unlike in some physics engines, the force does not reset itself during each step. Make sure that you are reseting the force between frames if that is what you intended.
@property cpVect force;

/// The rotation angle of the rigid body in radians.
@property cpFloat angle;

/// The angular velocity of the rigid body in radians per second.
@property cpFloat angVel;

/// The torque being applied to the rigid body. Like force, this property is not reset every frame.
@property cpFloat torque;

/// A unit length vector vector that stores the current rotation of the body as a 2D spinor or complex number. Can be used for fast rotation.
@property (readonly) cpVect rot;

/// Returns a pointer to the underlying cpBody C struct.
@property (readonly) cpBody *body;

/**
	An object that this constraint is associated with. You can use this get a reference to your game object or controller object from within callbacks.
	@attention Like most @c delegate properties this is a weak reference and does not call @c retain. This prevents reference cycles from occuring.
*/
@property (assign) id data;

/// Has the body been put to sleep by the space?
@property (readonly) bool isSleeping;

/// Has the body <strong>not</strong> been added to any spaces?
@property (readonly) bool isRogue;

/// Is the body the static body associated with a space?
@property (readonly) bool isStatic;

/**
  Convert from body local to world coordinates.
  Convert a point in world (absolute) coordinates to body local coordinates affected by the position and rotation of the rigid body.
*/
- (cpVect)local2world:(cpVect)v;

/**
  Convert from world to body local Coordinates.
  Convert a point in body local coordinates coordinates to world (absolute) coordinates.
*/
- (cpVect)world2local:(cpVect)v;

/**
  Reset force and torque.
  Set the force on this rigid body to cpvzero, and set the torque to 0.0.
*/
- (void)resetForces;

/**
  Apply a force to a rigid body. An offset of cpvzero is equivalent to adding directly to the force property.
  @param force A force in expressed in absolute (word) coordinates.
	@param offset An offset expressed in world coordinates. Note that it is still an offset, meaning that it's position is relative, but the rotation is not.
*/
- (void)applyForce:(cpVect)force offset:(cpVect)offset;

/**
  Apply an impulse to a rigid body.
  @param impulse An impulse in expressed in absolute (word) coordinates.
	@param offset An offset expressed in world coordinates. Note that it is still an offset, meaning that it's position is relative, but the rotation is not.
*/
- (void)applyImpulse:(cpVect)impulse offset:(cpVect)offset;

/// Wake up the body if it's sleeping, or reset the idle timer if it's active.
- (void)activate;

/**
	Force the body to sleep immediately. The body will be added to the same group as @group. When any object in a group is woken up, all of the bodies are woken up with it.
	If @c group is nil, then a new group is created and the body is added to it. It is an error pass a non-sleeping body as @c group.
	This is useful if you want an object to be inactive until something hits it such as a pile of boxes you want the player to plow through or a stalactite hanging from a cave ceiling.
	Make sure the body is fully set up before you call this. Adding this body or any shapes or constraints attached to it to a space, or modifying any of their properties automatically wake a body up.
*/
- (void)sleepWithGroup:(ChipmunkBody *)group;

/**
	Equivalent to [ChipmunkBody sleepWithGroup:nil]. That is the object is forced to sleep immediately, but is not grouped with any other sleeping bodies.
*/
- (void)sleep;

/// Implements the ChipmunkBaseObject protocol, not particularly useful outside of the library code
- (void)addToSpace:(ChipmunkSpace *)space;
/// Implements the ChipmunkBaseObject protocol, not particularly useful outside of the library code
- (void)removeFromSpace:(ChipmunkSpace *)space;

@end
