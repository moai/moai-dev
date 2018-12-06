# Introduction to Moai 2.0

This guide highlights some of changes between 1.0 and 2.0. It is not meant to be exhaustive, just an indicator of issues to be aware of when porting.

While there has been breaking renaming and refactoring for 2.0, the major concepts of 1.0 have been brought forward. Those already familiar with Moai should find many familiar concepts in 2.0. 

## Refactor of Prop classes

"Prop" has been split out and reassembled into a family of props. MOAIProp remains as an alias for MOAIGraphicsProp, but this is deprecated and will be removed.

The new prop classes to be aware of when porting are:

MOAIGraphicsProp - replacement for MOAIProp2D (if used with a deck and a single index)
MOAIGraphicsGridProp - replacement for MOAIProp2D (if used with a deck and a grid)

## Refactor of Deck classes

Moai 1.0's set of decks has been consolidated:

MOAIDrawDeck - deck for immediate mode drawing.
MOAIMetaTileDeck2D - deck that draws elements made up of tiles.
MOAISpriteDeck2D - deck that draws 2D graphics composed of one or more components (replacement for MOAIGfxQuad, etc.).
MOAIStretchPatch2D - deck that draws a graphics with stretchable sections (such as a 9-patch).
MOAITileDeck2D - deck that divides texture UV space into tiles.

Moai 2.0 decks also implicitely support multiple materials for deck elements. Drawable decks now derive from MOAIMaterialBatchHolder, which can be configured to map different blocks on indices onto different materials. In other words, a single deck element (in MOAISpriteDeck2D, for an example) might render its components from multiple textures and shaders, or a grid prop using MOAITileDeck2D might be made up of tiles from different textures.

MOAIDeckRemapper is now implemented a generic deck wrapper instead of a deck component. In 1.0, any deck could "have" a remapper. In 2.0, a remapper can "have" any deck.

## MOAITextLabel

MOAITextBox has been replaced with the more versatile MOAITextLabel. MOAITextLabel can optionally enforce horizontal and veritcal bounds on its contents. If both sets of bounds are enabled, it behaves like a text box.

MOAITextLabel also now supports a backing image. This can be supplied by any drawable deck, including MOAIStretchPatch2D. The background image is indexable.

Internally, there is also a better factoring of text shaping. While this hasn't been exposed yet, it paves the way for complex text layout.

## Default 3D Coordinate System

While 2D decks remain, there are no longer 2D variants of props or transforms. All coordinate systems (and helper animation methods) are now 3D by default. For developers porting their projects forward to Moai 2.0, making sure that all animation helper methods are updated to take the correct amount of paramaters will probably be the biggest task. For example:

prop:moveLoc(x, y, t) -- this is a mistake in 2.0; 't' will be interpreted as 'z' and the time param will default to 0

Is now:

prop:moveLoc(x, y, z, t)

## MOAICamera consolidation

2D, 3D or ortho cameras have been consolidated into a single class: MOAICamera. To set the camera type, use MOAICamera.setType() with one of MOAICamera.CAMERA_TYPE_3D, MOAICamera.CAMERA_TYPE_ORTHO, MOAICamera.CAMERA_TYPE_WINDOW.

The '3D' and 'ortho' camera types are simply perspective and orthographic projects. The 'window' camera type is an ortho projection that ignores that discards the Z axis (by scaling Z to 0). This elimates depth information, but also the effect of the near/far plane settings.

## Partition membership

MOAIPartitionHull now exists independently of MOAIProp. MOAIPartitionHull inherits MOAITransform and represents membership in a partition, dimenionality (having bounds or existing boundlessly in global space), and placement (having a location and orientation in space).

Because a MOAIPartitionHull can only be contained in one partition at a time, setPartition () is now the only way to add a hull to a partition.

This will most notably affect MOAILayer (which wraps a MOAIPartition) and any props that derive from MOAIPartitionHull (such as MOAIGraphicsProp). In Moai 2.0, you will write:

prop:setPartition(layer)

To add a prop to a layer.

## Expanded Shader Support

In Moai 2.0, shaders are implemented by MOAIShaderProgram and MOAIShader. MOAIShaderProgram represents the GPU shader itself; MOAIShader associates a shader program with a set of cached uniform values. In other words, a MOAIShaderProgram may be shared by multiple MOAIShader instances. The shader program will specify the uniforms the shader expects; the shader instances will cache and update those uniforms.

The shader uniform system has been given a complete overhaul to better reflect the capabilities of OpenGL. A much richer set of "pipeline globals" has been exposed for use as uniforms. Shaders now have fine control over how textures are loaded into texture stages.

I won't go into specific changes to the way shader uniforms are declared and set here. Documentation is coming; in the mean time review the code and samples.

## Expanded Materials System

MOAIGraphicsProp and drawable deck objects may be assigned material batches. Material batches encapsulate and expand Moai 1.0's notion of texture, shader and other drawing attributes. The interface for materials is meant to be similar to the original and convenience methods are provided to make setting up props and decks straightforward.

For example:

prop:setTexture('myTexture.png') -- set a texture into material batch; material ID defaults to '1'; material batch is created if none exists
prop:setTexture(2, 'myTexture.png') -- set a texture into material batch; material ID is 2
prop:setMaterialBatch(myMaterialBatch) -- set a material batch

Material settings cascade from prop to deck. All material attributes support a nil value; setting a non-nil value to a material attribute on a prop will mask the same attribute when the deck is rendered.

Also of note: the "convenience" blend modes are gone. Blend modes must now be set explicitly, with all three parameters (func, srcFactor, destFactor).

## Class-Scoped Debug Lines

MOAIDebugLines has been renamed to MOAIDebugLinesMgr and debug line styles are now scoped to classes. For example:

MOAIDebugLines.setStyle ( MOAIDebugLines.PROP_MODEL_BOUNDS, 2, 1, 1, 1 ) -- Moai 1.0
MOAIDebugLinesMgr.setStyle ( MOAIGraphicsProp.DEBUG_DRAW_MODEL_BOUNDS, 2, 1, 1, 1 ) -- Moai 2.0

## Revised Render Tree and Framebuffer System

In Moai 2.0, you can pass a table, function or a MOAIDrawable to MOAIRenderMgr.setRender(). MOAILayer derives from MOAIDrawable. Functions are used for immediate mode drawing. Tables are used to group and order drawing passes.

There are now several types of layer:

MOAITableLayer - renders a table of tables, functions or drawables with no view setup.
MOAITableViewLayer - renders a table of tables, functions or drawables using a view setup.
MOAIPartitionViewLayer - renders the contents of a partition using a view setup.

The "view" layers accept a viewport and (optionally) a camera. Because the partition layer uses the view to decide what to draw, there is no variant of a partition layer without a view.

Another change in Moai 2.0 is that offscreen framebuffers may be attached to layers. In Moai 1.0, layers were attached to framebuffers. MOAIFrameBuffer is no longer a drawable. To draw a stack of layers into an offscreen buffer, use a MOAITableLayer and setFrameBuffer().
