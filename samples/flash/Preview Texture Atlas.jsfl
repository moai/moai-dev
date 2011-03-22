//----------------------------------------------------------------//
function loadScript ( script ) {
	fl.runScript( fl.configURI + script ); 
}

loadScript ( 'Commands/moai/skyline.jsfl' );

var atlas = createTextureAtlas ();
createTextureAtlasDoc ( atlas );

