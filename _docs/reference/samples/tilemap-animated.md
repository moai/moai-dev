---
title: "Tilemap-animated"
---

This sample demonstrates using a MOAIDeckRemapper to animate a tile map by remapping the indices of its deck. Using a MOAIDeckRemapper is more efficient than changing individual data entries in the MOAIGrid. Remapping the indices has the advantage of only changing the way the grid is displayed without modifying the underlying data.

When a MOAIDeckRemapper is attached to a prop, any index used by that prop (including indices from a grid) will first be remapped.

The remapper works by defining a range of indices to remap. By default this range begins at index 1, but may be offset by changing its base. The remapper maintains an alternate value for each index inside of this range. This original index will be used to look up the alternate, which will be used in its place.

To make animation possible, each index in the remapper's range is exposed as a node attribute. Animation curves may applied to these attributes to animate the replacement values.

If you have a game scene you are rendering with a tile map and need to animate tiles to create effects such as flowing water or waving grass, using MOAIDeckRemapper with a MOAIAnim is a good way to accomplish this.

See [Understanding Decks and Props](understanding-decks-and-props.html) for a more detailed discussion of deck indexing. Also see Using MOAIGrid for a discussion of rendering tile maps.
