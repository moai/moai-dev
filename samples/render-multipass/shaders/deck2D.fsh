varying LOWP vec4 colorVarying;
varying MEDP vec2 uvVarying;

uniform sampler2D sampler;

void main () {
    gl_FragColor = texture2D ( sampler, uvVarying ) * colorVarying;
}