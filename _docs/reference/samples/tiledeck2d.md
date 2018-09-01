---
title: "TileDeck2D"
---

This sample demonstrates use of MOAITileDeck2D. MOAITileDeck2D uses a MOAIGridSpace to subdivide a texture into a uniform grid, each tile of which may be indexed. When rendering uniform rectangles from a texture to a quad, MOAITileDeck2D is more memory efficient than MOAIGfxQuadDeck2D.

MOAITileDeck2D only subdivides the UV space of the texture associated with the deck. All texture rectangles are rendered to the same rectangle in model space. By default this is a unit square, but may be configured to be any size rectangle.

If you have a sprite sheet with uniform entries or a tile sheet for use with a tile map then MOAITileDeck2D is a good choice.

See [Understanding Decks and Props](understanding-decks-and-props.html) and Using MOAIGrid for further discussion.
