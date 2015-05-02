---
title: "Binding C++ APIs to Moai using MOAILuaObject"
---

In this tutorial we'll cover techniques for binding instance objects (objects allocated with the 'new' operator) as well as singleton objects to Lua using MOAILuaObject.

For a discussion of how to add support for events and callback registration using setEvent () to instance and class objects and how to implement common objects such as nodes, props and decks see Advanced C++ Object Implementation using MOAILuaObject.

For a discussion of how Moai's C++ to Lua binding works see [How Moai's C++ to Lua Binding Works](../reference/how-moai's-c-to-lua-binding-works.html).

How to Bind Instance Objects to Lua
-----------------------------------

Let's look at a simple example object. As of Moai 1.0 r4, MOAIFoo is included as a template for you to use when adding classes to Moai. Just copy the .cpp and .h files, rename and start adding your own methods.

```objc
//================================================================//
// MOAIFoo
//================================================================//
class MOAIFoo :
    public virtual MOAILuaObject {
private:
 
    //----------------------------------------------------------------//
    static int      _classHello         ( lua_State* L );
    static int      _instanceHello      ( lua_State* L );

public:
 
    DECL_LUA_FACTORY ( MOAIFoo )

    //----------------------------------------------------------------//
                    MOAIFoo             ();
                    ~MOAIFoo            ();
    void            RegisterLuaClass    ( MOAILuaState& state );
    void            RegisterLuaFuncs    ( MOAILuaState& state );
};
```

As you can see, this class inherits from MOAILuaObject. As we make use of multiple inheritance, our convention is to always use virtual inheritance for MOAILuaObject.

Methods we intend to expose to Lua are private, static methods. We include them in the class so they will have access to its other private methods and constants. They are static so that their calling convention will allow them to be passed into Lua.

The DECL\_LUA\_FACTORY macro declares implementations for the abstract virtual methods required by MOAILuaObject. These methods are GetLuaClass (), RegisterLuaType () and TypeName (). As these methods are easily implemented using boilerplate code, it made sense to make a macro. The sole parameter passed to DECL\_LUA\_FACTORY is the class name.

Now let's look at the implementation:

```objc
#include <moaicore/MOAIFoo.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/** @name   classHello
    @text   Class (a.k.a. static) method. Prints the string 
            'MOAIFooclass foo!' to the console.

    @out    nil
*/
int MOAIFoo::_classHello ( lua_State* L ) {
    UNUSED ( L );
 
    printf ( "MOAIFoo class hello!\n" );
 
    return 0;
}

//----------------------------------------------------------------//
/** @name   instanceHello 
    @text   Prints the string 'MOAIFoo instance foo!' to the console.

    @out    nil
*/
int MOAIFoo::_instanceHello ( lua_State* L ) {

    // this macro initializes the 'self' variable 
    // and type checks arguments
    MOAI_LUA_SETUP ( MOAIFoo, "U" )
 
    printf ( "MOAIFoo instance hello!\n" );
 
    return 0;
}

//================================================================//
// MOAIFoo
//================================================================//

//----------------------------------------------------------------//
MOAIFoo::MOAIFoo () {
 
    // register all classes MOAIFoo derives from
    // we need this for custom RTTI implementation
    RTTI_BEGIN
        RTTI_EXTEND ( MOAILuaObject )
   
        // and any other objects from multiple inheritance...
        // RTTI_EXTEND ( MOAIFooBase )
    RTTI_END
}

//----------------------------------------------------------------//
MOAIFoo::~MOAIFoo () {
}

//----------------------------------------------------------------//
void MOAIFoo::RegisterLuaClass ( MOAILuaState& state ) {

    // call any initializers for base classes here:
    // MOAIFooBase::RegisterLuaClass ( state );

    // also register constants:
    // state.SetField ( -1, "FOO_CONST", ( u32 )FOO_CONST );

    // here are the class methods:
    luaL_Reg regTable [] = {
        { "classHello",     _classHello },
        { NULL, NULL }
    };

    luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFoo::RegisterLuaFuncs ( MOAILuaState& state ) {

    // call any initializers for base classes here:
    // MOAIFooBase::RegisterLuaFuncs ( state );

    // here are the instance methods:
    luaL_Reg regTable [] = {
        { "instanceHello",  _instanceHello },
        { NULL, NULL }
    };

    luaL_register ( state, 0, regTable );
}
```

That's all there is to it. The Lua methods at the top of the file show an example of a 'class' method and an 'instance' method. The class method may be invoked by calling MOAIFoo.classHello () and the instance method by calling foo:hello () where 'foo' is an instance of MOAIFoo created by calling MOAIFoo.new ().

The methods RegisterLuaClass () and RegisterLuaFuncs () both take a Lua table. MOAILuaState is a helper class that wrapps lua\_State. It is overloaded to return a pointer to lua\_State when cast, so you may use it with the Lua C API. RegisterLuaClass () receives the class table to which any addition class methods and constants should be added. RegisterLuaFuncs () receives the instance table which will be shared as a metatable of any instances of MOAIFoo. Add the class instance interface to the instance table.

When writing an instance method you can use the MOAI\_LUA\_SETUP macro to initialize a local variable called 'self' which will point at the instance of the class as well as to check any parameters to the method. The first parameter is traditionally 'U' (for userdata). You can find the declaration of MOAI\_LUA\_SETUP in MOAILogMessages.h and the implementation of CheckParams (), which MOAI\_LUA\_SETUP calls, in MOAILuaSetup.cpp.

The final item to note is the use of the RTTI macros in MOAIFoo's constructor. Because Moai implements a custom RTTI system, you need to register any classes inherited by your class in its constructor. The class inheritance registration block begins with RTTI\_BEGIN and ends with RTTI\_END. Each class inherited should be declared using RTTI\_EXTEND ().

To make the class available to the Lua runtime, just call:

```objc
REGISTER_LUA_CLASS ( MOAIFoo )
```

You can call this anywhere in your code as long as it's called before the class is needed by a script. For an example, see moaicore.cpp.

How to Bind Singletons to Lua
-----------------------------

Binding a singleton to Lua is a variation of binding an instance object. Usually a singleton has some global state associated with it, but you can also use singletons as a lazy way to bind the global methods of an API. We are very lazy, so we do this a lot.

Note that in Moai, 'global' and 'singleton' are almost always scoped to the current Moai context. In this regards they aren't true 'globals' in that they are allocated by the memory manager and will be swapped out if the context is changed. We use the singleton pattern to avoid having to pass around references to manager or context objects, thereby simplifying some aspects of our API. As previously mentioned, we are very lazy.

Here's the header of an example singleton:

```objc
#include <moaicore/MOAILua.h>

//================================================================//
// MOAIFooMgr
//================================================================//
/**  @name   MOAIFooMgr
 @text   Example singleton for extending Moai using MOAILuaObject.
               Copy this object, rename it and add your own stuff.
               Just don't forget to register it with the runtime
               using the REGISTER_LUA_CLASS macro (see moaicore.cpp).
*/
class MOAIFooMgr :
 public MOAIGlobalClass < MOAIFooMgr, MOAILuaObject > {
private:
 
    //----------------------------------------------------------------//
    static int      _singletonHello     ( lua_State* L );

public:
 
    DECL_LUA_SINGLETON ( MOAIFooMgr )

 //----------------------------------------------------------------//
                    MOAIFooMgr          ();
                    ~MOAIFooMgr         ();
    void            RegisterLuaClass    ( MOAILuaState& state );
};
```

MOAIGlobalClass \<\> is a template base class that integrates with Moai's context and singleton management system. It's parameters are the name of the class itself and a class it will inherit from (in this case MOAILuaObject).

DECL\_LUA\_SINGLETON is the singleton's analogue to DECL\_LUA\_FACTORY. It implements the same abstract virtual methods DECL\_LUA\_FACTORY as but provides the machinery to interface with a singleton instead of an instance.

As this class is a singleton, there is no need for the RegisterLuaFuncs () method.

Here's the implementation:

```objc
#include <moaicore/MOAIFooMgr.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/** @name   _singletonHello
    @text   Prints the string 'MOAIFooMgr singleton foo!' to the console.

    @out    nil
*/
int MOAIFooMgr::_singletonHello ( lua_State* L ) ( lua_State* L ) {
    UNUSED ( L );

    printf ( "MOAIFooMgr singleton foo!\n" );
 
    return 0;
}

//================================================================//
// MOAIFooMgr
//================================================================//

//----------------------------------------------------------------//
MOAIFooMgr::MOAIFooMgr () {
 
    // register all classes MOAIFooMgr derives from
    // we need this for custom RTTI implementation
    RTTI_BEGIN
        RTTI_EXTEND ( MOAILuaObject )
   
        // and any other objects from multiple inheritance...
        // RTTI_EXTEND ( MOAIFooMgrBase )
    RTTI_END
}

//----------------------------------------------------------------//
MOAIFooMgr::~MOAIFooMgr () {
}

//----------------------------------------------------------------//
void MOAIFooMgr::RegisterLuaClass ( MOAILuaState& state ) {

    // call any initializers for base classes here:
    // MOAIFooBase::RegisterLuaClass ( state );

    // also register constants:
    // state.SetField ( -1, "FOO_CONST", ( u32 )FOO_CONST );

    // here are the class methods:
    luaL_Reg regTable [] = {
        { "singletonHello",     _singletonHello },
        { NULL, NULL }
    };

    luaL_register ( state, 0, regTable );
}
```

As you can see, it is virtually identical to the implementation of an instance class save the omission of RegisterLuaFuncs ().

Just like an instance class you must also register the singleton with the runtime using REGISTER\_LUA\_CLASS :

```objc
REGISTER_LUA_CLASS ( MOAIFooMgr )
```

After registration, your singleton (or API) will be available for use in Lua.

Organizing your Code
--------------------

We do not recommend adding extension classes directly to moaicore.cpp or any of the files that ship with Moai. Instead, create a separate C++ file for your extension classes and register them there. This will make it easier for you to keep up to date with new versions of Moai.

Notes and Future Changes
------------------------

The virtual function name 'RegisterLuaFuncs ()' is a bit misleading as 'RegisterLuaClass ()' may also register functions. In fact, each method is just passed a Lua table placed at the top of Lua's stack for you to initialize. In addition, the choice to pass a MOAILuaState instead of a lua\_State confronts you with Moai's helper class, which you may have no use for - there is nothing that can be done with MOAILuaState that can't be done with the Lua API.

At some point the future we may rename RegisterLuaClass () and RegisterLuaFuncs () to InitLuaClassTable () and InitLuaInstanceTable () and pass in a pointer to lua\_State instead of a reference to MOAILuaState.

Another quirk is the ability to pass a base class to the MOAIGlobalClass \<\> template. This is a throwback to a time before we embraced multiple inheritance but needed to implement helper base classes via templates. In the future, MOAIGlobalClass () should virtual inherit MOAIObject and leave it to the child class to virtually inherit MOAILuaObject.

None of these changes are pressing as the current system works and they would be only cosmetic. Just be aware as you implement your extensions that a future release may require some find and replace work on your part.
