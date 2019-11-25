#version 330 core
out vec4 color;

in vec2 v_TexCoords;

uniform sampler2D u_InputTexture;

void main() {
    vec3 col = texture(u_InputTexture, v_TexCoords).rgb;
    float grayscale = (col.r + col.g + col.b) / 3.0f;
    color = vec4(grayscale, grayscale, grayscale, 1.0f);
}
