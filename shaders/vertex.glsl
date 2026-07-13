#version 330 core

layout(location = 0) in vec4 position;
out vec2 uvCoord;

uniform mat4 uMVP;

void main() {
    gl_Position = uMVP * position;
    uvCoord = position.xy * 0.5 + 0.5;
}