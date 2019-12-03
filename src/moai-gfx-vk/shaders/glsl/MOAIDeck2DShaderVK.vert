#version 450

layout ( location = 0 ) in vec4 inPos;
layout ( location = 1 ) in vec2 inUV;
layout ( location = 2 ) in vec4 inColor;

layout ( location = 0 ) out vec4 outColor;
layout ( location = 1 ) out vec2 outUV;

out gl_PerVertex {
    vec4 gl_Position;
};

void main () {
    gl_Position = inPos;
    outUV = inUV;
    outColor = inColor;
}
