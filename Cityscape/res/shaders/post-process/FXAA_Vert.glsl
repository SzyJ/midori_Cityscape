//vertex shader, nothing special here...
#version 400
layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec2 a_TexCoords;

void main () {
    gl_Position = vec4(a_Position.x, a_Position.y, 0.0f, 1.0f);
}
