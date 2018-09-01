---
title: "Wrapping modes"
---

You can think about MOAIAnimCurve as a sample of a function. For any numerical input, the curve returns an output. For example, you could make a curve that samples like a sine wave.

MOAIAnimCurve is best suited to representing a span of a function beginning at time (aka 'x0' or 't0') zero and continuing until some final time (aka 'x1' or 't1'). Since the typical use of curves is to drive animation of node attributes we usually think of them as sampling an animation signal from t0 to t1 where the resulting sample is stored in the anim curve starting at 0 and ending at t1-t0 (aka '<i>length</i>'). The length of a curve is set implicitly by the time value of its last sample.

When we animate, a timer (see [anim-curve](samples/anim-curve.html)) or anim (see [anim-driver](samples/anim-driver.html)) provides the curve with an input going from 0 to t1-t0 which produces an output that matches the original sample curve (which itself was most likely created by an animation or animation tool).

Of course, the timer providing input to the curve may be configured to emit a t value of less than 0 or greater that t1-t0. In this case, since the curve has only sampled t0 to t1 from the original function, what value should it return?

This is the reason for the wrapping modes. Wrapping modes tell the curve what value to use for inputs less than 0 or greater than length.

There are four wrapping modes:

-   MOAIAnimCurve.WRAP
-   MOAIAnimCurve.CLAMP
-   MOAIAnimCurve.MIRROR
-   MOAIAnimCurve.APPEND

MOAIAnimCurve.WRAP
------------------

MOAIAnimCurve.WRAP causes the curve to repeat its sample. If you image a curve describing a linear function that goes from 0 to 1 over a domain of 0 to 1, WRAP will produce a 'sawtooth' pattern as the value of t extends below 0 and past 1.

MOAIAnimCurve.CLAMP
-------------------

MOAIAnimCurve.CLAMP simply uses the value of the curve at t=0 for values of t less than 0 and the value of the curve at t=t1-t0 for values of t greater than length. If you picture this curve you will see a flat line meeting the start and end points and extending out.

MOAIAnimCurve.MIRROR
--------------------

MOAIAnimCurve.MIRROR inverts the curve on odd spans. If the original 0 to length span is even, then the next span (<i>length</i> to 2\*<i>length</i>) will be the inverse of the sample. If you picture this curve using the 0 to 1 linear example you will see what looks a chain of pyramids.

MOAIAnimCurve.APPEND
--------------------

MOAIAnimCurve.APPEND adds the value of the curve's last sample multiplied by <i>length</i> \* floor (t/<i>length</i>) to the value of the curve. In other words, the original curve is offset and 'appended' to the end of each span. If you picture this curve for the 0 to 1 linear example, you will see a line with a slope of 1 extending forward and backward.
