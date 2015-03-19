#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexCoord;

out vec3 ePosition;
out vec3 eNormal;


uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 MVP;

void main()
{
	mat4 MV = ViewMatrix * ModelMatrix;
	ePosition = vec3(MV * vec4(VertexPosition,1.0));
	eNormal = vec3(MV * vec4(VertexNormal,0.0));
	gl_Position = MVP * vec4(VertexPosition,1.0);
}