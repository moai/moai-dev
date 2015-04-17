---
title: "How Moai's C++ to Lua Binding Works"
---

Moai implements its own system for binding C++ objects to the Lua runtime. Our goals when implementing the system were to allow instances of C++ objects to be created from Lua and to be managed by Lua's garbage collector. At the same time we did not want the ownership by Lua to be mandatory or permanent: objects may be created by the new () operated and never exposed to objects, or ownership of objects may be passed back and forth between Lua and the C++ runtime.

The Metatable Stack
-------------------

Lua bound objects are represented by Lua userdata. The userdata simply contains a pointer to the instance of the C++ object. The userdata has three metatables attached to it: the Member Table, the Ref Table and the Interface Table:

userdata --\> { Member Table } --\> { Ref Table } --\> { Interface Table }

The metamethods are set up so that \_\_index will first check the Member Table then the Interface Table. The \_\_newindex metamethod will only alter the Member Table. The Ref Table is use internally by Moai and therefore omitted altogether.

The \_\_gc metamethod is handled by MOAILuaObject::\_gc amd the \_\_tostring metamethod is handled by MOAILuaObject::\_tostring.

The Class Table
---------------

An object' Class Table is not part of its metatable stack but is instead a global table registered in Lua's runtime. If the Class Table belongs to an instance object then it will contain a factory method to create instances of the object. The default factory method is new (). The Class Table will also contain any relevant class methods and constants.

MOAILuaObject
-------------

Any C++ class exposed to Lua must inherit from MOAILuaObject. This includes instance objects and singletons. MOAILuaClass inherits MOAIObject which implements Moai's reference counting and custom RTTI. The userdata registered with the Lua runtime points to the MOAILuaObject.

MOAILuaObject contains the helper method PushLuaUserdata () which will push the userdata associated with the instance of the object onto Lua's stack, or create (and push) a userdata if none exists. You can use PushLuaUserdata () to put a MOAILuaObject onto Lua's stack and MOAILuaState::GetLuaObject \<\>() to return (via dynamic cast) a pointer to an object from an element on Lua's stack.

MOAILuaClass, MOAILuaFactoryClass \<\> and MOAILuaSingletonClass \<\>
---------------------------------------------------------------------

MOAILuaClass is the base class for MOAILuaFactoryClass \<\> and MOAILuaSingletonClass \<\>. These classes contain the information needed to instantiate and manage Lua bound C++ instance objects and singletons. They also contain methods to register an object's Class Table into Lua's runtime as a global.

An pointer to a MOAILuaObject's MOAILuaClass must be returned by MOAILuaObject's GetLuaClass () method. The typical approach is to implement a MOAILuaFactoryClass \<\> or MOAILuaSingletonClass \<\> for an object as a global or singleton and to return a pointer to that. As a convenience, both MOAILuaFactoryClass \<\> and MOAILuaSingletonClass \<\> implement a static Get () method that uses MOAIGlobalsMgr to create a singleton and the DECL\_LUA\_FACTORY macro may be called inside of the declaration of a MOAILuaObject subclass to implement GetLuaClass ().

Reference Counting and Lua's Garbage Collector
----------------------------------------------

Making MOAIObject's reference counting system work with Lua's garbage collector requires some trickery. To understand it, we must first discuss how Moai tracks references to Lua objects.

MOAILuaRef is a C++ smart pointer that may be used to create either weak or strong references to objects in Lua. This is accomplished by using global tables in Lua. Strong references go into one table and weak references another (set up to be weak). The references are identified by index. References may also be moved between the weak and the strong tables.

Each instance of a MOAILuaObject keeps a reference to its Lua userdata so that we can easily access it, for example to pass to a callback. If we kept a strong reference to the userdata, then Lua would never be able to garbage collect it: the object keeps an index into a table which in turn references the object. As the table is global, the object will always be 'in scope' and will never be tagged for collection. To solve this, we keep a weak reference so that the object may be tagged for garbage collection if nothing else in Lua references it.

When a Moai reference counts an object, we do not want Lua to garbage collect it. Fortunately the solution is simple: when an object has a reference count greater than 0, we convert its internal reference to the userdata to a strong reference. When the reference count reaches 0, we convert the reference to the userdata back to a weak reference. If the reference count reaches 0 and no userdata exists, this means the object is not bound to Lua and we delete it immediately as we normally would.
