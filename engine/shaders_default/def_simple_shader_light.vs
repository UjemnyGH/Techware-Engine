#version 450 core

layout (std140, binding = 1) uniform CameraMat {
    mat4 projection;
    mat4 view;
};

uniform mat4 uTransform;

layout (location = 0) in vec4 iPos;
layout (location = 1) in vec2 iTexCoords;
layout (location = 2) in vec3 iNorm;
layout (location = 3) in float iTexId;
layout (location = 4) in float iTexLer;
layout (location = 5) in vec4 iCol;

out vec4 vPos;
out vec2 vTexCoords;
out vec3 vNorm;
out float vTexId;
out float vTexLer;
out vec4 vColor;

void main() {
    gl_Position = CameraMat.projection * CameraMat.view * uTransform * iPos;

    vPos = iPos;
    vTexCoords = iTexCoords;
    vNorm = iNorm;
    vTexId = iTexId;
    vTexLer = iTexLer;
    vColor = iCol;
}