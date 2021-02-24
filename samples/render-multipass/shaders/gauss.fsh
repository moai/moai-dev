
in MEDP vec2 uvVarying;

out MEDP vec4 fragmentColor;

uniform MEDP vec2 direction;
uniform MEDP sampler2D sampler;

// for gaussian blur
// this trick works by sampling at strategic locations between pixels to gather more information with fewer taps

//const HIGHP float offsets [ 3 ] = float [ 3 ](
//    0.0,
//    1.3846153846,
//    3.2307692308
//);

//const HIGHP float weights [ 3 ] = float [ 3 ](
//    0.2270270270,
//    0.3162162162,
//    0.0702702703
//);

const HIGHP float offsets [ 3 ] = float [ 3 ](
    0.0,
    3.5,
    7.0
);

const HIGHP float weights [ 3 ] = float [ 3 ](
    0.3,
    0.3162162162,
    0.0702702703
);

//----------------------------------------------------------------//
void main () {

    fragmentColor = texture ( sampler, uvVarying ) * weights [ 0 ];

    HIGHP vec2 size = vec2 ( textureSize ( sampler, 0 ));

    for ( int i = 1; i < 3; ++i ) {

        HIGHP float xOff = ( offsets [ i ] / size.x ) * direction.x;
        HIGHP float yOff = ( offsets [ i ] / size.y ) * direction.y;

        fragmentColor += texture ( sampler, uvVarying + vec2 ( xOff, yOff )) * weights [ i ];
        fragmentColor += texture ( sampler, uvVarying + vec2 ( -xOff, -yOff )) * weights [ i ];
    }
}
