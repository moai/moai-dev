#version 330

in vec4 position;
in vec2 uv;
in vec4 color;

out mediump vec2 uvVarying;

//----------------------------------------------------------------//
void main () {

    gl_Position = position;
    uvVarying = uv;
}
