---
title: "Anim-curve"
---

This samples demonstrates a different style of animation based on the dependency graph and linked node attributes. This is an implementation of the system used in 3D animation tools such as Maya.

In this sample we create a MOAIAnimCurve and configure it with two key frames. The first key is at time 0 seconds and has a value of 0. The second key is at time 1.5 seconds and has a value of 360. As the curve is evaluated and time goes from 0 to 1.5 seconds, the value of the curve will go from 0 to 360.

The curve is a node and has the attributes ATTR\_TIME and ATT\_VALUE. ATTR\_TIME is an input and ATTR\_VALUE is an output. We can link the curve's ATTR\_VALUE to the prop's ATTR\_Z\_ROT but nothing will happen. To animate the prop we need a timer.

MOAITimer is both a node and an action. Because it is an action it may be added to the action tree which will cause it to receive time=based updates every sim step. Because MOAITimer is also a node, it has attributes that may be linked to other nodes in the dependency graph.

MOAITimer exposes a single output attribute called ATTR\_TIME. To animate the prop, we link the timer's ATTR\_TIME output to the curve's ATTR\_TIME input and the curve's ATTR\_VALUE output to the prop's ATTR\_Z\_ROT:

MOAITimer.ATTR\_TIME -\> MOAIAnimCurve.ATTR\_TIME MOAIAnimCurve.ATTR\_VALUE -\> MOAIProp.ATTR\_Z\_ROT

Calling start () on the timer action with no inputs adds the timer to the root of the action tree which causes it to begin receiving time based updates. Each update causes the timer to progress, which changes the value of its ATTR\_TIME. This causes the curve's ATTR\_VALUE to change which, finally, animates the prop.

This sample is just a demonstration of attributes, curves and the dependency graph. In production you will probably not want to do animation this way but instead use curves with MOAIAnim. See the [anim-driver](anim-driver.html) sample for more about MOAIAnim.
