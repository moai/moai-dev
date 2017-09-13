---
title: "Anim-span"
---

Because MOAITimer objects support both a start and end time, they may be configured to operate over any time span.

This sample demonstrates using a time span and MOAITimer's PING\_PONG loop mode to plan an animation starting at t0=0.25 and engine at t1=1.5. The PING\_PONG loop mode means that the timers initial value will be t0. It will play until it reaches t1, then reverse its direction until it is back at t0. This will continue until the timer is explicitly stopped.

The curve applied by the anim begins, like all anim curves, at 0 and continues until the final key frame, in this case at t=2. The limited span of the timer means that only a section of this curve will be used by the anim. Using time spans to select only a section of a curve (or curves) to apply is one strategy for managing multiple animations: a single MOAIAnim may be used to bind one set of curves to the nodes being animated and just the start and end times of each animation may be stored. When you need to change which animation is being played, you can just change the timer's span and current time.

Organizing multiple animations using time spans is a valid approach that we neither endorse nor discourage.
