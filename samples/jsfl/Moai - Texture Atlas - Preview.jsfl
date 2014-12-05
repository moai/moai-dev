//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

fl.runScript ( fl.configURI + 'Commands/moai/Settings.jsfl' );
fl.runScript ( fl.configURI + 'Commands/moai/TextureAtlas.jsfl' );

var atlas = new TextureAtlas ( MOAI_TEXTURE_ATLAS_PADDING, MOAI_TEXTURE_ATLAS_FORCE_SQUARE );

atlas.createTextureAtlas ();
atlas.createTextureAtlasDoc ();
