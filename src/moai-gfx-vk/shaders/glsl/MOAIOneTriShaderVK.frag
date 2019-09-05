#version 450

layout (location = 0) in vec3 inColor;
layout (location = 1) in vec2 inUV;

layout ( binding = 1 ) uniform sampler2D texSampler;

layout ( location = 0 ) out vec4 outFragColor;

void main () {
    outFragColor = vec4 ( inColor * texture ( texSampler, inUV ).rgb, 1.0 );
}
