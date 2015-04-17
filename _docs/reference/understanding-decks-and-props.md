---
title: "Understanding Decks and Props"
---

Moai's instancing model is based around the concept of 'decks' and 'props.'

In game engine nomenclature, 'geometry' means 'some kind of spatial data.' Geometry is usually graphical: textured polygons, for example, but can also be any data that describes an object in space.

Modern game engines 'instance' geometry by re-using spatial data with multiple transforms. For example, if a piece of geometry describes an object, that object may be re-positioned anywhere in space just by adding an offset to it. This means we can position the object many times in the same scene just by storing an offset and a reference to the original spatial data. Even though there will appear to be many copies of the object on screen, the only difference between each copy is the offset. Because there is virtually no cost to applying the offset to the object (as it is drawn, for example), instancing is much more efficient than making multiple copies of the object's data in memory.

In Moai, a 'deck' is simply a set of geometry organized by numerical index. To draw a piece of geometry held in a deck we just need to know its index and the position on the screen to draw it at. Most decks in Moai support multiple items but a few, such as MOAIGfxQuad2D, do not. By convention, if a deck supports indexing its name will be post-fixed with the word 'Deck.' (These single-item decks may eventually be deprecated.) All Moai decks derive from the MOAIDeck base class.

A 'prop' represents the instance of a piece of geometry in space. This means there may be many props sharing the same item from a single deck. Each prop combines a transform (the offset used to place a deck item) with an index used to look up which item in the deck should be used. The index used to look up items in a deck is just a 32-bit number. In keeping with Lua's convention, indexing starts at 1. The index is 'wrapped' over the total number of items in the deck. Index 0 is special and will cause the item to be ignored by whatever process (such as rendering) is using the prop.

All Moai props derive from the MOAIProp base class, which also inherits MOAITransform and MOAINode.

Grids
-----

By default, a prop places a single deck item at a single location in space using a single index.

It is also possible to use a prop to place an array of deck items in a uniform pattern using a grid.

Moai's grid object defines both the spatial dimensions of a grid (i.e. cell size and offsets) with an array of numbers. When attached to a prop, the prop uses the grid to draw multiple deck entries, one for each grid cell. The dimensions and shape of the grid determine where the deck items are placed and the number associated with each cell by the grid is used as the deck index.

Drawing multiple deck items with a grid and a single is far more efficient that creating a unique prop for each item. See Using MOAIGrid for a more detailed explanation.

Deck Index Flags
----------------

The upper three bits of the 32-bit deck index are special. These bits may be used to flip the deck item along the x or y axis or to 'hide' the deck item. The 'hide' flag has the same effect of setting the deck index to 0, except the original value of the index need not be lost. The 'flip flags' were originally intended for 2D use, but will be extended to include a z axis flip as well as 90-degree rotation around each axis. The index flag constants are currently exposed as part of MOAIGridSpace, but, as they are used by all decks and props, will be moved to their own namespace in a future release.
