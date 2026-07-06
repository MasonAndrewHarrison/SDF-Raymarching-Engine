#version 330 core

layout(location = 0) out vec4 color;
in vec2 uvCoord;

uniform float uTime;
uniform vec2 uResolution;

void main() {

    vec2 uv = uvCoord;
    vec2 offset = uv - vec2(0.5, 0.5);
    uv.x *= uResolution.x/uResolution.y;

    float angle = uTime;         
    float c = cos(angle);
    float s = sin(angle);

    offset = vec2(offset.x * c - offset.y * s, offset.x * s + offset.y * c);
    offset *= vec2(3.0, 7.0);

    float d = exp(-dot(offset, offset));


    color = vec4(d, d, d, 1.0);
}
