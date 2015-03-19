#version 400

in float eDepth;
in vec3 eNormal;

layout (location = 0) out vec4 FragNormalDepth;
layout (location = 1) out vec3 FragAmbient;
layout (location = 2) out vec3 FragDiffuse;
layout (location = 3) out vec3 FragSpecular;

struct MaterialInfo
{
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
};

uniform MaterialInfo material;



void main()
{
	FragAmbient = material.Ka;
	FragDiffuse = material.Kd;
	FragSpecular = material.Ks;
	FragNormalDepth = vec4(eNormal * 0.5f + vec3(0.5f),eDepth);

}
	
	