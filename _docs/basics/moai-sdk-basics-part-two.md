---
title: "Moai SDK Basics Part Two"
---

This article discusses the Moai dependency graph architecture and the action tree. There’s a lot to discuss on the way to fully understanding the best way to do animation and manage game objects in Moai. That discussion starts with an under-the-hood look at how Moai works – you'll learn how to use that knowledge to organize your animation data next time.

The Action Tree
---------------

If you’ve looked at the demos included with the SDK you’ve probably noticed that some objects expose convenience methods for animation:

```lua
prop:moveRot ( 360, 1.5 )
```

This causes the prop to rotate 360 degrees for 1.5 seconds. If you want the sprite to do more than rotate, there are other convenience methods you can call:

```lua
sprite:moveLoc ( 64, 0, 1.5 ) -- move the sprite 64 units along the X axis
sprite:moveScl ( 1, 2, 1.5 ) -- add a scale factor of 1 to the X width of the sprite and 2 to the height
```

If you enter in all three calls, you will see that they combine. Each animation starts at the same moment in time. Your sprite will rotate, translate and scale all at once.

These simple convenience routines are actually special cases of a more generalized and powerful system.

The helpers called in these examples all work by creating a new instance of a MOAIEaseDriver object. MOAIEaseDriver is a subclass of MOAIAction, a special kind of object that receives an update and timestep every frame of the simulation. You can get the MOAIAction object created by an animation routine by assigning it to a variable:

```lua
rotateAction = sprite:moveRot ( 360, 1.5 )
```

The Moai runtime organizes actions into a tree. Any action added to the tree will receive updates. When removed from the tree the action (and its children) will no longer receive updates.

Most actions pass simulation updates to their children, though certain actions may modify this behavior. For example, an action might scale the timestep to cause its child actions to run in slow motion or to fast forward or control the order in which its children are updated.

By default, the actions created by the animation helper routines are automatically added to the root of the action tree. You can prevent this by removing an action from the action tree immediately after creation:

```lua
rotateAction = sprite:moveRot ( 360, 1.5 )
rotateAction:stop ()
```

You can create extra actions for the purpose of grouping actions together. The base class MOAIAction simply passes its update events along to its children:

```lua
rotateAction = sprite:moveRot ( 360, 1.5 )
translateAction = sprite:moveLoc ( 64, 0, 1.5 )

actionGroup = MOAIAction.new ()

actionGroup:addChild ( rotateAction )
actionGroup:addChild ( translateAction )
```

Note that you don’t have to explicitely stop the rotate or translate action. They are implicitely stopped by the call to addChild () which removes them from the root of the action tree. Since they are no longer connected to the action tree, they won’t receive any updates until actionGroup:start () is called. When the action tree is started, it will remain active in the action tree until all of its children have finished.

You can tell if an action is currently in the action tree by calling:

```lua
action:isActive ()
```

Actions have several states they can be in: ‘active,’ ‘busy’ and ‘done.’ An action is ‘active’ if it is in the action tree. An action is ‘busy’ if it is in the action tree and has more work to do. An action is ‘done’ if it has completed its work. In most cases, there is no practical difference between being ‘active’ and being ‘busy.’

Action Sequencing
-----------------

To create an action sequence you can use Lua coroutines. A coroutine is simply a function that can yield execution to be resumed at a later time. Moai provides a coroutine wrapper called MOAICoroutine. MOAICoroutine is derived from MOAIAction and may therefore be added to the action tree. The behavior of MOAICoroutine is simply to resume the coroutine every time an action update is received. The coroutine the action is running may then inspect the game state and optionally do some processing before yielding for the next thread.

Here's an example of a simple action sequence:

```lua
function threadFunc ()

    -- set up sprite here
    local action;

    action = sprite:moveRot ( 180, 1 )
    while action:isBusy () do coroutine:yield () end -- spin lock until action is done

    action = sprite:moveLoc ( 64, 0, 2 )
    while action:isBusy () do coroutine:yield () end

    action = sprite:moveScl ( -0.5, -0.5, 1 )
    while action:isBusy () do coroutine:yield () end

end

thread = MOAICoroutine.new ()
thread:run ( threadFunc )
```

In the above sequence, the sprite first rotates, then translates, then scales. Moai waits for each action to finish by spinlocking: we poll its state and yield until done.

There’s a slightly more efficient way to wait for an action to finish than spinlocking:

```lua
action = sprite:moveScl ( -0.5, -0.5, 1 )
MOAICoroutine.blockOnAction ( action )
```

Blocking prevents your coroutine from executing until the action it is blocked on finishes. You can call blockOnAction () during your coroutine. It will act on the currently running coroutine.

In practice you’ll use both spinlocking and blocking. While blocking is efficient, spinlocking allows you to put logic into your loop: you can yield on any set of conditions, not the just the completion of an action. For example, you might want your coroutine to wait until a button is pressed or a collision occurs.

Blocking vs. Callbacks
----------------------

If blocking isn’t to your liking, you can certainly build up sequences of actions by installing listeners on each action and writing callbacks to handle them. But before you go down that path, consider what this does to your code:

```lua
function doMoves ()
    local action = prop:moveRot ( 180, 1 )
    action:setListener ( MOAIAction.EVENT_STOP, actionDone1 )
end

function actionDone1 ()
    local action = prop:moveLoc ( 64, 0, 2 )
    action:setListener ( MOAIAction.EVENT_STOP, actionDone2 )
end

function actionDone2 ()
    prop:moveScl ( -0.5, -0.5, 1 )
end

doMoves ()
```

You have to create multiple methods and daisy-chain them together. This seems more like the kind of construct you’d see in Objective-C or C++ than Lua. Having to track these multiple functions can make the code less readable. Once a lot of logic and branching creeps in you can wind up with a real mess on your hands. Callbacks do have their place, but for coding long sequences, you can write much more naturalistic and readable code by taking advantage of coroutines. You can even create your own library of blocking functions to help out:

```lua
function wait ( action )
    while action:isBusy () do coroutine:yield () end
end

function threadFunc ()
    wait ( prop:moveRot ( 180, 1 ))
    wait ( prop:moveLoc ( 64, 0, 2 ))
    wait ( prop:moveScl ( -0.5, -0.5, 1 ))
end

thread = MOAIThread.new ()
thread:run ( threadFunc )
```

It’s hard to beat the contents of the thread function above for simple readability.

Animation Nuts & Bolts
----------------------

Now that you know a bit about the action tree, it's time to get into a little more detail about exactly how animation works in Moai.

The basic idea behind all animation is the process of changing some attribute of an object over time. The MOAIEaseDriver actions created by the animation helpers work this way: each action contains a set of deltas that are gradually applied to an object’s attributes over time. The effect of the deltas is to offset the value of the attributes. For example, an action might apply a delta with a magnitude of 64 units to the X translation attribute of an object over a period of 10 seconds. After the action is complete, the object’s X translation will have increased by 64 units. Deltas are also cumulative: there’s no reason that several deltas couldn’t be applied to the same attribute.

Under the hood, Moai accomplishes all this through its Attributes system. Certain Moai objects, like MOAITransform2D and MOAIProp2D, expose a set of Attributes than can be used for animation or to create relationships between nodes. Any object that inherits from MOAINode has this capability.

Attributes enable user configurable action-to-node and node-to-node binding. Importantly, node can be bound anonymously: the MOAIEaseDriver has no knowledge of what kind of node it is controlling. Because of this, it can be used to animate any Attribute on any node.

You can get and set the value of Attributes directly:

```lua
-- you can set the value of Attributes directly
prop:setAttr ( MOAIProp2D.ATTR_X_LOC, 32 )
prop:setAttr ( MOAIProp2D.ATTR_Y_LOC, 16 )

-- this is equivalent to:
prop:setLoc ( 32, 16 )

-- you can also get the value of Attributes
x = prop:getAttr ( MOAIProp2D.ATTR_X_LOC )
y = prop:getAttr ( MOAIProp2D.ATTR_X_LOC )

-- this is equivalent to:
x, y = prop:getLoc ()
```

As you can see from the above example, getLoc () and setLoc () are just convenience methods for accessing Attributes.

Attributes can be used by an object for both input and output. When one Attribute is connected to another, the first is said to be ‘driven’ by the second. Before the node that exposes the Attribute being driven is processed, the driven Attribute will take on the same value as the Attribute driving it. Any Attribute can be used to drive any other, provided the other Attribute is exposed by a different node and the connection doesn’t form any cycles between nodes.

This example uses the attributes of one prop to drive another:

```lua
prop1:setAttrLink ( MOAIProp2D.ATTR_X_LOC, prop2, MOAIProp2D.ATTR_Z_ROT )
prop2:moveRot ( 180, 2 )
```

The above command sets the X location attribute of the first sprite to be driven by the Z rotation attribute of the second sprite. As second sprite is rotates, we’ll see the first sprite translate along the X axis.

Actions, like MOAIEaseDriver, can operate on Attributes directly. You may have to specify which objects and attributes to operate on, but the actions themselves don’t list Attributes as outputs. In other words, actions apply their output to Attributes using a push model; nodes draw their Attribute values from other nodes using a pull model.

Here's a more generalized way of using MOAIEaseDriver, this time based on Attributes:

```lua
ease = MOAIEaseDriver.new ()

ease:reserveLinks ( 3 )
ease:setLink ( 1, prop, MOAIProp2D.ATTR_X_LOC, 64 )
ease:setLink ( 2, prop, MOAIProp2D.ATTR_Y_LOC, 64 )
ease:setLink ( 3, prop, MOAIProp2D.ATTR_Z_ROT, 360 )
ease:setLength ( 1.5 )
ease:start ()
```

The code snippet above creates an ease driver, reserves three channels then sets each channel to target a specific attribute of a single prop.

If you want multiple props to move in concert, that's easily arranged:

```lua
ease:setLink ( 1, prop1, MOAISprite.ATTR_Z_ROT, 180 )
ease:setLink ( 2, prop2, MOAISprite.ATTR_Z_ROT, -180 )
```

Moai’s Attribute system allows you to easily introduce new actions and objects to the Moai framework that can immediately operate with the rest of the system without the need to rewrite or update any bindings or animation code. In addition, this system provides a rich and evolving set of objects and actions that can drive parameters in new ways.

The Dependency Graph
--------------------

The main point of the preceding discussion of attributes and nodes was to give an introduction to the dependency graph. The purpose of the dependency graph is to resolve the order in which objects are updated. If one object depends on another, we want to be sure to update it only after we’ve updated the object it depends on. If the update order is processed correctly, visual and animation artifacts will be minimized. For example, creating a parent/child relationship between two transforms also creates a dependency. Without the dependency to ensure that the parent is updated before the child, the two objects could appear to be slightly out of sync if the child is updated first.

The classic approach to implementing a dependency graph is to first perform a topological sort over all of the nodes in the graph. The sort gives the correct ordering of the nodes with respect to their dependencies. After the sort, the nodes may be updated in order every frame until the structure of the graph changes and must be re-sorted.

Moai attempts to avoid sorting the entire graph on every change and updating the graph on every frame. Internally, Moai maintains a list of nodes scheduled for update. When a node is changed and needs to be updated, it is placed in the update list. The update then cascades to the node’s dependencies, stopping at any nodes that have already been scheduled.

When working with Moai it’s important to understand how the action tree is related to the dependency graph and how Moai performs its simulation loop.

In Moai, simulation steps are of fixed size. The simulation loop tries to fit as many simulation steps into the time elapsed between frames as it can. Every step, Moai first updates each action in the action tree. Only actions are passed the time step. Updating the action tree may result in dependency graph nodes being scheduled. These nodes will only be updated after processing the action tree has been finished.

The upshot of Moai’s two-step simulation loop is that nodes changed by actions will not propagate those changes to their dependents until after one simulation step has been completed. For example, if you create a parent/child relationship between two nodes, the child node’s transform will not be updated. If you immediately try to transform a point from world space unto the local space of the child node, the relationship to the parent will not be taken into account until the next update step. This usually isn’t a problem, but in the rare case that it would be you can force the node to resolve its dependencies and update ahead of time:

```lua
transform = MOAITransform.new ()
transform:setLoc ( 200, 0 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
prop:setParent ( transform )
layer:insertProp ( prop )

-- force the dep graph to update so prop:inside () will take parent transform into account
prop:forceUpdate ()

if prop:inside ( 200, 0 ) then
    print ( 'INSIDE!' )
end
```

Use forceUpdate () sparingly, if at all.

More to Come
------------

This article covered a lot of territory: programming for concurrency using the action tree, creating bindings between objects using attributes and links and the workings of the dependency graph.

At this point, physics notwithstanding, this series has been through the guts of Moai, but there are a few more 'basics' to cover. Still to come, some more practical ways to describe and manage animations using MOAIAnimCurve and MOAIAnim, how to create flipbook animations by animating the index attribute of MOAIProp2D and how to work with grids to create large, efficient tile maps for scrolling games. There will also be articles about handling input using MOAIInputDevice and how to use some of Moai’s built-in utility classes. Please stay tuned.
