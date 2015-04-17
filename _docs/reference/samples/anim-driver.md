---
title: "Anim-driver"
---

This sample shows a more robust and sophisticated way to approach animation than that shown in [anim-curve](anim-curve.html). Here we still use anim curves, but apply them using a MOAIAnim instead of using attribute links.

In the sample, we use a single MOAIAnim and two MOAIAnimCurves to simultaneously animate two props. The first and second curve are both applied to the first prop. The first curve is also applied to the second prop. The location of first prop is animated using absolute values. The location of the second prop is animated using a delta.

When you run the sample you will see the first prop 'hop' to the right of the screen and the reset to the origin. The second prop will match the x translation of the first prop but not the y. It will continue to slide to the right and eventually disappear off the side of the screen.

See [Using MOAIAnim](using-moaianim.html) for a more detailed discussion.
