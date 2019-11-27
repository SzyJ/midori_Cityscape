#version 420 core

layout(vertices = 4) out;

in Vertex {
    vec3 v_Position;
    vec2 v_TexCoord;
} IN[];

out Vertex {
    vec3 c_Position;
    vec2 c_TexCoord;
} OUT[];

uniform int u_AttachmentPoint;

void main(){
    if (gl_InvocationID == 0) {
        gl_TessLevelInner[0] = 50.0f;
        gl_TessLevelInner[1] = 50.0f;

        gl_TessLevelOuter[0] = 50.0f; // Neg x
        gl_TessLevelOuter[1] = 50.0f; // Neg z
        gl_TessLevelOuter[2] = 50.0f; // Pos x
        gl_TessLevelOuter[3] = 50.0f; // Pos z

        if (u_AttachmentPoint > 0 && u_AttachmentPoint <= 4) {
            gl_TessLevelOuter[u_AttachmentPoint - 1] = 1.0f;
        }
    }

    OUT[gl_InvocationID].c_Position = IN[gl_InvocationID].v_Position;
    OUT[gl_InvocationID].c_TexCoord = IN[gl_InvocationID].v_TexCoord;

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
