
-- test case
local TestMOAIImage = {}
_G.TestMOAIImage = TestMOAIImage

---
-- setUp
function TestMOAIImage:setUp()
    self.image = MOAIImage.new()
    self.image:load("assets/moai.png")
end

---
-- tearDown
function TestMOAIImage:tearDown()
end

---
-- test: bleedRect
function TestMOAIImage:test_bleedRect()
    self.image:bleedRect( 0, 0, 32, 32)
    self.image:writePNG( "../testout/TestMOAIImage_test_bleedRect.png" )
end

---
-- test: compare
function TestMOAIImage:test_compare()
    local imageA = MOAIImage.new()
    local imageB = MOAIImage.new()

    imageA:load("assets/moai.png")
    imageB:load("assets/cathead.png")

    assertTrue(self.image:compare(imageA))
    assertFalse(self.image:compare(imageB))
    assertFalse(imageA:compare(imageB))
end

---
-- test: convertColors
-- TODO: Error
function TestMOAIImage:test_convertColors()
    local imageFMT_A_8 = self.image:convertColors(MOAIImage.COLOR_FMT_A_8)
    local imageFMT_RGB_888 = self.image:convertColors(MOAIImage.COLOR_FMT_RGB_888)
    local imageFMT_RGB_565 = self.image:convertColors(MOAIImage.COLOR_FMT_RGB_565)
    local imageFMT_RGBA_5551 = self.image:convertColors(MOAIImage.COLOR_FMT_RGBA_5551)
    local imageFMT_RGBA_4444 = self.image:convertColors(MOAIImage.COLOR_FMT_RGBA_4444)
    local imageFMT_RGBA_8888 = self.image:convertColors(MOAIImage.COLOR_FMT_RGBA_8888)

    --imageFMT_A_8:writePNG( "../testout/TestMOAIImage_test_convertColors_FMT_A_8.png" )
    imageFMT_RGB_888:writePNG( "../testout/TestMOAIImage_test_convertColors_FMT_RGB_888.png" )
    --imageFMT_RGB_565:writePNG( "../testout/TestMOAIImage_test_convertColors_FMT_RGB_565.png" )
    --imageFMT_RGBA_5551:writePNG( "../testout/TestMOAIImage_test_convertColors_FMT_RGBA_5551.png" )
    --imageFMT_RGBA_4444:writePNG( "../testout/TestMOAIImage_test_convertColors_FMT_RGBA_4444.png" )
    imageFMT_RGBA_8888:writePNG( "../testout/TestMOAIImage_test_convertColors_FMT_RGBA_8888.png" )
end

---
-- test: copy
function TestMOAIImage:test_copy()
    local destImage = self.image:copy()

    assertTrue(self.image:compare(destImage))
    destImage:writePNG( "../testout/TestMOAIImage_test_copy.png" )
end

---
-- test: copyBits
function TestMOAIImage:test_copyBits()
    local srcImage = MOAIImage.new()
    srcImage:load("assets/cathead.png")

    self.image:copyBits(srcImage, 10, 20, 5, 10, 32, 32)
    self.image:writePNG( "../testout/TestMOAIImage_test_copyBits.png" )

    assertEquals(self.image:getColor32(5, 10), srcImage:getColor32(10, 20))
    assertEquals(self.image:getColor32(5 + 32 - 1, 10 + 32 - 1), srcImage:getColor32(10 + 32 - 1, 20 + 32 - 1))
    assertNotEquals(self.image:getColor32(5 + 32, 10 + 32), srcImage:getColor32(10 + 32, 20 + 32))
end

---
-- test: copyRect
function TestMOAIImage:test_copyRect()
    local srcImage = MOAIImage.new()
    srcImage:load("assets/cathead.png")

    self.image:copyRect(srcImage, 10, 20, 10 + 32, 20 + 32, 5, 10)
    self.image:writePNG( "../testout/TestMOAIImage_test_copyRect.png" )

    assertEquals(self.image:getColor32(5, 10), srcImage:getColor32(10, 20))
    assertEquals(self.image:getColor32(5 + 32 - 1, 10 + 32 - 1), srcImage:getColor32(10 + 32 - 1, 20 + 32 - 1))
    assertNotEquals(self.image:getColor32(5 + 32, 10 + 32), srcImage:getColor32(10 + 32, 20 + 32))
end

---
-- test: fillRect
-- TODO:Unimplemented
function TestMOAIImage:test_fillCircle()
    assertTrue(false)
end

---
-- test: fillRect
function TestMOAIImage:test_fillRect()
    self.image:fillRect ( 32, 32, 96, 96, 1, 1, 0, 1 )
    self.image:writePNG( "../testout/TestMOAIImage_test_fillRect.png" )

    local r, g, b, a = self.image:getRGBA(32, 32)
    assertEquals(r, 1)
    assertEquals(g, 1)
    assertEquals(b, 0)
    assertEquals(a, 1)
end

---
-- test: getColor32
function TestMOAIImage:test_getColor32()
    self.image:setColor32( 32, 32, 0xFFFF5555)
    assertEquals(self.image:getColor32(32, 32), 0xFFFF5555)
end

---
-- test: getFormat
function TestMOAIImage:test_getFormat()
    local format = self.image:getFormat()
    assertEquals(format, MOAIImage.COLOR_FMT_RGB_888)
end

---
-- test: load
function TestMOAIImage:test_load()
    local imageA = MOAIImage.new()
    local imageB = MOAIImage.new()

    imageA:load("assets/moai.png")
    imageB:load("assets/moai.jpg")
end
