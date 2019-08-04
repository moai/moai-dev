#version 450

layout ( location = 0 ) in vec4 inPos;
layout ( location = 1 ) in vec2 uv;
layout ( location = 2 ) in vec4 inColor;

layout ( location = 0 ) out vec4 outColor;

out gl_PerVertex {
    vec4 gl_Position;
};

void main () {
    gl_Position = inPos;
    outColor = inColor;
}
