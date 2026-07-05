#version 330 core

layout(location = 0) out vec4 color;
in vec2 uvCoord;

void main() {

    vec2 uv = uvCoord;


    color = vec4(uv.x, uv.y, 0.0, 1.0);
}
