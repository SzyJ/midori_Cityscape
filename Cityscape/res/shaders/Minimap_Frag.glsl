#version 330 core
out vec4 color;

in vec2 v_TexCoords;

uniform sampler2D u_InputTexture;

void main() {
    float opacity = 0.8f;
    vec3 col = texture(u_InputTexture, v_TexCoords).rgb;

    col = vec3(1.0f - col.r, 1.0f - col.g, 1.0f - col.b);
    col *= 25.0f;
    color = vec4(col, opacity);
}
