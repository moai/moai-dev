----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

--TODO: adapt this into a test

loremShort = 'Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce ultricies porttitor leo.'
loremLong = 'Lorem ipsum dolor sit amet, consectetur adipiscing elit. Duis id massa vel leo blandit pharetra. Aenean a nisl mi. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Nam quis magna sit amet diam fermentum consequat. Donec dapibus pharetra diam vel convallis. Pellentesque quis tellus mauris. Sed eget risus tortor, in cursus nisi. Sed ultrices nulla non nunc ullamcorper id venenatis urna ultrices. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Nam sodales tellus et diam imperdiet pharetra sagittis odio tempus. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nunc mollis adipiscing nibh ut malesuada. Proin rutrum volutpat est sed feugiat. Suspendisse at imperdiet justo. Pellentesque ullamcorper risus venenatis tellus elementum mattis. Quisque adipiscing feugiat orci vitae egestas.'

local base64TestVectors = {
	[ '' ] = {
		base64 = '',
		hex = '',
	},
	[ 'f' ] = {
		base64 = 'Zg==',
		hex = '66',
	},
	[ 'fo' ] = {
		base64 = 'Zm8=',
		hex = '666f',
	},
	[ 'foo' ] = {
		base64 = 'Zm9v',
		hex = '666f6f',
	},
	[ 'foob' ] = {
		base64 = 'Zm9vYg==',
		hex = '666f6f62',
	},
	[ 'fooba' ] = {
		base64 = 'Zm9vYmE=',
		hex = '666f6f6261',
	},
	[ 'foobar' ] = {
		base64 = 'Zm9vYmFy',
		hex = '666f6f626172',
	},
	[ loremShort ] = {
		base64 = 'TG9yZW0gaXBzdW0gZG9sb3Igc2l0IGFtZXQsIGNvbnNlY3RldHVyIGFkaXBpc2NpbmcgZWxpdC4gRnVzY2UgdWx0cmljaWVzIHBvcnR0aXRvciBsZW8u',
		hex = '4c6f72656d20697073756d20646f6c6f722073697420616d65742c20636f6e73656374657475722061646970697363696e6720656c69742e20467573636520756c7472696369657320706f72747469746f72206c656f2e',
	},
	[ loremLong ] = {
		base64 = 'TG9yZW0gaXBzdW0gZG9sb3Igc2l0IGFtZXQsIGNvbnNlY3RldHVyIGFkaXBpc2NpbmcgZWxpdC4gRHVpcyBpZCBtYXNzYSB2ZWwgbGVvIGJsYW5kaXQgcGhhcmV0cmEuIEFlbmVhbiBhIG5pc2wgbWkuIFZlc3RpYnVsdW0gYW50ZSBpcHN1bSBwcmltaXMgaW4gZmF1Y2lidXMgb3JjaSBsdWN0dXMgZXQgdWx0cmljZXMgcG9zdWVyZSBjdWJpbGlhIEN1cmFlOyBOYW0gcXVpcyBtYWduYSBzaXQgYW1ldCBkaWFtIGZlcm1lbnR1bSBjb25zZXF1YXQuIERvbmVjIGRhcGlidXMgcGhhcmV0cmEgZGlhbSB2ZWwgY29udmFsbGlzLiBQZWxsZW50ZXNxdWUgcXVpcyB0ZWxsdXMgbWF1cmlzLiBTZWQgZWdldCByaXN1cyB0b3J0b3IsIGluIGN1cnN1cyBuaXNpLiBTZWQgdWx0cmljZXMgbnVsbGEgbm9uIG51bmMgdWxsYW1jb3JwZXIgaWQgdmVuZW5hdGlzIHVybmEgdWx0cmljZXMuIEN1bSBzb2NpaXMgbmF0b3F1ZSBwZW5hdGlidXMgZXQgbWFnbmlzIGRpcyBwYXJ0dXJpZW50IG1vbnRlcywgbmFzY2V0dXIgcmlkaWN1bHVzIG11cy4gTmFtIHNvZGFsZXMgdGVsbHVzIGV0IGRpYW0gaW1wZXJkaWV0IHBoYXJldHJhIHNhZ2l0dGlzIG9kaW8gdGVtcHVzLiBMb3JlbSBpcHN1bSBkb2xvciBzaXQgYW1ldCwgY29uc2VjdGV0dXIgYWRpcGlzY2luZyBlbGl0LiBOdW5jIG1vbGxpcyBhZGlwaXNjaW5nIG5pYmggdXQgbWFsZXN1YWRhLiBQcm9pbiBydXRydW0gdm9sdXRwYXQgZXN0IHNlZCBmZXVnaWF0LiBTdXNwZW5kaXNzZSBhdCBpbXBlcmRpZXQganVzdG8uIFBlbGxlbnRlc3F1ZSB1bGxhbWNvcnBlciByaXN1cyB2ZW5lbmF0aXMgdGVsbHVzIGVsZW1lbnR1bSBtYXR0aXMuIFF1aXNxdWUgYWRpcGlzY2luZyBmZXVnaWF0IG9yY2kgdml0YWUgZWdlc3Rhcy4=',
		hex = '4c6f72656d20697073756d20646f6c6f722073697420616d65742c20636f6e73656374657475722061646970697363696e6720656c69742e2044756973206964206d617373612076656c206c656f20626c616e6469742070686172657472612e2041656e65616e2061206e69736c206d692e20566573746962756c756d20616e746520697073756d207072696d697320696e206661756369627573206f726369206c756374757320657420756c74726963657320706f737565726520637562696c69612043757261653b204e616d2071756973206d61676e612073697420616d6574206469616d206665726d656e74756d20636f6e7365717561742e20446f6e65632064617069627573207068617265747261206469616d2076656c20636f6e76616c6c69732e2050656c6c656e74657371756520717569732074656c6c7573206d61757269732e20536564206567657420726973757320746f72746f722c20696e20637572737573206e6973692e2053656420756c747269636573206e756c6c61206e6f6e206e756e6320756c6c616d636f727065722069642076656e656e617469732075726e6120756c7472696365732e2043756d20736f63696973206e61746f7175652070656e617469627573206574206d61676e6973206469732070617274757269656e74206d6f6e7465732c206e61736365747572207269646963756c7573206d75732e204e616d20736f64616c65732074656c6c7573206574206469616d20696d70657264696574207068617265747261207361676974746973206f64696f2074656d7075732e204c6f72656d20697073756d20646f6c6f722073697420616d65742c20636f6e73656374657475722061646970697363696e6720656c69742e204e756e63206d6f6c6c69732061646970697363696e67206e696268207574206d616c6573756164612e2050726f696e2072757472756d20766f6c7574706174206573742073656420666575676961742e2053757370656e646973736520617420696d70657264696574206a7573746f2e2050656c6c656e74657371756520756c6c616d636f727065722072697375732076656e656e617469732074656c6c757320656c656d656e74756d206d61747469732e20517569737175652061646970697363696e672066657567696174206f72636920766974616520656765737461732e',
	},
}

local printResult = function ( data, encodeFuncName, decodeFuncName, encoded, decoded, comp )

	local result = (( not comp ) or ( encoded == comp )) and ( data == decoded )
	local status = result and 'PASSED' or 'FAILED'

	print ( string.format ( '%s: %s, %s', status, encodeFuncName, decodeFuncName ))

	if not result then

		print ()

		print ( 'ORIGINAL' )
		print ( data )
		print ()

		print ( 'ENCODED' )
		print ( encoded )
		print ()

		if comp then
			print ( 'COMP' )
			print ( comp )
			print ()
		end
	end
end

local testDataBuffer = function ( data, encodeFuncName, decodeFuncName, comp )

	local encoded = MOAIDataBuffer [ encodeFuncName ]( data )
	local decoded = MOAIDataBuffer [ decodeFuncName ]( encoded )

	printResult ( data, encodeFuncName, decodeFuncName, encoded, decoded, comp )
end

local testAdapter = function ( data, openWriterFuncName, openReaderFuncName, comp )

	local len = #data

	local stream = MOAIMemStream.new ()
	stream:open ()

	local adapter = MOAIStreamAdapter.new ()

	adapter [ openWriterFuncName ]( adapter, stream )
	adapter:write ( data, len )
	adapter:close ()

	stream:seek ( 0 )
	local encoded = stream:read ( stream:getLength ())

	stream:seek ( 0 )
	adapter [ openReaderFuncName ]( adapter, stream )
	local decoded = adapter:read ( len )
	adapter:close ()

	printResult ( data, openWriterFuncName, openReaderFuncName, encoded, decoded, comp )
end

for k, v in pairs ( base64TestVectors ) do

	testDataBuffer ( k, 'base64Encode', 'base64Decode', v.base64  )
	testDataBuffer ( k, 'hexEncode', 'hexDecode', v.hex  )
	testDataBuffer ( k, 'deflate', 'inflate' )

	testAdapter ( k, 'openBase64Writer', 'openBase64Reader', v.base64 )
	testAdapter ( k, 'openHex', 'openHex', v.hex )
	testAdapter ( k, 'openDeflateWriter', 'openDeflateReader' )
end
