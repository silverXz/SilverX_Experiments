#version 400

layout (location = 0 ) in vec3 VertexPosition;
layout ( location = 1 ) in vec3 VertexNormal;
layout ( location = 2 ) in vec2 TexCoord;

out vec3 CamDir;
noperspective out vec2 Coord;

uniform mat4 InverseProjectionMatrix;

void main()
{
	Coord = TexCoord;
	vec4 eyePos = InverseProjectionMatrix * vec4(VertexPosition,1.0);
	CamDir = normalize( vec3(eyePos));
	gl_Position = vec4(VertexPosition,1.0f);
}