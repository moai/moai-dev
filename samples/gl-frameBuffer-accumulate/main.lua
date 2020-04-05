-- when rendering offscreen, the render target cannot also be sampled. this example uses two
-- offscreen buffers to render a frame and then uses that frame as an input for the next.
-- frameBuffer1 refpresents the *previous* frame. frameBuffer2 represents the *current* frame.
-- so we render frameBuffer1->frameBuffer2, then frameBuffer2->device.

-- a big thank you to @sshukul for motivating this example!

local vsh = [[
    attribute MEDP vec4 position;  //IN
    attribute MEDP vec2 uv; //IN
    varying MEDP vec2 uvVarying;  //OUT
    void main () {
        gl_Position = position;
        uvVarying = uv;
    }
]]

function left_green_box_shader ()

    local fsh = [[
        varying MEDP vec2 uvVarying;
        void main() {
            if (uvVarying.x < .05 && uvVarying.y > .45 && uvVarying.y <= .5) {
                gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0); 
            }
            else {
                gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0); 
            }
        }
    ]]
    local program = MOAIShaderProgram.new ()    
    program:setVertexAttribute ( 1, 'position' )
    program:setVertexAttribute ( 2, 'uv' )
    program:load ( vsh, fsh )

    local shader = MOAIShader.new ()
    shader:setProgram ( program )
    return shader
end

function move_green_right_shader ()

    -- NOTE: 0.00078125 is one offscreen pixel (1/1280). see the font snapping shaders for an
    -- example of how to derive this using a pipeline global loaded into a uniform.

    local fsh = [[  
        varying MEDP vec2 uvVarying;
        uniform sampler2D sampler;
        void main () {

            vec4 left = texture2D ( sampler, vec2 ( uvVarying.x - 0.00078125, uvVarying.y ));
            if ( left.y > 0.5 ) {
                gl_FragColor = left;
            }
            else {
                gl_FragColor = texture2D ( sampler, uvVarying );
            }
        }
    ]]
    
    local program = MOAIShaderProgram.new ()
    program:setVertexAttribute ( 1, 'position' )
    program:setVertexAttribute ( 2, 'uv' )
    program:load ( vsh, fsh )

    local shader = MOAIShader.new ()
    shader:setProgram ( program )
    return shader
end

MOAISim.openWindow ( "test", 1280, 720 )

viewport = MOAIViewport.new ()
viewport:setSize ( 1280, 720 )
viewport:setScale ( 1280, 720 )

-- need two offscreen buffers: one for reading, one for writing
frameBuffer1 = MOAIFrameBufferTexture.new ()
frameBuffer1:init ( 1280, 720 )

frameBuffer2 = MOAIFrameBufferTexture.new ()
frameBuffer2:init ( 1280, 720 )

-- one layer/prop/deck for offscreen rendering (render target is a framebuffer)
offscreenLayer = MOAIPartitionViewLayer.new ()
offscreenLayer:setViewport ( viewport )
offscreenLayer:setFrameBuffer ( frameBuffer2 ) -- render *to* frameBuffer2
offscreenLayer:pushRenderPass () -- add the layer to the render queue

offscreenQuad = MOAISpriteDeck2D.new ()
offscreenQuad:setTexture ( frameBuffer1 ) -- render *from* frameBuffer1 (empty to begin with, but initialized by green rect shader)
offscreenQuad:setRect ( -640, -360, 640, 360 )
offscreenQuad:setUVRect ( 0, 0, 1, 1 )
offscreenQuad:setShader ( left_green_box_shader ()) -- inititalize with green rect shader

offscreenProp = MOAIProp.new ()
offscreenProp:setDeck ( offscreenQuad )
offscreenProp:setPartition ( offscreenLayer )

-- one layer/prop/deck for screen rendering (render target is default - the device screen)
layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )
layer:pushRenderPass () -- add the layer to the render queue

quad = MOAISpriteDeck2D.new ()
quad:setTexture ( frameBuffer2 ) -- render *from* frameBuffer2
quad:setRect ( -640, -360, 640, 360 )
quad:setUVRect ( 0, 0, 1, 1 )

prop = MOAIProp.new ()
prop:setDeck ( quad )
prop:setPartition ( layer )

-- toggle the buffers every second (i.e. step the shader animation)
updateThread = MOAITimer.new ()
updateThread:setSpan ( 1.0 )
updateThread:setMode ( MOAITimer.LOOP )
updateThread:setListener ( MOAITimer.EVENT_TIMER_LOOP, function ()

    offscreenQuad:setShader ( move_green_right_shader ()) -- from now on, sample and move green color to the right

    -- toggle the buffers
    local temp = frameBuffer1
    frameBuffer1 = frameBuffer2
    frameBuffer2 = temp

    -- fb1 and fb2 have toggled, so update all their clients
    offscreenLayer:setFrameBuffer ( frameBuffer2 )
    offscreenQuad:setTexture ( frameBuffer1 )
    quad:setTexture ( frameBuffer2 )
end )
updateThread:start ()
