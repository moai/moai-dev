@class ChipmunkSpace;

/**
	Allows you to add composite objects to a space in a single method call.
	The easiest way to implement the ChipmunkObject protocol is to add a @c chipmunkObjects instance variable with a type of @c NSSet* to your class,
	create a synthesized property for it, and initialize it with the ChipmunkObjectFlatten() function.
*/
@protocol ChipmunkObject

/// Returns a set of ChipmunkBaseObject objects.
- (NSSet *)chipmunkObjects;

@end

/// Build a flattened set of ChipmunkBaseObjects from a list of ChipmunkObjects. Don't forget to @c nil terminate the list.
NSSet * ChipmunkObjectFlatten(id <ChipmunkObject> firstObject, ...);

/**
	This protocol is implemented by objects that know how to add themselves to a space.
	It's used internally as part of the ChipmunkObject protocol. You should never need to implement it yourself.
*/
@protocol ChipmunkBaseObject <ChipmunkObject>

@property (assign) id data;

- (void)addToSpace:(ChipmunkSpace *)space;
- (void)removeFromSpace:(ChipmunkSpace *)space;

@end
