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
function exportToLua ( padding, forceSquare ) {
	
	var document = fl.getDocumentDOM ();
	var folderPath = fl.browseForFolderURL ( 'Select a folder.' ) + '/';
	
	if ( folderPath &&  ( folderPath.length > 5 )) {
		
		var timeline = document.getTimeline ();
		var keyframes = getKeyframeList ( timeline );
		
		var atlas = new TextureAtlas ( padding, forceSquare );
		atlas.createTextureAtlas ();
		
		var spriteDeckBuilder = new SpriteDeckBuilder ();
		
		var lua = new LuaTable ();
		
		lua.set ( 'texture', document.name + '.png' );
		lua.set ( 'fps', document.frameRate );
		lua.set ( 'width', document.width );
		lua.set ( 'height', document.height );
		lua.set ( 'spriteDeck', spriteDeckBuilder.spriteDeck );
		lua.set ( 'name', timeline.name );
		lua.set ( 'length', timeline.frameCount );
		
		var luaFrames = lua.setTable ( 'frames' );
		
		for ( var i = 0; i < keyframes.length; ++i ) {
			var keyframe = keyframes [ i ];
			
			var luaFrame = luaFrames.pushTable ();
	
			luaFrame.set ( 'start', keyframe.idx );
			
			if ( keyframe.nElements ) {
				var id = spriteDeckBuilder.addSpriteForFrameIdx ( document, timeline, atlas, keyframe.idx );
				luaFrame.set ( 'id', id + 1 );
			}
			else {
				luaFrame.set ( 'id', -1 );
			}
		}
		
		var string = lua.toLuaLocalTable ( 'anim' );
		string += '\n';
		string += 'return anim\n';
		fl.trace ( string );
		
		var luaFilePath = folderPath + document.name + '.lua';
		if ( !FLfile.write ( luaFilePath, string )) {
			alert( "Error saving file" ); 
		}
		
		var pngFilePath = folderPath + document.name + '.png';
		atlasDoc = atlas.createTextureAtlasDoc ();
		atlasDoc.exportPNG ( pngFilePath, true, true );
		fl.closeDocument ( atlasDoc, false );
	}
}

//----------------------------------------------------------------//
function getKeyframeList ( timeline ) {
	
	var keyframes = new Array ();
	
	var i = 0;
	var lastFrame = timeline.frameCount - 1;
	for ( ; i < timeline.frameCount; ++i ) {
		
		var isKeyframe = false;
		var nElements = 0;
		
		var layers = timeline.layers;
		for ( var j = 0; j < timeline.layers.length; ++j ) {
			var layer = layers [ j ];
			
			var frame = layer.frames [ i ];
			if ( !frame ) continue;
			
			nElements += frame.elements.length;
			
			if ( frame.startFrame == i ) {
				isKeyframe = true;
			}
		}
		
		if ( isKeyframe  || ( i == lastFrame )) {
			var keyframe = {};
			keyframe.idx = i;
			keyframe.nElements = nElements;
			keyframes.push ( keyframe );
		}
	}
	return keyframes;
}

//----------------------------------------------------------------//
function isKeyframe ( timeline, frameIdx ) {
	
	var layers = timeline.layers;
	for ( var i = 0; i < timeline.layers.length; ++i ) {
		var layer = layers [ i ];
		
		var frames = layer.frames;
		if ( frameIdx >= frames.length ) continue;
		
		var frame = layer.frames [ frameIdx ];
		
		if ( !frame ) continue;
		if ( frame.startFrame != i ) continue;
		
		return true;
	}
	return false;
}

exportToLua ( MOAI_TEXTURE_ATLAS_PADDING, MOAI_TEXTURE_ATLAS_FORCE_SQUARE );
