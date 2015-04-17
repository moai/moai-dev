---
title: "Anim-listener"
---

This sample demonstrates two ways to registering for callbacks from MOAIAnim.

MOAIAnim inherits MOAITimer, which itself inherits MOAIAction. This means that MOAIAnim emits both timer and action events.

In this sample we set up listeners for MOAIAction.EVENT\_STOP, MOAITimer.EVENT\_TIMER\_LOOP and MOAITimer.EVENT\_TIMER\_KEYFRAME.

MOAIAction.EVENT\_STOP is send when the action is stopped. In this case, the action is stopped automatically by MOAITimer, which has not been instructed to loop. When MOAITimer's time reaches the end of its span, the timer will automatically remove itself from the action tree.

MOAITimer.EVENT\_TIMER\_LOOP is called at the end of the timer's cycle. It is called whether or not a loop mode is enabled. It indicates that the timer as reached the end of its span and will either loop or roll over. In future versions of Moai the name of this event may be changed to clarify its meaning.

In addition to its built-in events, MOAITimer also provides a way to generate custom events at any point in time by using an anim curve and its key frames. When a MOAIAnimCurve is attached to a MOAITimer using setCurve (), the timer will emit an event each time a key frame is encountered. Typical uses for key frame events include checking for a button press at a specific time or playing a sound effect.
