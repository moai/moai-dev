---
title: "Using MOAIAnim"
---

Problems with Attribute-Based Animation
---------------------------------------

In the [anim-curve](samples/anim-curve.html) sample we applied curve values by creating attribute links between node attributes. This is unworkable for a production game with a large number of animations for a couple of reasons.

First, it makes sharing anim curves (which can contain a lot of data) difficult. Since each link can only pull from a single source attribute, an anim curve can only be driven by one timer. This means that even though many attributes may be animated by the curve, they will all be synchronized.

Second, using attribute links for animation means that links will have to be repeatedly torn down and set up as animations change. This extra code to write and only adds overhead when switching animations on a character.

Animation with MOAIAnim
-----------------------

The solution is to use MOAIAnim. MOAIAnim derives from MOAITimer, so maintains a time counter and is updated as part of the action tree.

MOAIAnim may be configured to 'push' the values of a set of anim curves onto a set of node attributes each frame. This occurs during the action tree update instead of the dependency graph update. The upshot of this is that anim curves may be shared between multiple anims and applied with multiple time values.

To understand the usage pattern of MOAIAnim, imagine a game character that is represented by a single prop and is animated through ATTR\_X\_LOC, ATTR\_Y\_LOC and ATTR\_INDEX. The coordinate attributes may be used to translate the character on the screen, and the index is used to do flipbook animation by selecting what item from the character's deck should be displayed at any given time.

Now let's imagine we've designed three animations for the character: walk, run and jump. Since each animation uses three curves (to target three attributes) we can represent these animations with a total of nine curves, or store them serially (one after the other) in only three curves. It really doesn't matter which. Some games may even use a combination of both approaches.

There might be multiple instances of our character on the screen at any time, but all instances should share the same anim curves. To re-use the curves, create a unique set of MOAIAnims for each instance of the character. Store the MOAIAnims in the character's table so the logic for the character can easily access them. As each MOAIAnim is already bound to a the attributes or a specific character instance, all you have to worry about now is starting and stopping the animation you want that character to perform.

Two Ways to Push Curve Values to Attributes with MOAIAnim
---------------------------------------------------------

MOAIAnim can apply either the absolute or delta value of a curve to an attribute. This option is configured using setLink ().

When configured to apply the absolute value of the curve to the attribute, MOAIAnim will simply overwrite the value of the attribute with the value of the curve. If multiple anims are applied to the attribute, then the attribute will use the final value applied to it.

When configured to apply the delta value of the curve, the curve will be evaluated by the anim at the start and end of the time step and the difference in its value (the delta) will be <i>added</i> to the attribute. This is useful if you want the motion described by the curve to 'offset' the current value of the attribute, as you might in an animation for a character's walk or run cycle that also contains the distance the character is to travel.

Note that when applying curves as deltas you should use the MOAITimer.CONTINUE loop mode and MOAIAnimCurve.APPEND wrap mode on the anim and curve respectively.
