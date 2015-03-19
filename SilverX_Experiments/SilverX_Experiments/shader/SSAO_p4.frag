#version 400

layout ( location = 0 ) out vec4 FragColor;

in vec3 CamDir;
noperspective in vec2 Coord;

uniform sampler2D TexAmbient;
uniform sampler2D TexDiffuse;
uniform sampler2D TexSpecular;
uniform sampler2D TexNormalDepth;
uniform sampler2D TexAO;

struct LightInfo
{
	vec3 Position;
	vec3 Intensity;
};

uniform LightInfo Light0;

const float shininess = 128.0f;

vec3 phong(vec3 ePosition, vec3 eNormal, vec3 Ka, vec3 Kd, vec3 Ks,float Ao)
{
	vec3 l = normalize( Light0.Position - ePosition );
	vec3 s = normalize( -ePosition );
	vec3 n = normalize( eNormal );
	vec3 r = reflect( -l,n);
	return Light0.Intensity * ( Ao * Ka + Kd * max( dot(l,n),0.0) + Ks * pow( max(dot(r,s),0.0f),shininess));
}

void main()
{
	vec4 eNormDepth = texture( TexNormalDepth, Coord );
	vec3 ePosition = CamDir / CamDir.z * eNormDepth.a;
	vec3 eNormal = eNormDepth.xyz;
	vec3 Ka = texture(TexAmbient,Coord).xyz;
	vec3 Kd = texture( TexDiffuse,Coord).xyz;
	vec3 Ks = texture( TexSpecular,Coord).xyz;
	float AO = texture( TexAO,Coord).x;
	FragColor = vec4(phong(ePosition,eNormal,Ka,Kd,Ks,AO),1.0f);
	//float nearClip = 1.0f;
	//float farClip = 100.0f;
	//float linearDepth = ( -eNormDepth.a - nearClip ) / ( farClip - nearClip );
	//FragColor = vec4( vec3(linearDepth),1.0f);
}