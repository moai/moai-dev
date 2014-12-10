//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//


//----------------------------------------------------------------//
var indentStr = function ( n ) {

	var str = '';
	for ( var i = 0; i < n; ++i ) {
		str += '\t';
	}
	return str;
}

//----------------------------------------------------------------//
var luaEscape = function ( val ) {

	var type = typeof val;
	
	if ( type == 'string' ) {
		return '\'' + val + '\'';
	}
	else if ( type == 'boolean' ) {
		return val ? 'true' : 'false';
	}
	return val;
}

//----------------------------------------------------------------//
var luaKeyVal = function ( key, val, indent ) {

	if ( typeof val == 'undefined' ) return;
	
	key = luaEscape ( key );
	if ( val.toLua ) {
		
		var lua = '';
		lua += indentStr ( indent ) + '[ ' + key + ' ] = {\n';
		lua += val.toLua ( indent + 1 );
		lua += indentStr ( indent ) + '},\n';
		return lua;
	}
	
	val = luaEscape ( val );
	return indentStr ( indent ) + '[ ' + key + ' ] = ' + val + ',\n';
}

//================================================================//
// LuaTable
//================================================================//
function LuaTable () {
	
	this.numeric = new Array ();
	this.named = {};
}

//----------------------------------------------------------------//
LuaTable.prototype.get = function ( index ) {
    
	var type = typeof index;
	
	if ( type == 'string' ) {
		return this.named [ index ];
	}
	else if ( type == 'number' ) {
		return this.numeric [ index ];
	}
}

//----------------------------------------------------------------//
LuaTable.prototype.push = function ( item ) {

	idx = this.numeric.length;
	this.numeric.push ( item );
	return idx;
}

//----------------------------------------------------------------//
LuaTable.prototype.pushTable = function () {

	var lua = new LuaTable ();
	this.push ( lua );
	return lua;
}

//----------------------------------------------------------------//
LuaTable.prototype.set = function ( index, item ) {

	var type = typeof index;
	
	if ( type == 'string' ) {
		this.named [ index ] = item;
	}
	else if ( type == 'number' ) {
		this.numeric [ index ] = item;
	}
}

//----------------------------------------------------------------//
LuaTable.prototype.setTable = function ( index ) {

	var lua = new LuaTable ();
	this.set ( index, lua );
	return lua;
}

//----------------------------------------------------------------//
LuaTable.prototype.toLua = function ( indent ) {
	
	if ( !indent ) {
		indent = 0;
	}
	
	var lua = '';
	
	var named = this.named;
	for ( var key in named ){
		
		var val = named [ key ];
		lua += luaKeyVal ( key, val, indent );
	}
	
	var numeric = this.numeric;
	for ( var i = 0; i < numeric.length; ++i ){
		
		var val = numeric [ i ];
		if ( val ) {
			lua += luaKeyVal ( i + 1, val, indent );
		}
	}
	
	return lua;
}

//----------------------------------------------------------------//
LuaTable.prototype.toLuaLocalTable = function ( name ) {
	
	var lua = 'local ' + name + ' = {\n';
	lua += this.toLua ( 1 );
	lua += '}\n';
	return lua;
}

//----------------------------------------------------------------//
LuaTable.prototype.toLuaTable = function ( name ) {
	
	var lua = name + ' = {\n';
	lua += this.toLua ( 1 );
	lua += '}\n';
	return lua;
}

//----------------------------------------------------------------//
function test () {
	
	var table = new LuaTable ();
	
	table.set ( 'foo', 'foo!' );
	table.set ( 'bar', 37 );
	table.set ( 2, 'baz!' );
	
	var table2 = new LuaTable ();
	table2.set ( 'biff', 'fuh!' );
	table2.set ( 'blapp', 37 );
	table2.set ( 2, 'blotto!' );
	table.set ( 5, table2 );
	
	var lua = table.toLuaLocalTable ( 'table' );
	fl.trace ( lua );
}
