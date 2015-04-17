---
title: "Chase"
---

This sample demonstrates how to set up a MOAIEaseDriver to cause the attributes of one node to 'chase' the attributes of another node.

Usually, an ease driver works by applying a set of deltas to the attributes of a 'driven' node. The deltas start at 0 and end at a constant. For example, all of the 'move' and 'seek' animation helper methods create ease drivers and initialize them using constants.

In addition to accepting a constant, MOAIEaseDriver's setLink () method is overloaded to also accept a source node and attribute. Passing in a valid source (numerical) attribute will cause the ease driver to target the current value of that attribute at each update. This will have the effect of aligning the driven attributes with attributes of a target node over time.
