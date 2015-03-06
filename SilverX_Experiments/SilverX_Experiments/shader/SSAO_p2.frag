#version 400

layout ( location = 0 ) out vec3 TexAO;

in vec2 Coord;

uniform sampler2D TexPosition;
uniform sampler2D TexNormal;

const int radius = 1;
const ivec2 samples[4] = ivec2[4](ivec2(radius,radius),ivec2(radius,-radius),ivec2(-radius,radius),ivec2(-radius,-radius));
	//ivec2(radius,0),ivec2(-radius,0),ivec2(0,radius),ivec2(0,-radius));

float DoAmbientOcclusion()
{
	vec3 n = normalize(vec3(texture(TexNormal,Coord)));
	vec3 p = vec3(texture(TexPosition,Coord));

	float ao = 0.0f;

	for( int i = 0 ; i < 4 ; ++i )
	{
		vec3 sp = vec3(textureOffset(TexPosition,Coord,samples[i]));
		vec3 sn = normalize(vec3(textureOffset(TexNormal,Coord,samples[i])));
		vec3 v = sp - p;
		float d = length(v);
		v = normalize(v);
		ao += max(0.0f,dot(v,n)) * (1.0/(1.0+d));
	}
	return ao;
}

void main()
{
	TexAO = vec3(DoAmbientOcclusion());
}