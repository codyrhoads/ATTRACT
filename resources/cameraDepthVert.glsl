#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

void main() {
    gl_Position = P * V * M * vec4(aPos.xyz, 1.0);
}
