#version 420 core

layout(quads, cw) in;

in Vertex {
    vec3 c_Position;
    vec2 c_TexCoord;
} IN[];

out Vertex {
    vec3 e_Position;
    vec3 e_Normal;
    vec2 e_TexCoord;
} OUT;

layout (std140, binding = 0) uniform MVP {
    mat4 u_ViewProjection;
    vec3 u_CameraPos;
    float u_Padding;
    mat4 u_Projection;
    mat4 u_StaticView;
};

uniform mat4 u_Transform;
uniform float u_TotalTime;
uniform sampler2D u_WaterNormal;
uniform sampler2D u_WaterDisplacement;

vec4 QuadMixVec4(vec4 a, vec4 b, vec4 c, vec4 d) {
    vec4 p0 = mix(a, c, gl_TessCoord.x);
    vec4 p1 = mix(b, d, gl_TessCoord.x);

    return mix(p0, p1, gl_TessCoord.y);
}

vec3 QuadMixVec3(vec3 a, vec3 b, vec3 c, vec3 d) {
    vec3 p0 = mix(a, c, gl_TessCoord.x);
    vec3 p1 = mix(b, d, gl_TessCoord.x);

    return mix(p0, p1, gl_TessCoord.y);
}

vec2 QuadMixVec2(vec2 a, vec2 b, vec2 c, vec2 d) {
    vec2 p0 = mix(a, c, gl_TessCoord.x);
    vec2 p1 = mix(b, d, gl_TessCoord.x);

    return mix(p0, p1, gl_TessCoord.y);
}

void main(){
    
    vec2 texCoord = QuadMixVec2(
        IN[0].c_TexCoord,
        IN[1].c_TexCoord,
        IN[2].c_TexCoord,
        IN[3].c_TexCoord
    );
    vec2 sampleCoords = texCoord * 2.0f;
    sampleCoords.y += u_TotalTime * 0.04f;
    OUT.e_TexCoord = sampleCoords;
    
    vec3 normals = texture(u_WaterNormal, sampleCoords).rbg;
    normals = normalize(normals);
    OUT.e_Normal = normals;

    vec3 worldPos = QuadMixVec3(
        IN[0].c_Position,
        IN[1].c_Position,
        IN[2].c_Position,
        IN[3].c_Position
    );

    worldPos.y += texture(u_WaterDisplacement, sampleCoords).r * 0.2f;
    OUT.e_Position = worldPos;

    gl_Position = u_ViewProjection * u_Transform * vec4(worldPos, 1.0);
}
