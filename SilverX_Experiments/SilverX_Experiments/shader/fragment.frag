#version 400

in vec3 ePosition;
in vec3 eNormal;

layout (location = 0) out vec4 FragColor;


struct LightInfo
{
	vec3 Intensity;
	vec3 Position;
};

uniform LightInfo Light0;

struct MaterialInfo
{
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float Shininess;
};

uniform MaterialInfo material;

vec3 phong()
{
	vec3 n = normalize(eNormal);
	vec3 v = normalize(-ePosition);
	vec3 l = normalize(Light0.Position - ePosition);
	vec3 r = reflect(-l,n);
	return Light0.Intensity * (material.Ka + material.Kd * max( dot(l,n),0.0 ) + material.Ks * pow( max(dot(r,v),0.0),material.Shininess ));
}

void main()
{
	FragColor = vec4( phong(),1.0);
	//vec3 n = normalize(eNormal);
	//FragColor = vec4( n * 0.5f + vec3(0.5f),1.0f);
}
	
	