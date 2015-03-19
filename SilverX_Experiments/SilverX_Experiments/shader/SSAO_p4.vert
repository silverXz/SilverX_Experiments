#version 400

layout ( location = 0 ) in vec3 VertexPosition;
layout ( location = 1 ) in vec3 VertexNormal;
layout ( location = 2 ) in vec2 VertexCoord;

noperspective out vec2 Coord;
out vec3 CamDir;

uniform mat4 InverseProjectionMatrix;

void main()
{
	Coord = VertexCoord;
	vec4 eCorner = InverseProjectionMatrix * vec4( VertexPosition,1.0);
	CamDir = normalize( eCorner.xyz );
	gl_Position = vec4( VertexPosition,1.0f);
}