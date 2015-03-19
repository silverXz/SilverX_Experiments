#version 400

in vec4 eNormalDepth;

layout ( location = 0 ) out vec4 FragNormalDepth;
layout ( location = 1 ) out vec3 FragAmbient;
layout ( location = 2 ) out vec3 FragDiffuse;
layout ( location = 3 ) out vec3 FragSpecular;


struct MaterialInfo
{
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
};

uniform MaterialInfo material;


void main()
{
	FragNormalDepth = eNormalDepth;
	FragAmbient = material.Ka;
	FragDiffuse = material.Kd;
	FragSpecular = material.Ks;
}