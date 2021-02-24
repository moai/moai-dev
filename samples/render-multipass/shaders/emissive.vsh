
in vec4 position;
in vec2 uv;
in vec4 color;

out MEDP vec2 uvVarying;
out LOWP vec4 colorVarying;

void main () {
    gl_Position = position;
    uvVarying = uv;
    colorVarying = color;
}
