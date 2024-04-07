#version 440

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec3 worldPos;
    vec2 texCoords;
} gs_in[3];

out GS_OUT {
    vec3 worldPosition;
    vec3 worldNormal;
    vec2 textureCoords;
} gs_out;

void main() {
    
    vec3 a = gs_in[1].worldPos - gs_in[0].worldPos;
    vec3 b = gs_in[2].worldPos - gs_in[0].worldPos;
    vec3 normal = cross(a, b);

    gl_Position = gl_in[0].gl_Position;
    gs_out.worldPosition = gs_in[0].worldPos;
    gs_out.worldNormal = normal;
    gs_out.textureCoords = gs_in[0].texCoords;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    gs_out.worldPosition = gs_in[1].worldPos;
    gs_out.worldNormal = normal;
    gs_out.textureCoords = gs_in[1].texCoords;
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    gs_out.worldPosition = gs_in[2].worldPos;
    gs_out.worldNormal = normal;
    gs_out.textureCoords = gs_in[2].texCoords;
    EmitVertex();

    EndPrimitive();
}