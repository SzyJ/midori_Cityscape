#version 420 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out Vertex {
    vec3 v_Position;
    vec2 v_TexCoord;
} OUT;

void main() {
    OUT.v_Position = a_Position;
    OUT.v_TexCoord = a_TexCoord;

    gl_Position = vec4(a_Position, 1.0);
}
