#version 400

in vec3 ePosition;
in vec3 eNormal;

layout (location = 0) out vec4 None;
layout (location = 1) out vec3 FragEyePosition;
layout (location = 2) out vec3 FragNormal;
layout (location = 3) out vec3 FragAmbient;
layout (location = 4) out vec3 FragDiffuse;
layout (location = 5) out vec3 FragSpecular;

struct MaterialInfo
{
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float Shininess;
};

uniform MaterialInfo material;



void main()
{
	None = vec4(0.0f);
	FragEyePosition = ePosition;
	FragNormal = normalize(eNormal);
	FragAmbient = material.Ka;
	FragDiffuse = material.Kd;
	FragSpecular = material.Ks;
}
	
	