---
title: "GfxQuadListDeck2D"
---

This sample demonstrates use of MOAIGfxQuadListDeck2D. Like MOAIGfxQuadDeck2D, MOAIGfxQuadListDeck2D supports indexing. See [Understanding Decks and Props](understanding-decks-and-props.html) for further discussion.

MOAIGfxQuadDeck2D may be used to draw a <i>list</i> of textured quads selected, by index, from a set of such lists of quads. All quads share the same texture but may have different model and UV coordinates. This is useful for rendering composite objects that display multiple entries on a sprite sheet or texture atlas.

The internal structure of MOAIGfxQuadListDeck2D is designed to allow for re-use of individual model and UV space quads. Because of this, four tables must be initialized:

-   Model space quads
-   UV space quads
-   Pairs
-   Lists

The two tables of quads (model and UV space) should be used to allocate only as many <i>unique</i> quads as will be required.

The pairs table associates model space with UV space quads. Pairs may be duplicated if a model/UV pair is required in more than one list.

The lists table stores only the base pair and total number of pairs required to render each list. When the deck needs to render a list (given an index), it first looks up the base pair and size of the list in the lists table. Then, each pair of screen and UV quads is rendered, in order, starting at the base pair.

To give an example: imagine you want to animate a character's eyes blinking. To do this you need two indices in your deck: eyes open and eyes shut. You need two model space quads (the character and its eyes) and three UV space quads (the character, eyes open and eyes shut). You will need four pairs (character, eyes open, character, eyes shut) and two lists (with the base pair being the character and a length of two so the character as well as its eyes will be rendered). Once you've set this up you can render each frame of the multi-quad composite by index to create a blinking animation.
