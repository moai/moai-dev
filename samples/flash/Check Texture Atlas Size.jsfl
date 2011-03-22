//----------------------------------------------------------------//
function loadScript ( script ) {
	fl.runScript( fl.configURI + script ); 
}

loadScript ( 'Commands/moai/skyline.jsfl' );

var packed = getPackListForLibrary ();
var size = getTextureSize ( packed );

fl.trace ( 'Texture Atlas Size: ' + size.width + ' x ' + size.height );

