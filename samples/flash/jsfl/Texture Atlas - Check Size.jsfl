//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

fl.runScript ( fl.configURI + 'Commands/moai/TextureAtlas.jsfl' );

var atlas = new TextureAtlas ( 1, false );

var packed = atlas.getPackListForLibrary ();
var size = atlas.getTextureSize ( packed );

fl.trace ( 'Texture Atlas Size: ' + size.width + ' x ' + size.height );
