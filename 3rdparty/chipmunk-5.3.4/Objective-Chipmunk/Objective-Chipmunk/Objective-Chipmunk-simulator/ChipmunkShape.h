/// Abstract base class for collsion shape types.
@interface ChipmunkShape : NSObject <ChipmunkBaseObject> {
@public
	id data;
}

/// Returns a pointer to the underlying cpShape C struct.
@property (readonly) cpShape *shape;

/// The ChipmunkBody that this shape is attached to.
@property (retain) ChipmunkBody *body;

/// The axis-aligned bounding box for this shape.
@property (readonly) cpBB bb;

/// Sensor shapes send collision callback messages, but don't create a collision response.
@property BOOL sensor;

/// How bouncy this shape is.
@property cpFloat elasticity;

/// How much friction this shape has.
@property cpFloat friction;

/**
	The velocity of the shape's surface.
	This velocity is used in the collision response when calculating the friction only.
*/
@property cpVect surfaceVel;

/**
	An object reference used as a collision type identifier. This is used when defining collision handlers.
	@attention Like most @c delegate properties this is a weak reference and does not call @c retain.
*/
@property (assign) id collisionType;

/**
	An object reference used as a collision group identifier. Shapes with the same group do not collide.
	@attention Like most @c delegate properties this is a weak reference and does not call @c retain.
*/
@property (assign) id group;

/// A layer bitmask that defines which objects 
@property (assign) cpLayers layers;

/**
	An object that this shape is associated with. You can use this get a reference to your game object or controller object from within callbacks.
	@attention Like most @c delegate properties this is a weak reference and does not call @c retain. This prevents reference cycles from occuring.
*/
@property (assign) id data;

/// Update and cache the axis-aligned bounding box for this shape.
- (cpBB)cacheBB;

/// Check if a point in absolute coordinates lies within the shape.
- (bool)pointQuery:(cpVect)point;

@end


/// Holds collision information from segment queries. You should never need to create one.
@interface ChipmunkSegmentQueryInfo : NSObject {
@private
	cpSegmentQueryInfo _info;
	cpVect _start, _end;
}

- (id)initWithStart:(cpVect)start end:(cpVect)end;

/// Returns a pointer to the underlying cpSegmentQueryInfo C struct.
@property (readonly) cpSegmentQueryInfo *info;

/// The ChipmunkShape found.
@property (readonly) ChipmunkShape *shape;

/// The percentage between the start and end points where the collision occurred.
@property (readonly) cpFloat t;

/// The normal of the collision with the shape.
@property (readonly) cpVect normal;

/// The point of the collision in absolute (world) coordinates.
@property (readonly) cpVect point;

/// The distance from the start point where the collision occurred.
@property (readonly) cpFloat dist;

/// The start point.
@property (readonly) cpVect start;

/// The end point.
@property (readonly) cpVect end;

@end


/// Holds collision information from segment queries. You should never need to create one.
@interface ChipmunkShapeQueryInfo : NSObject {
@private
	ChipmunkShape *_shape;
	cpContactPointSet _contactPoints;
}

- (id)initWithShape:(ChipmunkShape *)shape andPoints:(cpContactPointSet *)set;

@property (readonly) ChipmunkShape *shape;
@property (readonly) cpContactPointSet *contactPoints;

@end


/// A perfect circle shape.
@interface ChipmunkCircleShape : ChipmunkShape {
@private
	cpCircleShape _shape;
}

/// Create an autoreleased circle shape with the given radius and offset from the center of gravity.
+ (id)circleWithBody:(ChipmunkBody *)body radius:(cpFloat)radius offset:(cpVect)offset;

/// Initialize a circle shape with the given radius and offset from the center of gravity.
- (id)initWithBody:(ChipmunkBody *)body radius:(cpFloat)radius offset:(cpVect)offset;

/// The radius of the circle.
@property (readonly) cpFloat radius;

/// The offset from the center of gravity.
@property (readonly) cpVect offset;

@end


/// A beveled (rounded) segment shape.
@interface ChipmunkSegmentShape : ChipmunkShape {
@private
	cpSegmentShape _shape;
}

/// Create an autoreleased segment shape with the given endpoints and radius.
+ (id)segmentWithBody:(ChipmunkBody *)body from:(cpVect)a to:(cpVect)b radius:(cpFloat)radius;

/// Initialize a segment shape with the given endpoints and radius.
- (id)initWithBody:(ChipmunkBody *)body from:(cpVect)a to:(cpVect)b radius:(cpFloat)radius;

/// The start of the segment shape.
@property (readonly) cpVect a;

/// The end of the segment shape.
@property (readonly) cpVect b;

/// The normal of the segment shape.
@property (readonly) cpVect normal;

/// The beveling radius of the segment shape.
@property (readonly) cpFloat radius;

@end


/// A convex polygon shape.
@interface ChipmunkPolyShape : ChipmunkShape {
@private
	cpPolyShape _shape;
}

/// Create an autoreleased polygon shape from the given vertex and offset from the center of gravity.
+ (id)polyWithBody:(ChipmunkBody *)body count:(int)count verts:(cpVect *)verts offset:(cpVect)offset;

/// Create an autoreleased box shape centered on the center of gravity.
+ (id)boxWithBody:(ChipmunkBody *)body width:(cpFloat)width height:(cpFloat)height;

/// Initialize a polygon shape from the given vertex and offset from the center of gravity.
- (id)initWithBody:(ChipmunkBody *)body count:(int)count verts:(cpVect *)verts offset:(cpVect)offset;

/// Initialize a box shape centered on the center of gravity.
- (id)initBoxWithBody:(ChipmunkBody *)body width:(cpFloat)width height:(cpFloat)height;

/// The number of vertexes in this polygon.
@property (readonly) int count;

/// Access the vertexes of this polygon.
- (cpVect)getVertex:(int)index;

@end

/// A subclass of ChipmunkCircleShape that is added as a static shape when using ChipmunkSpace.add:.
@interface ChipmunkStaticCircleShape : ChipmunkCircleShape
@end


/// A subclass of ChipmunkSegmentShape that is added as a static shape when using ChipmunkSpace.add:.
@interface ChipmunkStaticSegmentShape : ChipmunkSegmentShape
@end

/// A subclass of ChipmunkPolyShape that is added as a static shape when using ChipmunkSpace.add:.
@interface ChipmunkStaticPolyShape : ChipmunkPolyShape
@end
