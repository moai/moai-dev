----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

-- test vector hash generation:
-- http://www.timestampgenerator.com/

testVectors = {
	'',
	'a',
	'abc',
	'Moai SDK',
	'Moai Cloud',
	'message digest',
	'abcdefghijklmnopqrstuvwxyz',
	'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789',
	'12345678901234567890123456789012345678901234567890123456789012345678901234567890',
}

resultsAdler32 = {
	'00000001', -- ''
	'00620062', -- 'a',
	'024d0127', -- 'abc',
	'0c180289', -- 'Moai SDK'
	'1334039e', -- 'Moai Cloud'
	'29750586', -- 'message digest',
	'90860b20', -- 'abcdefghijklmnopqrstuvwxyz',
	'8adb150c', -- 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789',
	'97b61069', -- '12345678901234567890123456789012345678901234567890123456789012345678901234567890',
}

resultsCRC32 = {
	'00000000', -- ''
	'6b9b9319', -- 'a',
	'352441c2', -- 'abc',
	'461dd054', -- 'Moai SDK'
	'3b68c8fb', -- 'Moai Cloud'
	'20159d7f', -- 'message digest',
	'4c2750bd', -- 'abcdefghijklmnopqrstuvwxyz',
	'1fc2e6d2', -- 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789',
	'7ca94a72', -- '12345678901234567890123456789012345678901234567890123456789012345678901234567890',
}

resultsCRC32b = {
	'00000000', -- ''
	'e8b7be43', -- 'a',
	'352441c2', -- 'abc',
	'461dd054', -- 'Moai SDK'
	'3b68c8fb', -- 'Moai Cloud'
	'20159d7f', -- 'message digest',
	'4c2750bd', -- 'abcdefghijklmnopqrstuvwxyz',
	'1fc2e6d2', -- 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789',
	'7ca94a72', -- '12345678901234567890123456789012345678901234567890123456789012345678901234567890',
}

resultsMD5 = {
	'd41d8cd98f00b204e9800998ecf8427e', -- ''
	'0cc175b9c0f1b6a831c399e269772661', -- 'a',
	'900150983cd24fb0d6963f7d28e17f72', -- 'abc',
	'339254469b112909acbf7899747185b3', -- 'Moai SDK'
	'a345004cf8ec5f82e7b74891f91f3e7b', -- 'Moai Cloud'
	'f96b697d7cb7938d525a2f31aaf161d0', -- 'message digest',
	'c3fcd3d76192e4007dfb496cca67e13b', -- 'abcdefghijklmnopqrstuvwxyz',
	'd174ab98d277d9f5a5611c2c9f419d9f', -- 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789',
	'57edf4a22be3c955ac49da2e2107b67a', -- '12345678901234567890123456789012345678901234567890123456789012345678901234567890',
}

resultsSHA1 = {
	'da39a3ee5e6b4b0d3255bfef95601890afd80709', -- ''
	'86f7e437faa5a7fce15d1ddcb9eaeaea377667b8', -- 'a',
	'a9993e364706816aba3e25717850c26c9cd0d89d', -- 'abc',
	'97cbe3262280c59ef6a81d4477ddfd7d3bd24725', -- 'Moai SDK'
	'4a4369fb2f86b0f00ee8d661a9dfc0aa2f5a1f10', -- 'Moai Cloud'
	'c12252ceda8be8994d5fa0290a47231c1d16aae3', -- 'message digest',
	'32d10c7b8cf96570ca04ce37f2a19d84240d3a89', -- 'abcdefghijklmnopqrstuvwxyz',
	'761c457bf73b14d27e9e9265c46f4b4dda11f940', -- 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789',
	'50abf5706a150990a08b2c5ea40fa0e585554732', -- '12345678901234567890123456789012345678901234567890123456789012345678901234567890',
}

resultsSHA224 = {
	'd14a028c2a3a2bc9476102bb288234c415a2b01f828ea62ac5b3e42f', -- ''
	'abd37534c7d9a2efb9465de931cd7055ffdb8879563ae98078d6d6d5', -- 'a'
	'23097d223405d8228642a477bda255b32aadbce4bda0b3f7e36c9da7', -- 'abc'
	'0fd58033830f9c9bd2af0726df7f8e5edff3ddcfb6a6a2b771cde1b1', -- 'Moai SDK'
	'f81437a5bcfe271d06402785bb847bb0735a02d92a30dff5e79f1792', -- 'Moai Cloud'
	'2cb21c83ae2f004de7e81c3c7019cbcb65b71ab656b22d6d0c39b8eb', -- 'message digest',
	'45a5f72c39c5cff2522eb3429799e49e5f44b356ef926bcf390dccc2', -- 'abcdefghijklmnopqrstuvwxyz',
	'bff72b4fcb7d75e5632900ac5f90d219e05e97a7bde72e740db393d9', -- 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789',
	'b50aecbe4e9bb0b57bc5f3ae760a8e01db24f203fb3cdcd13148046e', -- '12345678901234567890123456789012345678901234567890123456789012345678901234567890'
}

resultsSHA256 = {
	'e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855', -- ''
	'ca978112ca1bbdcafac231b39a23dc4da786eff8147c4e72b9807785afee48bb', -- 'a'
	'ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad', -- 'abc'
	'b0dc800391022b504d4a150919b5ebaf3cc9edff3256d149d8bdb00196ad9a6a', -- 'Moai SDK'
	'3643c30a54c9609d345a3b75029a646a838b9e0867b85250873be96ee935f110', -- 'Moai Cloud'
	'f7846f55cf23e14eebeab5b4e1550cad5b509e3348fbc4efa3a1413d393cb650', -- 'message digest',
	'71c480df93d6ae2f1efad1447c66c9525e316218cf51fc8d9ed832f2daf18b73', -- 'abcdefghijklmnopqrstuvwxyz',
	'db4bfcbd4da0cd85a60c3c37d3fbd8805c77f15fc6b1fdfe614ee0a7c8fdb4c0', -- 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789',
	'f371bc4a311f2b009eef952dd83ca80e2b60026c8e935592d0f9c308453c813e', -- '12345678901234567890123456789012345678901234567890123456789012345678901234567890'
}

resultsSHA384 = {
	'38b060a751ac96384cd9327eb1b1e36a21fdb71114be07434c0cc7bf63f6e1da274edebfe76f65fbd51ad2f14898b95b', -- ''
	'54a59b9f22b0b80880d8427e548b7c23abd873486e1f035dce9cd697e85175033caa88e6d57bc35efae0b5afd3145f31', -- 'a',
	'cb00753f45a35e8bb5a03d699ac65007272c32ab0eded1631a8b605a43ff5bed8086072ba1e7cc2358baeca134c825a7', -- 'abc',
	'ea97255a500d9744337db77b02a1ad0c047722a155b555cdf313d0699b59fd7cc1eb88314cec24f478a0bef23ad3dc03', -- 'Moai SDK'
	'bd53bbf1a28dd35ecc98023034e7d51d66bd3351e03f53caa20164afeeda24947fbbdf978abcad62d6f4c0b1343eae08', -- 'Moai Cloud'
	'473ed35167ec1f5d8e550368a3db39be54639f828868e9454c239fc8b52e3c61dbd0d8b4de1390c256dcbb5d5fd99cd5', -- 'message digest',
	'feb67349df3db6f5924815d6c3dc133f091809213731fe5c7b5f4999e463479ff2877f5f2936fa63bb43784b12f3ebb4', -- 'abcdefghijklmnopqrstuvwxyz',
	'1761336e3f7cbfe51deb137f026f89e01a448e3b1fafa64039c1464ee8732f11a5341a6f41e0c202294736ed64db1a84', -- 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789',
	'b12932b0627d1c060942f5447764155655bd4da0c9afa6dd9b9ef53129af1b8fb0195996d2de9ca0df9d821ffee67026', -- '12345678901234567890123456789012345678901234567890123456789012345678901234567890',
}

resultsSHA512 = {
	'cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e', -- ''
	'1f40fc92da241694750979ee6cf582f2d5d7d28e18335de05abc54d0560e0f5302860c652bf08d560252aa5e74210546f369fbbbce8c12cfc7957b2652fe9a75', -- 'a',
	'ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a2192992a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f', -- 'abc',
	'b80942ddee7054a139c0d2928bf302ab76288e40c25f3b80dcd11dff08b1e46c80c868c7f08a165cd71fca93931a06f9f9f1d514920d969e95ecaaee0e77ad43', -- 'Moai SDK'
	'a9540558c7cc7dc1e4873762a4cfd1af4c2fa1bd8d8073ad05562ca5a6f76d87e6536c0c217cc9a614a99ef04e3d26045d28618f3a5bca4caa2baf16d259ec26', -- 'Moai Cloud'
	'107dbf389d9e9f71a3a95f6c055b9251bc5268c2be16d6c13492ea45b0199f3309e16455ab1e96118e8a905d5597b72038ddb372a89826046de66687bb420e7c', -- 'message digest',
	'4dbff86cc2ca1bae1e16468a05cb9881c97f1753bce3619034898faa1aabe429955a1bf8ec483d7421fe3c1646613a59ed5441fb0f321389f77f48a879c7b1f1', -- 'abcdefghijklmnopqrstuvwxyz',
	'1e07be23c26a86ea37ea810c8ec7809352515a970e9253c26f536cfc7a9996c45c8370583e0a78fa4a90041d71a4ceab7423f19c71b9d5a3e01249f0bebd5894', -- 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789',
	'72ec1ef1124a45b047e8b7c75a932195135bb61de24ec0d1914042246e0aec3a2354e093d76f3048b456764346900cb130d2a4fd5dd16abb5e30bcb850dee843', -- '12345678901234567890123456789012345678901234567890123456789012345678901234567890',
}

resultsWhirlpool = {
	'19fa61d75522a4669b44e39c1d2e1726c530232130d407f89afee0964997f7a73e83be698b288febcf88e3e03c4f0757ea8964e59b63d93708b138cc42a66eb3', -- ''
	'8aca2602792aec6f11a67206531fb7d7f0dff59413145e6973c45001d0087b42d11bc645413aeff63a42391a39145a591a92200d560195e53b478584fdae231a', -- 'a',
	'4e2448a4c6f486bb16b6562c73b4020bf3043e3a731bce721ae1b303d97e6d4c7181eebdb6c57e277d0e34957114cbd6c797fc9d95d8b582d225292076d4eef5', -- 'abc',
	'319425947af11ea0a73495011355ee217dc4d6180443b30216e2def2f4e8f459efff1aa0bc095a586ad5937fdbd49225f8c3b5533a5adb1b562419a74adad028', -- 'Moai SDK'
	'ca7a0c1c0fb3904e66c9136a2946f2c534fdda2d616b3159a08e6d484259d9231a1b3a5d664a933fe429ad9366d79056791bd2b5507bfd225471c65119bc6ee8', -- 'Moai Cloud'
	'378c84a4126e2dc6e56dcc7458377aac838d00032230f53ce1f5700c0ffb4d3b8421557659ef55c106b4b52ac5a4aaa692ed920052838f3362e86dbd37a8903e', -- 'message digest',
	'f1d754662636ffe92c82ebb9212a484a8d38631ead4238f5442ee13b8054e41b08bf2a9251c30b6a0b8aae86177ab4a6f68f673e7207865d5d9819a3dba4eb3b', -- 'abcdefghijklmnopqrstuvwxyz',
	'dc37e008cf9ee69bf11f00ed9aba26901dd7c28cdec066cc6af42e40f82f3a1e08eba26629129d8fb7cb57211b9281a65517cc879d7b962142c65f5a7af01467', -- 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789',
	'466ef18babb0154d25b9d38a6414f5c08784372bccb204d6549c4afadb6014294d5bd8df2a6c44e538cd047b2681a51a2c60481e88c5a20b2c2a80cf3a9a083b', -- '12345678901234567890123456789012345678901234567890123456789012345678901234567890',
}

function md5 ( data )
	local writer = MOAIHashWriter.new ()
	writer:openMD5 ()
	writer:write ( data )
	writer:close ()
	return writer:getHashHex ()
end

function adler32 ( data )
	local writer = MOAIHashWriter.new ()
	writer:openAdler32 ()
	writer:write ( data )
	writer:close ()
	return writer:getHashHex ()
end

function crc32 ( data )
	local writer = MOAIHashWriter.new ()
	writer:openCRC32 ()
	writer:write ( data )
	writer:close ()
	return writer:getHashHex ()
end

function crc32b ( data )
	local writer = MOAIHashWriter.new ()
	writer:openCRC32b ()
	writer:write ( data )
	writer:close ()
	return writer:getHashHex ()
end

function sha1 ( data )
	local writer = MOAIHashWriter.new ()
	writer:openSHA1 ()
	writer:write ( data )
	writer:close ()
	return writer:getHashHex ()
end

function sha256 ( data )
	local writer = MOAIHashWriter.new ()
	writer:openSHA256 ()
	writer:write ( data )
	writer:close ()
	return writer:getHashHex ()
end

function sha224 ( data )
	local writer = MOAIHashWriter.new ()
	writer:openSHA224 ()
	writer:write ( data )
	writer:close ()
	return writer:getHashHex ()
end

function sha384 ( data )
	local writer = MOAIHashWriter.new ()
	writer:openSHA384 ()
	writer:write ( data )
	writer:close ()
	return writer:getHashHex ()
end

function sha512 ( data )
	local writer = MOAIHashWriter.new ()
	writer:openSHA512 ()
	writer:write ( data )
	writer:close ()
	return writer:getHashHex ()
end

function whirlpool ( data )
	local writer = MOAIHashWriter.new ()
	writer:openWhirlpool ()
	writer:write ( data )
	writer:close ()
	return writer:getHashHex ()
end

function test ( name, func, vector, result )
	local ok = result == func ( vector ) and 'OK' or 'ERROR'
	print ( string.format ( '  %s %s', name, ok ))
end

for i, v in ipairs ( testVectors ) do

	print ( string.format ( 'vector %d', i ))
	test ( 'MD5', md5, v, resultsMD5 [ i ])
	test ( 'Adler32', adler32, v, resultsAdler32 [ i ])
	test ( 'CRC32', crc32, v, resultsCRC32 [ i ])
	test ( 'CRC32b', crc32b, v, resultsCRC32b [ i ])
	test ( 'SHA1', sha1, v, resultsSHA1 [ i ])
	test ( 'SHA224', sha224, v, resultsSHA224 [ i ])
	test ( 'SHA256', sha256, v, resultsSHA256 [ i ])
	test ( 'SHA384', sha384, v, resultsSHA384 [ i ])
	test ( 'SHA512', sha512, v, resultsSHA512 [ i ])
	test ( 'Whirlpool', whirlpool, v, resultsWhirlpool [ i ])
	
	print ()
end
