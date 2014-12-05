//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

fl.runScript ( fl.configURI + 'Commands/moai/Settings.jsfl' );
fl.runScript ( fl.configURI + 'Commands/moai/LuaTable.jsfl' );
fl.runScript ( fl.configURI + 'Commands/moai/TextureAtlas.jsfl' );
fl.runScript ( fl.configURI + 'Commands/moai/SpriteDeckBuilder.jsfl' );

//----------------------------------------------------------------//
function exportSpriteLibrary ( padding, forceSquare ) {
	
	var document = fl.getDocumentDOM ();
	var folderPath = fl.browseForFolderURL ( 'Select a folder.' ) + '/';
	if ( !( folderPath &&  folderPath.length )) return;
	
	var luaFilePath = folderPath + document.name + '.lua';
	var pngFilePath = folderPath + document.name + '.png';
	
	var atlas = new TextureAtlas ( padding, forceSquare );
	atlas.createTextureAtlas ();
	
	var spriteLibs = new LuaTable ();
	spriteLibs.set ( 'texture', document.name + '.png' );
	
	var timelines = document.timelines;
	for ( var i = 0; i < timelines.length; ++i ) {
		var timeline = timelines [ i ];
		var layers = timeline.layers;
		
		var spriteDeckBuilder = new SpriteDeckBuilder ();
		spriteLibs.push ( spriteDeckBuilder.spriteDeck );
		
		for ( var frameIdx = 0; frameIdx < timeline.frameCount; ++frameIdx ) {
			spriteDeckBuilder.addSpriteForFrameIdx ( document, timeline, atlas, frameIdx );
		}
	}
	
	var lua = spriteLibs.toLuaLocalTable ( 'spriteDecks' );
	lua += '\n';
	lua += 'return spriteDecks\n';
	fl.trace ( lua );
	
	if ( !FLfile.write ( luaFilePath, lua )) {
		alert( "Error saving file" ); 
	}
	
	atlasDoc = atlas.createTextureAtlasDoc ();
	atlasDoc.exportPNG ( pngFilePath, true, true );
	fl.closeDocument ( atlasDoc, false );
}
	
exportSpriteLibrary ( MOAI_TEXTURE_ATLAS_PADDING, MOAI_TEXTURE_ATLAS_FORCE_SQUARE );
