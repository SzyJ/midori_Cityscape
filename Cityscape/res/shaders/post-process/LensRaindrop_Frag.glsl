#version 330 core
out vec4 color;

in vec2 v_TexCoords;

uniform sampler2D u_InputTexture;
uniform float u_TotalTime;
uniform uint u_FrameWidth;
uniform uint u_FrameHeight;

float N21(vec2 p) {
    p = fract(p * vec2(123.45, 345.45));
    p += dot(p, p + 34.345);
    return fract(p.x * p.y);
}

void main() {
    // Config
    const float gridSize = 5.0f;
    const float distortion = -0.5f;

    // Base Color
    vec3 col = vec3(0.0f, 0.0f, 0.0f);

    float t = mod(u_TotalTime, 7200);

    //vec2 asp = normalize(vec2(u_FrameWidth * 0.5f, u_FrameHeight));
    vec2 invAspect = vec2(2.0f, 1.0f);

    vec2 uv = v_TexCoords * gridSize * invAspect;
    uv.y += t * 0.25f;
    vec2 gv = fract(uv) - 0.45f;

    vec2 id = floor(uv);

    float rng = N21(id);
    t += rng * 2 * 3.15;
    float w = (v_TexCoords.y) * 10.0f;
    float x = (rng - 0.5f) * 0.5f;

    float wiggle = sin(3 * w) * sin(w) * sin(w) * sin(w) * sin(w) * sin(w) * sin(w);
    x += (0.4f - abs(x)) * wiggle;

    //float x = sin(3 * w) * pow(sin(w), 6.0f) * 0.4f;
    float y = sin(t + sin(t + sin(t) * 0.5f)) * -0.4f;
    y -= (gv.x - x) * (gv.x - x);

    vec2 dropPos = (gv - vec2(x, y)) / invAspect;
    float drop = smoothstep(0.05, 0.03, length(dropPos));

    vec2 trailPos = (gv - vec2(x, t * 0.25f)) / invAspect;
    trailPos.y = (fract(trailPos.y * 8) - 0.5f) / 8;
    float trail = smoothstep(0.03, 0.01, length(trailPos));

    float fogTrail = smoothstep(-0.05f, 0.05f, dropPos.y); // Only show trail above drop
    fogTrail *= smoothstep(0.5f, y, gv.y);

    trail *= fogTrail;

    fogTrail *= smoothstep(0.05f, 0.04f, abs(dropPos.x));

    col += fogTrail * 0.5f;
    col += trail;
    col += drop;

    //if (gv.x > 0.48f  || gv.y > 0.49f) {
    //    col = vec3(1.0f, 0.0f, 0.0f);
    //}

    vec2 offset = drop * dropPos + vec2(trail) * trailPos;
    vec4 frameTex = texture(u_InputTexture, v_TexCoords + (offset * distortion));

    color = vec4(frameTex.rgb, 1.0f);
}
