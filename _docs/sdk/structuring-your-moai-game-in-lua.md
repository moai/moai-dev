---
title: "Structuring Your Moai Game in Lua"
order: -800
---

Moai's object model is so flat and componentized and the Lua programming languages is so minimalist and permissive that some developers experiences a sort of 'decompression sickness' when faced with the task of learning it. Even if it's clear what each individual part does, the lack of an overall structure or system can be disorienting.

The problem is largely philosophical: as software developers we're trained to think in terms of rigid systems of objects, design patterns and re-usable code. You might set out with the great idea to build a system of classes with clearly defined interfaces that inherit from one another when specialized behavior is needed. You might model all this in Lua using any number of class and inheritance patterns to try and enforce your vision. But, if you're perceptive, you'll soon start to suspect that all of the work you're doing to build an architecture really doesn't add much practical value. You may find yourself in the situation of having spent a lot of time defining a traditional OOP-style inheritance hierarchy and a big pile of fancy data structures only to realize you've created nothing more than an especially convoluted way of initializing tables.

Fortunately, there's an easy solution: don't do it. Don't spend your valuable time trying to build a big, rigid architecture in Lua. Instead, write only what you need as you need it and try to avoid creating dependencies in your code as much as possible. The fewer 'systems' you invest in the fewer dependencies you'll have and the smaller your maintenance headache will be. If you can break your game down into discreet implementations with as little shared state as possible you will find it much easier to modify, maintain and even delegate your work. In other words, weigh the cost of implementing architecture against the benefit of reusable code. In a language as lean and expressive language as Lua, it seldom pencils out.

Of course there are a few systems that most games find useful. The trick is to make these as high level as possible and to be vigilant in limiting their scope to do the bare minimum required to get the job done. All of the systems we're about to discuss can be implemented (in their simplest forms) with a few paragraphs (or even lines) of Lua code.

The archetypal systems we've seen emerge (and use ourselves) are:

-   The Resource Cache
-   The Rig
-   The Scene Manager

The Resource Cache
------------------

The Resource Cache is responsible for sharing immutable resources. Immutable resources are usually things like textures, sounds, grids, meshes - in short, data objects.

The simplest Resource Cache implementation is usually a weak table with a few factory methods. The factory methods check the weak table for instances of the resources and, if they don't exist, create them and store them in the weak table before returning them. If a mutable version of a resource is needed, a method may be provided to load it directly and not store it in the cache. Since resources are held in the weak table by key/value pairs, the cache doesn't need to know anything about the types of resources it contains. In a minimalist embodiment the cache may not even include factory methods: a key generation and factory method for each resource type could be maintained independently from the cache, thereby removing all presumption and dependency from the implementation.

The Rig
-------

In our internal studio nomenclature, a 'rig' is just a table of Moai objects, miscellaneous fields and methods. In other words, a table that contains at least one Moai framework object and pretty much anything else. A rig may represent a character, a group of characters, a level, a user interface widget, or whatever. Every 'object' we deal with in a game is a rig and rigs include everything needed to implement the object: resources, nodes, actions, coroutines, variables, etc.

We don't do anything up front to differentiate between rigs. From the perspective of methods that operates on a rigs, if a given rig knows how to 'walk' and 'quack' then it must be a duck. As long as the method checks up front, it may be applied to any kind of rig. This approach is known as 'duck typing' and is a powerful, composition based may of thinking about objects readily available in dynamically typed languages such as Lua.

Rigs are prepared by initializers that specifically name the components of the game they represeny. For example, if we're writing Super Mario Bros. we might have factory methods like initMario (), initGoomba (), initLevel (), etc. The emphasis here is on specificity: the initMario () method might internally call a more generic initSprite () that adds some standard objects and methods expected by the rest of the game but, after this, the Mario initializer would then go on to decorate the rig with whatever objects, methods, resources and actions are specific to Mario.

The Scene Manager
-----------------

The job of the Scene Manager is to maintain lists of rigs and to perform batch operations on them such as registering their members to be displayed by the rendering system or processed by the physics system. What constitutes a 'Scene' and what kinds of batch operations need to be performed on the rigs therein are highly game dependent and are subject to change. Usually there will be just a very few methods that the Scene Manager looks for and these methods will be provided by rig initializers or implemented in an ad hoc fashion per rig.

Of course, the Scene Manager could be further abstracted to a set of tables and some methods that perform batch operations on their contents. This would probably be a more Moai-like approach, but it's hard to walk away from the notion of a 'scene' even though that construct is often more semantic than practical. The things we typically think about a 'scene' doing (like moving batches of objects on and of screen or building a render stack) could just as easily be done in an ad hoc fashion by stateless methods in a library operating on lists.

For an example of a scene manager, see [the WolfClicker state-manager](https://github.com/moai/moai-dev/blob/master/samples-legacy/tutorials/WolfClicker/modules/state-manager.lua).

Paradigm Shift
--------------

If you've been paying attention you'll realize that the above isn't much more than hand-waving. We've been asked a lot about the best way to structure a Moai game in Lua and have done our best to dodge the question because we just don't think there's a lot of 'there' there. The best answer to "How should I structure my game?" may be "As little as possible" or even "Don't do it at all."

So what's the take-away?

-   Avoid writing big systems and data structures.
-   Only code what you need when you need it.
-   Favor code re-use through stateless libraries of functions instead of stateful systems and classes.
-   Use composition, not inheritance.
-   Use 'duck typing' - deduce the capabilities of objects at runtime.
-   Use initializers to build up rigs with predetermined behaviors.
-   Include rig members by convention and clearly document what operator methods will look for in a rig when executing.
-   Divide and conquer: approach your game as a series of special cases.
-   Avoid dependencies and tangles: these make your code rigid and hard to maintain.

While this might seem confusing at first, one you get the hang of it you'll find you can write generic and powerful systems that are surprisingly concise and easy to maintain. If you embrace this approach you'll think less about architecture and more about 'what happens next' in your game; in other words, more time writing game logic and less time managing data structures.
