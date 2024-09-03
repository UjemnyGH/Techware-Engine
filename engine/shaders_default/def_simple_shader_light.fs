#version 450 core

uniform sampler2DArray mTexture;

layout (std140, binding = 2) uniform LightingDefaultValues {
    float ambient;
    float diffuse;
    float specular;
};

uniform float uColorMapLayer;
uniform float uSpecularMapLayer;
uniform float uNormalMapLayer;
uniform float uGlossMapLayer;
uniform float uCavityMapLayer;
uniform float uTranslucencyMapLayer;

in vec4 vPos;
in vec2 vTexCoords;
in vec3 vNorm;
in vec4 vColor;

out vec4 oCol;

void main() {
    oCol = vCol;
}