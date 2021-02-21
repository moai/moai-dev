#version 330

in mediump vec2 uvVarying;

out mediump vec4 fragmentColor;

uniform mediump vec2 direction;
uniform mediump sampler2D sampler0;

uniform highp float offsets [ 3 ];
uniform highp float weights [ 3 ];

// for gaussian blur
// this trick works by sampling at strategic locations between pixels to gather more information with fewer taps

//  offsets [ 0 ] = 0.0;
//  offsets [ 1 ] = 1.3846153846;
//  offsets [ 2 ] = 3.2307692308;
//
//  weights [ 0 ] = 0.2270270270;
//  weights [ 1 ] = 0.3162162162;
//  weights [ 2 ] = 0.0702702703;

//----------------------------------------------------------------//
void main () {

    fragmentColor = texture ( sampler0, uvVarying ) * weights [ 0 ];

    highp vec2 size = vec2 ( textureSize ( sampler0, 0 ));

    for ( int i = 1; i < 3; ++i ) {

        highp float xOff = ( offsets [ i ] / size.x ) * direction.x;
        highp float yOff = ( offsets [ i ] / size.y ) * direction.y;

        fragmentColor += texture ( sampler0, uvVarying + vec2 ( xOff, yOff )) * weights [ i ];
        fragmentColor += texture ( sampler0, uvVarying + vec2 ( -xOff, -yOff )) * weights [ i ];
    }
}
