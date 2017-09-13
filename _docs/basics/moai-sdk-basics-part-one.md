---
title: "Moai SDK Basics Part One"
---

If you’re ready to take on the challenge of learning a more flexible and open framework then you presumably want better performance and/or the opportunity to customize and innovate, even down to writing custom assembly language if you need to. It can be a lot of work, but when you have a mission critical feature to ship or a performance hitch you have to fix, there’s nothing quite like having under-the-hood access to your system.

Moai offers you these possibilities, but there’s a learning curve: the object model is highly factored and pretty low level. Future tutorials will cover some ways of dealing with this, but this Moai Basics series examines the guts of the engine from the ground up.

The Lua Object Model
--------------------

The Moai object model is written in C++. Each object class is exposed through a global Lua class table of the same name. So if the C++ class is called MOAIFoo, there will also be a table called MOAIFoo in the global Lua context.

These tables contain class methods (global functions that don’t take an instance of the class as a parameter) as well as constants and factory methods. If a class is a singleton then its class table won’t contain any factory methods. Otherwise, there will typically be a method called new () that may be used to create new instances of the class.

The class tables are metatables on userdata and, as such, Lua will prevent you from changing their contents.

Instances of classes do not inherit the global class table. Instead, they are created with a commonly shared metatable that contains the instance methods of the class. Class instances also have a per-instance table that may be used for customization. You can freely add your own members or hide members in any instance of a class; this will not affect the other instances.

To access members of a class table, always use the dot operator:

```lua
MOAIFoo.new ()
MOAIFoo.SOME_CONST
```

To access members of an instance table, use the colon operator or (when calling methods) use the dot operator and pass in the instance:

```lua
foo = MOAIFoo.new ()
foo:doSomething ()
foo.doSomething ( foo )
foo.doSomething = myDoSomething -- this is OK
```

Moai framework objects will be garbage collected just like regular Lua objects. That said, they are also reference counted by the C++ runtime, so when you add them to Moai containers or systems that Moai manages, they will remain until no longer needed:

```lua
foo = MOAIFoo.new ()
container = MOAIFooContainer.new ()

container:insertFoo ( foo ) -- foo will be reference counted
foo = nil -- foo will not be garbage collected yet

container = nil -- now container and contents may be garbage collected
```

While Moai container objects typically reference count their members, inter-object relationships usually do not:

```lua
foo1 = MOAIFoo.new ()
foo2 = MOAIFoo.new ()
 
foo1:setSomeRelationship ( foo2 ) -- weak reference to foo2
foo2 = nil -- foo2 may be garbage collected
```

Some Moai objects inherit from other Moai objects. Keep this in mind when checking the reference. If one object inherits another, it may be used interchangeably with the object it inherits.

Setting Up
----------

Moai is designed to run inside a host application and render to an OpenGL surface. The surface is provided by the host application. When Moai renders, it may render to the entire surface or just a rectangular section of the surface. This section is called a viewport and you need to define one before you start rendering:

```lua
viewport = MOAIViewport.new ()
viewport:setSize ( 480, 320 )
viewport:setScale ( 480, 320 )
```

The size of the viewport is the viewport’s rectangle on the OpenGL surface provided by the host. The setSize () method is overloaded to accept the full rectangle:

```lua
viewport:setSize ( 100, 100, 200, 200 )
```

Viewports are defined in a typical ‘window style’ coordinate system in which the origin is in the upper left hand corner of the drawing surface and the Y-axis descends down the screen. The above code creates a viewport with its upper left hand corner at (100, 100) and its lower right hand corner at (200, 200).

To support resolution independence, Moai allows you to set a scalar on each axis of the viewport. The viewport scale determines how many ‘world units’ will be visible in the viewport. A ‘world unit’ is simply an arbitrary measure of scale that you may choose when describing the size of objects in your game. ‘World space’ is the coordinate system in which these objects interact in the simulation. When they are rendered to the screen, their world units are converted to pixels based on a scale.

Now, it may happen that your world units correspond to pixels:

```lua
viewport:setSize ( 480, 320 )
viewport:setScale ( 480, 320 ) – viewport will show 480 x 320 world units
```

This is an easy way to think, especially if you are coming from an art background. But remember that world units are something you can choose based in your own preferences:

```lua
viewport:setSize ( 480, 320 )
viewport:setScale ( 15, 10 ) -- viewport will show 15 x 10 world units
```

The above viewport will display a 1 x 1 ‘unit’ object as 32 x 32 pixels on the screen. You might do this if you want to describe your game in some other coordinate system – meters, for example, or maybe tiles if you are making a tile-based arcade game. In the previous example 1 ‘meter’ or ‘tile’ (it’s up to you) happens to be equivalent to 32 pixels.

One of the nice things about setting up your viewport this way is that you immediately solve a big part of the resolution independence problem:

```lua
-- retina display
viewport:setSize ( 960, 640 )
viewport:setScale ( 15, 10 ) -- viewport will still show 15 x 10 world units
```

As you can see, the size of the viewport changed, but not the scale. The contents of the viewport will simply stretch to fill the larger viewport. (I’ll talk about how to also show higher resolution assets later).

Moai’s default coordinate system is set up for simulation instead of UI, as intial development was focused toward simulation and arcade games. If you are coming from an authoring environment this might throw you for a loop.

In Moai, the origin of the world is at the center of the view, not the upper left hand corner. If you place an object in the world at the origin, you will see it sitting there in the center of your view. In addition, the Y axis is positive, moving up the screen instead of down. So a point with a Y value of 100 will be higher on the screen than a point with a Y value of 10. This matches the way we measure things in the real world: we measure height starting at the ground and increasing toward the sky, hence the label of a ‘simulation style’ coordinate system: when describing objects in it, you are using a system of measurement similar to that used in the real world.

There are times to use a window style coordinate system. For example, when laying out a UI you are probably most used to thinking in window style coordinates. You can do this with a little extra configuration:

```lua
viewport:setSize ( 480, 320 )
viewport:setScale ( 480, -320 ) -- use negative Y axis
viewport:setOffset ( -1, 1 ) -- offset projection origin by -1, 1 in OpenGL projection space
```

The above code snippet sets up a viewport with a window style coordinate system. The origin is at the upper left hand corner of the viewport and the Y axis increases down the screen. The viewport offset is set in projection space. Projection space is a 2 x 2 rectangle with an ascending Y axis. The value of ( -1, 1 ) moves the projection one half screen to the left and 1 half screen up, thereby placing the origin at the upper left hand corner of the view.

Decks and Props
---------------

Decks and props are Moai’s objects for describing geometry and placing that geometry in a scene.

Many options were considered before deciding on these names. The names that were finally chosen are admittedly far from ideal in terms of metaphor. The names that were closest to capturing the idea behind these objects were either already taken (‘set,’ ‘library,’ ‘instance’), too overloaded and narrow (‘sprite,’ ‘shape’) or overreaching (‘game object,’ ‘gob’).

‘Sprite’ was a near miss for ‘prop’ and ‘set’ was an alternate choice for ‘deck.’ Some detail on the thinking behind the final choices is provided below.

In Moai, a scene graph object is called a ‘Prop.’ A prop is a combination of a location in space and the geometry that is placed there. ‘Geometry’ here just means the ‘stuff’ that represents the object – the triangles, quads, splines – however the object is defined.

A prop is different (and heavier) than a plain location or transform in that it has dimensionality given by its geometry, whereas a coordinate or a transform has none. At the same time, a prop only references (or instances) the geometry it places in the world; the prop is not the same thing as the geometry and geometry may have a one-to-many relationship with props.

So a prop is similar to what you might understand a sprite to be, but it’s more than that. Conceptually, a prop might place a volume, sound or collision surface. While a prop could be the placement of anything that has dimensionality, the term ‘sprite’ is usually reserved for something purely visual. Some engines start out using the term ‘sprite’ but wind up adding lots of additional baked in behavior (ui events, physics) that start to subvert the classically understood meaning of the term.

As stated above, props only instance geometry. In Moai, geometry is held in ‘Decks.’ To visualize this, think of a deck of cards. Unfortunately, the metaphor breaks down almost immediately. With a real deck of cards, you can draw a card and place it on the table in front of you. When you do this it is no longer in the deck. Not the case with our Moai deck. Instead of placing the card itself on the table, you instance it… with a prop. In addition, you can instance the same card many times over by using many props: there’s a one to many relationship between an item in a deck and the props that instance it.

So, from the top, but a bit more concisely: in Moai, geometry is kept in decks. There are different types of deck for different types of geometry. Decks are indexed by number and may be instanced in the world by props. A single item in a deck may be instanced many times over.

And a bit more concretely: let’s say you wanted to make a texture atlas and render one of the images held in the atlas. You’d need a deck that can describe a texture atlas. The ‘geometry’ of the deck would be the vertices of each image in the atlas and their corresponding UV coordinates. Then you’d need a prop to instance that geometry in the world so you can render it and move it around. You’d connect the deck to the prop and set the prop’s index to display the geometry you care about.

Here's an example:

```lua
-- load the sprite sheet; this will be the source for our sprite
spriteSheet = MOAITileDeck2D.new ()
spriteSheet:setTexture ( ‘frames.png’ ) -- load a texture
spriteSheet:setSize ( 8, 8 ) -- subdivide the texture into 8 x 8 = 64 frames
spriteSheet:setRect ( -32, -32, 32, 32 ) -- set the world space dimensions of the sprites

-- create a sprite and initialize it
sprite = MOAIProp2D.new ()
sprite:setDeck ( spriteSheet )
sprite:setIndex ( 3 ) -- show the third image in the deck
```

If you’ve already looked through our samples, you’ve probably seen plenty of code that looks like this. This example intentionally goes against the standard class naming to try and illustrate the concept in more familiar terms.

You’ll notice that a MOAITileDeck2D as used as an image source. This is done here only because this deck type is easy to initialize. MOAITileDeck2D just subdivides an image into N x M tiles. You can also set a tile width and tile height in UV space (more about this later).

One quirk of the decision to bake the idea of indices directly into MOAIProp2D and MOAIDeck2D (as opposed to more specialized classes) is that even deck types meant to contain only a single item may also be indexed:

```lua
-- create a deck representing a single textured quad
texturedQuad = MOAIGfxQuad2D.new ()
texturedQuad:setTexture ( ‘test.png’ ) -- load an image to use as the quad’s texture
texturedQuad:setRect ( -32, -32, 32, 32 ) -- set the world space dimensions of the quad

-- create a sprite and initialize it
sprite = MOAIProp2D.new ()
sprite:setDeck ( texturedQuad )
sprite:setIndex ( 3 ) -- this index is ignored by MOAIGfxQuad2D
```

By convention, a deck with no indices just ignores the out of range index. Note that this is up to the individual deck type: MOAIGfxQuad2D ignores indices when drawing; other deck types may behave differently.

Moai convention is to omit the term ‘Deck’ from the name of decks that don’t support indices. For example, a MOAIFoo would be a single Foo; MOAIFooDeck would support multiple Foo’s you can look up by index.

As you’ve probably gathered, there’s a host of deck types to choose from depending on your needs. As of this writing, all of them are compatible with MOAIProp2D. Here’s a quick summary:

-   MOAIGfxQuad2D – A single textured quad.
-   MOAIGfxQuadDeck2D – An array of textured quads (from one texture). Use this for basic sprite sheets and texture atlases.
-   MOAIGfxQuadListDeck2D – An array of lists of textured quads (from one texture). Use this for advanced sprite sheets.
-   MOAIMesh – Can render a custom vertex buffer object.
-   MOAIStretchPatch2D – A single ‘patch’ that may contain stretchable rows and columns.
-   MOAISurfaceDeck2D – A remnant of Moai’s original physics system. Unused at this time; left in at this time for developer convenience.
-   MOAITileDeck2D – Subdivides a texture into N x M tiles that may then be indexed. Suitable for tile maps and sprite sheets where every sprite is the same size.

UV Coordinates
--------------

Graphics geometry in Moai is initialized using UV coordinates. Most likely you’re already familiar with UV mapping, UV coordinates and UV transforms.

Just be aware that Moai assumes you’re going to be working in a simulation style coordinate system and for this reason whenever default UV values are provided, Moai assumes vertices with higher Y values are lower in UV coordinate space. For example:

```lua
texturedQuad = MOAIGfxQuad2D.new () -- default UV coords are ( 0, 1, 1, 0 )
texturedQuad:setRect ( -32, -32, 32, 32 )
```

If you are working in a window style viewport coordinate system, you will need to explicitly set your UV coordinates to the reverse of this or your images may appear upside down:

```lua
texturedQuad = MOAIGfxQuad2D.new ()
texturedQuad:setRect ( 0, 0, 64, 64 )
texturedQuad:setUVRect ( 0, 0, 1, 1 )
```

UV Coordinates and Resolution Independence
------------------------------------------

One of the nice things about using UV coordinates is that you get the rest of the solution to resolution independence (the first part being the viewport scale, remember?).

If you set up your decks properly with UV coordinates then you can scale your textures at will. They will always render correctly no matter the resolution of the display. So, when you need to go from a regular display to a tablet or a retina display, just double your viewport size (but leave the viewport scale alone) and load double sized textures (for anything you want to render at the higher resolution). And you’re done. If you’ve set up your coordinate systems correctly, everything will Just Work ™.

Rendering Props in Layers
-------------------------

That covers props and decks, but there’s one more necessary thing to do before anything will show up on the screen.

Moai doesn’t render props directly. It instead renders layers. To set up rendering, you build a stack of layers. Each layer renders independently, one on top of the other and may have its own partitioning scheme for optimizing spatial queries and view culling (partitioning will be discussed in a future article).

Each layer needs to be associated with a viewport. This is mandatory for rendering. You may also associate the layer with a camera transform. If you don’t supply a camera transform, then an identity transform will be used.

When setting up a scene, you typically want to group as many props of a given type in each layer as makes sense. For example, for an arcade game you might want to use two layers: one for the characters and environment and one for the HUD (Heads Up Display). Since each layer can have its own viewport, you might use a simulation style coordinate system in the game layer and a window style coordinate system in the UI overlay layer. It’s really up to you.

Here’s an example that sets up a layer and renders a single prop:

```lua
-- open a window (not needed on mobile platforms)
MOAISim.openWindow ( ‘test’, 320, 480 )

-- create a viewport
viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

-- create a layer and sets its viewport
layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
 
-- push the layer onto the render stack
MOAISim.pushRenderPass ( layer )

-- create a deck representing a single textured quad
gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( ‘test.png’ ) -- load an image to use as the quad’s texture
gfxQuad:setRect ( -64, -64, 64, 64 ) -- set the dimensions of the quad

-- create a prop and initialize it
prop = MOAIProp2D.new ()
prop :setDeck (gfxQuad)

-- add the prop to the layer
layer:insertProp ( prop )
```

Transforms
----------

Moai uses affine transforms to position objects in space. If you have experience with other game engines, the concept is most likely familiar to you.

Moai props derive from transforms, but you can also create pure transforms:

```lua
transform = MOAITransform.new ()
transform:setLoc ( 100, 100 )
```

You can create a parent/child relationship between transforms:

```lua
transform1 = MOAITransform.new ()
transform2 = MOAITransform.new ()
transform2:setParent ( transform1 )
```

Child transforms will inherit the position, orientation and scale of their parents. Because MOAIProp2D has MOAITransform as a base class, you can also create a parent/child relationship between props and transforms:

```lua
transform = MOAITransform.new ()
prop = MOAIProp2D.new ()
prop:setParent ( transform )
```

And between props:

```lua
prop1 = MOAIProp2D.new ()
prop2 = MOAIProp2D.new ()
prop2:setParent (prop1)
```

Since transforms don’t have any shape or size, you do not need to add them to layers; they have no dimension and therefore cannot exist in a layer:

```lua
transform = MOAITransform.new ()
prop = MOAIProp2D.new ()
prop:setParent ( transform )

layer:insertProp ( prop ) -- insert the prop
layer:insertProp ( transform ) -- this has no meaning
```

Ad you’ve probably gathered, you can use transforms to move multiple props (and other transforms in unison:

```lua
root = MOAITransform.new ()
prop1 = MOAIProp2D.new ()
prop1:setParent ( root )
prop1:setLoc ( -10, 0 ) -- prop 1 is to the left of root

prop2 = MOAIProp2D.new ()
prop2:setParent ( root )
prop2:setLoc ( 10, 0 ) -- prop 2 is to the right of root

root:setLoc ( 100, 100 ) -- prop1 at (90, 100); prop2 is at (110, 100)
```

If you’re used to authoring tools, you may assume that creating a parent/child relationship also creates a group. Be careful; it doesn’t. Moai doesn’t make any additional assumptions about the relationships of objects in a spatial hierarchy. If you create a parent/child relationship between props, Moai will not, for example, automatically add the child prop to a layer if its parent is added:

```lua
prop2:setParent ( prop1 )
layer:addProp ( prop1 ) -- prop 2 will not be added to the layer
```

As you can see, Moai takes a low level view of things. ‘Groups’ are a high level concept with a lot of implications across layer membership, attribute inheritance and event propagation. The key idea is that you can (and should) create all of these relationships in Lua; a future tutorial will explain how. You can even create a Lua wrapper that mimics the group and event semantics of your favorite authoring framework. Even though baking assumptions about things like groups into the engine might make Moai more intuitive for some users, the constraints it would place on the design didn’t seem worth it. With Moai, it’s better to start from the low level view of things and use the right tool (Lua) for the job of creating high level constructs.

How Moai Renders a Scene
------------------------

Those are the basics of transforms, props, decks and layers. Now for an overview of how Moai actually performs its drawing.

The Moai simulation runs independently of rendering. The simulation loop is responsible for resolving the spatial hierarchy (among other things). When you create parent/child relationships between transforms, those relationships aren’t resolved until the next time the simulation loop runs. Fortunately, this happens immediately after update in the Lua runtime.

As spatial hierarchies are resolved, the results are cached in each transform node to be use in rendering.

To render a scene, the Moai runtime steps through each layer on its stack and performs a spatial query using the viewport rect. This query eliminates any props that are off screen and don’t need to be drawn and returns a list of those that do. Once the list of visible props has been obtained, the objects in that list are sorted (using a radix sort) by priority. Finally, each prop in the sorted list is visited and rendered. The prop sets up the drawing transformation and then calls on its deck to render the geometry.

As Moai performs its rendering, OpenGL state is cached to avoid redundant state changes. In addition, since Moai mostly renders textured quads, these quads are transformed by the CPU and copied to dynamic vertex buffer. The vertex buffer is rendered (and flushed) only when it fills up or there is a state change such as loading a new texture or shader.

Right now props are sorted based on their priority. In a (near) future version of Moai, you’ll be able to turn on the Z-buffer and skip the sort each frame. You’ll also be able to choose other attributes on which to sort: the X or Y location of the object, or even its shader state.

What’s Next?
------------

To review, this article covered some basics about how Moai binds to Lua, how to set up viewports and work in different coordinate systems, the relationship between props, decks and layers and how to use transforms.

There are still a few more basics to cover: animation and game logic using the action tree, and dependency graph relationships using attributes and links. Once you understand these concepts and are comfortable using Lua to wrap the Moai primitives into higher level constructs, you should be ready to write some fairly sophisticated games.
