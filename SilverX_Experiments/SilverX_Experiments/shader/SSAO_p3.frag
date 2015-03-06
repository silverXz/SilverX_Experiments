#version 400

layout ( location = 0 ) out vec4 FragColor;

in vec2 Coord;

uniform sampler2D TexAO;
uniform sampler2D TexPosition;
uniform sampler2D TexNormal;
uniform sampler2D TexAmbient;
uniform sampler2D TexDiffuse;
uniform sampler2D TexSpecular;

struct LightInfo
{
	vec3 Intensity;
	vec3 Position;
};

const float Shininess = 128.0f;

uniform LightInfo Light0;

vec3 phong(vec3 ePosition, vec3 eNormal, vec3 Ka, vec3 Kd, vec3 Ks,float ao )
{
	vec3 n = normalize(eNormal);
	vec3 v = normalize(-ePosition);
	vec3 l = normalize(Light0.Position - ePosition);
	vec3 r = reflect(-l,n);
	return Light0.Intensity * ((1.0 - ao) * Ka +  Kd * max( dot(l,n),0.0 ) + Ks * pow( max(dot(r,v),0.0),Shininess ));
}

void main()
{
	vec3 ePosition = vec3(texture(TexPosition,Coord));
	vec3 eNormal = vec3(texture(TexNormal,Coord));
	vec3 Ka = vec3(texture(TexAmbient,Coord));
	vec3 Kd = vec3(texture(TexDiffuse,Coord));
	vec3 Ks = vec3(texture(TexSpecular,Coord));
	float ao = texture(TexAO,Coord).x;

	//vec3 v = normalize(eNormal);
	//v = v + vec3(1.0f);
	//v = v * 0.5f;
	//FragColor = vec4(v,1.0f);

	FragColor = vec4(phong(ePosition,eNormal,Ka,Kd,Ks,ao),1.0f);

	//FragColor = vec4(vec3(ao),1.0);


	//FragColor = vec4(color,1.0f);   // Fetch Color and render As Normal;
	//FragColor = vec4( (normal.x + 1.0f)*0.5f,(normal.y+1.0f)*0.5f,(normal.z+1.0f)*0.5f,1.0f);	// Scale normal component to [0,1] and render it.


}