#version 420 core

layout(location = 0) out vec4 color;

in Vertex {
    vec3 e_Position;
    vec3 e_Normal;
    vec2 e_TexCoord;
} IN;

struct PointLight {
    vec3 Color;
    float LinearAttenuation;

    vec3 Position;
    float QuadraticAttenuation;
};

struct DirectionalLight {
    vec3 Color;
    float Strength;

    vec3 Direction;
    float Padding0;
};

struct SpotLight {
    vec3 Color;
    float InnerCutoff;

    vec3 Position;
    float OuterCutoff;

    vec3 Direction;
    float DistanceCutoff;
};

layout (std140, binding = 0) uniform MVP {
    mat4 u_ViewProjection;
    vec3 u_CameraPos;
    float u_Padding;
    mat4 u_Projection;
    mat4 u_StaticView;
};

layout (std140, binding = 1) uniform Lights {
    vec3 u_AmbientColor;
    float u_AmbientStrength;

    uint u_PointLightCount;
    uint u_DirectionalLightCount;
    uint u_SpotLightCount;
    float u_Padding0;

    PointLight u_PointLights[70];
    DirectionalLight u_DirectionalLights[10];
    SpotLight u_SpotLights[10];
};

uniform samplerCube u_CubeMapTexture;

void main() {
    float ratio = 1.00 / 1.52;
    vec3 I = normalize(u_CameraPos - IN.e_Position);
    vec3 R = refract(-I, normalize(-IN.e_Normal), ratio);
    R.y = -R.y;
    R = normalize(R);

    vec3 baseColor = texture(u_CubeMapTexture, R).rgb;
    baseColor *= 0.25f;
    //baseColor += 0.5f;



    //vec2 sampleCoords = IN.e_TexCoord * 5.0f;
    //sampleCoords.y += u_TotalTime * 0.25f;
    //baseColor = 1.0f - baseColor;
    //baseColor += vec3(0.55f, 0.65f, 0.74f);
    baseColor += vec3(0.3f, 0.4f, 0.49f);

    vec3 diffuse = vec3(0.0f, 0.0f, 0.0f);
    vec3 specular = vec3(0.0f, 0.0f, 0.0f);

    // Directional Lights
    for (int i = 0; i < u_DirectionalLightCount; ++i) {
        float diff = max(dot(IN.e_Normal, -u_DirectionalLights[i].Direction), 0.0f);
        diffuse += u_DirectionalLights[i].Strength * diff * u_DirectionalLights[i].Color;

        vec3 viewDir = normalize(u_CameraPos - IN.e_Position);
        vec3 halfWayDir = normalize(u_DirectionalLights[i].Direction + viewDir);
        float spec = pow(max(dot(IN.e_Normal, halfWayDir), 0.0f), 32);
        specular += u_DirectionalLights[i].Strength * spec * u_DirectionalLights[i].Color;
    }

    vec3 ambient = u_AmbientColor * u_AmbientStrength;
    vec3 result = (ambient + diffuse + specular) * baseColor;

    float gamma = 2.2f;
    result = pow(result, vec3(gamma));
    color = vec4(result, 1.0f);
    //color = vec4(IN.e_Normal, 1.0f);
    //color = vec4(IN.e_TexCoord, 0.0f, 1.0f);
}
