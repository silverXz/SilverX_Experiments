#version 400

layout ( location = 0 ) out vec3 TexAO;

in vec3 CamDir;
noperspective in vec2 Coord;

uniform sampler2D TexNormalDepth;
uniform sampler2D TexRandom;
uniform vec3 Samples[16];

uniform float power;
uniform vec2 NoiseScale;
uniform float Radius;

uniform mat4 ProjectionMatrix;

void main()
{
	
	vec4 eNormDepth = texture(TexNormalDepth,Coord);
	vec3 origin = CamDir / CamDir.z * eNormDepth.a;
	vec3 eNormal = normalize(eNormDepth.xyz);
	vec3 rvec = texture(TexRandom,Coord * NoiseScale).xyz;
	vec3 tangent = normalize( rvec - eNormal * dot( rvec,eNormal));
	vec3 bitangent = normalize(cross( eNormal, tangent ));
	mat3 tbn = mat3(tangent,bitangent,eNormal);

	float ao = 0.0f;
	
	for( int i = 0 ; i < 16 ; i++ )
	{
		vec3 s = tbn * Samples[i];
		s = s * Radius + origin;

		vec4 offset = vec4(s,1.0f);

		offset = ProjectionMatrix * offset;
		offset.xy /= offset.w;
		offset.xy = offset.xy * 0.5 + vec2(0.5f);

		float sampleDepth = texture(TexNormalDepth,offset.xy).a;

		float rangeCheck = smoothstep(0.0,1.0, Radius/ abs(origin.z-sampleDepth));
		ao += rangeCheck * step(s.z,sampleDepth);
		//float rangeCheck = abs(origin.z - sampleDepth ) < Radius ? 1.0f : 0.0f;
		//ao += ( sampleDepth <= s.z ? 0.0 : 1.0 ) * rangeCheck;
	}

	ao = 1.0f - ( ao / 16.0f);



	TexAO = vec3(pow(ao,power));
}