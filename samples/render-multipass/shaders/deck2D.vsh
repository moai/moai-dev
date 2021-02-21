attribute vec4 position;
attribute vec2 uv;
attribute vec4 color;

varying LOWP vec4 colorVarying;
varying MEDP vec2 uvVarying;

void main () {
    gl_Position = position;
    uvVarying = uv;
    colorVarying = color;
}