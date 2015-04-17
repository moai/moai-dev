---
title: "The Moai Runtime"
order: -900
---

The [Moai](http://getmoai.com) SDK uses an Object-Oriented Reactive approach. The (re)action chain is defined in Lua and enacted in C++, which allows to get the best of both worlds, expressibility and speed.

Moai's runtime is based around a dependency graph and a tree of actions that operates on that graph. Most APIs in Moai deal with creating and managing objects that are evaluated every frame to produce simulation and animation. You should think about Moai as a big box of LEGOs or Tinkertoys with motors: Moai has many little pieces that may be 'snapped together' to produce a larger whole. As a game programmer, you'll spend most of your time setting up and tearing down sets of these objects like a puppeteer moving puppets and props on and off of a stage.

In Moai, simulation occurs in three phases. This is what Moai's simulation loop (i.e. the game loop) does every time step:

1.  Process the input queue
2.  Process the action tree
3.  Process the node graph

You might be wondering why there is no rendering step in the game loop. In Moai, rendering is divorced from the game loop and occurs at its own pace. On some systems it is not uncommon for the simulation to perform multiple simulation steps for every frame rendered. We'll talk more about rendering later.

Let's break down Moai's game loop piece by piece:

Process the Input Queue
-----------------------

Input events come from the system Moai is running on. For the purposes of the simulation step, we want input to be processed in a predictable fashion. To do this we capture all input events, store them in a queue and respond to them, in order, at the beginning of our game loop. Once the input events have been all processed, the game loop may proceed. Any input events that occur during the game loop will be enqueued for the next simulation step.

Process the Action Tree
-----------------------

The action tree is the step in the game loop where game logic and animation is typically performed. This might be scripted behavior implemented in a Lua coroutine or behavior applied by one of Moai's animation objects.

In some graph-based game engines there is no separate step for 'animations' and 'actions.' In Moai it would be possible to model all actions and animation using only the node graph, but the typical form such graphs take is of a big set of upstream 'behavior' nodes that control the game and a big set of downstream 'object' nodes that are controlled (transforms, groups, renderables and physics bodies). For Moai, it made sense to formalize this distinction with the addition of the action tree to specifically manage the upstream behavior controllers.

The action tree is just that: a tree of actions. Each action is responsible for invoking its children. There is a root action that exists only to have children attached to it. To the root action you will attach animation objects and Lua coroutines that drive the behavior of your game. You can 'pause' the game simply by swapping out the root action. You can also build families of actions that work together.

In Moai, only actions (and objects derived from actions) receive a time step: the node graph evaluation and the rendering pass are (from a simulation perspective) thought to occur instantaneously. This means no Moai node graph object should ever change its state based on either the simulation time or the number of times the node graph is processed or rendered. Only the action tree should perform any time-based changes to game state.

Process the Node Graph
----------------------

Actions perform time-based operations on nodes. For example, an action might change the position of a renderable node a little bit each frame to create the effect of animation. Because nodes may have dependencies on each other and need to behave in a predictable manner based on these dependencies, no changes applied to nodes by actions go in effect until the entire node graph is evaluated.

In actuality, Moai does not evaluate the entire dependency node graph every frame. Moai instead attempts to evaluate only the portions of the node graph that have been modified. When an action modifies a node, that node is scheduled for an update along with any nodes it depends on and any nodes that depend on it. Instead of performing a topological sort of the entire node graph (as some engines do), Moai keeps nodes in order by applying a few simple rules as nodes are scheduled. This means that adding and removing nodes from a scene and creating dependencies between nodes is fairly cheap and happens in more or less linear time. It also means that even in a scene with thousands of nodes, only those being actively animated or modified by the action tree will be updated each frame.

Rendering
---------

As mentioned, rendering occurs independently of simulation. A small set of the objects available in Moai are renderable - they know how to render themselves to the screen. Moai's render manager is a singleton to which a Lua table containing render objects may be registered. When Moai is asked by the system to render a frame, the render manager simply iterates through this table and draws each renderable object. The table should be a 1-based numerical array; the render manager will continue to render objects until it encounters a nil value in the table. Any table used for rendering may also contain subtables, which will also have their contents rendered.
