varying LOWP vec4 colorVarying;
varying MEDP vec2 uvVarying;

uniform sampler2D sampler;

void main () {
    gl_FragColor = pow ( texture2D ( sampler, uvVarying ) * colorVarying, vec4 ( 4.0, 4.0, 4.0, 1.0 ));
}