//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc.
// All Rights Reserved.
// http://getmoai.com
//----------------------------------------------------------------//

fl.runScript ( fl.configURI + 'Commands/moai/Settings.jsfl' );
fl.runScript ( fl.configURI + 'Commands/moai/LuaTable.jsfl' );
fl.runScript ( fl.configURI + 'Commands/moai/TextureAtlas.jsfl' );

//----------------------------------------------------------------//
function exportGfxQuadDeck2D ( padding, forceSquare ) {
   
    var document = fl.getDocumentDOM ();
    var folderPath = fl.browseForFolderURL ( 'Select a folder.' ) + '/';
    if ( !( folderPath &&  folderPath.length )) return;
   
    var luaFilePath = folderPath + document.name + '.lua';
    var pngFilePath = folderPath + document.name + '.png';
   
    var atlas = new TextureAtlas ( padding, forceSquare );
    atlas.createTextureAtlas ();
   
    var symbolLibs = atlas.createLuaBrushDeck()
    symbolLibs.set ( 'texture', document.name + '.png' );
   
    var lua = symbolLibs.toLuaLocalTable ( 'brushDeck' );
    lua += '\n';
    lua += 'return brushDeck\n';
    fl.trace ( lua );
   
    if ( !FLfile.write ( luaFilePath, lua )) {
        alert( "Error saving file" );
    }
   
    atlasDoc = atlas.createTextureAtlasDoc ();
    atlasDoc.exportPNG ( pngFilePath, true, true );
    fl.closeDocument ( atlasDoc, false );
}
   
exportGfxQuadDeck2D ( MOAI_TEXTURE_ATLAS_PADDING, MOAI_TEXTURE_ATLAS_FORCE_SQUARE );